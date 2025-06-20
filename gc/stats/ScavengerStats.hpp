/*******************************************************************************
 * Copyright IBM Corp. and others 1991
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
 *******************************************************************************/

#if !defined(SCAVENGERSTATS_HPP_)
#define SCAVENGERSTATS_HPP_

class MM_EnvironmentBase;

#include "omrcfg.h"
#include "omrcomp.h"
#include "omrgcconsts.h"
#include "modronbase.h"
#include "modronopt.h"
#include "objectdescription.h"

#include "Math.hpp"

#define OMR_SCAVENGER_DISTANCE_BINS 32
#define OMR_SCAVENGER_CACHESIZE_BINS 16

#define SCAVENGER_FLIP_HISTORY_SIZE 16

/**
 * Storage for statistics relevant to a scavenging (semi-space copying) collector.
 * @ingroup GC_Stats
 */
class MM_ScavengerStats
{
public:

	struct FlipHistory {
		uintptr_t _tenureMask; /**< The historical tenure age */
		/* Array sizes are OBJECT_HEADER_AGE_MAX + 2 because we want to include age 0 through max as well as age -1 (never flipped) */
		uintptr_t _flipBytes[OBJECT_HEADER_AGE_MAX+2]; /**< The historical number of bytes flipped in each age group */
		uintptr_t _tenureBytes[OBJECT_HEADER_AGE_MAX+2]; /**< The historical number of bytes tenured in each age group */
	};

	uintptr_t _gcCount;  /**< Count of the number of GC cycles that have occurred */
	uintptr_t _rememberedSetOverflow;
	uintptr_t _causedRememberedSetOverflow;
	uintptr_t _scanCacheOverflow;
	uintptr_t _scanCacheAllocationFromHeap;
	uint64_t  _scanCacheAllocationDurationDuringSavenger;
	uintptr_t _backout;
	uintptr_t _flipCount;
	uintptr_t _flipBytes;
	uintptr_t _tenureAggregateCount;
	uintptr_t _tenureAggregateBytes;
#if defined(OMR_GC_LARGE_OBJECT_AREA)	
	uintptr_t _tenureLOACount;
	uintptr_t _tenureLOABytes;
#endif /* OMR_GC_LARGE_OBJECT_AREA */	
	uintptr_t _failedTenureCount;
	uintptr_t _failedTenureBytes;
	uintptr_t _failedTenureLargest;
	uintptr_t _failedFlipCount;
	uintptr_t _failedFlipBytes;
	uintptr_t _tenureAge;
#if defined(J9MODRON_TGC_PARALLEL_STATISTICS)
	uintptr_t _releaseScanListCount;
	uintptr_t _acquireFreeListCount;
	uintptr_t _releaseFreeListCount;
	uintptr_t _acquireScanListCount;
	uintptr_t _acquireListLockCount;  /**< cumulative (for scan&free list) lock count. if this number is much larger than cumulative acquire list count, it indicates over-splitting */
	uintptr_t _aliasToCopyCacheCount;
	uintptr_t _arraySplitCount;
	uintptr_t _arraySplitAmount;
	uintptr_t _workStallCount; /**< The number of times the thread stalled, and subsequently received more work */
	uintptr_t _completeStallCount; /**< The number of times the thread stalled, and waited for all other threads to complete working */
	uintptr_t _syncStallCount; /**< The number of times the thread stalled at a sync point */
	uint64_t _workStallTime; /**< The time, in hi-res ticks, the thread spent stalled waiting to receive more work */
	uint64_t _completeStallTime; /**< The time, in hi-res ticks, the thread spent stalled waiting for all other threads to complete working */
	uint64_t _syncStallTime; /**< The time, in hi-res ticks, the thread spent stalled at a sync point */
	uintptr_t _totalDeepStructures; /**<  The number of deep structures that are scanned with priority (number of deepScanOutline function calls) */
	uintptr_t _totalObjsDeepScanned; /**< The total number of deep structure objects that are special treated (number of copyAndForward with priority)*/
	uintptr_t _depthDeepestStructure; /**< Length of longest deep structure that is special treated */
	uintptr_t _copyScanUpdates;
#endif /* J9MODRON_TGC_PARALLEL_STATISTICS */

