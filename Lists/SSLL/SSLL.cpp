#include "SSLL.h"

#include <iostream>

int main(void){

  SSLL<int> * ssll = new SSLL<int>;
  std::cout << std::endl << "Testing push_front(5)" << std::endl;
  ssll->push_front(5);

  std::cout << std::endl << "Testing push_back(6)" << std::endl;
  ssll->push_back(6);

  std::cout << std::endl << "Testing push_front(4)" << std::endl;
  ssll->push_front(4);
  ssll->insert(2,2);
  ssll->insert(1,2);
  ssll->print(std::cout);
  std::cout << std::endl << "Testing Replace(7,2)" << std::endl;
  ssll->replace(7,2);
  ssll->print(std::cout);
  ssll->clear();
  std::cout << "adding 500 elements!" << std::endl;
  for(int i = 0; i < 500; ++i){
    ssll->push_back(i);
  }
  std::cout << "removing even elements!" << std::endl;
  for(int i = 0; i < 500; ++i){
    if (i % 2 != 0)
      ssll->remove(i);
  }
  int * contents = ssll->contents();
  std::cout << std::endl << "Testing contents array, should have 500 elements" << std::endl;
  for(int i = 0; i < ssll->length(); ++i){
    std::cout << *contents++ << ',';
  }
  std::cout << std::endl;

  SSLL<int>::iterator iter = ssll->begin();
  SSLL<int>::iterator end = ssll->end();

  std::cout << "Begin iter:" << * iter << std::endl;
  std::cout << "End iter:" << * end << std::endl;

  return 0;
}
