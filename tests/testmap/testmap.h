//
// Created by paolo on 08/01/21.
//

#ifndef MARIO_MAKER_TESTMAP_H
#define MARIO_MAKER_TESTMAP_H
#include "gtest/gtest.h"

class TestMap : public ::testing::Test {

    TestMap();

    virtual ~TestMap();

    virtual void SetUp();

    virtual void TearDown();
};


#endif //MARIO_MAKER_TESTMAP_H
