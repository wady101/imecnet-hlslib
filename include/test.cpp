#include"interpret.hpp"
#include<iostream>
#include<hls_stream.h>
#include<typeinfo>

using namespace std;
using namespace hls;

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
template <size_t rows, size_t cols>
// void process_2d_array_template(int (&array)[rows][cols])
void process_2d_array_template(int array[][cols])
// void process_2d_array_template(int &array)
{
    std::cout << __func__ << std::endl;

    for (size_t i = 0; i < rows; ++i)
    {
        std::cout << i << ": ";
        
        for (size_t j = 0; j < cols; ++j)
            std::cout << array[i][j] << '\t';
        std::cout << std::endl;
    }
}

template <typename TDstI = Identity>
void streammasking(){
ap_uint<2> a[]={1,2,3,4,5,6,7,8,9,1};
auto  Stream = Identity().template operator()<hls::stream<ap_uint<10>> >();
for (unsigned i = 0; i<10; i++){ 
    Stream(i) = a[i];
}
printf("Stream  %s \n", Stream);



}

int main(){
//First test:    
    //print();
//Second test:
    // extractor();
// Third test:
    // int a[2][3] = { {1,2,3},{4,5,6} };
    // int b[1] = {1};
    // // process_2d_array_template(a);
    // process_2d_array_template<2,3>(a);
// Fourth test:
streammasking();

return 0;
}
