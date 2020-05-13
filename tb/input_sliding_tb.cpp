#include <hls_stream.h>
using namespace hls;
#include "ap_int.h"

#include "wallace_MEC.h"
#include "config_tb.hpp"
#include "bnn-library.h"

void Testbench(stream<ap_uint<IFM_Channels*INPUT_PRECISION> > & in, stream<ap_uint<IFM_Channels*INPUT_PRECISION> > & out, unsigned int numReps)
{
#pragma HLS DATAFLOW
hls::stream<ap_uint<SIMD*INPUT_PRECISION> > in_simd("in_simd");
#pragma HLS STREAM variable=in_simd depth=4 
hls::stream<ap_uint<SIMD*INPUT_PRECISION> > out_simd("out_simd");
#pragma HLS STREAM variable=out_simd depth=4

StreamingDataWidthConverter_Batch<IFM_Channels*INPUT_PRECISION, SIMD*INPUT_PRECISION, IFMDim*IFMDim>(in, in_simd, numReps);


MEC<KERNEL_DIM, IFM_Channels,INPUT_PRECISION, IFMDim, OFMDim,
     SIMD>(in_simd, out_simd, numReps);

StreamingDataWidthConverter_Batch<SIMD*INPUT_PRECISION, IFM_Channels*INPUT_PRECISION, KERNEL_DIM*KERNEL_DIM*OFMDim*OFMDim*IFM_Channels/SIMD>(out_simd, out, numReps);

}
	