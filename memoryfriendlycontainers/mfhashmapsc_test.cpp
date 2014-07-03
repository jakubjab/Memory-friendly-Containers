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

#include "mfhashmapsc.h"
#include "object.h"
#include "gtest/gtest.h"


class HashmapTest : public ::testing::Test {
protected:
    HashmapTest() : m1(1), m10(10)
    {}
    
    mfhashmapsc<int, object> m0;
    mfhashmapsc<int, object> m1;
    mfhashmapsc<int, object> m10;
};

TEST_F(HashmapTest, Initial) {
    EXPECT_EQ(0, m0.size());
    EXPECT_EQ(0, m0.capacity());

    EXPECT_EQ(0, m1.size());
    EXPECT_EQ(1, m1.capacity());

    EXPECT_EQ(0, m10.size());
    EXPECT_EQ(10, m10.capacity());
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
	m10.insert(0, object("X"));
	m10.insert(1, object("A"));
	m10.insert(2, object("B"));
	m10.insert(3, object("C"));
	m10.insert(4, object("D"));

//    std::cout << m10 << std::endl;

    {
        char found[] = "00000";
        for(mfhashmapsc<int, object>::iterator i = m10.begin(), e = m10.end(); i != e; ++i)
        {
//            std::cout << "[" << i->first << "]: " << i->second << std::endl;
            static_assert(!std::is_const<typeof(i->second)>::value, "second is const");
            ASSERT_LE(0, i->first);
            ASSERT_GE(4, i->first);
            found[i->first] = '1';
        }
        EXPECT_STREQ("11111", found);
    }
    
    {
        char found[] = "00000";
        for(mfhashmapsc<int, object>::iterator i = m10.begin(), e = m10.end(); i != e; i++)
        {
//            std::cout << "[" << i->first << "]: " << i->second << std::endl;
            static_assert(!std::is_const<typeof(i->second)>::value, "second is const");
            ASSERT_LE(0, i->first);
            ASSERT_GE(4, i->first);
            found[i->first] = '1';
        }
        EXPECT_STREQ("11111", found);        
    }
}

TEST_F(HashmapTest, ConstIterator)
{
	m10.insert(0, object("X"));
	m10.insert(1, object("A"));
	m10.insert(2, object("B"));
	m10.insert(3, object("C"));
	m10.insert(4, object("D"));
    
//    std::cout << m10 << std::endl;

    {
        char found[] = "00000";
        for(mfhashmapsc<int, object>::const_iterator i = m10.begin(), e = m10.end(); i != e; ++i)
        {
//          std::cout << "[" << i->first << "]: " << i->second << std::endl;
            static_assert(std::is_const<typeof(i->second)>::value, "second is not const");
            ASSERT_LE(0, i->first);
            ASSERT_GE(4, i->first);
            found[i->first] = '1';
        }
    }
    {
        char found[] = "00000";
        for(mfhashmapsc<int, object>::const_iterator i = m10.begin(), e = m10.end(); i != e; i++)
        {
//          std::cout << "[" << i->first << "]: " << i->second << std::endl;
            static_assert(std::is_const<typeof(i->second)>::value, "second is not const");
            ASSERT_LE(0, i->first);
            ASSERT_GE(4, i->first);
            found[i->first] = '1';
        }
    }
}

TEST_F(HashmapTest, OperatorBrackets)
{
	m10.insert(1, object("A"));
	m10.insert(2, object("B"));
	m10.insert(3, object("C"));
	m10.insert(4, object("D"));
    
    object& a = m10[1];
    EXPECT_EQ(object("A"), a);

    object const& ac = m10[1];
    EXPECT_EQ(object("A"), ac);

    EXPECT_EQ(object("B"), m10[2]);
    EXPECT_EQ(object("C"), m10[3]);
    EXPECT_EQ(object("D"), m10[4]);

    EXPECT_EQ(m10.none(), m10[5]);
    EXPECT_EQ(m10.none(), m10[6]);
    EXPECT_EQ(m10.none(), m10[-1]);
}

TEST_F(HashmapTest, Swap)
{
    swap(m1, m10);

//    EXPECT_EQ(0, m10.size());
//    EXPECT_EQ(1, m10.capacity());
    
//    EXPECT_EQ(0, m1.size());
//    EXPECT_EQ(10, m1.capacity());
}
