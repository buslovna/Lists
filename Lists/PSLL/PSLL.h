#ifndef _PSLL_H_
#define _PSLL_H_

#include "List.h"
#include <stdexcept>
#include <iostream>

template <typename E>
class PSLL : public List<E>{
public:
   typedef bool (*comp_func)(E, E);
   PSLL():head(nullptr), tail(nullptr), pool(nullptr){}
   PSLL(const PSLL & orig);
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

	 inline bool is_empty() const { return head == nullptr; }
	 inline bool is_full() override {return false;}
   inline bool pool_empty() {
     if (check_pool()){
       Node * temp = pool;
       for(int i = 0; i < pool_length()/2; ++i){
         temp = temp->next;
         delete pool;
         pool = temp;
       }
     }
     return pool == nullptr;
   }
   inline bool check_pool() {return ((length() > 100 && pool_length() > .5*length()) ? true : false) ;}

	 int length() override;
   int pool_length();
	 void clear() override;
   bool contains(E value, bool(*fp)(E,E)) override;
   bool compare_function(E val1, E val2){return (val1 == val2? true : false);}
   PSLL& operator =(const PSLL &rhs);

   std::ostream& print( std::ostream& out) override; //takes output stream
   E * contents() override;

   PSLL(PSLL&& orig):head(nullptr), tail(nullptr), pool(nullptr){
     head = orig.head;
     tail = orig.tail;
     orig.head = nullptr;
     orig.tail = nullptr;
     orig.pool = nullptr;
   };
   PSLL& operator= (PSLL && orig){
     if (this != &orig){ }
     clear();
     head = orig.head;
     tail = orig.tail;
     orig.head = nullptr;
     orig.tail = nullptr;
     orig.pool = nullptr;

     return *this;
   };

  ~PSLL(){  clear();  };

private:

  class Node;
  Node * head;
  Node * tail;
  Node * pool;

public:
 template <typename DataE>
 class PSLL_Iter
 {
 public:
   // type aliases required for C++ iterator compatibility
   using value_type =         E;
   using reference =          E&;
   using pointer =            E*;
   using difference_type =    std::ptrdiff_t;
   using iterator_category =  std::forward_iterator_tag;
   // type aliases for prettier code
   using self_type =      PSLL_Iter;
   using self_reference = PSLL_Iter&;
   friend class PSLL<DataE>;

   //factories to make iterators

   private:
     Node* here;

   public:
    explicit PSLL_Iter( Node* start = nullptr ):here( start ) {}
    PSLL_Iter( const PSLL_Iter& src ) : here( src.here ) {}

