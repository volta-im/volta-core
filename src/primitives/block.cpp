// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Copyright (c) 2018-2018 The Volta Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <utilstrencodings.h>
#include <crypto/common.h>

#include <crypto/pow/scrypt.h>
#include <crypto/pow/hashgroestl.h>
#include <crypto/pow/hashblake.h>
#include <crypto/pow/hashx17.h>
#include <crypto/pow/Lyra2RE.h>
#include <crypto/pow/hashx16s.h>

int ALGO = ALGO_BLAKE;

uint256 CBlockHeader::GetHash() const
{
    // return SerializeHash(*this);
    return GetPoWHash(ALGO_BLAKE);
}

uint256 CBlockHeader::GetPoWHash(int algo) const
{
    uint256 thash;
    switch (algo)
    {
        case ALGO_GROESTL:
            return HashGroestl(BEGIN(nVersion), END(nNonce));
        case ALGO_X17:
            return HashX17(BEGIN(nVersion), END(nNonce));
        case ALGO_LYRA2RE:
            lyra2re2_hash(BEGIN(nVersion), BEGIN(thash));
            break;
        case ALGO_X16S:
            return HashX16s(BEGIN(nVersion), END(nNonce), hashPrevBlock);
        case ALGO_SCRYPT:
            scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(thash));
        default:
        case ALGO_BLAKE:
            return HashBlake(BEGIN(nVersion), END(nNonce));    
    }
    return thash;
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
