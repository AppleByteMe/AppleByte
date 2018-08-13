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

    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || BlockLastSolved->nHeight < PastBlocksMin) 
        return bnProofOfWorkLimit.GetCompact(); 

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) 
	{
        if (PastBlocksMax > 0 && i > PastBlocksMax) 
			break; 

        CountBlocks++;

        if(CountBlocks <= PastBlocksMin) 
		{
            if (CountBlocks == 1) 
			{ 
				PastDifficultyAverage.SetCompact(BlockReading->nBits);
			} else { 
				mathsInput.SetCompact(BlockReading->nBits);
				mathsOne = mathsInput - PastDifficultyAveragePrev;
				mathsTwo = mathsOne / CountBlocks;
				mathsThree = mathsTwo + PastDifficultyAveragePrev;
				PastDifficultyAverage = mathsThree;
			}

            PastDifficultyAveragePrev = PastDifficultyAverage;
        }

        if (LastBlockTime > 0)
		{
            int64_t Diff = (LastBlockTime - BlockReading->GetBlockTime());

            if(nBlockTimeCount <= PastBlocksMin) 
			{
                nBlockTimeCount++;

                if (nBlockTimeCount == 1) 
				{ 
					nBlockTimeAverage = Diff; 
				} else { 
					nBlockTimeAverage = ((Diff - nBlockTimeAveragePrev) / nBlockTimeCount) + nBlockTimeAveragePrev; 
				}

                nBlockTimeAveragePrev = nBlockTimeAverage;
			}

            nBlockTimeCount2++;
            nBlockTimeSum2 += Diff;
		}

        LastBlockTime = BlockReading->GetBlockTime();

        if (BlockReading->pprev == NULL) 
		{
			assert(BlockReading);
			break; 
		}

        BlockReading = BlockReading->pprev;
    }

    CBigNum bnNew(PastDifficultyAverage);

	if (nBlockTimeCount != 0 && nBlockTimeCount2 != 0) 
	{
		double SmartAverage = ((((long double) nBlockTimeAverage) * 0.7) + (((long double) nBlockTimeSum2 / (long double) nBlockTimeCount2) * 0.3));

        if (SmartAverage < 1)
			SmartAverage = 1;

        double Shift = params.nPowTargetSpacing / SmartAverage;

        double fActualTimespan = ((long double) CountBlocks * (double) params.nPowTargetSpacing) / Shift;
        double fTargetTimespan = ((long double) CountBlocks * (double) params.nPowTargetSpacing);

		if (fActualTimespan < fTargetTimespan / 3)
			fActualTimespan = fTargetTimespan / 3;

		if (fActualTimespan > fTargetTimespan * 3)
			fActualTimespan = fTargetTimespan * 3;

		int64_t nActualTimespan = fActualTimespan;
		int64_t nTargetTimespan = fTargetTimespan;

		// Retarget
		bnNew *= nActualTimespan;
		bnNew /= nTargetTimespan;
	}

    if (bnNew > bnProofOfWorkLimit)
        bnNew = bnProofOfWorkLimit;

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
