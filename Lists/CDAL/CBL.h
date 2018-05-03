namespace cop3530{

#ifndef _CBL_H_
#define _CBL_H_

#include "List.h"
#include <stdexcept>


template <typename E>
class CBL : public List<E> {
public:
	typedef bool(*comp_func)(E, E);
	CBL(size_t capacity) :size(0), capacity(capacity), original_capacity(capacity), data(new E[capacity]()) {}
	CBL() { CBL(50); }
	CBL(const CBL & orig);

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
	CBL& operator=(const CBL &orig);

	~CBL()
	{
		delete[] data;
	}

private:
	E * data;
	size_t original_capacity;
	size_t capacity;
	size_t size;
  E head = 0;
	E tail = 0;

	E next_index(E );
  E prev_index(E );
	void shift_right(E, E);
	void shift_left(E, E);
	bool check_cap() { return (capacity > 2 * original_capacity && size < capacity * .5) ? true : false; }


public:
	template <typename DataE>
	class CBL_Iter
	{
	public:
		// type aliases required for C++ iterator compatibility
		using value_type = E;
		using reference = E&;
		using pointer = E*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
		// type aliases for prettier code
		using self_type = CBL_Iter;
		using self_reference = CBL_Iter&;
		friend class CBL<DataE>;

		//factories to make iterators
	private:
		E* here;

	public:
		explicit CBL_Iter(E* start = nullptr) :here(start) {}
		CBL_Iter(const CBL_Iter& src) : here(src.here) {}

		static self_type make_begin(E * node) {
			CBL_Iter i(node);
			return i;
		}
		static self_type make_end(E * node) {
			CBL_Iter i(node);
			return i;
		}
		reference operator*() const {
			if (!here)
				throw new std::runtime_error("Iterator exhausted: no such point");

			return here->data;
		}
		pointer operator->() const {
			if (!here)
				throw new std::runtime_error("Iterator exhausted: no such point");

			return &(here->data);
		}

		self_reference operator=(CBL_Iter<E> const& src) {
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

		bool operator==(CBL_Iter<E> const& rhs) const {
			return (&rhs.here == here);
		}
		bool operator!=(CBL_Iter<E> const& rhs) const {
			return !(&rhs.here == here);
		}
	};

public:
	using size_t = std::size_t;
	using value_type = E;
	using iterator = CBL_Iter<E>;
	using const_iterator = CBL_Iter<E>;

	iterator begin() { return iterator::make_begin(this->data); }
	iterator end() { return iterator::make_end(this->tail); }

	const_iterator begin() const { return const_iterator::make_begin(*this); }
	const_iterator end() const { return const_iterator::make_end(*this); }


};
/////////////copy constructor////////////////////
////needs work///////////////////
template <typename E>
CBL<E>::CBL(const CBL & orig) {
	if (orig.data == nullptr)
		data = head = tail = 0;
	else {
		size = orig.size;
		capacity = orig.capacity;
    original_capacity = orig.original_capacity;
		data = new E[capacity];
		tail = 0;
		for (int i = 0; i < size; ++i){
			data[orig.head] = orig.data[orig.head];
      orig.head = next_index(orig.head);
		}
	}
}
//////////////copy assignment//////////////////////
template <typename E>
CBL<E>&
CBL<E>::operator=(const CBL &orig) {
	CBL<E> temp(orig);
	std::swap(temp.data, data);
  std::swap(temp.head, head);
	std::swap(temp.tail, tail);
	std::swap(temp.size, size);
	std::swap(temp.capacity, capacity);
	return *this;
}
//////////////////////shift_left///////////////////////
template <typename E>
void
CBL<E>::shift_left(E head, E tail) {
	for (int i = 0; i < size; ++i) {
		data[prev_index(head)] = data[head];
    head = next_index(head);
	}
}

//////////////////////shift_right///////////////////////
template <typename E>
void
CBL<E>::shift_right(E head, E _tail) {
	//std::cout << "head - tail is:" << tail - head << std::endl;
	for (int i = 0; i < size; ++i) {
    data[_tail] = data[prev_index(_tail)];
    _tail = prev_index(_tail);
  }

}
//////////////////////next_index///////////////////////
template <typename E>
E
CBL<E>::next_index(E index) {
	if (++index == capacity)
		index = 0;
	return index;
}
//////////////////////prev_index///////////////////////
template <typename E>
E
CBL<E>::prev_index(E index) {
	if (--index == -1)
		index = capacity-1;
	return index;
}
//////////////////////remove///////////////////////
//////////NEEDS WORKS///////////////////////////////
template <typename E>
E
CBL<E>::remove(int position) {
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
			E n = head;
			while (position-- > 0) {
				n = next_index(n);
			}
			//shift all elements to the left
			shift_left(n, tail);
			--tail;
			--size;
		}
	}
}

