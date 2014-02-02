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


#ifndef memoryfriendlycontainers_mfvector_h
#define memoryfriendlycontainers_mfvector_h

#include <algorithm>
#include <cstddef>
//#include <purify.h>
#include <type_traits>

#include <ostream>

template<typename T> class evector;
template<typename T> std::ostream& operator<<(std::ostream&,
                                              const evector<T>& v);

template<typename T>
class evector {
	friend std::ostream& operator<<<T> (std::ostream& o, const evector<T>& v);
    
	typedef typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type uninitialized_T;
public:
	explicit evector(size_t capacity = 0)
	{
		init(capacity);
		start_watch();
		std::cout << this << ": constructor" << std::endl;
	}
    
	evector(const evector& org)
	{
		if (&org != this) {
			size_ = org.size_;
			capacity_ = org.capacity_;
			if (capacity_) {
				data_ = (T*) (new uninitialized_T[capacity_]);
				std::uninitialized_copy_n(org.data_, size_, data_);
			} else {
				data_ = 0;
			}
			start_watch();
		}
		std::cout << this << ": copy constructor from " << (&org) << std::endl;
	}
    
	evector(evector&& org)
	{
		init();
		swap_unwatched(org);
		start_watch();
		std::cout << this << ": move constructor from " << (&org) << std::endl;
	}
    
	evector& operator=(const evector& org)
	{
		stop_watch();
		evector tmp(org);
		swap_unwatched(tmp);
		std::cout << this << ": copy assignment" << std::endl;
		start_watch();
		return *this;
	}
    
	evector& operator=(evector&& org)
	{
		stop_watch();
		swap_unwatched(org);
		std::cout << this << ": move assignment from " << (&org) << std::endl;
		start_watch();
		return *this;
	}
    
	~evector()
	{
		std::cout << this << ": destructor" << std::endl;
		stop_watch();
		destroy();
	}
    
	std::size_t capacity() const
	{
		return capacity_;
	}
    
	std::size_t size() const
	{
		return size_;
	}
    
	const T* begin() const
	{
		return data_;
	}
    
	T* begin()
	{
		return data_;
	}
    
	const T* end() const
	{
		return data_ + size_;
	}
    
	T* end()
	{
		return data_ + size_;
	}
    
	void push_back(const T& x)
	{
		std::cout << "evector::push_back(const T& x)\n";
		if (size_ < capacity_)
		{
			stop_watch();
			std::cout << "evector::push_back(const T& x), adding at " << &data_[size_] << "-----------------------\n";
			new (&data_[size_++]) T(x);
			start_watch();
		}
	}
    
	void push_back(T&& x)
	{
		std::cout << "evector::push_back(T&& x)\n";
		if (size_ < capacity_)
		{
			stop_watch();
			std::cout << "evector::push_back(T&& x), adding at " << &data_[size_] << "-----------------------\n";
			new (&data_[size_++]) T(std::move(x));
			start_watch();
		}
	}
    
	T& operator[](size_t n)
	{
		return data_[n];
	}
    
	const T& operator[](size_t n) const
	{
		return data_[n];
	}
    
	void swap(evector<T>& v)
	{
		stop_watch();
		v.stop_watch();
		swap_unwatched(v);
		start_watch();
		v.start_watch();
	}
    
	void clear()
	{
		stop_watch();
		clear_unwatched();
		start_watch();
	}
    
	T* erase(T const* position)
	{
		return erase(position, position + 1);
	}
    
	T* erase(T const* first, T const* last)
	{
		stop_watch();
		//std::move(last, end(), first);
		T * i = const_cast<T*>(first);
		T * j = const_cast<T*>(last);
		T * e = const_cast<T*>(end());
		while (j != end())
		{
			*i++ = std::move(*j++);
		}
		while (i != e)
		{
			i->~T();
			++i;
			--size_;
		}
		start_watch();
		return const_cast<T*>(first);
	}
    
public:
	T* data_;
	std::size_t capacity_;
	std::size_t size_;
	int watch;
    
	void swap_unwatched(evector<T>& v)
	{
		std::swap(capacity_, v.capacity_);
		std::swap(size_, v.size_);
		std::swap(data_, v.data_);
		std::swap(watch, v.watch);
	}
    
	void clear_unwatched()
	{
		while (size_--)
		{
			data_[size_].~T();
		}
	}
    
	void start_watch()
	{
		if (size_ < capacity_)
		{
			watch = purify_watch_n((char *) &data_[size_], (capacity_ - size_) * sizeof(T), (char *) "rw");
		}
		else
		{
			watch = -1;
		}
	}
	void stop_watch()
	{
		if (watch != -1)
		{
			purify_watch_remove(watch);
		}
	}
    
	void init(size_t capacity = 0)
	{
		capacity_ = capacity;
		size_ = 0;
		if (capacity)
		{
			data_ = (T*) (new uninitialized_T[capacity]);
		}
		else
		{
			data_ = 0;
		}
	}
	void destroy()
	{
		clear_unwatched();
		delete[] ((uninitialized_T *) data_);
	}
};

template<typename T>
void swap(evector<T>& a, evector<T>& b)
{
	a.swap(b);
}

template<typename T>
std::ostream& operator<<(std::ostream& o, const evector<T>& v)
{
	o << std::dec << "evector at " << std::hex << (void *) &v << std::dec
    << "(size " << v.size_ << ", capacity " << v.capacity_ << ", data "
    << std::hex << v.data_ << " [" << std::dec;
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(o, ", "));
	o << "]," << " watch " << v.watch << ")";
    
	return o;
}


#endif
