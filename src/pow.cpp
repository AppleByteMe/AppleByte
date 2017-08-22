// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"
#include "bignum.h"

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
	CBigNum bnProofOfWorkLimit;
	bnProofOfWorkLimit.SetCompact(504365055);
    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
	int64 nBlockTimeAverage = 0;
	int64 nBlockTimeAveragePrev = 0;
	int64 nBlockTimeCount = 0;
	int64 nBlockTimeSum2 = 0;
    int64 nBlockTimeCount2 = 0;
    int64 LastBlockTime = 0;
    int64 PastBlocksMin = 14;
    int64 PastBlocksMax = 140;
    int64 CountBlocks = 0;
    CBigNum PastDifficultyAverage;
    CBigNum PastDifficultyAveragePrev;
	CBigNum mathsInput;
	CBigNum mathsOne;
	CBigNum mathsTwo;
	CBigNum mathsThree;
	
	int nHeight = pindexLast->nHeight + 1;
	
	bool fDebug = false;
	
	if (fDebug)
		LogPrintf("DGW: %d Beginning BlockReading %s \n", nHeight, BlockReading->ToString());
		
    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || BlockLastSolved->nHeight < PastBlocksMin) 
	{
		if (fDebug)
			LogPrintf("DGW: %d first conditonal returned minimum \n", nHeight);

        return bnProofOfWorkLimit.GetCompact(); 
    }

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) 
	{
		if (fDebug)
			LogPrintf("DGW: %d Loop i: %d, BlockReading->nHeight: %d \n", nHeight, i, BlockReading->nHeight);

        if (PastBlocksMax > 0 && i > PastBlocksMax) 
		{
			if (fDebug)
				LogPrintf("DGW: %d Break at %d \n", nHeight, i);
		
			break; 
		}
		
        CountBlocks++;
		
		if (fDebug)
			LogPrintf("DGW: %d CountBlocks %lld \n", nHeight, CountBlocks);
		
        if(CountBlocks <= PastBlocksMin) 
		{
            if (CountBlocks == 1) 
			{ 
				PastDifficultyAverage.SetCompact(BlockReading->nBits);
				
				if (fDebug)
					LogPrintf("DGW: %d BlockReading->nBits %08x, PastDifficultyAveragePrev: %s, Countblocks: %lld \n", nHeight, BlockReading->nBits, PastDifficultyAveragePrev.ToString().c_str(), CountBlocks);
			} else { 
				mathsInput.SetCompact(BlockReading->nBits);
				
				if (fDebug)
					LogPrintf("DGW: %d BlockReading->nBits %08x, mathsInput: %s, PastDifficultyAveragePrev: %s, Countblocks: %lld \n", nHeight, BlockReading->nBits, mathsInput.ToString().c_str(), PastDifficultyAveragePrev.ToString().c_str(), CountBlocks);
				
				mathsOne = mathsInput - PastDifficultyAveragePrev;
				mathsTwo = mathsOne / CountBlocks;
				mathsThree = mathsTwo + PastDifficultyAveragePrev;
				
				if (fDebug)
					LogPrintf("DGW: %d mathsOne: %s, mathsTwo: %s, mathsThree: %s \n", nHeight, mathsOne.ToString().c_str(), mathsTwo.ToString().c_str(), mathsThree.ToString().c_str());
				
				PastDifficultyAverage = mathsThree;
			}
			
			if (fDebug)
				LogPrintf("DGW: %d PastDifficultyAverage %s \n", nHeight, PastDifficultyAverage.ToString().c_str());
			
            PastDifficultyAveragePrev = PastDifficultyAverage;
        }

		if (fDebug)
			LogPrintf("DGW: %d LastBlockTime %lld \n", nHeight, LastBlockTime);
		
        if (LastBlockTime > 0)
		{
            int64_t Diff = (LastBlockTime - BlockReading->GetBlockTime());
			
			if (fDebug)
				LogPrintf("DGW: %d Diff %lld \n", nHeight, Diff);
			
            if(nBlockTimeCount <= PastBlocksMin) 
			{
                nBlockTimeCount++;
				
				if (fDebug)
					LogPrintf("DGW: %d nBlockTimeCount <= PastBlocksMin: true, nBlockTimeCount: %lld \n", nHeight, nBlockTimeCount);

                if (nBlockTimeCount == 1) 
				{ 
					nBlockTimeAverage = Diff; 
				} else { 
					nBlockTimeAverage = ((Diff - nBlockTimeAveragePrev) / nBlockTimeCount) + nBlockTimeAveragePrev; 
				}
				
                nBlockTimeAveragePrev = nBlockTimeAverage;
				
				if (fDebug)
					LogPrintf("DGW: %d nBlockTimeCount <= PastBlocksMin: true, nBlockTimeAveragePrev/nBlockTimeAverage %lld \n", nHeight, nBlockTimeAverage);
            }
			
            nBlockTimeCount2++;
            nBlockTimeSum2 += Diff;
			
			if (fDebug)
				LogPrintf("DGW: %d End of if (LastBlockTime > 0), nBlockTimeCount2 %lld, nBlockTimeSum2 %lld\n", nHeight, nBlockTimeCount2, nBlockTimeSum2);
        }
		
        LastBlockTime = BlockReading->GetBlockTime();

        if (BlockReading->pprev == NULL) 
		{
			if (fDebug)
				LogPrintf("DGW: %d BlockReading->pprev == NULL \n", nHeight);
		
			assert(BlockReading);
			break; 
		}
		
        BlockReading = BlockReading->pprev;
		
		if (fDebug)
			LogPrintf("DGW: %d End of for loop BlockReading %s \n", nHeight, BlockReading->ToString());
    }

    CBigNum bnNew(PastDifficultyAverage);
	
	if (fDebug)
		LogPrintf("DGW: %d bnNew.GetCompact() %s \n", nHeight, bnNew.ToString().c_str());
	
	if (nBlockTimeCount != 0 && nBlockTimeCount2 != 0) 
	{
		double SmartAverage = ((((long double) nBlockTimeAverage) * 0.7) + (((long double) nBlockTimeSum2 / (long double) nBlockTimeCount2) * 0.3));
		
		if (fDebug)
			LogPrintf("DGW: %d SmartAverage %f \n", nHeight, SmartAverage);
		
        if (SmartAverage < 1)
			SmartAverage = 1;
		
        double Shift = params.nPowTargetSpacing / SmartAverage;
		
		if (fDebug)
			LogPrintf("DGW: %d Shift %f \n", nHeight, Shift);

        double fActualTimespan = ((long double) CountBlocks * (double) params.nPowTargetSpacing) / Shift;
        double fTargetTimespan = ((long double) CountBlocks * (double) params.nPowTargetSpacing);
		
		if (fDebug) {
			LogPrintf("DGW: %d Before fActualTimespan %f, fTargetTimespan %f \n", nHeight, fActualTimespan, fTargetTimespan);
			LogPrintf("DGW: %d nPowTargetSpacing %f \n", nHeight, (double) params.nPowTargetSpacing);
		}

		if (fActualTimespan < fTargetTimespan / 3)
			fActualTimespan = fTargetTimespan / 3;
		
		if (fActualTimespan > fTargetTimespan * 3)
			fActualTimespan = fTargetTimespan * 3;
		
		if (fDebug)
			LogPrintf("DGW: %d After fActualTimespan %f, fTargetTimespan %f \n", nHeight, fActualTimespan, fTargetTimespan);
		
		int64_t nActualTimespan = fActualTimespan;
		int64_t nTargetTimespan = fTargetTimespan;
		
		if (fDebug)
			LogPrintf("DGW: %d int64_t nActualTimespan %lld, int64_t nTargetTimespan %lld \n", nHeight, nActualTimespan, nTargetTimespan);

		// Retarget
		bnNew *= nActualTimespan;
		bnNew /= nTargetTimespan;
		
		if (fDebug)
			LogPrintf("DGW: %d Retarget bnNew.GetCompact() %d \n", nHeight, bnNew.ToString().c_str());
	}

    if (bnNew > bnProofOfWorkLimit)
	{
        bnNew = bnProofOfWorkLimit;
		
		if (fDebug)
			LogPrintf("DGW: %d bnNew > bnProofOfWorkLimit bnNew.GetCompact() %d \n", nHeight, bnNew.ToString().c_str());
    }

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
