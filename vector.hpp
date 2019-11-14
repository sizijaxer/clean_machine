///begin()
///end()
///push_back()
///size()
///empty()
///clear()

#include <stdlib.h>


template<class T>
class vector {
public:
	typedef size_t size_type;
	typedef T value_type;
	typedef T* iterator;
	typedef const T* const_iterator;

	vector();
	vector(size_type n, const value_type& val = value_type());
	vector(const vector& x);
	~vector();

	void reserve(size_type n);
	size_type size() const;
	bool empty() const;

	value_type& operator [] (size_type n);
	const value_type& operator [] (size_type n) const;
	vector& operator = (const vector& x);

	void clear();
	value_type& front();
	const value_type& front() const;

	void push_back(const value_type& val);

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

private:
	size_type size_ = 0;
	size_type capacity_ = 0; // 2^n
	value_type* buffer_ = nullptr;

}; // class vector

template<class T> inline
vector<T>::vector() : size_(0), capacity_(0), buffer_(nullptr){}

template<class T> inline
vector<T>::vector(size_type n, const value_type& val)
{
	reserve(n);

	size_ = n;
	for (size_t i = 0; i < n; ++i)
		buffer_[i] = val;
}

template<class T> inline
vector<T>::vector(const vector& x)
{
	reserve(x.size_);

	size_ = x.size_;
	for (size_t i = 0; i < x.size_; ++i)
		buffer_[i] = x.buffer_[i];
}

template<class T> inline
vector<T>::~vector()
{
	if (buffer_)
		delete [] buffer_;
}

template<class T> inline
void vector<T>::reserve(size_type n)
{
	if (capacity_ < n) {
		delete [] buffer_;

		capacity_ = 1;
		if (capacity_ < n) {
			capacity_ = 2;
			while (capacity_ < n) {
				capacity_ *= 2;
			}
		}
		buffer_ = new value_type[capacity_];
	}
}

template<class T> inline
size_t vector<T>::size() const
{
	return size_;
}

template<class T> inline
bool vector<T>::empty() const
{
	return size_ == 0 ? true : false;
}

template<class T> inline
T& vector<T>::operator [] (size_type n)
{
	if (size_ <= n) abort();
	return buffer_[n];
}

template<class T> inline
const T& vector<T>::operator [] (size_type n) const
{
	if (size_ <= n) abort();
	return buffer_[n];
}

template<class T> inline
vector<T>& vector<T>::operator = (const vector& x)
{
	reserve(x.size_);

	size_ = x.size_;
	for (size_t i = 0; i < size_; ++i)
		buffer_[i] = x.buffer_[i];

	return *this;
}

template<class T> inline
void vector<T>::clear()
{
	size_ = 0;
}

template<class T> inline
void vector<T>::push_back(const value_type& val)
{
	if (size_ >= capacity_) {
		T* tmp = new T[size_];
		for (size_t i = 0; i < size_; ++i)
			tmp[i] = buffer_[i];

		reserve(size_+1);

		for (size_t i = 0; i < size_; ++i)
			buffer_[i] = tmp[i];

		delete [] tmp;
	}

	buffer_[size_] = val;
	size_ += 1;
}

template<class T> inline
T* vector<T>::begin()
{
	return buffer_;
}

template<class T> inline
const T* vector<T>::begin() const
{
	return buffer_;
}

template<class T> inline
T* vector<T>::end()
{
	return buffer_ + size_;
}

template<class T> inline
const T* vector<T>::end() const
{
	return buffer_ + size_;
}
