#include "PSLL.h"

#include <iostream>

int main(void){

  PSLL<int> * psll = new PSLL<int>;
  for(int i = 1; i < 101; ++i){
    if (i % 2 == 0)
      psll->push_front(i);
    else
      psll->push_back(i);
  }
  std::cout << "Testing length: should be 100:" << psll->length() << std::endl;
  psll->print(std::cout);
  std::cout << std::endl;
  for(int i = 0; i < 50; ++i){
    psll->pop_back();
  }
  std::cout << "Testing length: should be 50:" << psll->length() << std::endl;
  psll->print(std::cout);
  std::cout << "Testing pool length: should be 50:" << psll->pool_length() << std::endl;
  for(int i = 0; i < 50; ++i){
    psll->push_front(i);
  }
  return 0;
}
