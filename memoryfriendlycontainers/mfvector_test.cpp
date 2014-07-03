//
// Copyright (c) 2014 Jakub Jabłoński
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <cassert>
#include <iostream>
#include <iterator>
#include <ostream>
#include <vector>
#include <type_traits>
#include <unistd.h>

#include "mfvector.h"
#include "object.h"
#include "gtest/gtest.h"

class VectorTest : public ::testing::Test
{
protected:
    VectorTest() : v1(1), v10(10)
    {}
    
    mfvector<object> v0;
    mfvector<object> v1;
    mfvector<object> v10;
};

TEST_F(VectorTest, Initial)
{
    EXPECT_EQ(0, v0.size());
    EXPECT_EQ(0, v0.capacity());
	EXPECT_EQ(v0.begin(), v0.end());
    
    EXPECT_EQ(0, v1.size());
    EXPECT_EQ(1, v1.capacity());
	EXPECT_EQ(v1.begin(), v1.end());
    
    EXPECT_EQ(0, v10.size());
    EXPECT_EQ(10, v10.capacity());
	EXPECT_EQ(v10.begin(), v10.end());
}

TEST_F(VectorTest, PushBack)
{
	v0.push_back(object("a"));
    EXPECT_EQ(0, v0.size());
    EXPECT_EQ(0, v0.capacity());
	EXPECT_EQ(v1.begin(), v1.end());
    
	v1.push_back(object("a"));
    EXPECT_EQ(1, v1.size());
    EXPECT_EQ(1, v1.capacity());
	EXPECT_LT(v1.begin(), v1.end());
	EXPECT_EQ(v1.begin() + 1, v1.end());

	v10.push_back(object("a"));
    EXPECT_EQ(1, v10.size());
    EXPECT_EQ(10, v10.capacity());
	EXPECT_LT(v10.begin(), v10.end());
	EXPECT_EQ(v10.begin() + 1, v10.end());

	v10.push_back(object("b"));
    EXPECT_EQ(2, v10.size());
    EXPECT_EQ(10, v10.capacity());
	EXPECT_EQ(v10.begin() + 2, v10.end());
}

TEST_F(VectorTest, Erase)
{
	v10.push_back(object("a"));
	v10.push_back(object("b"));
    
    object* r = v10.erase(v10.begin(), v10.end());
    EXPECT_EQ(r, v10.begin());
}