/////////////////////contents//////////////////////
template <typename E>
E *
CBL<E>::contents() {
	if (!is_empty()) {
		int l = head;
		E * arr = new E[l];
		E * copy = arr;
		E * n = data;

		for(int i = 0; i < size; ++i) {
			*copy++ = data[l];
      l = next_index(l);
		}
		return arr;
	}
	else
		throw std::runtime_error("Error in contents()");

}
/////////////////////print/////////////////////////
template <typename E>
std::ostream&
CBL<E>::print(std::ostream& output) {
	if (is_empty())
		output << "<empty list>";
	else {
		E n = head;
		output << "[";
		while (n != tail) {
			if (n == head)
				output << data[n];
			else
				output << "-" << data[n];
			n = next_index(n);
		}
		output << "]";
	}
	return output;
}
////////////////////contains////////////////////////
template <typename E>
bool
CBL<E>::contains(E value, bool(*fp)(E, E)) {
	if (is_empty())
		throw std::runtime_error("Error in contains())");
	else {
		E n = head;
		while (n != tail) {
			if (data[n] == value)
				return true;
      n = next_index(n);
		}
		return false;
	}
}

///////////////////peek_front///////////////////////
template <typename E>
E
CBL<E>::peek_front() {
	if (is_empty())
		throw std::runtime_error("Error in peek_front()! List is currently empty");
	return data[head];

}
template <typename E>
E
CBL<E>::peek_back() {
	if (is_empty())
		throw std::runtime_error("Error in peek_back()! List is currently empty");
	return data[prev_index(tail)];

}
///////////////////item_at///////////////////////
template <typename E>
E
CBL<E>::item_at(int position) {
	if (position > length())
		throw std::runtime_error("Error in item_at(position!)");
	else {
    int i = head;
    for(int j = 0; j < position; ++j)
      i = next_index(i);
		return(data[i]);
	}
}
/////////////////////pop_back////////////////////
template <typename E>
void
CBL<E>::pop_back() {
	if (is_empty()) {
		throw std::runtime_error("Error in pop_back(value, position!)");
	}
	else {
		tail = prev_index(tail);
		--size;
	}
}
/////////////////////pop_front////////////////////
template <typename E>
void
CBL<E>::pop_front() {
	if (is_empty()) {
		throw std::runtime_error("Error in pop_front(value, position!)");
	}
	else {
    head = next_index(head);
    --size;
	}
}
///////////////////////repalce///////////////////
template <typename E>
E
CBL<E>::replace(E value, int position) {
	if (position > length())
		throw std::runtime_error("Error in replace(value, position!)");
	else {
    int i = head;
    int _data;
    for(int j = 0; j < position; ++j)
      i = next_index(i);
		_data = data[i];
    data[i] = value;
	}
}

///////////////////////realloc///////////////////
template <typename E>
void
CBL<E>::realloc(double val) {
	size_t new_cap = size_t(capacity*(val));

	E * new_arr = new E[new_cap]();

	for (int i = head; i != tail; i = next_index(i)){
		new_arr[i] = data[i];
		tail = next_index(tail);
	}
	delete[] data;
	data = new_arr;
	capacity = new_cap;
}
///////////////////////push front///////////////////
template <typename E>
void
CBL<E>::push_front(E value) {
	if (is_empty()) {
		data[head] = value;
		tail = next_index(tail);
	}
	else if (is_full()) {
		realloc(1.5);
		head = prev_index(head);
		data[head] = value;
	}
	else {
		//MAKE A FUNCTION FOR THIS STUFF
		if (check_cap()) {
			std::cout << "Changing size smaller" << std::endl << std::endl;
			realloc(.75);
			shift_right(head, tail);
			*data = value;
			tail = next_index(tail);
		}
		else {
			//std::cout << "here3" << std::endl;
			head = prev_index(head);
			data[head] = value;
		}
	}
	++size;
}
///////////////////////push back///////////////////
template <typename E>
void
CBL<E>::push_back(E value) {
	if (check_cap()) {
		std::cout << "Not enough slots used, changing size and reallocating!" << std::endl;
		realloc(.75);
		data[tail] = value;
    tail = next_index(tail);
	}
	else if (is_full()) {
		this->realloc(1.5);
		data[tail] = value;
		tail = next_index(tail);

	}
	else {
		data[tail] = value;
		tail = next_index(tail);
	}
	++size;
}
///////////////////////insert at///////////////////
template <typename E>
void
CBL<E>::insert(E value, int position) {
	if (length() < position)
		throw std::runtime_error("Error in insert(value, position!)");
	else if (position == 0)
		push_front(value);
	else if (position == size)
		push_back(value);
	else {
		E n = head;
		while (position-- > 0) {
			n = next_index(n);
		}
		shift_right(n, tail);
		data[n] = value;
		tail = next_index(tail);
		++size;
	}
}


///////////////////////length//////////////////////
template <typename E>
int
CBL<E>::length() {
	if (is_full())
		return capacity;
	else if (is_empty())
		return 0;
  else if(tail< head)
    return head-tail;
	else return(tail - head);
}
/////////////////clear////////////////////////////////
template <typename E>
void
CBL<E>::clear() {
	head = tail = 0;
}

#endif
}
