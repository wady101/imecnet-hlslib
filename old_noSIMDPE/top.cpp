#include <config.h>
#include <hls_stream.h>
#include "ap_int.h"
#include <iostream>
#include <string>
#include <weights.hpp>
#include <docomputer.hpp>
#include <activations.hpp>
#include <dma.h>
#include <streamtools.h>
#include <utils.hpp>
#include <maxpool.h>
#include <mvau.hpp>

using namespace hls;
using namespace std;


//static BinaryWeights<BRAM_NUM,BRAM_DEPTH,BRAM_WIDTH> weights0;
static BinaryWeights<CONVW1_DIM4,CONVW1_DIM3,CONVW1_DIM2,CONVW1_DIM1>  weights0;
static BinaryWeights<CONVW2_DIM4,CONVW2_DIM3,CONVW2_DIM2,CONVW2_DIM1>  weights1;
static BinaryWeights<CONVW4_DIM4,CONVW4_DIM3,CONVW4_DIM2,CONVW4_DIM1>  weights2;
static BinaryWeights<CONVW5_DIM4,CONVW5_DIM3,CONVW5_DIM2,CONVW5_DIM1>  weights3;
static BinaryWeights<CONVW7_DIM4,CONVW7_DIM3,CONVW7_DIM2,CONVW7_DIM1>  weights4;
static BinaryWeights<CONVW8_DIM4,CONVW8_DIM3,CONVW8_DIM2,CONVW8_DIM1>  weights5;
static BinaryWeights<CONVW9_DIM4,CONVW9_DIM3,CONVW9_DIM2,CONVW9_DIM1>  weights6;
//static BinaryWeights<L7_SIMD, L7_PE, L7_WMEM>  weights7;
//static BinaryWeights<L8_SIMD, L8_PE, L8_WMEM>  weights8;

//static ThresholdsActivation<BNB1_DIM1,ap_fixed<8,16>> thres0;
//static ThresholdsActivation<BNB1_DIM2,ap_fixed<8,16>> thres1;
//static ThresholdsActivation<BNB1_DIM3,ap_fixed<8,16>> thres2;
//static ThresholdsActivation<BNB1_DIM4,ap_fixed<8,16>> thres3;
//static ThresholdsActivation<BNB1_DIM5,ap_fixed<8,16>> thres4;
//static ThresholdsActivation<BNB1_DIM6,ap_fixed<8,16>> thres5;
//static ThresholdsActivation<BNB1_DIM7,ap_fixed<8,16>> thres6;

static ThresholdsActivation<BNB1_DIM1,ap_fixed<12,12>,0,ap_int<2>,ap_int<2>> thres0;
static ThresholdsActivation<BNB2_DIM1,ap_fixed<12,12>,0,ap_int<2>,ap_int<2>> thres1;
static ThresholdsActivation<BNB4_DIM1,ap_fixed<12,12>,0,ap_int<2>,ap_int<2>> thres2;
static ThresholdsActivation<BNB5_DIM1,ap_fixed<12,12>,0,ap_int<2>,ap_int<2>> thres3;
static ThresholdsActivation<BNB7_DIM1,ap_fixed<12,12>,0,ap_int<2>,ap_int<2>> thres4;
static ThresholdsActivation<BNB8_DIM1,ap_fixed<12,12>,0,ap_int<2>,ap_int<2>> thres5;
static ThresholdsActivation<BNB9_DIM1,ap_fixed<12,12>,0,ap_int<1>,ap_int<2>> thres6;


