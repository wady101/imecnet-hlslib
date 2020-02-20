#include <hls_stream.h>
using namespace hls; 
#include "ap_int.h"
// #include "config_tb.hpp"
#include "memdata.h"
#include "bnn-library.h"
#include "wallace_convlayer.h"

void Testbench_conv(stream<ap_uint<IFM_Channels1*INPUT_PRECISION> > & in, stream<ap_uint<OFM_Channels1*ACTIVATION_PRECISION> > & out, unsigned int numReps){
#pragma HLS DATAFLOW
	ConvLayer_Wallace<KERNEL_DIM, IFM_Channels1, IFMDim1, OFM_Channels1, OFMDim1, SIMD1, PE1, ap_uint<SIMD1>, ap_int<8> >(in, out, m_weights, threshs, numReps);
}