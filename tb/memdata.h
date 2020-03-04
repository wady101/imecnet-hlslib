#define KERNEL_DIM 1 
#define SIMD1 1 
#define PE1 1 
#define MMV1 2 
#define WIDTH 1 
#define IFM_Channels1 1 
#define OFM_Channels1 1 
#define IFMDim1 4 
#define OFMDim1 4 
#define STRIDE1 1 
#define INPUT_PRECISION 1 
#define WMEM1 1 
#define TMEM1 1 
#define ACTIVATION_PRECISION 1 
#ifndef PARAMS_HPP
#define PARAMS_HPP
static ap_uint<1> m_weights [1][1] = {{0b1 } }; 
static ap_uint<8> threshs [1][1] = {{10} }; 
#endif 
