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

#ifndef memoryfriendlycontainers_mfhashmapsc_h
#define memoryfriendlycontainers_mfhashmapsc_h


#include <algorithm>
#include <cstddef>
#include <functional>
#include "purify.h"
#include <type_traits>
#include <iterator>
#include <ostream>

template<typename K, typename V> class mfhashmapsc;
template<typename K, typename V> std::ostream& operator<<(std::ostream&, const mfhashmapsc<K, V>& v);

template<typename V>
struct mfhash
{
	std::size_t mask;
    
	std::size_t operator()(V key)
	{
		return std::hash<V>(key) & mask;
	}
};

template<>
struct mfhash<int>
{
	std::size_t mask;
    
	std::size_t operator()(int key)
	{
//		return key & mask;
        
		std::size_t h = 0;
        
		h += key, h += (h << 10), h ^= (h >> 6);
        
		h += (h << 3);
		h ^= (h >> 11);
		h += (h << 15);
        
		return h & mask;
	}
};


/**
 * Hash table using separate chaining with linked lists.
 *
 * capacity_ == 6
 * hash(A) == 0
 * hash(B) == 1
 * hash(C) == 2
 * hash(D) == 2
 *
 *              ------ ------ ------
 * buckets_ -> | 0 E0 | 1 E2 | 2 E4 |
 *              ------ ------ ------
 *                 |     |     |
 *                 |     |      -----------------------
 *                 |     |                             |
 *                 |      -----------                  |
 *                 |                 |                 |
 *                 v                 v                 v
 *                 -------- -------- -------- -------- -------- --------
 * entries_ ----> | A null |     E3 | B null |   null | C   E5 | D null |
 *                 -------- -------- -------- -------- -------- --------
 *                           ^    \            ^             \   ^
 *                           |     \          /               \ /
 *                          /       ---------
 * free_entries_ -----------
 */
template<typename K, typename V>
class mfhashmapsc
{
	friend std::ostream& operator<<<K, V> (std::ostream& o, const mfhashmapsc<K, V>& v);
    
public:
	typedef K key_type;
	typedef std::pair<const K, V> value_type;
	typedef std::pair<const K, const V> const_value_type;
	typedef V mapped_type;
	typedef std::size_t size_type;

private:
	struct entry_t
	{
		value_type value;
		entry_t* next_entry;
        
		entry_t(const K& key, const V& v, entry_t* next_entry) : value(key, v), next_entry(next_entry)
		{}
	};
    
	struct bucket_t
	{
		entry_t* first_entry;
        
		bucket_t() : first_entry(nullptr)
		{}
	};
    
    template<bool is_const_iterator>
	struct iter : public std::iterator<std::forward_iterator_tag, value_type>
	{
        typedef typename std::conditional<is_const_iterator, value_type const*, value_type*>::type pointer;
        typedef typename std::conditional<is_const_iterator, value_type const&, value_type&>::type reference;
        
        iter(mfhashmapsc* map, std::size_t bucketIx, entry_t* entry) : map(map), bucketIx(bucketIx), entry(entry) {}
        iter(const iter<false>& other) : map(other.map), bucketIx(other.bucketIx), entry(other.entry) {}
        
		pointer operator->() const
		{
			return &entry->value;
		}
        
		reference operator*() const
		{
			return entry->value;
		}
        
		iter& operator++()
		{
			if (entry->next_entry)
			{
				entry = entry->next_entry;
			}
			else
			{
				while (bucketIx < map->hashsize_ && !(entry = map->buckets_[++bucketIx].first_entry))
				{
				}
                if (bucketIx == map->hashsize_)
                {
                    bucketIx = 0;
                    entry = nullptr;
                }
			}
            
			return *this;
		}
		iter operator++(int)
		{
            iter org(*this);
            operator++();
			return org;
		}
        
		mfhashmapsc* map;
		std::size_t bucketIx;
		entry_t* entry;
        
        friend std::ostream& operator<<(std::ostream& o, const iter& i)
        {
            o << "mfhashmapsc iterator at " << std::hex << (void *) &i << std::dec << "\n";
            return o;
        }
        
        friend bool operator==(const iter& lhs, const iter& rhs)
        {
            return (lhs.map == rhs.map && lhs.bucketIx == rhs.bucketIx && lhs.entry == rhs.entry);
        }
        
        friend bool operator!=(const iter& lhs, const iter& rhs)
        {
            return !(lhs == rhs);
        }
	};
    
public:
	typedef iter<false> iterator;
	typedef iter<true> const_iterator;

private:
	typedef typename std::aligned_storage<sizeof(entry_t), std::alignment_of<entry_t>::value>::type uninitialized_entry;
	typedef typename std::aligned_storage<sizeof(bucket_t), std::alignment_of<bucket_t>::value>::type uninitialized_bucket;
    
    /** Value returned from different functions in case of error. */
	V none_;
    
public:
    explicit mfhashmapsc(size_t capacity = 0)
	{
		init(capacity);
//		std::cout << this << ": constructor" << std::endl;
	}
    
	mfhashmapsc(const mfhashmapsc& org)
	{
		if (&org != this)
		{
			size_ = org.size_;
			capacity_ = org.capacity_;
		}
//		std::cout << this << ": copy constructor from " << (&org) << std::endl;
	}
    
