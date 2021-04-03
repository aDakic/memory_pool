#include <gtest/gtest.h>
#include "memory_pool/memory_pool.h"
#include <iostream>

TEST(test_memory_pool, construction)
{
    memory_pool pool(memory_config{4,12}, memory_config{2, 2});
    uint8_t a = 5;
     int* ptr = (int *) pool.allocate(sizeof(int) * 1);
    *ptr = a;

         int* ptr1 = (int *) pool.allocate(sizeof(int)* 1);
    *ptr1 = 6;

             int* ptr2 = (int *) pool.allocate(sizeof(int) * 1);
    *ptr2 = 7;
     std::cout << "aaaaaa" << *ptr  <<*ptr1 << *ptr2 << std::endl;
    // std::cout << "bbbbbb" << std::endl;


    pool.deallocate((uint8_t*)ptr, sizeof(int) * 1);
    pool.deallocate((uint8_t*) ptr1, sizeof(int) * 1);
    pool.deallocate((uint8_t*)ptr2, sizeof(int) * 1);

}