# Copyright (c) 2013-2016 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS) $(GPROF_LDFLAGS) $(SANITIZER_LDFLAGS)
AM_CXXFLAGS = $(DEBUG_CXXFLAGS) $(HARDENED_CXXFLAGS) $(WARN_CXXFLAGS) $(NOWARN_CXXFLAGS) $(ERROR_CXXFLAGS) $(GPROF_CXXFLAGS) $(SANITIZER_CXXFLAGS)
AM_CPPFLAGS = $(DEBUG_CPPFLAGS) $(HARDENED_CPPFLAGS)
AM_LIBTOOLFLAGS = --preserve-dup-deps
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

VOLTA_INCLUDES=-I$(builddir) $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

VOLTA_INCLUDES += -I$(srcdir)/secp256k1/include
VOLTA_INCLUDES += $(UNIVALUE_CFLAGS)

libvolta_SERVER=libvolta_server.a
libvolta_COMMON=libvolta_common.a
libvolta_CONSENSUS=libvolta_consensus.a
libvolta_CLI=libvolta_cli.a
libvolta_UTIL=libvolta_util.a
libvolta_CRYPTO_BASE=crypto/libvolta_crypto_base.a
libvoltaQT=qt/libvoltaqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_ZMQ
libvolta_ZMQ=libvolta_zmq.a
endif
if BUILD_VOLTA_LIBS
libvoltaCONSENSUS=libvoltaconsensus.la
endif
if ENABLE_WALLET
libvolta_WALLET=libvolta_wallet.a
endif

libvolta_CRYPTO= $(libvolta_CRYPTO_BASE)
if ENABLE_SSE41
libvolta_CRYPTO_SSE41 = crypto/libvolta_crypto_sse41.a
libvolta_CRYPTO += $(libvolta_CRYPTO_SSE41)
endif
if ENABLE_AVX2
libvolta_CRYPTO_AVX2 = crypto/libvolta_crypto_avx2.a
libvolta_CRYPTO += $(libvolta_CRYPTO_AVX2)
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(libvolta_CRYPTO) \
  $(libvolta_UTIL) \
  $(libvolta_COMMON) \
  $(libvolta_CONSENSUS) \
  $(libvolta_SERVER) \
  $(libvolta_CLI) \
  $(libvolta_WALLET) \
  $(libvolta_ZMQ)

lib_LTLIBRARIES = $(libvoltaCONSENSUS)

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_VOLTAD
  bin_PROGRAMS += voltad
endif

if BUILD_VOLTA_UTILS
  bin_PROGRAMS += volta-cli volta-tx
endif

.PHONY: FORCE check-symbols check-security
# volta core #
VOLTA_CORE_H = \
  addrdb.h \
  addrman.h \
  base58.h \
  bech32.h \
  bloom.h \
  blockencodings.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/tx_verify.h \
  core_io.h \
  core_memusage.h \
  cuckoocache.h \
  fs.h \
  httprpc.h \
  httpserver.h \
  index/base.h \
  index/txindex.h \
  indirectmap.h \
  init.h \
  interfaces/handler.h \
  interfaces/node.h \
  interfaces/wallet.h \
  key.h \
  key_io.h \
  keystore.h \
  dbwrapper.h \
  limitedmap.h \
  logging.h \
  memusage.h \
  merkleblock.h \
  miner.h \
  net.h \
  net_processing.h \
  netaddress.h \
  netbase.h \
  netmessagemaker.h \
  noui.h \
  policy/feerate.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  protocol.h \
  pow.h \
  random.h \
  reverse_iterator.h \
  reverselock.h \
  rpc/blockchain.h \
  rpc/client.h \
  rpc/mining.h \
  rpc/protocol.h \
  rpc/server.h \
  rpc/rawtransaction.h \
  rpc/register.h \
  rpc/util.h \
  scheduler.h \
  script/ismine.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/events.h \
  support/lockedpool.h \
  sync.h \
  threadsafety.h \
  threadinterrupt.h \
  timedata.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utiltime.h \
  validation.h \
  validationinterface.h \
  versionbits.h \
  walletinitinterface.h \
  wallet/coincontrol.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/feebumper.h \
  wallet/fees.h \
  wallet/rpcwallet.h \
  wallet/wallet.h \
  wallet/walletdb.h \
  wallet/walletutil.h \
  wallet/coinselection.h \
  warnings.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h\
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh "$(abs_top_builddir)/src/obj/build.h" \
	  "$(abs_top_srcdir)"
