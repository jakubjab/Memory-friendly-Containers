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

#include <iostream>
#include <vector>
#include <type_traits>
#include <unordered_map>

#include "mfvector.h"
#include "hashmapsc.h"
#include "object.h"
#include "gtest/gtest.h"
#include "debugallocator.h"

template<typename T>
void type_size_alignment(const char *n)
{
	std::cout << "sizeof/alignment of " << n << ": " << sizeof(T) << " / "
    << std::alignment_of<T>::value << std::endl;
}

int main_evector_test(void);
int main_ehashmap_test(void);


class Arena {
    void* p;
    int s;
public:
    Arena(void* pp, int ss) : p(pp), s(ss)
    {}
};

template<class T> struct My_alloc {
    Arena& a;
    My_alloc(Arena& aa) : a(aa) { }
    // usual allocator stuff
};


int main(int argc, char **argv)
{
//    Arena my_arena1(new char[100000],100000);
//    Arena my_arena2(new char[1000000],1000000);
//    std::vector<int,My_alloc<int>> v1(My_alloc<int>{my_arena1});
    
    
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    
//    main_evector_test();
//	main_ehashmap_test();
    
//    typedef std::unordered_map<int, int, std::hash<int>, std::equal_to<int>, DebugAllocator< std::pair<const int, int>>> mymap;
//    typedef std::hash<int> myhash;
//    typedef std::equal_to<int> myequal_to;
//    mymap m(10, myhash(), myequal_to(), DebugAllocator< std::pair<const int, int>>());
//	return 0;
}
