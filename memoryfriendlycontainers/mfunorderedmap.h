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

#ifndef memoryfriendlycontainers_mfunorderedmap_h
#define memoryfriendlycontainers_mfunorderedmap_h


#include <algorithm>
#include <cstddef>
#include <functional>
//#include <purify.h>
#include <type_traits>

#include <ostream>

template<typename K, typename T> class ehashmap;
template<typename K, typename T> std::ostream& operator<<(std::ostream&, const ehashmap<K, T>& v);

template<typename T>
struct ehash
{
	std::size_t mask;
    
	std::size_t operator()(T key)
	{
		return std::hash<T>(key) & mask;
	}
};

template<>
struct ehash<int>
{
	std::size_t mask;
    
	std::size_t operator()(int key)
	{
		// This is an extremely efficient and fast hash function.
		std::size_t h = 0;
        
		h += key, h += (h << 10), h ^= (h >> 6);
        
		h += (h << 3);
		h ^= (h >> 11);
		h += (h << 15);
        
		return h & mask;
	}
};

template<typename K, typename T>
class mfunorderedmap {
	friend std::ostream& operator<<<K, T> (std::ostream& o, const ehashmap<K, T>& v);
    
	typedef std::pair<K, T> entry_t;
	typedef std::pair<K, T*> bucket_t;
	typedef typename std::aligned_storage<sizeof(entry_t),
    std::alignment_of<entry_t>::value>::type uninitialized_entry;
	typedef typename std::aligned_storage<sizeof(bucket_t),
    std::alignment_of<bucket_t>::value>::type uninitialized_bucket;
    
public:
	explicit mfunorderedmap(size_t capacity = 0)
	{
		init(capacity);
		std::cout << this << ": constructor" << std::endl;
	}
    
	mfunorderedmap(const mfunorderedmap& org)
	{
		if (&org != this) {
			size_ = org.size_;
			capacity_ = org.capacity_;
		}
		std::cout << this << ": copy constructor from " << (&org) << std::endl;
	}
    
	mfunorderedmap(mfunorderedmap&& org)
	{
		init();
		swap_unwatched(org);
		std::cout << this << ": move constructor from " << (&org) << std::endl;
	}
    
	mfunorderedmap& operator=(const mfunorderedmap& org)
	{
		mfunorderedmap tmp(org);
		swap_unwatched(tmp);
		std::cout << this << ": copy assignment" << std::endl;
		return *this;
	}
    
	mfunorderedmap& operator=(mfunorderedmap&& org)
	{
		swap_unwatched(org);
		std::cout << this << ": move assignment from " << (&org) << std::endl;
		return *this;
	}
    
	~mfunorderedmap()
	{
		std::cout << this << ": destructor" << std::endl;
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
    
	void insert(K key, T value)
	{
		std::size_t hashkey = hash_fn(key);
		std::cout << this << ": insert of key " << key << ", hashkey " << hashkey << std::endl;
        
        
	}
    
public:
	entry_t* entries_;
	bucket_t* buckets_;
	std::size_t capacity_;
	std::size_t hashsize_;
	std::size_t hashmask_;
	std::size_t size_;
	ehash<K> hash_fn;
    
	void swap_unwatched(ehashmap<K, T>& v)
	{
	}
    
	void clear_unwatched()
	{
	}
    
	void init(size_t capacity = 0)
	{
		capacity_ = capacity;
		size_ = 0;
        
		hashsize_ = 16;    // Smallest size of hash table.
		while (hashsize_ <= capacity_)
			hashsize_ <<= 1;
		hashsize_ >>= 1;   // Half size is enough.
		hash_fn.mask = hashsize_ - 1;
        
		if (capacity)
		{
			entries_ = (entry_t*) (new uninitialized_entry[capacity]);
			buckets_ = (bucket_t*) (new uninitialized_bucket[hashsize_]);
		}
		else
		{
			entries_ = 0;
			buckets_ = 0;
		}
	}
	void destroy()
	{
		clear_unwatched();
	}
};

template<typename K, typename T>
void swap(ehashmap<K, T>& a, ehashmap<K, T>& b)
{
	a.swap(b);
}

template<typename K, typename T>
std::ostream& operator<<(std::ostream& o, const ehashmap<K, T>& v)
{
	o << std::dec << "ehashmap at " << std::hex << (void *) &v << std::dec
    << "(size " << v.size_
    << ", capacity " << v.capacity_
    << ", hashsize " << v.hashsize_
    << ", mask " << v.hash_fn.mask
    << ")";
    
	return o;
}


#endif