libvolta_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between voltad and volta-qt
libvolta_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(VOLTA_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libvolta_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libvolta_server_a_SOURCES = \
  addrdb.cpp \
  addrman.cpp \
  bloom.cpp \
  blockencodings.cpp \
  chain.cpp \
  checkpoints.cpp \
  consensus/tx_verify.cpp \
  httprpc.cpp \
  httpserver.cpp \
  index/base.cpp \
  index/txindex.cpp \
  init.cpp \
  dbwrapper.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  net_processing.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  policy/rbf.cpp \
  pow.cpp \
  rest.cpp \
  rpc/blockchain.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtransaction.cpp \
  rpc/server.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  ui_interface.cpp \
  validation.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  $(VOLTA_CORE_H)

if ENABLE_ZMQ
libvolta_zmq_a_CPPFLAGS = $(VOLTA_INCLUDES) $(ZMQ_CFLAGS)
libvolta_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libvolta_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between voltad and volta-qt, but only linked
# when wallet enabled
libvolta_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(VOLTA_INCLUDES)
libvolta_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libvolta_wallet_a_SOURCES = \
  interfaces/wallet.cpp \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/feebumper.cpp \
  wallet/fees.cpp \
  wallet/init.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/walletdb.cpp \
  wallet/walletutil.cpp \
  wallet/coinselection.cpp \
  $(VOLTA_CORE_H)

# crypto primitives library
crypto_libvolta_crypto_base_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libvolta_crypto_base_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libvolta_crypto_base_a_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
  crypto/chacha20.h \
  crypto/chacha20.cpp \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h \
  crypto/pow/sph_blake.h \
  crypto/pow/sph_cubehash.h \
  crypto/pow/sph_echo.h \
  crypto/pow/sph_fugue.h \
  crypto/pow/sph_groestl.h \
  crypto/pow/sph_hamsi.h \
  crypto/pow/sph_haval.h \
  crypto/pow/sph_jh.h \
  crypto/pow/sph_keccak.h \
  crypto/pow/sph_luffa.h \
  crypto/pow/sph_sha2.h \
  crypto/pow/sph_sha2big.c \
  crypto/pow/sph_shabal.h \
  crypto/pow/sph_shavite.h \
  crypto/pow/sph_simd.h \
  crypto/pow/sph_skein.h \
  crypto/pow/sph_types.h \
  crypto/pow/sph_whirlpool.h \
  crypto/pow/bmw.c \
  crypto/pow/whirlpool.c \
  crypto/pow/Sponge.h \
  crypto/pow/blake.c \
  crypto/pow/blake2.h \
  crypto/pow/blake2_impl.h \
  crypto/pow/blake2s-ref.c \
  crypto/pow/cubehash.c \
  crypto/pow/echo.c \
  crypto/pow/fugue.c \
  crypto/pow/groestl.c \
  crypto/pow/hamsi.c \
  crypto/pow/hashblake.h \
  crypto/pow/hashgroestl.h \
  crypto/pow/hashquark.h \
  crypto/pow/hashqubit.h \
  crypto/pow/hashskein.h \
  crypto/pow/hashx11.h \
  crypto/pow/hashx13.h \
  crypto/pow/hashx15.h \
  crypto/pow/hashx17.h \
  crypto/pow/hashx16s.h \
  crypto/pow/haval.c \
  crypto/pow/jh.c \
  crypto/pow/keccak.c \
  crypto/pow/luffa.c \
  crypto/pow/Lyra2.c \
  crypto/pow/Lyra2.h \
  crypto/pow/Lyra2RE.c \
  crypto/pow/Lyra2RE.h \
  crypto/pow/scrypt.cpp \
  crypto/pow/scrypt.h \
  crypto/pow/shabal.c \
  crypto/pow/shavite.c \
  crypto/pow/simd.c \
  crypto/pow/skein.c \
  crypto/pow/Sponge.c \
  crypto/pow/sph_bmw.h


if USE_ASM
crypto_libvolta_crypto_base_a_SOURCES += crypto/sha256_sse4.cpp
endif

crypto_libvolta_crypto_sse41_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libvolta_crypto_sse41_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libvolta_crypto_sse41_a_CXXFLAGS += $(SSE41_CXXFLAGS)
crypto_libvolta_crypto_sse41_a_CPPFLAGS += -DENABLE_SSE41
crypto_libvolta_crypto_sse41_a_SOURCES = crypto/sha256_sse41.cpp

crypto_libvolta_crypto_avx2_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libvolta_crypto_avx2_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libvolta_crypto_avx2_a_CXXFLAGS += $(AVX2_CXXFLAGS)
crypto_libvolta_crypto_avx2_a_CPPFLAGS += -DENABLE_AVX2
crypto_libvolta_crypto_avx2_a_SOURCES = crypto/sha256_avx2.cpp

# consensus: shared between all executables that validate any consensus rules.
libvolta_consensus_a_CPPFLAGS = $(AM_CPPFLAGS) $(VOLTA_INCLUDES)
libvolta_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libvolta_consensus_a_SOURCES = \
  amount.h \
  arith_uint256.cpp \
  arith_uint256.h \
  consensus/merkle.cpp \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  hash.cpp \
  hash.h \
  prevector.h \
  primitives/block.cpp \
  primitives/block.h \
  primitives/transaction.cpp \
  primitives/transaction.h \
  pubkey.cpp \
  pubkey.h \
  script/voltaconsensus.cpp \
  script/interpreter.cpp \
  script/interpreter.h \
  script/script.cpp \
  script/script.h \
  script/script_error.cpp \
  script/script_error.h \
  serialize.h \
  span.h \
  tinyformat.h \
  uint256.cpp \
  uint256.h \
  utilstrencodings.cpp \
  utilstrencodings.h \
  version.h

# common: shared between voltad, and volta-qt and non-server tools
libvolta_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(VOLTA_INCLUDES)
libvolta_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libvolta_common_a_SOURCES = \
  base58.cpp \
  bech32.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  key.cpp \
  key_io.cpp \
  keystore.cpp \
  netaddress.cpp \
  netbase.cpp \
  policy/feerate.cpp \
  protocol.cpp \
  scheduler.cpp \
  script/ismine.cpp \
  script/sign.cpp \
  script/standard.cpp \
  warnings.cpp \
  $(VOLTA_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libvolta_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(VOLTA_INCLUDES)
libvolta_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libvolta_util_a_SOURCES = \
  support/lockedpool.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  fs.cpp \
  interfaces/handler.cpp \
  interfaces/node.cpp \
  logging.cpp \
  random.cpp \
  rpc/protocol.cpp \
  rpc/util.cpp \
  support/cleanse.cpp \
  sync.cpp \
  threadinterrupt.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(VOLTA_CORE_H)

if GLIBC_BACK_COMPAT
libvolta_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between volta-cli and volta-qt
libvolta_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(VOLTA_INCLUDES)
libvolta_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libvolta_cli_a_SOURCES = \
  rpc/client.cpp \
  $(VOLTA_CORE_H)

nodist_libvolta_util_a_SOURCES = $(srcdir)/obj/build.h
#

# voltad binary #
voltad_SOURCES = voltad.cpp
voltad_CPPFLAGS = $(AM_CPPFLAGS) $(VOLTA_INCLUDES)
voltad_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
voltad_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
voltad_SOURCES += voltad-res.rc
endif

voltad_LDADD = \
  $(libvolta_SERVER) \
  $(libvolta_WALLET) \
  $(libvolta_COMMON) \
  $(LIBUNIVALUE) \
  $(libvolta_UTIL) \
  $(libvolta_ZMQ) \
  $(libvolta_CONSENSUS) \
  $(libvolta_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBLEVELDB_SSE42) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

voltad_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

# if BUILD_DARWIN
# voltad_CPPFLAGS += -I/usr/local/opt/libomp/include
# voltad_LDFLAGS += -L/usr/local/opt/libomp/lib
# voltad_LDADD += -lomp
# endif

# volta-cli binary #
volta_cli_SOURCES = volta-cli.cpp
volta_cli_CPPFLAGS = $(AM_CPPFLAGS) $(VOLTA_INCLUDES) $(EVENT_CFLAGS)
volta_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
volta_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
volta_cli_SOURCES += volta-cli-res.rc
endif

volta_cli_LDADD = \
  $(libvolta_CLI) \
  $(LIBUNIVALUE) \
  $(libvolta_UTIL) \
  $(libvolta_CRYPTO)

volta_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# volta-tx binary #
volta_tx_SOURCES = volta-tx.cpp
volta_tx_CPPFLAGS = $(AM_CPPFLAGS) $(VOLTA_INCLUDES)
volta_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
volta_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
volta_tx_SOURCES += volta-tx-res.rc
endif

volta_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(libvolta_COMMON) \
  $(libvolta_UTIL) \
  $(libvolta_CONSENSUS) \
  $(libvolta_CRYPTO) \
  $(LIBSECP256K1)

volta_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# voltaconsensus library #
if BUILD_VOLTA_LIBS
include_HEADERS = script/voltaconsensus.h
libvoltaconsensus_la_SOURCES = $(crypto_libvolta_crypto_base_a_SOURCES) $(libvolta_consensus_a_SOURCES)

if GLIBC_BACK_COMPAT
  libvoltaconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libvoltaconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libvoltaconsensus_la_LIBADD = $(LIBSECP256K1)
libvoltaconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_VOLTA_INTERNAL
libvoltaconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CTAES_DIST =  crypto/ctaes/bench.c
CTAES_DIST += crypto/ctaes/ctaes.c
CTAES_DIST += crypto/ctaes/ctaes.h
CTAES_DIST += crypto/ctaes/README.md
CTAES_DIST += crypto/ctaes/test.c

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno
CLEANFILES += obj/build.h

EXTRA_DIST = $(CTAES_DIST)

config/volta-config.h: config/stamp-h1
	@$(MAKE) -C $(top_builddir) $(subdir)/$(@)
config/stamp-h1: $(top_srcdir)/$(subdir)/config/volta-config.h.in $(top_builddir)/config.status
	$(AM_V_at)$(MAKE) -C $(top_builddir) $(subdir)/$(@)
$(top_srcdir)/$(subdir)/config/volta-config.h.in:  $(am__configure_deps)
	$(AM_V_at)$(MAKE) -C $(top_srcdir) $(subdir)/config/volta-config.h.in

clean-local:
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf test/__pycache__

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
