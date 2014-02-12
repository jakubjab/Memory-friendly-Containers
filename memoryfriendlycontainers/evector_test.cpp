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

int main_evector_test()
{
	evector<object> v1;
	assert(v1.size() == 0);
	assert(v1.capacity() == 0);
	assert(v1.begin() == v1.end());

	evector<object> v2(4);
	assert(v2.size() == 0);
	assert(v2.capacity() == 4);
	assert(v2.begin() == v2.end());

	v2.push_back(object("a"));
	assert(v2.size() == 1);
	assert(v2.capacity() == 4);
	assert((v2.begin() + 1) == v2.end());

    v2.push_back(object("b"));
	assert(v2.size() == 2);
	assert(v2.capacity() == 4);
	assert((v2.begin() + 2) == v2.end());

    object* r = v2.erase(v2.begin(), v2.begin() + 2);
    assert(r == v2.begin());
    
	return 0;
}