void DoMemInit(unsigned int targetLayer,unsigned int targetdim4,unsigned int targetdim3,unsigned int targetdim2, ap_uint<64> val)
{
	switch(targetLayer){
	case 0:
			weights0.weights_m[targetdim4][targetdim3][targetdim2] = val;
			break;
	case 1:
			thres0.m_thresholds[targetdim4] = val;
			break;
	case 2:
			weights1.weights_m[targetdim4][targetdim3][targetdim2] = val;
			break;
	case 4:
			thres1.m_thresholds[targetdim4] = val;
			break;
	case 5:
			weights2.weights_m[targetdim4][targetdim3][targetdim2] = val;
			break;
	case 6:
			thres2.m_thresholds[targetdim4] = val;
			break;
	case 7:
			weights3.weights_m[targetdim4][targetdim3][targetdim2] = val;
			break;
	case 8:
			thres3.m_thresholds[targetdim4] = val;
			break;
	case 9:
			weights4.weights_m[targetdim4][targetdim3][targetdim2] = val;
			break;
	case 10:
			thres4.m_thresholds[targetdim4] = val;
			break;
	case 11:
			weights5.weights_m[targetdim4][targetdim3][targetdim2] = val;
			break;
	case 12:
			thres5.m_thresholds[targetdim4] = val;
			break;
	case 13:
			weights6.weights_m[targetdim4][targetdim3][targetdim2] = val;
			break;
	case 14:
			thres6.m_thresholds[targetdim4] = val;
			break;


	}

}
void doCompute(ap_uint<64> *in, ap_uint<64> *out, unsigned int numReps){ //didn't add numreps
#pragma HLS DATAFLOW
  stream<ap_uint<64>> inter0("DoCompute.inter0");
  stream<ap_uint<192>> inter0_1("DoCompute.inter0_1");
  stream<ap_uint<24>> inter0_2("DoCompute.inter0_2");
#pragma HLS STREAM variable=inter0_2 depth=128
  stream<ap_uint<64>> inter1("DoCompute.inter1");
#pragma HLS STREAM variable=inter1 depth=128
  stream<ap_uint<64>> inter2("DoCompute.inter2");
  stream<ap_uint<64>> inter3("DoCompute.inter3");
#pragma HLS STREAM variable=inter3 depth=128
  stream<ap_uint<128>> inter4("DoCompute.inter4");
#pragma HLS STREAM variable=inter4 depth=128
  stream<ap_uint<128>> inter5("DoCompute.inter5");
  stream<ap_uint<128>> inter6("DoCompute.inter6");
#pragma HLS STREAM variable=inter6 depth=81
  stream<ap_uint<256>> inter7("DoCompute.inter7");
#pragma HLS STREAM variable=inter7 depth=1
  stream<ap_uint<256>> inter8("DoCompute.inter8");
#pragma HLS STREAM variable=inter8 depth=1
  stream<ap_uint<64>> inter9("DoCompute.inter9");
#pragma HLS STREAM variable=inter9 depth=128
  stream<ap_uint<64>> inter10("DoCompute.inter10");
#pragma HLS STREAM variable=inter10 depth=3
  stream<ap_uint<64>> memOutStrm("DoCompute.memOutStrm");
   const unsigned int inBits = 32 * 32 * 3 * 8;
  const unsigned int outBits = 7 * 16; // number of layers * number of output bits per image
  //Lite to stream conversion
  Mem2Stream_Batch<64, inBits/8>(in, inter0, numReps); 
  // bufffer fit
  StreamingDataWidthConverter_Batch<64, 192, (32 * 32 * 3 * 8) / 64>(inter0, inter0_1, numReps);
  StreamingDataWidthConverter_Batch<192, 24, (32 * 32 * 3 * 8) / 192>(inter0_1, inter0_2, numReps);
  ConvLayer_Batch<CONVW1_DIM2, CONVW1_DIM3, 32, CONVW1_DIM4, CONVW1_DIM_IMAGE, SIMD_1, PE_1, Slice<ap_fixed<8, 1, AP_TRN, AP_SAT>>, Slice<ap_int<2>>, Recast<Binary>>(inter0_2, inter1, weights0, thres0, numReps, ap_resource_lut());
  // input , input - CONVW1_DIM2 + 1
 ConvLayer_Batch<CONVW2_DIM2, CONVW2_DIM3, CONVW1_DIM_IMAGE, CONVW2_DIM4, CONVW2_DIM_IMAGE, SIMD_2, PE_2,  Slice<ap_int<2> >, Slice<ap_int<2>>, Recast<Binary>>(inter1, inter2, weights1, thres1, numReps, ap_resource_lut());
 StreamingMaxPool_Precision_Batch<CONVW3_DIM_IMAGE, CONVW3_MAXPOOL_KER, CONVW2_DIM4, ap_int<2>, -1>(inter2, inter3, numReps);
 ConvLayer_Batch<CONVW4_DIM2, CONVW4_DIM3, CONVW4_DIM_IMAGE, CONVW4_DIM4, CONVW4_DIM_IMAGE, SIMD_3, PE_3,  Slice<ap_int<2> >, Slice<ap_int<2>>, Recast<Binary>>(inter3, inter4, weights2, thres2, numReps, ap_resource_lut());
 ConvLayer_Batch<CONVW5_DIM2, CONVW5_DIM3, CONVW5_DIM_IMAGE, CONVW5_DIM4, CONVW5_DIM_IMAGE, SIMD_4, PE_4,  Slice<ap_int<2> >, Slice<ap_int<2>>, Recast<Binary>>(inter4, inter5, weights3, thres3, numReps, ap_resource_lut());
StreamingMaxPool_Precision_Batch<CONVW6_DIM_IMAGE, CONVW6_MAXPOOL_KER, CONVW5_DIM4, ap_int<2>, -1>(inter5, inter6, numReps);
ConvLayer_Batch<CONVW7_DIM2, CONVW7_DIM3, CONVW7_DIM_IMAGE, CONVW7_DIM4, CONVW7_DIM_IMAGE, SIMD_5, PE_5,  Slice<ap_int<2> >, Slice<ap_int<2>>, Recast<Binary>>(inter6, inter7, weights4, thres4, numReps, ap_resource_lut());
ConvLayer_Batch<CONVW8_DIM2, CONVW8_DIM3, CONVW8_DIM_IMAGE, CONVW8_DIM4, CONVW8_DIM_IMAGE, SIMD_6, PE_6,  Slice<ap_int<2> >, Slice<ap_int<2>>, Recast<Binary>>(inter7, inter8, weights5, thres5, numReps, ap_resource_lut());
ConvLayer_Batch<CONVW9_DIM2, CONVW9_DIM3, CONVW9_DIM_IMAGE, CONVW9_DIM4, CONVW9_DIM_IMAGE, SIMD_7, PE_7,  Slice<ap_int<2> >, Slice<ap_int<2>>, Recast<Binary>>(inter8, inter9, weights6, thres6, numReps, ap_resource_lut());
StreamingMaxPool_Precision_Batch<CONVW10_DIM_IMAGE, CONVW10_MAXPOOL_KER, CONVW9_DIM4, ap_int<2>, -1>(inter9, static_cast<hls::stream<ap_uint<16 * PE_9>>&>(memOutStrm), numReps);

Stream2Mem_Batch<64, outBits/8>(memOutStrm, out, numReps);
}


