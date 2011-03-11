#ifndef __M2033_SHARED_PTR__
#define __M2033_SHARED_PTR__

namespace m2033
{
	template <class T>
	class shared_ptr
	{
	public:
		inline shared_ptr();
		inline shared_ptr( const shared_ptr<T> &p );
		template <class A> explicit inline shared_ptr( A *ptr );
		virtual ~shared_ptr();

		inline shared_ptr<T>& operator = ( const shared_ptr<T> &p );

		inline T* operator*() const;
		inline T* operator->() const;
		inline T* ptr() const;

		inline bool is_null() const;
		inline unsigned ref_count() const;

		inline void release();

	private:

		T			*ptr_;
		unsigned	*ref_count_;
	};

	template <class T> inline shared_ptr<T>::shared_ptr() : ptr_(0), ref_count_(0) {}
	template <class T> inline shared_ptr<T>::shared_ptr( const shared_ptr<T> &p ) { operator=(p); }
	template <class T> template <class A> inline shared_ptr<T>::shared_ptr( A *ptr ) : ptr_(ptr), ref_count_( ptr ? new unsigned : 0 ) {}
	template <class T> shared_ptr<T>::~shared_ptr() { release(); }
	template <class T> inline T* shared_ptr<T>::operator*() const { assert(ptr_); return ptr_; }
	template <class T> inline T* shared_ptr<T>::operator->() const { assert(ptr_); return ptr_; }
	template <class T> inline T* shared_ptr<T>::ptr() const { return ptr_; }
	template <class T> inline bool shared_ptr<T>::is_null() const { return ptr_ == 0; }

	template <class T> inline shared_ptr<T>& shared_ptr<T>::operator = ( const shared_ptr<T> &p )
	{
		if( ptr_ == p.ptr_ )
			return *this;

		ptr_ = p.ptr_;
		ref_count_ = p.ref_count_;
		if( ref_count_ ) (*ref_count_)++;

		return *this;
	}

	template <class T>
	inline void shared_ptr<T>::release()
	{
		if( ref_count_ != 0 )
		{
			if( --(*ref_count_) == 0 )
			{
				delete ref_count_;
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