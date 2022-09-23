/**
 * @file: VirtualMemory.cpp
 * @authors: Muaz.Abdeen (300575297)
 *           almogco (207005950)
 * @brief:
 */

// ------------------------------ includes ------------------------------------------

#include "VirtualMemory.h"
#include "PhysicalMemory.h"

// ------------------------------ macros & constants --------------------------------

#define SUCCESS 1
#define FAILURE 0

// ------------------------------ GLOBAL VARIABLES -----------------------------------
				/** IMPORTANT : DO NOT USE GLOBAL VARIABLES **/
// ------------------------------ NON API FUNCTIONS ----------------------------------

word_t max(word_t num1, word_t num2) { return num1 > num2 ? num1 : num2; }
word_t min(word_t num1, word_t num2) { return num1 < num2 ? num1 : num2; }
void clearTable(uint64_t frameIndex);
int mapPageToFrame(const uint64_t &pageIDX, uint64_t &frameIDX);
int getFrameIDX(word_t &frameIDX, word_t &lastAddedFrameIDX);
word_t getEmptyFrame(word_t rootFrameIDX, word_t layer, const word_t &lastAddedFrameIDX);
word_t getUnusedFrame(word_t rootFrameIDX, word_t layer);
uint64_t getEvictedFrameIDX(word_t rootFrameIDX, word_t layer,
							uint64_t pageIndex, uint64_t &evictedPageIDX,
							uint64_t &evictedFrameIDX,
							uint32_t &maxPathWeight, uint32_t curPathWeight);
int evictFrame(uint64_t &frameIDX);
void unlinkFrame(word_t rootFrameIDX, word_t layer, const word_t &frameToUnlink);

// -----------------------------------------------------------------------------------

/**
 * @brief clears the page table pointed by the given frameIndex,
 * 		  that is, to assign 0 to all of the frame words.
 * @param frameIndex index of the frame pointing to the table.
 */
void clearTable(uint64_t frameIndex) {
	for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
		PMwrite(frameIndex * PAGE_SIZE + i, 0);
	}
}

/**
 * @brief Get a frame containing an empty table – where all rows are 0.
 * 		  We don’t have to evict it, but we do have to remove the reference
 * 		  to this table from its parent.
 * @param rootFrameIDX index of the root frame
 * @param layer current layer of the tree
 * @param lastAddedFrameIDX Index of the last frame was mapped to a page.
 * 		  					The empty frame must be differ than it, cause this
 * 		  					frame is in the ancestors tree of our target.
 * @return minimal index of an empty frame differed from the table we've just created.
 */
word_t getEmptyFrame(word_t rootFrameIDX, word_t layer, const word_t &lastAddedFrameIDX) {
	word_t currentFrameIDX;
	word_t minEmptyFrameIDX = NUM_FRAMES;
	int emptyRows = 0;
	for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
		PMread(rootFrameIDX * PAGE_SIZE + i, &currentFrameIDX);
		if (currentFrameIDX > 0 && layer < TABLES_DEPTH - 1) {
			word_t emptyIndex = getEmptyFrame(currentFrameIDX, layer+1, lastAddedFrameIDX);
			minEmptyFrameIDX = min(minEmptyFrameIDX, emptyIndex);
		} else if (currentFrameIDX == 0) { emptyRows++; }
		// the empty frame is not the parent
		if (emptyRows == PAGE_SIZE && rootFrameIDX != lastAddedFrameIDX) {
			minEmptyFrameIDX = rootFrameIDX;
		}
	}
	return minEmptyFrameIDX;
}

/**
 * @brief Get an unused frame - all rows contain garbage.
 * 		  We fill frames in order so this is the minimal among the unused frames.
 * @param rootFrameIDX index of the root frame
 * @param layer current layer of the tree
 * @return The index next to the maximal frame Index referenced from any table we visit.
 */
word_t getUnusedFrame(word_t rootFrameIDX, word_t layer) {
	word_t currentFrameIDX;
	word_t maxFrameIDX = 0;
	for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
		PMread(rootFrameIDX * PAGE_SIZE + i, &currentFrameIDX);
		maxFrameIDX = max(currentFrameIDX, maxFrameIDX);
		if (currentFrameIDX > 0 && layer < TABLES_DEPTH - 1) {
			maxFrameIDX = max(maxFrameIDX, getUnusedFrame(currentFrameIDX, layer+1) - 1);
		}
	}
	return maxFrameIDX + 1;
}

