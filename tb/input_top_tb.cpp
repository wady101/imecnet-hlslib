#include <hls_stream.h>
using namespace hls; 
#include "ap_int.h"
// #include "config_tb.hpp"


#include "bnn-library.h"
#include "memdata.h"
#include "wallace_convlayer.h"

void Testbench_conv(stream<ap_uint<IFM_Channels1> > & in, stream<ap_uint<OFM_Channels1> > & out, unsigned int numReps){
	ConvLayer_Wallace<KERNEL_DIM, IFM_Channels1, IFMDim1, OFM_Channels1, OFMDim1, SIMD1, PE1, WMEM1, TMEM1, ap_uint<SIMD1>, ap_uint<8>>(in, out, m_weights, threshs, numReps);
}