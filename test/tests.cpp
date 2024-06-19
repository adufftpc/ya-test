#include "../include/tau/tau.h"
#include "../include/StaticAllocator.h"


static uint8_t buffer[BLOCK_AllOC_REGION_SIZE] = { 0 };


TAU_MAIN()
TEST(common, allocate_all) {
    BlockAllocator ba(buffer, BLOCK_AllOC_REGION_SIZE, BLOCK_ALlOC_BLOCK_SIZE);
    for (uint32_t i = 0; i < BLOCK_AllOC_REGION_SIZE / BLOCK_ALlOC_BLOCK_SIZE; ++i) {
        uint8_t* block = static_cast<uint8_t*>(ba.allocate());
        REQUIRE_NE(block, nullptr);
        REQUIRE_EQ(block, buffer + (i * BLOCK_ALlOC_BLOCK_SIZE));
    }
}

TEST(common, allocate_more_than_available) {
    BlockAllocator ba(buffer, BLOCK_AllOC_REGION_SIZE, BLOCK_ALlOC_BLOCK_SIZE);
    for (uint32_t i = 0; i < BLOCK_AllOC_REGION_SIZE / BLOCK_ALlOC_BLOCK_SIZE; ++i) {
        uint8_t* block = static_cast<uint8_t*>(ba.allocate());
    }
    uint8_t* extraBlock = static_cast<uint8_t*>(ba.allocate());
    REQUIRE_EQ(extraBlock, nullptr);
}

TEST(common, check_deallocation_and_reusing) {
    BlockAllocator ba(buffer, BLOCK_AllOC_REGION_SIZE, BLOCK_ALlOC_BLOCK_SIZE);
    uint8_t *block0, *block1, *block2;

    block0 = static_cast<uint8_t*>(ba.allocate());
    ba.deallocate(block0 + 1);  // Deallocation is not done for not aligned address
    block1 = static_cast<uint8_t*>(ba.allocate());
    ba.deallocate(block0);  // Deallocation is done and next block should reuse block0
    block2 = static_cast<uint8_t*>(ba.allocate());

    REQUIRE_EQ(block1 - block0, BLOCK_ALlOC_BLOCK_SIZE);
    REQUIRE_EQ(block2 - block0, 0);
}
