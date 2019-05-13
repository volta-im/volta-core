// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Copyright (c) 2018-2018 The Volta Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util.h>

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int algo, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    return GetNextTargetRequired(pindexLast, algo, params);
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);
    // LogPrintf("%s > %s\n", params.powLimit.ToString().c_str(), bnTarget.ToString().c_str());
    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // LogPrintf("%s > %s\n", hash.ToString().c_str(), bnTarget.ToString().c_str());
    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}

unsigned int GetNextTargetRequired_V1(const CBlockIndex* pindexLast, int algo, const Consensus::Params& params)
{
    const int64_t nTargetTimespan = 1 * 15;  // quick work-around for the old global timespan
    unsigned int nStakeTargetSpacing = 30;		// 30 seconds POS block spacing
    const int64_t nTargetSpacingWorkMax = 2 * nStakeTargetSpacing; // 1 minutes

    if (pindexLast == NULL)
        return UintToArith256(params.powLimit).GetCompact(); // genesis block

    // Proof-of-Stake blocks has own target limit
    arith_uint256 bnTargetLimit = UintToArith256(params.powLimit);

    const CBlockIndex* pindexPrev = pindexLast->GetAncestor(pindexLast->nHeight -1);
    if (pindexPrev == nullptr || pindexPrev->pprev == nullptr)
        return bnTargetLimit.GetCompact(); // first block
    const CBlockIndex* pindexPrevPrev = pindexPrev->GetAncestor(pindexLast->nHeight -2); 
    if (pindexPrevPrev->pprev == NULL)
        return bnTargetLimit.GetCompact(); // second block

    if(pindexLast->nHeight < 450)
	    return bnTargetLimit.GetCompact();

    int64_t nActualSpacing = pindexPrev->GetBlockTime() - pindexPrevPrev->GetBlockTime();

    if(nActualSpacing < 0)
    {
	    nActualSpacing = 1;
    }
    else if(nActualSpacing > nTargetTimespan)
    {
	    nActualSpacing = nTargetTimespan;
    }

    // ppcoin: target change every block
    // ppcoin: retarget with exponential moving toward target spacing
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexPrev->nBits); // FIXME: probably 15, but needs a check
    int64_t nTargetSpacing = std::min(nTargetSpacingWorkMax, (int64_t) nStakeTargetSpacing * (1 + pindexLast->nHeight - pindexPrev->nHeight));
    int64_t nInterval = nTargetTimespan / nTargetSpacing;
    bnNew *= ((nInterval - 1) * nTargetSpacing + nActualSpacing + nActualSpacing);
    bnNew /= ((nInterval + 1) * nTargetSpacing);

    if (bnNew > bnTargetLimit)
        bnNew = bnTargetLimit;

    return bnNew.GetCompact();
}

unsigned int LwmaCalculateNextWorkRequired(const CBlockIndex* pindexLast, int algo, const Consensus::Params& params)
{

    // LWMA for BTC clones
    // Algorithm by zawy, LWMA idea by Tom Harding
    // Code by h4x3rotab of BTC Gold, modified/updated by zawy
    // https://github.com/zawy12/difficulty-algorithms/issues/3#issuecomment-388386175
    //  FTL must be changed to about N*T/20 = 360 for T=120 and N=60 coins.
    //  FTL is MAX_FUTURE_BLOCK_TIME in chain.h.
    //  FTL in Ignition, Numus, and others can be found in main.h as DRIFT.
    //  Some coins took out a variable, and need to change the 2*60*60 here:
    //  if (block.GetBlockTime() > nAdjustedTime + 2 * 60 * 60)
    const int64_t T = GetTargetSpacing(pindexLast->nHeight);
    int64_t FTL = pindexLast->nHeight > 1000000 ? 6 * T : GetMaxClockDrift(pindexLast->nHeight);
    const int64_t N = 60; // Avg Window
    const int64_t k = N*(N+1)*T/2; 
    const int height = pindexLast->nHeight + 1;
    const bool debugprint = false; // TODO: implement
    assert(height > N);

    arith_uint256 sum_target;
    int64_t t = 0, j = 0;
    int64_t solvetime;

    std::vector<const CBlockIndex*> SameAlgoBlocks;
    for (int c = height-1; SameAlgoBlocks.size() < (N + 1); c--){
        const CBlockIndex* block = pindexLast->GetAncestor(c); // -1 after execution
        if (block->GetBlockHeader().GetAlgo() == algo){
            SameAlgoBlocks.push_back(block);
        }
        if (c < 100){ // If there are not enough blocks with this algo, return with an algo that *can* use less blocks
            return GetNextTargetRequired_V1(pindexLast, algo, params);
        }
    }
    // Creates vector with {block1000, block997, block993}, so we start at the back

    // Loop through N most recent blocks. starting with the lowest blockheight
    for (int i = N; i > 0; i--) {
        const CBlockIndex* block = SameAlgoBlocks[i-1]; // pindexLast->GetAncestor(i);
        const CBlockIndex* block_Prev = SameAlgoBlocks[i]; //block->GetAncestor(i - 1);
        solvetime = block->GetBlockTime() - block_Prev->GetBlockTime();
        solvetime = std::max(-FTL, std::min(solvetime, 6*T));
        
        j++;
        t += solvetime * j;  // Weighted solvetime sum.

        // Target sum divided by a factor, (k N^2).
        // The factor is a part of the final equation. However we divide sum_target here to avoid
        // potential overflow.
        arith_uint256 target;
        target.SetCompact(block->nBits);
        sum_target += target / (k * N);
    }
    // Keep t reasonable in case strange solvetimes occurred.
    if (t < k/10 ) {   t = k/10;  }

    const arith_uint256 pow_limit = UintToArith256(params.powLimit);
    arith_uint256 next_target = t * sum_target;
    if (next_target > pow_limit) {
        next_target = pow_limit;
    }

    return next_target.GetCompact();
}



unsigned int GetNextTargetRequired(const CBlockIndex* pindexLast, int algo, const Consensus::Params& params)
{
    if (pindexLast->nHeight < 200){//first 200 blocks with default retarget
        return GetNextTargetRequired_V1(pindexLast, algo, params);
    }
    return LwmaCalculateNextWorkRequired(pindexLast, algo, params); // And finally LWMA (XVTGPU implementation)
}

unsigned int GetAlgoWeight(int algo)
{
    switch (algo)
    {
        case ALGO_GROESTL:
            return (unsigned int)(0.005 * 100000);
        case ALGO_BLAKE:
            return (unsigned int)(0.00015 * 100000);
        case ALGO_X17:
            return (unsigned int)(6 * 100000);
        case ALGO_LYRA2RE:
            return (unsigned int)(6 * 100000);
        case ALGO_X16S:
            return (unsigned int)(7 * 100000);
        case ALGO_SCRYPT:
            return (unsigned int)(1.4 * 100000);
        default: // Lowest
            printf("GetAlgoWeight(): can't find algo %d", algo);
            return (unsigned int)(0.00015 * 100000);
    }
}
