#include<iostream>
#include<ap_int.h>
#include"weights.h"

using namespace std;

void printb(unsigned int v) {
    unsigned int i, s = 1<<((sizeof(v)<<3)-1); // s = only most significant bit at 1
    for (i = s; i; i>>=1) printf("%d", v & i || 0 );
}
int main(){
  printb(m_weights0[0][0]);
  cout<<endl<<threshs0[0][0];
    cout<<endl;
return 0;
}
