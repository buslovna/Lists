
#ifndef LIST_H_
#define LIST_H_
#include <iostream>
#include <ostream>

template<typename DataE>
class List
{
public:


	virtual void insert(DataE value, int position) = 0;
	virtual void push_back(DataE value) = 0;
	virtual void push_front(DataE value) = 0;
	virtual DataE replace( DataE value, int position ) = 0; //replaces the existing element at
																										 //the specified position with the specified element and return the original element.
  virtual DataE remove(int position) = 0;
	virtual void pop_back() = 0;
	virtual void pop_front() = 0;
	virtual DataE item_at(int position) = 0;
	virtual DataE peek_front() = 0;
	virtual DataE peek_back() = 0;

	virtual bool is_empty() const = 0;
	virtual bool is_full() = 0;
	virtual int length() = 0;
	virtual void clear() = 0;
	virtual bool contains(DataE value, bool(*fp)(DataE,DataE)) = 0;

 	virtual std::ostream& print(std::ostream&) = 0; //takes output stream
	virtual DataE * contents() = 0;

	virtual ~List(){};

};

#endif
