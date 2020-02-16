#include <hls_stream.h>
using namespace hls;
#include "ap_int.h"
#include "bnn-library.h"

#include "weights.hpp"
#include "activations.hpp"
#include "interpret.hpp"
#include "mvau.hpp"
#include "config.h"
#include "weights.h"
#include "wallace_convlayer.h"
#include "wallace_GEMM.h"
#include "wallace_mvau.hpp"


void DoCompute(ap_uint<64> *in, ap_uint<64> *out, const unsigned int numReps) {
#pragma HLS DATAFLOW
  stream<ap_uint<64>> inter0("DoCompute.inter0");
  stream<ap_uint<192>> inter0_1("DoCompute.inter0_1");
  stream<ap_uint<24>> inter0_2("DoCompute.inter0_2");
 const unsigned int inBits = 32 * 32 * 3 * 8;
  const unsigned int outBits = L8_MH * 16;

  Mem2Stream_Batch<64, inBits/8>(in, inter0, numReps);
  StreamingDataWidthConverter_Batch<64, 192, (32 * 32 * 3 * 8) / 64>(inter0, inter0_1, numReps);
  CASSERT_DATAFLOW(192%L0_SIMD==0)
  StreamingDataWidthConverter_Batch<192, 24, (32 * 32 * 3 * 8) / 192>(inter0_1, inter0_2, numReps);
ConvLayer_Wallace<L0_K, L0_IFM_CH, L0_IFM_DIM, L0_OFM_CH, L0_OFM_DIM, L0_SIMD, L0_PE, Slice<ap_fixed<8, 1, AP_TRN, AP_SAT>>, Slice<ap_int<2>>, Recast<Binary>>(inter0_2, inter1, m_weights0, threshs0, numReps, ap_resource_lut());

}

int main() {
    doCompute
    return 0;
}