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
void extractor() {
// extracting a single bit from ap_uint 
static ap_uint<2> m_weights [1][5] = {{0b10 ,0b11 ,0b01 ,0b00 ,0b11 } }; 
ap_uint<2> mask = 1;
ap_uint<1> ptr;
 for (unsigned i = 0; i<5; i++) {
   for (unsigned j = 0; j<2; j++) {
 printf("weights %s\n", m_weights[0][i].to_string().c_str());
auto ctr =  (m_weights[0][i] & (mask<<j)) ;
ptr = ctr >> j; 
// cout<<ctr<<endl;
printf("ctr %s\n", ptr.to_string().c_str());
 }
 }
}
int main(){
//print();
extractor();
return 0;
}
