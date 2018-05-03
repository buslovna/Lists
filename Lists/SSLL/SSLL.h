#ifndef _SSLL_H_
#define _SSLL_H_

#include "List.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iterator>

template <typename E>
class SSLL : public List<E>{
public:

   typedef bool (*comp_func)(E, E);
   SSLL():head(nullptr), tail(nullptr){comp = compare_function;}
   SSLL(const SSLL & orig);
   void insert(E value, int position) override;
	 void push_back(E value) override;
	 void push_front(E value) override;
	 E replace( E value, int position ) override; //replaces the existing element at
																										 //the specified position with the specified element and return the original element.
   E remove(int position) override;
	 void pop_back() override;
	 void pop_front() override;
	 E item_at(int position) override;
	 E peek_front() override;
	 E peek_back() override;

	 inline bool is_empty() const{ return head == nullptr; }
	 inline bool is_full() override {return false;}
	 int length() override;
	 void clear() override;
	 bool contains(E value, bool(*fp)(E,E)) override;
   bool compare_function(E val1, E val2){return (val1 == val2? true : false);}
 	 std::ostream& print(std::ostream& out) override; //takes output stream
	 E * contents() override;
   SSLL& operator =(const SSLL &rhs);

   SSLL(SSLL&& orig):head(nullptr), tail(nullptr){
     head = orig.head;
     tail = orig.tail;
     orig.head = nullptr;
     orig.tail = nullptr;
   };
   SSLL & operator= (SSLL && orig){
     if (this != &orig){ }
     clear();
     head = orig.head;
     tail = orig.tail;
     orig.head = nullptr;
     orig.tail = nullptr;

     return *this;
   };
  ~SSLL(){  clear(); };

private:
  class Node;
  Node * head;
  Node * tail;
  bool (SSLL::*comp)(E,E);

public:
 template <typename DataE>
 class SSLL_Iter
 {
 public:
   // type aliases required for C++ iterator compatibility
   using value_type =         E;
   using reference =          E&;
   using pointer =            E*;
   using difference_type =    std::ptrdiff_t;
   using iterator_category =  std::forward_iterator_tag;
   // type aliases for prettier code
   using self_type =      SSLL_Iter;
   using self_reference = SSLL_Iter&;
   friend class SSLL<DataE>;

   //factories to make iterators

   private:
     Node* here;

   public:
    explicit SSLL_Iter( Node* start = nullptr ):here( start ) {}
    SSLL_Iter( const SSLL_Iter& src ) : here( src.here ) {}

    static self_type make_begin(Node * node){
      SSLL_Iter i(node);
      return i;
    }
    static self_type make_end(Node * node){
      SSLL_Iter i(node);
      return i;
    }
    reference operator*() const {
      if(!here)
        throw new std::runtime_error( "Iterator exhausted: no such point" );

      return here->data;
    }
     pointer operator->() const {
       if(!here)
         throw new std::runtime_error( "Iterator exhausted: no such point" );
       return &(here->data);
     }

     self_reference operator=( SSLL_Iter<E> const& src ){
       if(this == &src)
        return *this;
      here = src.here;
      return *this;
     }

     self_reference operator++() {
       if(here->next)
        here = here->next;
      return *this;
     } // preincrement

     self_type operator++(int) {
       self_type tmp(*this);
       ++(*this);
       return tmp;
     } // postincrement

     bool operator==( SSLL_Iter<E> const& rhs ) const {
       return (&rhs.here == here);
     }
     bool operator!=( SSLL_Iter<E> const& rhs) const {
       return !(&rhs.here == here);
     }
   };

 public:
   using size_t = std::size_t;
   using value_type = E;
   using iterator = SSLL_Iter<E>;
   using const_iterator = SSLL_Iter<E>;

   iterator begin() {return iterator::make_begin(this->head);}
   iterator end() {return iterator::make_end(this->tail);}

   const_iterator begin() const { return const_iterator::make_begin( this->head ); }
   const_iterator end() const { return const_iterator::make_end( this->tail ); }
};

/////////////////////NODE//////////////////////////
template <typename E>
class SSLL<E>::Node{
public:
  Node():next(nullptr){}
  Node(E val):data(val), next(nullptr){}
  E data;
  Node * next;
};

/////////////copy constructor////////////////////
template <typename E>
SSLL<E>::SSLL(const SSLL & orig){
  if (orig.head == nullptr)
   head = tail = nullptr;
  else{
    head = new Node(orig.head->data);
    Node *current = head;
    Node *objHead = orig.head;
    Node *temp = objHead;
    while (temp->next != NULL) {
         current->next = new Node(temp->next->data);
         temp = temp->next;
         current = current->next;
    }
    tail = new Node(temp->data);
  }
}
//////////////copy assignment//////////////////////
template <typename E>
SSLL<E>&
SSLL<E>::operator=(const SSLL &orig){
  SSLL<E> temp(orig);
  std::swap(temp.head, head);
  std::swap(temp.tail, tail);
  return *this;
}
//////////////////////remove///////////////////////
template <typename E>
E
SSLL<E>::remove(int position){
  if(is_empty())
    throw std::runtime_error("Error in remove(position!)");
  else if(position == 1){
    E data;

    Node * n = head;
    data = head->data;
    head = head->next;
    delete n;
    return data;
  }
  else if( position == length()){
    Node * n = head;
    E data;
    while (n->next != tail)
      n = n->next;
    data = n->data;
    tail = n;
    delete n->next;

    return data;
  }
}