	/* Stats Used Specifically for Adaptive Threading */
	uint64_t _startTime; /**< Timestamp taken when worker starts the scavenge task */
	uint64_t _endTime; /**< Timestamp taken when worker completes the scavenge task */

	/* The time, in hi-res ticks, the thread spent stalled notifying other
	 * threads during scavenge. Note: this is not all inclusive, it records notify
	 * stall times only relevant to adaptive threading (e.g doesn't include backout cases)
	 */
	uint64_t _notifyStallTime;
	uint64_t _adjustedSyncStallTime; /**< The time, in hi-res ticks, the thread spent stalled at a sync point ADJUSTED to account for critical section time */

	/* Average (weighted) number of bytes free after a collection and
	 * average number of bytes promoted by a collection. Used by 
	 * concurrent collector to trigger concurrent when scavenger enabled.
	 */ 
	uintptr_t _avgInitialFree;
	uintptr_t _avgTenureBytes;
	uintptr_t _avgTenureBytesDeviation; /**< The average, weighted deviation of the tenureBytes*/
	uintptr_t _avgExpectedFlipBytes; /**< Recent historical average amount of objects flip/survived in Nursery, per a cycle. */
	
	uintptr_t _tiltRatio;	/**< use to pass tiltRatio to verbose */

	bool _nextScavengeWillPercolate;
	
#if defined(OMR_GC_LARGE_OBJECT_AREA)	
	uintptr_t _avgTenureLOABytes;
#endif /* OMR_GC_LARGE_OBJECT_AREA */

	uintptr_t _flipDiscardBytes;		/**< Bytes of survivor discarded by copy scan cache */
	uintptr_t _tenureDiscardBytes;		/**< Bytes of tenure discarded by copy scan cache */

	uintptr_t _survivorTLHRemainderCount;
	uintptr_t _tenureTLHRemainderCount;

	uintptr_t _semiSpaceAllocBytesAcumulation; /**< Bytes allocated in new space between scavenges, updated on global collects. */
	uintptr_t _tenureSpaceAllocBytesAcumulation; /**< Bytes allocated in tenure space between scavenges, updated on global collects. */

	uintptr_t _semiSpaceAllocationCountLarge;
	uintptr_t _semiSpaceAllocationCountSmall;
	uintptr_t _tenureSpaceAllocationCountLarge;
	uintptr_t _tenureSpaceAllocationCountSmall;

	uintptr_t _tenureExpandedBytes; /**< Bytes by which the heap expanded in order to complete the collection */
	uintptr_t _tenureExpandedCount; /**< The number of times the heap was expanded in order to complete the collection */
	uint64_t _tenureExpandedTime; /**< Time taken expanding the heap in order to complete the collection, in hi-res ticks */

	uint64_t _leafObjectCount;
	uint64_t _copy_distance_counts[OMR_SCAVENGER_DISTANCE_BINS];
	uint64_t _copy_cachesize_counts[OMR_SCAVENGER_CACHESIZE_BINS];
	uint64_t _copy_cachesize_sum;

