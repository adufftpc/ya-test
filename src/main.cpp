#include <iostream>
#include "../include/StaticAllocator.h"


static uint8_t buffer[BLOCK_AllOC_REGION_SIZE] = { 0 };


int main(int, char**)
{
    std::cout << "Starting static allocator main\n";
    BlockAllocator ba(buffer, BLOCK_AllOC_REGION_SIZE, BLOCK_ALlOC_BLOCK_SIZE);

    uint8_t* block = static_cast<uint8_t*>(ba.allocate());
    uint8_t* block2 = static_cast<uint8_t*>(ba.allocate());
    ba.deallocate(block);
    uint8_t* block3 = static_cast<uint8_t*>(ba.allocate());
    ba.deallocate(block3);
    ba.deallocate(block2);


    std::cout << "Done!\n";
}
