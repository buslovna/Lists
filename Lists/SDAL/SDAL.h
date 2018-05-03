#ifndef _SDAL_H_
#define _SDAL_H_

#include "List.h"
#include <stdexcept>


template <typename E>
class SDAL : public List<E> {
public:
	typedef bool(*comp_func)(E, E);
	SDAL(size_t capacity) :size(0), capacity(capacity), original_capacity(capacity), data(new E[capacity + 1]()) {}
	SDAL() { SDAL(50); }
	SDAL(const SDAL & orig);

	void realloc(double val);


	void insert(E value, int position) override;
	void push_back(E value) override;
	void push_front(E value) override;
	E replace(E value, int position) override; //replaces the existing element at
											   //the specified position with the specified element and return the original element.
	E remove(int position) override;
	void pop_back() override;
	void pop_front() override;
	E item_at(int position) override;
	E peek_front() override;
	E peek_back() override;

	inline bool is_empty() const { return (size == 0); }
	inline bool is_full() override { return (size >= capacity); }
	int length() override;
	void clear() override;
	bool contains(E value, bool(*fp)(E, E)) override;

	std::ostream& print(std::ostream&) override; //takes output stream
	E * contents() override;
	SDAL& operator=(const SDAL &orig);

	~SDAL()
	{
		delete[] data;
	}

private:
	E * data;
	size_t original_capacity;
	size_t capacity;
	size_t size;
	E * tail = data;

	E * next_index(E *);
	void shift_right(E*, E*);
	void shift_left(E*, E*);
	bool check_cap() { return (capacity > 2 * original_capacity && size < capacity * .5) ? true : false; }


public:
	template <typename DataE>
	class SDAL_Iter
	{
	public:
		// type aliases required for C++ iterator compatibility
		using value_type = E;
		using reference = E&;
		using pointer = E*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		// type aliases for prettier code
		using self_type = SDAL_Iter;
		using self_reference = SDAL_Iter&;
		

		//factories to make iterators
	private:
		E* here;

	public:
		explicit SDAL_Iter(E* start = nullptr) :here(start) {}
		SDAL_Iter(const SDAL_Iter& src) : here(src.here) {}

		static self_type make_begin(E * node) {
			SDAL_Iter i(node);
			return i;
		}
		static self_type make_end(E * node) {
			SDAL_Iter i(node);
			return i;
		}
		reference operator*() const {
			if (!here)
				throw new std::runtime_error("Iterator exhausted: no such point");

			return *here;
		}
		pointer operator->() const {
			if (!here)
				throw new std::runtime_error("Iterator exhausted: no such point");

			return &(here);
		}

		self_reference operator=(SDAL_Iter<E> const& src) {
			if (this == &src)
				return *this;
			here = src.here;

			return *this;

		}

		self_reference operator++() {
			++here;
			return *this;
		} // preincrement

		self_type operator++(int) {
			self_type tmp(*this);
			++(*this);
			return tmp;
		} // postincrement

		bool operator==(SDAL_Iter<E> const& rhs) const {
			return (&rhs.here == here);
		}
		bool operator!=(SDAL_Iter<E> const& rhs) const {
			return !(&rhs.here == here);
		}
	};

public:
	using size_t = std::size_t;
	using value_type = E;
	using iterator = SDAL_Iter<E>;
	using const_iterator = SDAL_Iter<E>;

	iterator begin() { return iterator::make_begin(this->data); }
	iterator end() { return iterator::make_end(this->tail-1); }

	const_iterator begin() const { return const_iterator::make_begin(*this); }
	const_iterator end() const { return const_iterator::make_end(*this); }


};
/////////////copy constructor////////////////////
template <typename E>
SDAL<E>::SDAL(const SDAL & orig) {
	if (orig.data == nullptr)
		data = tail = nullptr;
	else {
		size = orig.size;
		capacity = orig.capacity;
		data = new E[capacity + 1];
		tail = data;
		for (int i = 0; i < orig.size; ++i)
		{
			*tail++ = orig.data[i];
		}
	}
}
//////////////copy assignment//////////////////////
template <typename E>
SDAL<E>&
SDAL<E>::operator=(const SDAL &orig) {
	SDAL<E> temp(orig);
	std::swap(temp.data, data);
	std::swap(temp.tail, tail);
	std::swap(temp.size, size);
	std::swap(temp.capacity, capacity);
	return *this;
}
//////////////////////shift_left///////////////////////
template <typename E>
void
SDAL<E>::shift_left(E* head, E* tail) {
	int count = tail - head;
	for (int i = 0; i != count; ++i) {
		*(head + i + 1) = *(head + i);
	}
	--tail;
}

//////////////////////shift_right///////////////////////
template <typename E>
void
SDAL<E>::shift_right(E* head, E* tail) {
	//std::cout << "head - tail is:" << tail - head << std::endl;
	for (int i = tail - head - 1; i >= 0; --i) {
		*(head + i + 1) = *(head + i);
	}

}
//////////////////////next_index///////////////////////
template <typename E>
E *
SDAL<E>::next_index(E * node) {
	if (++node == (data + capacity + 1))
		node = data;
	return node;
}
//////////////////////remove///////////////////////
//////////NEEDS WORKS///////////////////////////////
template <typename E>
E
SDAL<E>::remove(int position) {
	if (is_empty())
		throw std::runtime_error("Error in remove(position!)");

	else {
		if (length() < position - 1)
			throw std::runtime_error("Error in remove(position!)");
		//find position
		else if (position == 0)
			pop_front();
		else if (position == length() - 1)
			pop_back();
		else {
			E* pos = data;
			while (position > 0) {
				++pos; --position;
			}
			//shift all elements to the left
			shift_left(pos, tail);
			--tail;
			--size;
		}
	}

}