	uint64_t _slotsCopied; /**< The number of slots copied by the thread since _slotsScanned was last sampled and reset */
	uint64_t _slotsScanned; /**< The number of slots scanned by the thread since _slotsCopied was last sampled and reset */
	
#if defined(OMR_GC_CONCURRENT_SCAVENGER)
	uint64_t _readObjectBarrierCopy; /**< Number of objects copied by read barrier */
	uint64_t _readObjectBarrierUpdate; /**< Number of reference slots updates, which may be (often is) preceded by object copy */ 
	uintptr_t _readObjectBarrierFlipBytes; /**< Bytes flipped through read barrier. Meaningful in the global-incremental stats struct, but not in a thread local. */
	uintptr_t _readObjectBarrierTenureBytes; /**< Bytes tenured through read barrier. Meaningful in the global-incremental stats struct, but not in a thread local. */
#endif /* OMR_GC_CONCURRENT_SCAVENGER */

protected:

private:
public:
	uintptr_t _flipHistoryNewIndex; /**< Index in to the first dimension of _flipHistory for the freshest history. */
	FlipHistory _flipHistory[SCAVENGER_FLIP_HISTORY_SIZE]; /**< Array for storing object flip stats. */

public:
	/**
	 * GC thread calls this to reset its slots copied/scanned counts and the scavenger's scaling factor before
	 * starting participation in completeScan(). This ensures that the scaling factor is maximal until all gc
	 * threads are participating.
	 *
	 * @param[in] copyScanRatio pointer to the scavenger's scaling factor
	 */
	MMINLINE void
	resetCopyScanCounts()
	{
		_slotsCopied = _slotsScanned = 0;
	}

#if defined(J9MODRON_TGC_PARALLEL_STATISTICS)
	MMINLINE void 
	addToWorkStallTime(uint64_t startTime, uint64_t endTime)
	{
		_workStallCount += 1;
		_workStallTime += (endTime - startTime);
	}
	
	MMINLINE void 
	addToCompleteStallTime(uint64_t startTime, uint64_t endTime)
	{
		_completeStallCount += 1;
		_completeStallTime += (endTime - startTime);
	}

	MMINLINE void 
	addToSyncStallTime(uint64_t startTime, uint64_t endTime, uint64_t criticalSectionDuration = 0)
	{
		uint64_t criticalTime = criticalSectionDuration;
		uint64_t deltaTime = endTime - startTime;

		/*
		 * criticalSectionDuration and deltaTime can be measured on clocks running on different cores asynchronously.
		 * It means for close intervals (despite clocks are monotonic) criticalTime can be measured
		 * slightly larger than deltaTime. So, criticalSectionDuration should be adjusted in this case.
		 */
		if (criticalTime > deltaTime) {
			criticalTime = deltaTime;
		}

		_syncStallCount += 1;
		_syncStallTime += deltaTime;
		_adjustedSyncStallTime += (deltaTime - criticalTime);
	}
	
	MMINLINE void
	addToNotifyStallTime(uint64_t startTime, uint64_t endTime)
	{
		_notifyStallTime += (endTime - startTime);
	}

	/**
	 * Get the total stall time
	 * @return the time in hi-res ticks
	 */
	MMINLINE uint64_t 
	getStallTime()
	{
		return _workStallTime + _completeStallTime + _syncStallTime;
	}
#endif /* J9MODRON_TGC_PARALLEL_STATISTICS */

	MMINLINE void
	countCopyDistance(uintptr_t fromAddr, uintptr_t toAddr)
	{
		if (0 != fromAddr && 0 != toAddr) {
			uintptr_t delta = fromAddr ^ toAddr;
			uintptr_t bin = MM_Math::floorLog2(delta);
			if (delta > ((uintptr_t)1 << bin)) {
				bin += 1;
			}
			if (OMR_SCAVENGER_DISTANCE_BINS <= bin) {
				bin = OMR_SCAVENGER_DISTANCE_BINS - 1;
			}
			_copy_distance_counts[bin] += 1;
		}
	}

	MMINLINE void
	countCopyCacheSize(uint64_t copyCacheSize, uint64_t copyCacheSizeMax)
	{
		uint64_t binSize = copyCacheSizeMax / OMR_SCAVENGER_CACHESIZE_BINS;
		uint64_t binSlot = copyCacheSize / binSize;
		if (OMR_SCAVENGER_CACHESIZE_BINS <= binSlot) {
			binSlot = OMR_SCAVENGER_CACHESIZE_BINS - 1;
		}
		_copy_cachesize_counts[binSlot] += 1;
		_copy_cachesize_sum += copyCacheSize;
	}

	void clear(bool firstIncrement);
	
	/**
	 * @return true if at least one full Scavenge cycle is complete (stats are calculated once, at the end of each cycle)
	 */
	bool isAvailable(MM_EnvironmentBase *env);
	
	MM_ScavengerStats();

	struct FlipHistory* getFlipHistory(uintptr_t lookback);
};

#endif /* SCAVENGERSTATS_HPP_ */
