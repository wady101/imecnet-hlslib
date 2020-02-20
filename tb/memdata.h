#define KERNEL_DIM 3 
#define SIMD1 1 
#define PE1 1 
#define MMV1 2 
#define WIDTH 1 
#define IFM_Channels1 1 
#define OFM_Channels1 1 
#define IFMDim1 8 
#define OFMDim1 6 
#define STRIDE1 1 
#define INPUT_PRECISION 4 
#define TILE1 9 
#define ACTIVATION_PRECISION 8 
#ifndef PARAMS_HPP
#define PARAMS_HPP
static ap_uint<1> m_weights [1][9] = {{0b1 ,0b0 ,0b1 ,0b1 ,0b0 ,0b0 ,0b0 ,0b1 ,0b0 } }; 
static ap_uint<8> threshs [1][1] = {{1} }; 
#endif 
