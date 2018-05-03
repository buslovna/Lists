#ifndef _CDAL_H_
#define _CDAL_H_

#include "List.h"
#include <stdexcept>
#include <iostream>
namespace cop3530{

template <typename E>
class CDAL : public List<E>{
public:
   typedef bool(*comp_func)(E, E);
   CDAL():head(nullptr), tail(nullptr){}
   CDAL(const CDAL &L2){                                           //copy constructor
               data = L2.data;
               head = L2.head;
               tail = L2.tail;
  }
  CDAL& operator=(CDAL orig) {
            std::swap(haid, orig.head);
            std::swap(tail, orig.tail);
            return *this;
        }
  CDAL (CDAL && orig) : head(orig.head), tail(orig.tail){     //move constuctor
                    delete orig.head;
                    delete orig.tail;
  }
  ~CDAL(){                                                        //destructor
            delete head;
  }
   void insert(E value, int pos) override;
	 void push_back(E value) override;
	 void push_front(E value) override;
	 E replace( E value, int pos ) override; //replaces the existing value at
																										 //the specified pos with the specified value and return the original value.
   E remove(int pos) override;
	 void pop_back() override;
	 void pop_front() override;
	 E item_at(int pos) override;
	 E peek_front() override;
	 E peek_back() override;

	 inline bool is_empty() const { return (head == tail); }
	 inline bool is_full() override {return (head == next_index(tail));}
	 int length() override;
	 void clear() override;
	 bool contains(E value) override;


 	 std::ostream& print(std::ostream&) override; //takes output stream
	 E * contents() override;

  ~CDAL(){  clear();  };


private:
  class Node;
  int size;
  Node * head;
  E tail;
  E * data;

  E * next_index(E *);
  void shift_right(E*, E*);
  void shift_left(E*, E*);

}


template <typename E>
class CDAL<E>::Node{
public:
  Node():next(nullptr),data(new E [50]){}
  E data[50];
  Node* next;
};

template <typename E>
  void CDAL<E>::insert(E value, int pos){
      int num = pos/50;
      Node* temp = this->head;
      for(int i = 0; i<num; ++i)
          temp = temp->next;

      E temp_elm = temp->data[pos%50];
      E temp_elm2 = temp->data[pos%50+1];
      temp->data[pos%50] = value;

      for(int i=(pos%50)+1; i<tail+1; ++i){
          temp->data[i] = temp_elm;
          temp_elm = temp_elm2;
          temp_elm2 = temp->data[i+1];
      }
      tail = tail + 1;
  }
}// end namespace
#endif
