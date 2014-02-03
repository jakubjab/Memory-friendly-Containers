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



#ifndef memoryfriendlycontainers_object_h
#define memoryfriendlycontainers_object_h

#include <iostream>
#include <ostream>

struct object;
std::ostream& operator<<(std::ostream& o, const object& x);

struct object
{
	std::string name;
    
	object(const char *n = "unnamed")
	{
		init(n);
		std::cout << *this << ": constructor" << std::endl;
	}
	object(const object& org)
	{
		name = org.name;
		std::cout << (*this) << ": copy constructor from " << org << std::endl;
	}
	object(object&& org)
	{
		init();
		swap(org);
		std::cout << (*this) << ": move constructor from " << org << std::endl;
	}
    
	object& operator=(const object& org)
	{
		object o(org);
		swap(o);
		std::cout << (*this) << ": copy assignment operator from " << org
        << std::endl;
		return *this;
	}
	object& operator=(object&& org)
	{
		swap(org);
		std::cout << (*this) << ": move assignment operator from " << org << std::endl;
		return *this;
	}
    
	~object()
	{
		destroy();
		std::cout << (*this) << ": destructor" << std::endl;
	}
    
	void init(const char *n = "unnamed")
	{
		name = n;
		std::cout << (*this) << ": init" << std::endl;
	}
	void destroy()
	{
		std::cout << (*this) << ": destroy" << std::endl;
	}
	void swap(object& o)
	{
		name.swap(o.name);
		std::cout << (*this) << ": swap" << std::endl;
	}
};


#endif