/////////////////////contents//////////////////////
template <typename E>
E *
SDAL<E>::contents() {
	if (!is_empty()) {
		int l = length();
		E * arr = new E[l];
		E * copy = arr;
		E * n = data;

		while (n != tail) {
			*copy++ = *n++;
		}
		return arr;
	}
	else
		throw std::runtime_error("Error in contents()");

}
/////////////////////print/////////////////////////
template <typename E>
std::ostream&
SDAL<E>::print(std::ostream& output) {
	if (is_empty())
		output << "<empty list>";
	else {
		E * n = data;
		output << "[";
		while (n != tail) {
			if (n == data)
				output << *n;
			else
				output << "-" << *n;
			++n;
		}
		output << "]";
	}
	return output;
}
////////////////////contains////////////////////////
template <typename E>
bool
SDAL<E>::contains(E value, bool(*fp)(E, E)) {
	if (is_empty())
		throw std::runtime_error("Error in contains())");
	else {
		E * n = data;
		while (n) {
			if (*n++ = value)
				return true;
		}
		return false;
	}
}

///////////////////peek_front///////////////////////
template <typename E>
E
SDAL<E>::peek_front() {
	if (is_empty())
		throw std::runtime_error("Error in peek_front()! List is currently empty");
	return *data;

}
template <typename E>
E
SDAL<E>::peek_back() {
	if (is_empty())
		throw std::runtime_error("Error in peek_back()! List is currently empty");
	return *(tail - 1);

}
///////////////////item_at///////////////////////
template <typename E>
E
SDAL<E>::item_at(int position) {
	if (position > length())
		throw std::runtime_error("Error in item_at(position!)");
	else {
		return(*(data + position - 1));
	}
}
/////////////////////pop_back////////////////////
template <typename E>
void
SDAL<E>::pop_back() {
	if (is_empty()) {
		throw std::runtime_error("Error in pop_back(value, position!)");
	}
	else {
		--tail;
		--size;
	}
}
/////////////////////pop_front////////////////////
template <typename E>
void
SDAL<E>::pop_front() {
	if (is_empty()) {
		throw std::runtime_error("Error in pop_front(value, position!)");
	}
	else {
		shift_left(data, tail);
		--tail;
		--size;
	}
}
///////////////////////repalce///////////////////
template <typename E>
E
SDAL<E>::replace(E value, int position) {
	if (position > length())
		throw std::runtime_error("Error in replace(value, position!)");
	else {
		E val = *(data + position - 1);
		*(data + position - 1) = value;
		return val;
	}
}

///////////////////////realloc///////////////////
template <typename E>
void
SDAL<E>::realloc(double val) {
	size_t new_cap = size_t(capacity*(val));
	tail = nullptr;
	E * new_arr = new E[new_cap]();

	for (int i = 0; i < size; ++i)
		new_arr[i] = data[i];
	delete[] data;
	data = new_arr;
	tail = data + size;
	capacity = new_cap;
}
///////////////////////push front///////////////////
template <typename E>
void
SDAL<E>::push_front(E value) {
	if (is_empty()) {
		*tail = value;
		tail = next_index(tail);
	}
	else if (is_full()) {
		realloc(1.5);
		*tail = value;
		tail = next_index(tail);
	}
	else {
		//MAKE A FUNCTION FOR THIS STUFF
		if (check_cap()) {
			std::cout << "Changing size smaller" << std::endl << std::endl;
			realloc(.75);
			shift_right(data, tail);
			*data = value;
			tail = next_index(tail);
		}
		else {
			//std::cout << "here3" << std::endl;
			shift_right(data, tail);
			tail = next_index(tail);
			*data = value;
		}
	}
	++size;
}
///////////////////////push back///////////////////
template <typename E>
void
SDAL<E>::push_back(E value) {
	if (check_cap()) {
		std::cout << "Not enough slots used, changing size and reallocating!" << std::endl;
		realloc(.75);
		tail = next_index(tail);
		shift_right(data, tail);
		*tail = value;
	}
	else if (is_full()) {
		this->realloc(1.5);
		*tail = value;
		tail = next_index(tail);

	}
	else {
		*tail = value;
		tail = next_index(tail);
	}
	++size;
}
///////////////////////insert at///////////////////
template <typename E>
void
SDAL<E>::insert(E value, int position) {
	if (length() < position)
		throw std::runtime_error("Error in insert(value, position!)");
	else if (position == 0)
		push_front(value);
	else if (position == size)
		push_back(value);
	else {
		E * n = data;
		while (position-- > 0) {
			++n;
		}
		shift_right(n, tail);
		*n = value;
		tail = next_index(tail);
		++size;
	}
}


///////////////////////length//////////////////////
template <typename E>
int
SDAL<E>::length() {
	if (is_full())
		return capacity;
	else if (is_empty())
		return 0;
	else return(tail - data);
}
/////////////////clear////////////////////////////////
template <typename E>
void
SDAL<E>::clear() {
	std::cout << std::endl << "In clear";
	//for(int i = 0; i < capacity; ++i)

	//data = 0;
}

#endif
