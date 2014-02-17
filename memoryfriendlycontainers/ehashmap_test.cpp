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

#include "hashmapsc.h"
#include "object.h"
#include "gtest/gtest.h"

//bool operator!=(hashmapsc<int, object>::iter const& lhs, hashmapsc<int, object>::iter const& rhs)
//{
//    return !(lhs == rhs);
//}

//bool operator==(hashmapsc<int, object>::iter const& lhs, hashmapsc<int, object>::iter const& rhs)
//{
//    return (lhs.map == rhs.map && lhs.bucketIx == rhs.bucketIx && lhs.entry == rhs.entry);
//}

//bool operator!=(hashmapsc<int, object>::iter<std::__1::pair<int const, object> > const&, hashmapsc<int, object>::iter<std::__1::pair<int const, object> > const&)
//{
//    return false;
//}



class HashmapTest : public ::testing::Test {
protected:
    HashmapTest() : m1(1), m10(10) { }

    virtual void SetUp() { }
    
    // virtual void TearDown() {}
    
    hashmapsc<int, object> m0;
    hashmapsc<int, object> m1;
    hashmapsc<int, object> m10;
};

TEST_F(HashmapTest, Initial) {
    EXPECT_EQ(0, m0.size());
    EXPECT_EQ(0, m0.capacity());

    EXPECT_EQ(0, m1.size());
    EXPECT_EQ(1, m1.capacity());
}

TEST_F(HashmapTest, Insert)
{
    object a("a");
	m1.insert(3, a);
    EXPECT_EQ(1, m1.size());
    EXPECT_EQ(a, m1[3]);
}

TEST_F(HashmapTest, Iterator)
{
	m10.insert(1, object("A"));
	m10.insert(2, object("B"));
	m10.insert(3, object("C"));
	m10.insert(4, object("D"));

    std::cout << m10 << std::endl;

    for (hashmapsc<int, object>::iterator i = m10.begin(), e = m10.end(); operator!=<int, object>(i, e); ++i)
    {
    }
}