    static self_type make_begin(Node * node){
      PSLL_Iter i(node);
      return i;
    }
    static self_type make_end(Node * node){
      PSLL_Iter i(node);
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

     self_reference operator=( PSLL_Iter<E> const& src ) {
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

     bool operator==( PSLL_Iter<E> const& rhs ) const {
       return (&rhs.here == here);
     }
     bool operator!=( PSLL_Iter<E> const& rhs) const {
       return !(&rhs.here == here);
     }
   };

 public:
   using size_t = std::size_t;
   using value_type = E;
   using iterator = PSLL_Iter<E>;
   using const_iterator = PSLL_Iter<E>;

   iterator begin() {return iterator::make_begin(this->head);}
   iterator end() {return iterator::make_end(this->tail);}

   const_iterator begin() const { return const_iterator::make_begin( *this ); }
   const_iterator end() const { return const_iterator::make_end( *this ); }

};
/////////////////////NODE//////////////////////////
template <typename E>
class PSLL<E>::Node{
public:
  Node():next(nullptr){}
  Node(E val):data(val), next(nullptr){}
  E data;
  Node * next;
};
//////////////////////COPY CONS////////////////////
template <typename E>
PSLL<E>::PSLL(const PSLL & orig){
  if (orig.head == nullptr)
   head = tail = pool = nullptr;
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
    pool = nullptr;
  }
}
template <typename E>
PSLL<E>&
PSLL<E>::operator=(const PSLL &orig){
  PSLL<E> temp(orig);
  std::swap(temp.head, head);
  std::swap(temp.tail, tail);
  return *this;
}
//////////////////////remove///////////////////////
template <typename E>
E
PSLL<E>::remove(int position){
  if(is_empty())
    throw std::runtime_error("Error in remove(position)!");

  else if (position == 0){
    Node * node = head;
    E data = head->data;
    head = head->next;

    node->next = pool;
    pool = node;
  }
  else{
      Node * n = head;
      E data;
      while(--position > 0)
        n = n->next;
      data = n->next->data;
      if (n->next->next = nullptr)
        tail = n;
      n->next->next = pool;
      pool = n->next;

      return data;
  }

}

/////////////////////contents//////////////////////
template <typename E>
E *
PSLL<E>::contents(){
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
PSLL<E>::print( std::ostream& output){
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
PSLL<E>::contains(E value, bool (*fp)(E,E)){
  if(is_empty())
    throw std::runtime_error("Error in contains()");
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
PSLL<E>::peek_front(){
  if(is_empty())
    throw std::runtime_error("Error in item_at(position!)");
  return head->data;

}
template <typename E>
E
PSLL<E>::peek_back(){
  if(is_empty())
    throw std::runtime_error("Error in item_at(position!)");
  return tail->data;

}
///////////////////item_at///////////////////////
template <typename E>
E
PSLL<E>::item_at(int position){
  if(position > length())
    throw std::runtime_error("Error in item_at(position)!");
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
PSLL<E>::pop_back(){
  if(is_empty()){
      throw std::runtime_error("Error in pop_back()");
  }
  else {
    Node * n = head;
    while(n->next != tail)
      n = n->next;
    tail->next = pool;
    pool = tail;
    n->next = nullptr;
    tail = n;
    //std::cout << "Current tail is:" << tail->data << std::endl;
  }
}
/////////////////////pop_front////////////////////
template <typename E>
void
PSLL<E>::pop_front(){
  if(is_empty()){
      throw std::runtime_error("Error in pop_front()");
  }
  else {
    Node * n = head;
    head = head->next;
    n->next = pool;
    pool = n;
  }
}
///////////////////////replace///////////////////
template <typename E>
E
PSLL<E>::replace(E value, int position){
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
PSLL<E>::push_front(E value){
  if(is_empty()){
    if(!pool_empty()){
      Node * n = pool;
      n->data = value;
      pool = pool->next;
      n->next = nullptr;
      head = tail = n;
    }
    else{
      Node * n = new Node(value);
      head = tail = n;
    }
  }
  else {
    if(!pool_empty()){
      Node * n = pool;
      n->data = value;
      pool = pool->next;
      n->next = head;
      head = n;
    }
    else{
      Node * n = new Node(value);
      n->next = head;
      head = n;
    }
  }
}
///////////////////////push back///////////////////
template <typename E>
void
PSLL<E>::push_back(E value){
  if(is_empty()){
    if(!pool_empty()){
      Node * n = pool;
      n->data = value;
      pool = pool->next;
      n->next = nullptr;
      head = tail = n;
    }
    else{
      Node * n = new Node(value);
      head = tail = n;
    }
  }
  else {
    if(pool_empty()){
      tail->next = new Node(value);
      tail = tail->next;
    }
    else{
      Node * n = pool;
      n->data = value;
      pool = pool->next;
      n->next = nullptr;
      tail->next = n;
      tail = n;
    }
  }
}
///////////////////////insert at///////////////////
template <typename E>
void
PSLL<E>::insert(E value, int position){
  if (length() < position)
    throw std::runtime_error("Error in insert(value, position!)");
  else if(position == 0){
    if(pool_empty()){
      Node * n = new Node(value);
      n->next = head;
      head = n;
    }
    else{
      Node * n = pool;
      n->data = value;
      pool = pool->next;
      n->next = head;
      head = n;
    }
  }
  else if(position == length()){
    if(pool_empty()){
      Node * n = new Node(value);
      tail->next = n;
      tail = n;
    }
    else{
      Node * n = pool;
      n->data = value;
      pool = pool->next;
      tail->next = n;
      tail = n;
    }
  }
  else{
    if(pool_empty()){
      Node * n = head;
      while (--position > 0){
        n = n->next;
      }
      Node * temp = new Node(value);
      temp->next = n->next;
      n->next = temp;
    }
    else{
      Node * n = head;
      while (--position > 0){
        n = n->next;
      }
      Node * temp = pool;
      pool = pool -> next;
      temp->data = value;
      temp->next = n->next;
      n->next = temp;
    }
  }
}
///////////////////////pool length//////////////////////
template <typename E>
int
PSLL<E>::pool_length(){
	int length = 0;
	if (pool_empty())
		return length;
	else
	{
		Node * n = pool;
		while (n != nullptr)
		{
			++length;
			n = n->next;
		}
	}
	return length;
}

///////////////////////length//////////////////////
template <typename E>
int
PSLL<E>::length(){
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
PSLL<E>::clear(){
  if(head){
    Node * node = head;
    while(node){
      node = node->next;
      delete head;
      head = node;
    }
  head = tail = nullptr;
  }

  if(!pool_empty()){
    Node * node = pool;
    while(node){
      node = node->next;
      delete pool;
      pool = node;
    }
  }
  pool = nullptr;
}

#endif
