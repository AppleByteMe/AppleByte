// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <checkpoints.h>

#include <chain.h>
#include <chainparams.h>
#include <reverse_iterator.h>
#include <validation.h>

#include <stdint.h>


namespace Checkpoints {

    CBlockIndex* GetLastCheckpoint(const CCheckpointData& data)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        for (const MapCheckpoints::value_type& i : reverse_iterate(checkpoints))
        {
            const uint256& hash = i.second;
            BlockMap::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return nullptr;
    }

    int GetTotalBlocksEstimate(const CCheckpointData& data)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        if (checkpoints.empty())
            return 0;

        return checkpoints.rbegin()->first;
    }
	
    uint256 GetLastAvailableCheckpoint(const CCheckpointData& data)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        for (const MapCheckpoints::value_type& i : reverse_iterate(checkpoints))
        {
            const uint256& hash = i.second;
            if (mapBlockIndex.count(hash) && chainActive.Contains(mapBlockIndex[hash]))
                return hash;
        }
        return(Params().GetConsensus().hashGenesisBlock);
    }

    uint256 GetLatestHardenedCheckpoint(const CCheckpointData& data)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        if (checkpoints.empty())
            return Params().GetConsensus().hashGenesisBlock;

        return (checkpoints.rbegin()->second);
    }
} // namespace Checkpoints