/**
 * @brief Evict from the RAM a frame that is mapped to a data page
 * 		  where the sum of the node weights from the root page to the
 * 		  required page + the page is maximal, if the two weights are equal,
 * 		  we will pick page with the smallest number to be evacuated.
 * @param rootFrameIDX index of the root frame.
 * @param layer current layer of the tree.
 * @param pageIndex current index of the target page, we update it according to
 *        the path from the root to a leaf, where the data page is mapped.
 * @param evictedPageIDX Index of the page that mapped to the frame to be evicted.
 * @param evictedFrameIDX Index of the frame to be evicted from the RAM.
 * @param maxPathWeight the weight of the maximal path.
 * @param curPathWeight the weight of the current path.
 * @return Index of the frame to be evicted from the RAM.
 */
uint64_t getEvictedFrameIDX(word_t rootFrameIDX, word_t layer,
						    uint64_t pageIndex, uint64_t &evictedPageIDX,
						    uint64_t &evictedFrameIDX,
						    uint32_t &maxPathWeight, uint32_t curPathWeight) {

	word_t currentFrameIDX = rootFrameIDX;
	if (layer == TABLES_DEPTH) {
		curPathWeight += pageIndex % 2 ? WEIGHT_ODD : WEIGHT_EVEN;
		if (curPathWeight > maxPathWeight) {
			maxPathWeight = curPathWeight;
			evictedFrameIDX = currentFrameIDX;
			evictedPageIDX = pageIndex;
		}
		/* if the two weights are equal, we will pick page with
		* the smallest number to be evacuated */
		else if (curPathWeight == maxPathWeight && pageIndex < evictedPageIDX) {
			maxPathWeight = curPathWeight;
			evictedFrameIDX = currentFrameIDX;
			evictedPageIDX = pageIndex;
		}
		return evictedFrameIDX;
	}

	for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
		PMread(rootFrameIDX * PAGE_SIZE + i, &currentFrameIDX);
		if (currentFrameIDX != 0) {
			evictedFrameIDX = getEvictedFrameIDX(currentFrameIDX, layer + 1,
							pageIndex + (i << (OFFSET_WIDTH * (TABLES_DEPTH - layer - 1))),
							evictedPageIDX, evictedFrameIDX, maxPathWeight,
							curPathWeight + (currentFrameIDX % 2 ? WEIGHT_ODD : WEIGHT_EVEN));
		}
	}
	// if evictedFrameIDX is 0, that is can not evict any frame
	return evictedFrameIDX;
}

/**
 * @brief evict a frame from the RAM.
 * @param frameIDX the index of the evicted frame will be stored here.
 * @return 1 if evacuation completed successfully, 0 otherwise.
 */
int evictFrame(uint64_t &frameIDX) {
	uint32_t maxPathWeight = WEIGHT_EVEN;
	uint64_t evictedPageIDX;
	frameIDX = getEvictedFrameIDX(0, 0,
							   0, evictedPageIDX, frameIDX,
							   maxPathWeight, WEIGHT_EVEN);
	if (frameIDX > 0) {
		PMevict(frameIDX, evictedPageIDX);
		return SUCCESS;
	}
	return FAILURE;
}

/**
 * @brief unlink a frame from its parent table.
 * @param rootFrameIDX index of the root frame.
 * @param layer current layer of the tree.
 * @param frameToUnlink index of the frame to be unlinked.
 */
void unlinkFrame(word_t rootFrameIDX, word_t layer, const word_t &frameToUnlink) {
	word_t frameIndex;
	for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
		PMread(rootFrameIDX * PAGE_SIZE + i, &frameIndex);
		if (frameIndex == frameToUnlink) {
			PMwrite(rootFrameIDX * PAGE_SIZE + i, 0);
			return;
		}
		if (frameIndex > 0 && layer < TABLES_DEPTH - 1) {
			unlinkFrame(frameIndex, layer+1, frameToUnlink);
		}
	}
}

/**
 * @brief get a frame index to be used in building the frames tree.
 * @param frameIDX the index of the frame will be stored here.
 * @param lastAddedFrameIDX Index of the last frame was mapped to a page.
 * 		  					The frame we get must be differ than this frame,
 * 		  					because it is in the ancestors tree of our target.
 * @return 1 if the process completed successfully, 0 otherwise.
 */
