#include"interpret.hpp"
#include<iostream>
#include<typeinfo>

using namespace std;

// Identity is used as an interpreter command to be compatible with acitvations (finn)
template <typename poke = Identity,typename dest = Identity>
void print(){
cout<<4*poke::width<<endl;
if (4 * poke::width == dest::width * 3)
  cout<<"noshit";
cout<<poke::width<<endl;
cout<<typeid(4*poke::width).name()<<endl;
const unsigned int multiplying_factor = 3/24;
cout<<multiplying_factor<<endl;
}
int main(){
print();
return 0;
}
