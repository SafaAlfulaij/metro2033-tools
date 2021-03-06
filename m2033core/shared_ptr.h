/******************************************************************************
This source file is part of metro2033-tools
For the latest info, see http://code.google.com/p/metro2033-tools/

Copyright (c) 2010-2011 Ivan Shishkin <codingdude@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

#ifndef __M2033_SHARED_PTR__
#define __M2033_SHARED_PTR__

namespace m2033
{
	template <class T>
	class shared_ptr
	{
	public:
		typedef void (*delete_func)(T *ptr);

		inline shared_ptr();
		inline shared_ptr( const shared_ptr<T> &p );
		template <class A> explicit inline shared_ptr( A *ptr, delete_func deleter = 0 );
		virtual ~shared_ptr();

		inline shared_ptr<T>& operator = ( const shared_ptr<T> &p );

		inline T* operator*() const;
		inline T* operator->() const;
		inline T* ptr() const;

		inline bool is_null() const;
		inline unsigned ref_count() const;

		inline void release();

	private:

		T				*ptr_;
		unsigned		*ref_count_;

		delete_func		m_deleter;
	};

	template <class T> inline shared_ptr<T>::shared_ptr() : ptr_(0), ref_count_(0), m_deleter(0) {}
	template <class T> inline shared_ptr<T>::shared_ptr( const shared_ptr<T> &p ) : ptr_(0), ref_count_(0), m_deleter(0) { operator=(p); }
	template <class T> template <class A> inline shared_ptr<T>::shared_ptr( A *ptr, delete_func deleter) : ptr_(ptr), ref_count_( ptr ? new unsigned : 0 ), m_deleter(deleter) { if(ref_count_ != 0) *ref_count_ = 0; }
	template <class T> shared_ptr<T>::~shared_ptr() { release(); }
	template <class T> inline T* shared_ptr<T>::operator*() const { assert(ptr_); return ptr_; }
	template <class T> inline T* shared_ptr<T>::operator->() const { assert(ptr_); return ptr_; }
	template <class T> inline T* shared_ptr<T>::ptr() const { return ptr_; }
	template <class T> inline bool shared_ptr<T>::is_null() const { return ptr_ == 0; }

	template <class T> inline shared_ptr<T>& shared_ptr<T>::operator = ( const shared_ptr<T> &p )
	{
		if( ptr_ == p.ptr_ )
			return *this;

		release();

		ptr_ = p.ptr_;
		ref_count_ = p.ref_count_;
		m_deleter = p.m_deleter;
		if( ref_count_ ) (*ref_count_)++;

		return *this;
	}

	template <class T>
	inline void shared_ptr<T>::release()
	{
		if( ref_count_ != 0 )
		{
			if( (*ref_count_)-- == 0 ) {
				delete ref_count_;
				if( m_deleter != 0 )
					m_deleter(ptr_);
				else
					delete ptr_;
			}

			ref_count_ = 0;
			ptr_ = 0;
		}
	}

	template <class T>
	inline unsigned shared_ptr<T>::ref_count() const
	{
		if( ref_count_ )
			return *ref_count_;

		return 0;
	}
}

#endif // __M2033_SHARED_PTR__