int getFrameIDX(word_t &frameIDX, word_t &lastAddedFrameIDX) {
	/* (1) get an empty table, and remove the reference to this table from its parent. */
	frameIDX = getEmptyFrame(0, 0, lastAddedFrameIDX);
	if (frameIDX > 0 && frameIDX < NUM_FRAMES) {
		unlinkFrame(0,0, frameIDX);
		return SUCCESS;
	}
	/* (2) get an unused frame. */
	frameIDX = getUnusedFrame(0, 0);
	if (frameIDX > 0 && frameIDX < NUM_FRAMES) { return SUCCESS; }

	/* (3) If all frames are already used, then a page must be swapped out from some frame
	 * 	   in order to replace it with the relevant page (a table or actual page). */
	uint64_t evictedFrame = 0;
	if (evictFrame(evictedFrame)) {
		frameIDX = evictedFrame;
		unlinkFrame(0,0, frameIDX);
		return SUCCESS;
	}
	return FAILURE;
}

/**
 * @brief Map the given page to a legal frame in the RAM.
 * @param pageIDX index of the page to be mapped.
 * @param frameIDX the index of the frame will be stored here.
 * @return 1 if the mapping completed successfully, 0 otherwise.
 */
int mapPageToFrame(const uint64_t &pageIDX, uint64_t &frameIDX) {
	word_t curTableAddr = 0;
	word_t nextTableAddr;
	for (auto layer = 0; layer < TABLES_DEPTH; ++layer) {
		word_t offset = (pageIDX >> (OFFSET_WIDTH * (TABLES_DEPTH - layer - 1))) % PAGE_SIZE;
		PMread(curTableAddr * PAGE_SIZE + offset, &nextTableAddr);
		if (nextTableAddr == 0) {
			// (1) Find an unused frame or evict a page from some frame.
			if (!getFrameIDX(nextTableAddr, curTableAddr)) { return FAILURE; }
			// (2) Write 0 in all of its contents (only necessary in tables)
			if (layer < TABLES_DEPTH - 1) { clearTable(nextTableAddr); }
			// (3) Restore the page we are looking for to frame (only necessary in actual pages)
			else {
//				clearTable(nextTableAddr);
				PMrestore(nextTableAddr, pageIDX);
			}
			// (4) link the table to its parents.
			PMwrite(curTableAddr * PAGE_SIZE + offset, nextTableAddr);
		}
		curTableAddr = nextTableAddr;
	}
	frameIDX = (uint64_t) curTableAddr;
	return SUCCESS;
}


// ================================================================================== //
// ============================== LIBRARY API ======================================= //
// ================================================================================== //

/**
 * @brief Initialize the virtual memory
 * 		  According to the given design, VMInitialize() only has to clear frame 0.
 *
 * 		  			## ALREADY IMPLEMENTED FOR US ##
 */
void VMinitialize() {
	if (NUM_FRAMES != 0) {
		clearTable(0);
	}
}

/**
 * @brief reads a word from the given virtual address and puts its content in *value.
 * @param virtualAddress virtualAddress to read from.
 * @param value value to be read.
 * @return returns 1 on success, or 0 on failure (if the address cannot be mapped to
 * 		   a physical address for any reason).
 */
int VMread(uint64_t virtualAddress, word_t* value) {
	// bad virtualAddress
	if (virtualAddress < 0 || virtualAddress >= VIRTUAL_MEMORY_SIZE) { return FAILURE; }

	// I. separate page index from offset
	uint64_t pageIDX = virtualAddress / PAGE_SIZE;
	uint64_t offset = virtualAddress % PAGE_SIZE;

	// II. map pageIDX to frameIDX in the RAM
	uint64_t frameIDX;
	if (!mapPageToFrame(pageIDX, frameIDX)) { return FAILURE; }

	// III. generate physicalAddress
	uint64_t physicalAddress = frameIDX * PAGE_SIZE + offset;

	// VI. read from RAM to *value
	PMread(physicalAddress, value);

    return SUCCESS;
}

/**
 * @brief writes a word to the given virtual address
 * @param virtualAddress virtualAddress to write to.
 * @param value value to be written.
 * @return returns 1 on success, or 0 on failure (if the address cannot be mapped to
 * 		   a physical address for any reason).
 */
int VMwrite(uint64_t virtualAddress, word_t value) {
	// bad virtualAddress
	if (virtualAddress < 0 || virtualAddress >= VIRTUAL_MEMORY_SIZE) { return FAILURE; }

	// I. separate page index from offset
	uint64_t pageIDX = virtualAddress / PAGE_SIZE;
	uint64_t offset = virtualAddress % PAGE_SIZE;

	// II. map pageIDX to frameIDX in the RAM
	uint64_t frameIDX = 0;
	if (!mapPageToFrame(pageIDX, frameIDX)) { return FAILURE; }

	// III. generate physicalAddress
	uint64_t physicalAddress = frameIDX * PAGE_SIZE + offset;

	// VI. write *value to RAM
	PMwrite(physicalAddress, value);

    return SUCCESS;
}