void BlackBoxJam(ap_uint<64> *in, ap_uint<64> *out, bool doInit,
		unsigned int targetLayer, unsigned int targetdim4,
		unsigned int targetdim3, unsigned int targetdim2, ap_uint<64> val, unsigned int numReps) {
// signals to be mapped to the AXI Lite slave port
// #pragma HLS INTERFACE axis port = return bundle=data
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS INTERFACE s_axilite port=doInit bundle=control
#pragma HLS INTERFACE s_axilite port=targetLayer bundle=control
#pragma HLS INTERFACE s_axilite port=targetdim4 bundle=control
#pragma HLS INTERFACE s_axilite port=targetdim3 bundle=control
#pragma HLS INTERFACE s_axilite port=targetdim2 bundle=control
#pragma HLS INTERFACE s_axilite port=val bundle=control
#pragma HLS INTERFACE s_axilite port=numReps bundle=control//didn't add numreps 
// signals to be mapped to the AXI master port (hostmem)
#pragma HLS INTERFACE m_axi offset=slave port=in bundle=hostmem depth=512
#pragma HLS INTERFACE s_axilite port=in bundle=control
#pragma HLS INTERFACE m_axi offset=slave port=out bundle=hostmem depth=16
#pragma HLS INTERFACE s_axilite port=out bundle=control

// partition PE arrays
#pragma HLS ARRAY_PARTITION variable=weights0.weights_m complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs0.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs0.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights1.weights_m complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs1.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs1.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights2.weights_m complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs2.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs2.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights3.weights_m complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs3.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs3.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights4.weights_m complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs4.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs4.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights5.weights_m complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs5.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs5.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights6.weights_m complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs6.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs6.m_thresholds complete dim=3
//#pragma HLS ARRAY_PARTITION variable=weights7.weights_m complete dim=1
//#pragma HLS ARRAY_PARTITION variable=threshs7.m_thresholds complete dim=1
//#pragma HLS ARRAY_PARTITION variable=threshs7.m_thresholds complete dim=3
//#pragma HLS ARRAY_PARTITION variable=weights8.weights_m complete dim=1

  if (doInit) {
    DoMemInit(targetLayer, targetdim4, targetdim3, targetdim2 , val);//didn't add numreps
  } else {
    doCompute(in, out, numReps);
  }
}
