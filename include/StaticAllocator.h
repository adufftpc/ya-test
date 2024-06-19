#pragma once

#include <cstdint>
#include <mutex>
#include <vector>


#define BLOCK_AllOC_REGION_SIZE     (1024)
#define BLOCK_ALlOC_BLOCK_SIZE      (32)


class BlockAllocator
{
public:
    BlockAllocator(uint8_t *regionPtr, uint32_t regionSize, uint32_t blockSize)
        : head(regionPtr),
        regionSize(regionSize),
        blockSize(blockSize),
        totalBlocks(regionSize / blockSize),
        freeBlocks(totalBlocks),
        lastPosition(-1),
        _byte(0), _bit(0) {
        if (freeBlocks && regionPtr != nullptr) {
            flags.resize(freeBlocks / 8 + ((freeBlocks % 8) != 0), 0xFF);
        }
    }

    void* allocate() {
        std::lock_guard<std::mutex> lg(mtx);
        void* ptr = nullptr;
        if (!freeBlocks)
            return ptr;

        // Strategy is to try allocate next block first, then if it is not free,
        // start iterating from the beginning
        if (checkFreePosition(lastPosition + 1)) {
            clearPositionFlag(++lastPosition);
            freeBlocks--;
            return getBlockByPosition(lastPosition);
        }

        for (uint32_t byte = 0; byte < flags.size(); ++byte) {
            // Skip byte if there is no free blocks
            if (0 == flags[byte])
                continue;

            uint8_t bit = 0;
            // while (0 == ((flags[byte] >> bit) & 0x1)) {
            while (!checkFreeByteBit(byte, bit) && bit < 8) {
                bit++;
            }
            // ptr = head + blockSize * ((byte << 3) + bit);
            ptr = getBlockByByteBit(byte, bit); // Byte and bit are in range by design
            // flags[byte] &= ~(1 << bit);
            clearByteBitFlag(byte, bit);
            freeBlocks--;
            lastPosition = (byte << 3) + bit;
            break;
        }
        return ptr;
    }

    void deallocate(uint8_t* ptr) {
        std::lock_guard<std::mutex> lg(mtx);

        // Check out of bounds
        if (ptr < head || (ptr - head) >= regionSize)
            return;
        // Check block alignment
        if ((ptr - head) % blockSize)
            return;

        uint32_t byte /* = ((ptr - head) / blockSize) / 8 */ ;
        uint8_t bit /*= ((ptr - head) / blockSize) % 8 */;
        encodePosition((ptr - head) / blockSize, byte, bit);
        // flags[byte] |= (1 << bit);
        setByteBitFlag(byte, bit);
        freeBlocks++;
        // Update lastPosition so the last deallocated block will be reused
        lastPosition = (ptr - head) / blockSize - 1;
        return;
    }

private:
    uint8_t* head;
    uint32_t regionSize;
    uint32_t blockSize;
    uint32_t totalBlocks;
    uint32_t freeBlocks;
    uint32_t lastPosition;
    uint32_t _byte;
    uint8_t _bit;
    std::mutex mtx;
    std::vector<uint8_t> flags;

    void encodePosition(uint32_t position, uint32_t &retByte, uint8_t &retBit) {
        // div by 8
        retByte = position >> 3;
        // if divisor is power of 2, reminder is dividend & (divisor - 1)
        retBit = position & 0x7;
    }

    bool checkFreeByteBit(uint32_t& byte, uint8_t& bit) {
        if (byte >= flags.size() || bit >= 8)
            return false;
        return flags[byte] & (1 << bit);
    }

    bool checkFreePosition(uint32_t position) {
        if (position >= totalBlocks)
            return false;
        encodePosition(position, _byte, _bit);
        return checkFreeByteBit(_byte, _bit);
    }


    bool setByteBitFlag(uint32_t& byte, uint8_t& bit) {
        if (byte >= flags.size() || bit >= 8)
            return false;
        flags[byte] |= (1 << bit);
        return true;
    }

    bool setPositionFlag(uint32_t& position) {
        if (position >= totalBlocks)
            return false;
        encodePosition(position, _byte, _bit);
        setByteBitFlag(_byte, _bit);
        return true;
    }


    bool clearByteBitFlag(uint32_t& byte, uint8_t& bit) {
        if (byte >= flags.size() || bit >= 8)
            return false;
        flags[byte] &= ~(1 << bit);
        return true;
    }

    bool clearPositionFlag(uint32_t& position) {
        if (position >= totalBlocks)
            return false;
        encodePosition(position, _byte, _bit);
        clearByteBitFlag(_byte, _bit);
        return true;
    }


    void* getBlockByByteBit(uint32_t& byte, uint8_t& bit) {
        if (byte >= flags.size() || bit >= 8)
            return nullptr;
        return static_cast<void*>(head + blockSize * ((byte << 3) + bit));
    }

    void* getBlockByPosition(uint32_t& position) {
        if (position >= totalBlocks)
            return nullptr;
        encodePosition(position, _byte, _bit);
        return getBlockByByteBit(_byte, _bit);
    }
};
