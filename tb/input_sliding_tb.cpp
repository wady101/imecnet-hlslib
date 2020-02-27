#include <hls_stream.h>
using namespace hls;
#include "ap_int.h"

#include "wallace_MEC.h"
#include "config_tb.hpp"

void Testbench(stream<ap_uint<SIMD*INPUT_PRECISION> > & in, stream<ap_uint<SIMD*INPUT_PRECISION> > & out, unsigned int numReps)
{
MEC<KERNEL_DIM, IFM_Channels,INPUT_PRECISION, IFMDim, OFMDim,
     SIMD>(in, out, numReps);
}
	