	mfhashmapsc(mfhashmapsc&& org)
	{
		init();
		swap(org);
//		std::cout << this << ": move constructor from " << (&org) << std::endl;
	}
    
	mfhashmapsc& operator=(const mfhashmapsc& org)
	{
		mfhashmapsc tmp(org);
		swap(tmp);
//		std::cout << this << ": copy assignment" << std::endl;
		return *this;
	}
    
	mfhashmapsc& operator=(mfhashmapsc&& org)
	{
		swap(org);
//		std::cout << this << ": move assignment from " << (&org) << std::endl;
		return *this;
	}
    
	~mfhashmapsc()
	{
//		std::cout << this << ": destructor" << std::endl;
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
    
	V& operator[](const K& key)
	{
		std::size_t keyhash = hash_fn(key);
		for (entry_t* e = buckets_[keyhash].first_entry; e; e = e->next_entry)
		{
			if (e->value.first == key)
			{
				return e->value.second;
			}
		}
		return none_;
	}
    
	V const& operator[](const K& key) const
	{
		std::size_t keyhash = hash_fn(key);
		for (entry_t* e = buckets_[keyhash].first_entry; e; e = e->next_entry)
		{
			if (e->value.first == key)
			{
				return e->value.second;
			}
		}
		return none_;
	}
    
	void insert(K key, V value)
	{
		std::size_t keyhash = hash_fn(key);
//		std::cout << this << ": insert of key " << key << ", keyhash " <<keyhash << std::endl;
        
		if (free_entries_)
		{
			entry_t* new_entry = free_entries_;
//			std::cout << this << ": new entry " << new_entry << std::endl;
			free_entries_ = free_entries_->next_entry;
			new (new_entry) entry_t(key, value, buckets_[keyhash].first_entry);
			buckets_[keyhash].first_entry = new_entry;
			size_++;
		}
	}
    
	iterator begin()
	{
        if (buckets_)
        {
            for (std::size_t i = 0; i < hashsize_; ++i)
            {
                if (buckets_[i].first_entry)
                {
                    return iterator(this, i, buckets_[i].first_entry);
                }
            }
        }
        return iterator(this, 0, nullptr);
	}
    const_iterator begin() const
    {
        return iterator(this, 0, nullptr);
    }
    const_iterator cbegin() const
    {
        return iterator(this, 0, nullptr);
    }
    
	iterator end()
	{
        return iterator(this, 0, nullptr);
	}
    const_iterator end() const
    {
        return iterator(this, 0, nullptr);
    }
    const_iterator cend() const
    {
        return iterator(this, 0, nullptr);
    }
    
	void swap(mfhashmapsc<K, V>& v)
	{
	}
    
    V const& none() const
    {
        return none_;
    }
    
private:
	entry_t* entries_;
	entry_t* free_entries_;
	bucket_t* buckets_;
	std::size_t capacity_;
	std::size_t hashsize_;
	std::size_t hashmask_;
	std::size_t size_;
	mfhash<K> hash_fn;
    
	void init(size_t capacity = 0)
	{
		capacity_ = capacity;
		size_ = 0;
        
		hashsize_ = 16; // Smallest size of hash table.
		while (hashsize_ <= capacity_)
		{
			hashsize_ <<= 1;
		}
		hashsize_ >>= 1; // Half size is enough.
		hash_fn.mask = hashsize_ - 1;
        
		if (capacity)
		{
			entries_ = (entry_t *)new uninitialized_entry[capacity];
			buckets_ = new bucket_t[hashsize_];
			free_entries_ = entries_;
			for (std::size_t i = 0; i < capacity - 1; ++i)
			{
				entries_[i].next_entry = &entries_[i + 1];
			}
			entries_[capacity - 1].next_entry = 0;
		}
		else
		{
			entries_ = 0;
			buckets_ = 0;
		}
	}
    
	void destroy()
	{
        if (buckets_)
        {
            for (std::size_t i = 0; i < hashsize_; ++i)
            {
                for (typename mfhashmapsc<K, V>::entry_t* e = buckets_[i].first_entry; e; e = e->next_entry)
                {
                    e->value.~value_type();
                }
            }
            delete [] buckets_;
        }
		delete [] (uninitialized_entry *)entries_;
	}
};

template<typename K, typename V>
void swap(mfhashmapsc<K, V>& a, mfhashmapsc<K, V>& b)
{
	a.swap(b);
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& o, const mfhashmapsc<K, V>& v)
{
	o << "mfhashmapsc at " << std::hex << (void *) &v << std::dec << "(size "
    << v.size_ << ", capacity " << v.capacity_ << ", hashsize "
    << v.hashsize_ << ", mask " << v.hash_fn.mask << ")\n";
    
	for (std::size_t i = 0; i < v.hashsize_; ++i)
	{
		o << " bucket[" << i << "] entries:\n";
		for (typename mfhashmapsc<K, V>::entry_t* e = v.buckets_[i].first_entry; e; e
             = e->next_entry)
		{
			o << "    " << e << ", key " << e->value.first << ", value " << e->value.second
            << "\n";
		}
	}
    
	o << " free entries:";
	for (typename mfhashmapsc<K, V>::entry_t* e = v.free_entries_; e; e
         = e->next_entry)
	{
		o << " " << e;
	}
	o << std::endl;
    
	return o;
}


#endif
