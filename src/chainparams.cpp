// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * Genesis Block:
 * CBlock(hash=e8492193de64cc8c5ec569841f009d575832c630dd730b1c00af94238a4aba79, input=010000000000000000000000000000000000000000000000000000000000000000000000edfffb6e330631cac324c1c212787a803c71ba9020da96ef284370c2b90debab554e5e53f0ff0f1e85cae4fc, PoW=000003a2a84effb2fa2ebdecd1ab6cbbf703697b74c3ada89791c4d826c27658, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=abeb0db9c2704328ef96da2090ba713c807a7812c2c124c3ca3106336efbffed, nTime=1398689365, nBits=1e0ffff0, nNonce=4242852485, vtx=1)
 * CTransaction(hash=abeb0db9c2704328ef96da2090ba713c807a7812c2c124c3ca3106336efbffed, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 * CTxIn(COutPoint(0000000000000000000000000000000000000000000000000000000000000000, 4294967295), coinbase 04ffff001d01042c4e657720596f726b202d20204170706c654279746520636f6d696e67207468697320417072696c2032303134)
 * CTxOut(nValue=50.00000000, scriptPubKey=040184710fa689ad5023690c80f3a4)
 * vMerkleTree: abeb0db9c2704328ef96da2090ba713c807a7812c2c124c3ca3106336efbffed 
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "New York -  AppleByte coming this April 2014";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 751;
        consensus.BIP34Hash = uint256S("0x96bd7809b1aebb5f12416f4f58f070c02b960f9263af2b10184df66515d110a1");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.checkpointPubKey = "041c7cda200eff11786757b7a21b72799db193bc86bf34f0307f282bba8775f99ff8dcbf625a32b23660333d3a92a7aa3d85cb7302eabfd661a7a7cb128d38b1c1";
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 8064; // 80%
        consensus.nMinerConfirmationWindow = 10080;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1504224000; // Sep 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1535760000; // Sep 1st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1504224000; // Oct 1st, 2017.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1535760000; // Oct 1st, 2018

        // Deployment of SEGSIGNAL
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nStartTime = 1509580800; // November 2nd, 2017.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nTimeout = 1541116800; // November 2nd, 2018.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nOverrideMinerConfirmationWindow = 1680; // ~2.33 days
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nOverrideRuleChangeActivationThreshold = 1176; // 70%

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xea;
        pchMessageStart[1] = 0xaf;
        pchMessageStart[2] = 0xa5;
        pchMessageStart[3] = 0xca;
        nDefaultPort = 8608;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1398689365, 4242852485, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xe8492193de64cc8c5ec569841f009d575832c630dd730b1c00af94238a4aba79"));
        assert(genesis.hashMerkleRoot == uint256S("0xabeb0db9c2704328ef96da2090ba713c807a7812c2c124c3ca3106336efbffed"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("artbyte.me", "dnsseed1.artbyte.me"));
        vSeeds.push_back(CDNSSeedData("artbyte.me", "dnsseed2.artbyte.me"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,151);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 100001, uint256S("0x33c84e3d5dc11ee219fed43ee3055af4180b574c8f3466980180d4b5aee9d52e"))
            ( 450000, uint256S("0xf0ec5a3b0360f87032ece16c79141e2ae4061a8eb014d72ab7921b4fe1edc800"))
            ( 840000, uint256S("0xb65cf618824e6550442d4bfa52acc91cca0ced7f9ce0f6ca600f75165b30dce5"))
            ( 1095070, uint256S("0xdb58911c736d27e46f62c8c24a519dca8bd02047ab4f0f6e93f89a54ffd45652")),
            1533385344, // * UNIX timestamp of last checkpoint block
            1918123,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            1000.0     // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v4)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nMajorityEnforceBlockUpgrade = 500;
        consensus.nMajorityRejectBlockOutdated = 750;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 751;
        consensus.BIP34Hash = uint256S("0x0");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.checkpointPubKey = "042984cb406a34e80b4d95cc263635222d9be752c9196e4d58ed3e6cb602d9119ee8794cac93b69757b163cdeb7ee3fd0293bf1fb15ec379bc1373f50b959e8e6a";
        consensus.nPowTargetSpacing = 10;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1026; // 95% for testchains
        consensus.nMinerConfirmationWindow = 1080;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1501545600; // August 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533081600; // August 1st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1501545600; // August 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1533081600; // August 1st, 2018

        // Deployment of SEGSIGNAL
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nStartTime = 1509699600; // Nov 3rd, 2017.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nTimeout = 1541235600; // Nov 3rd, 2017.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nOverrideMinerConfirmationWindow = 540;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nOverrideRuleChangeActivationThreshold = 378;

        pchMessageStart[0] = 0xeb;
        pchMessageStart[1] = 0xb0;
        pchMessageStart[2] = 0xa6;
        pchMessageStart[3] = 0xcb;
        nDefaultPort = 18608;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1501675200, 38496, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x02e890de125e4e85965ed783e85be2e7f63f5c1cf59ed997c4796d8d987bf9a4"));
        assert(genesis.hashMerkleRoot == uint256S("0xabeb0db9c2704328ef96da2090ba713c807a7812c2c124c3ca3106336efbffed"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("artbyte.me", "testnet-dnsseed1.artbyte.me"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,65);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,193);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;


        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x02e890de125e4e85965ed783e85be2e7f63f5c1cf59ed997c4796d8d987bf9a4")),
            1501675200,
            1,
            100
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.checkpointPubKey = "042984cb406a34e80b4d95cc263635222d9be752c9196e4d58ed3e6cb602d9119ee8794cac93b69757b163cdeb7ee3fd0293bf1fb15ec379bc1373f50b959e8e6a";
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nOverrideMinerConfirmationWindow = 48;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGSIGNAL].nOverrideRuleChangeActivationThreshold = 29; // 60%

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 2, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xa7c01b3dba78294c8fd0fa14e94933e4325e6cd842e0dd2c8c5cd8c7731ebe89"));
        assert(genesis.hashMerkleRoot == uint256S("0xabeb0db9c2704328ef96da2090ba713c807a7812c2c124c3ca3106336efbffed"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206")),
            0,
            0,
            0
        };
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
 
