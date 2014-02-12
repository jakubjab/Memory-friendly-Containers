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

#include "mfunorderedmap.h"
#include "object.h"
#include "gtest/gtest.h"

class HashmapTest : public ::testing::Test {
protected:
    HashmapTest() : m1(1) {
    }
    virtual void SetUp() {
    }
    
    // virtual void TearDown() {}
    
    ehashmap<int, object> m1;
};

TEST_F(HashmapTest, IsEmptyInitially) {
    ASSERT_EQ(0, 0);
}

int main_ehashmap_test(void)
{
	ehashmap<int, object> m1(100);
    
	m1.insert(3, object("a"));
	m1.insert(50, object("b"));
	m1.insert(50, object("bb"));
	m1.insert(69, object("c"));
	m1.insert(89, object("d"));
	m1.insert(17, object("A"));
	m1.insert(55, object("B"));
	m1.insert(75, object("C"));
	m1.insert(0, object("XXX"));
    
	std::cout << m1 << std::endl;
    
	std::cout << "lookup 3: " << m1[3] << std::endl;
	std::cout << "lookup 50: " << m1[50] << std::endl;
	std::cout << "lookup 69: " << m1[69] << std::endl;
	std::cout << "lookup 123: " << m1[123] << std::endl;
    
	return 0;
}

TEST(HashmapInsert, Insert)
{
	ehashmap<int, object> m1(100);
	m1.insert(3, object("a"));
}