/////////////////////contents//////////////////////
template <typename E>
E *
SSLL<E>::contents(){
  if(!is_empty()){
    int l = length();
    E * arr = new E [l];
    E * copy = arr;
    Node * n = head;

    while (n){
      *copy++ = n->data;
      n = n->next;
    }
    return arr;
  }
  else
    throw std::runtime_error("Error in item_at(position!)");
}
/////////////////////print/////////////////////////
template <typename E>
std::ostream&
SSLL<E>::print(std::ostream& output){
  if(is_empty())
    output << "<empty list>";
  else{
    Node * n = head;
    output << "[";
    while(n){
      if (n == head)
        output << n->data;
      else
        output<< "-" << n->data;
      n = n->next;
    }
    output << "]";
  }
  return output;
}
////////////////////contains////////////////////////
template <typename E>
bool
SSLL<E>::contains(E value, bool (*fp)(E,E)){
  if(is_empty())
    throw std::runtime_error("Error in item_at(position!)");
  else{
    Node * n = head;
    while(n){
      if (fp(n->data, value))
        return true;
      n = n->next;
    }
    return false;
  }
}
///////////////////peek_front///////////////////////
template <typename E>
E
SSLL<E>::peek_front(){
  if(is_empty())
    throw std::runtime_error("Error in item_at(position!)");
  return head->data;

}
template <typename E>
E
SSLL<E>::peek_back(){
  if(is_empty())
    throw std::runtime_error("Error in item_at(position!)");
  return tail->data;

}
///////////////////item_at///////////////////////
template <typename E>
E
SSLL<E>::item_at(int position){
  if(position > length())
    throw std::runtime_error("Error in item_at(position!)");
  else{
    Node* n = new Node;
    while(--position>0)
      n = n->next;
    return n->data;
  }
}
/////////////////////pop_back////////////////////
template <typename E>
void
SSLL<E>::pop_back(){
  if(is_empty()){
      throw std::runtime_error("Error in pop_back(value, position!)");
  }
  else {
    Node * n = head;
    while(n->next != tail)
      n = n->next;
    delete tail;
    tail = n;
  }
}
/////////////////////pop_front////////////////////
template <typename E>
void
SSLL<E>::pop_front(){
  if(is_empty()){
      throw std::runtime_error("Error in pop_front(value, position!)");
  }
  else {
    Node * n = head;
    head = head->next;
    delete n;
  }
}
///////////////////////repalce///////////////////
template <typename E>
E
SSLL<E>::replace(E value, int position){
  if(position > length())
    throw std::runtime_error("Error in replace(value, position!)");
  else{
    Node * n = head;
    E old;
    for(int i = 1; i != position; ++i)
      n = n->next;
    old = n->data;
    n->data = value;
    return old;
  }

}
///////////////////////push front///////////////////
template <typename E>
void
SSLL<E>::push_front(E value){
  if(is_empty()){
    Node * n = new Node(value);
    head = tail = n;
  }
  else {
    //shift_right();
    Node * n = new Node(value);
    n->next = head;
    head = n;
  }
}
///////////////////////push back///////////////////
template <typename E>
void
SSLL<E>::push_back(E value){
  if(is_empty()){
    Node * n = new Node(value);
    head = tail = n;
  }
  else {
    tail->next = new Node(value);
    tail = tail->next;
  }
}
///////////////////////insert at///////////////////
template <typename E>
void
SSLL<E>::insert(E value, int position){
  if (length() < position)
    throw std::runtime_error("Error in insert(value, position!)");
  else if(position == 1){
    Node * n = new Node(value);
    n->next = head;
    head = n;
  }
  else if(position == length()){
    Node * n = new Node(value);
    tail->next = n;
    tail = n;
  }
  else{
    Node * n = head;
    while (--position > 1){
      n = n->next;
    }
    Node * temp = new Node(value);
    temp->next = n->next;
    n->next = temp;
  }
}
///////////////////////length//////////////////////
template <typename E>
int
SSLL<E>::length(){
	int length = 0;
	if (is_empty())
		return length;
	else
	{
		Node * n = head;
		while (n != nullptr)
		{
			++length;
			n = n->next;
		}
	}
	return length;
}
/////////////////clear////////////////////////////////
template <typename E>
void
SSLL<E>::clear(){
  if(head){
    Node * node = head;
  	while(node){
  		node = node->next;
  		delete head;
  		head = node;
  	}
  head = tail = nullptr;
  }
}

#endif
/////////////////////////ostream.......
