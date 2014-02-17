

#ifndef DEBUGALLOCATOR2_H
#define DEBUGALLOCATOR2_H

#include <iostream>
#include <iomanip>

/* =======================================================
 * = .
 * ======================================================= */
template <typename T>
class DebugAllocator2
{
public:
    
    // type definitions
    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;
    
    // rebind allocator to type U
    template <class U>
    struct rebind
    {
        typedef DebugAllocator2<U> other;
    };
    
    // return address of values
    pointer address (reference value) const
    {
        return &value;
    }
    const_pointer address (const_reference value) const
    {
        return &value;
    }
    
    /* constructors and destructor
     * - nothing to do because the allocator has no state
     */
    DebugAllocator2() throw()
    {
    }
    DebugAllocator2(const DebugAllocator2&) throw()
    {
    }
    
    template <class U>
    DebugAllocator2 (const DebugAllocator2<U>&) throw()
    {
    }
    ~DebugAllocator2() throw()
    {
    }
    
    // return maximum number of elements that can be allocated
    size_type max_size () const throw()
    {
        return 1000;
    }
    
    // allocate but don't initialize num elements of type T
    pointer allocate (size_type num, const void* = 0)
    {
        // print message and allocate memory with global new
        pointer ret = (pointer)(::operator new(num*sizeof(T)));
        std::cerr << "DebugAllocator2 at " << this << ": allocated " << num << " element(s)" << " of size " << sizeof(T) << " at " << (void*)ret << std::endl;
        return ret;
    }
    
    // initialize elements of allocated storage p with value value
    void construct (pointer p, const T& value)
    {
        // initialize memory with placement new
        new((void*)p)T(value);
    }
    
    // destroy elements of initialized storage p
    void destroy (pointer p)
    {
        // destroy objects by calling their destructor
        p->~T();
    }
    
    // deallocate storage p of deleted elements
    void deallocate (pointer p, size_type num)
    {
        // print message and deallocate memory with global delete
        std::cerr << "DebugAllocator2 at " << this << ": deallocate " << num << " element(s)" << " of size " << sizeof(T) << " at " << (void*)p << std::endl;
        ::operator delete((void*)p);
    }
};

template< class T1, class T2 >
bool operator==(const DebugAllocator2<T1>& lhs, const DebugAllocator2<T2>& rhs)
{
    return (&lhs == &rhs);
}

template< class T1, class T2 >
bool operator!=(const DebugAllocator2<T1>& lhs, const DebugAllocator2<T2>& rhs)
{
    return (&lhs != &rhs);
}


#endif
