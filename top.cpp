#include <config.h>
#include <hls_stream.h>
#include "ap_int.h"
#include <iostream>
#include <string>
#include <weights.hpp>
#include <activations.hpp>

using namespace hls;
using namespace std;

#define CASSERT_DATAFLOW(x) {if (!(x)) {std::cout<< "CASSERT_DATAFLOW condition is not met " << endl; exit(-1);	}}

//static BinaryWeights<BRAM_NUM,BRAM_DEPTH,BRAM_WIDTH> weights0;
static BinaryWeights<CONVW1_DIM4,CONVW1_DIM3,CONVW1_DIM2,CONVW1_DIM1>  weights0;
static BinaryWeights<CONVW2_DIM4,CONVW2_DIM3,CONVW2_DIM2,CONVW2_DIM1>  weights1;
static BinaryWeights<CONVW3_DIM4,CONVW3_DIM3,CONVW3_DIM2,CONVW3_DIM1>  weights2;
static BinaryWeights<CONVW4_DIM4,CONVW4_DIM3,CONVW4_DIM2,CONVW4_DIM1>  weights3;
static BinaryWeights<CONVW5_DIM4,CONVW5_DIM3,CONVW5_DIM2,CONVW5_DIM1>  weights4;
static BinaryWeights<CONVW6_DIM4,CONVW6_DIM3,CONVW6_DIM2,CONVW6_DIM1>  weights5;
static BinaryWeights<CONVW7_DIM4,CONVW7_DIM3,CONVW7_DIM2,CONVW7_DIM1>  weights6;
//static BinaryWeights<L7_SIMD, L7_PE, L7_WMEM>  weights7;
//static BinaryWeights<L8_SIMD, L8_PE, L8_WMEM>  weights8;

//static ThresholdsActivation<BNB1_DIM1,ap_fixed<8,16>> thres0;
//static ThresholdsActivation<BNB1_DIM2,ap_fixed<8,16>> thres1;
//static ThresholdsActivation<BNB1_DIM3,ap_fixed<8,16>> thres2;
//static ThresholdsActivation<BNB1_DIM4,ap_fixed<8,16>> thres3;
//static ThresholdsActivation<BNB1_DIM5,ap_fixed<8,16>> thres4;
//static ThresholdsActivation<BNB1_DIM6,ap_fixed<8,16>> thres5;
//static ThresholdsActivation<BNB1_DIM7,ap_fixed<8,16>> thres6;

static ThresholdsActivation<BNB1_DIM1>,ap_fixed<12,12>,-1> thres0;
static ThresholdsActivation<BNB2_DIM1>,ap_fixed<12,12>> thres1;
static ThresholdsActivation<BNB3_DIM1>,ap_fixed<12,12>> thres2;
static ThresholdsActivation<BNB4_DIM1>,ap_fixed<12,12>> thres3;
static ThresholdsActivation<BNB5_DIM1>,ap_fixed<12,12>> thres4;
static ThresholdsActivation<BNB6_DIM1>,ap_fixed<12,12>> thres5;
static ThresholdsActivation<BNB7_DIM1>,ap_fixed<12,12>> thres6;


void DoMemInit(unsigned int targetLayer,unsigned int targetdim4,unsigned int targetdim3,unsigned int targetdim2, unsigned int targetdim1, ap_uint<64> val)
{
	switch(targetLayer){
	case 0:
			weights0.weights_m[targetdim4][targetdim3][targetdim2][targetdim1] = val;
			break;
	case 1:
			thres0.weights_m[targetdim4] = val;
			break;
	case 2:
			weights1.init_weights[targetdim4][targetdim3][targetdim2][targetdim1] = val;
			break;
	case 4:
			thres1.init_thres[targetdim4] = val;
			break;
	case 5:
			weights2.init_weights[targetdim4][targetdim3][targetdim2][targetdim1] = val;
			break;
	case 6:
			thres2.init_thres[targetdim4] = val;
			break;
	case 7:
			weights3.init_weights[targetdim4][targetdim3][targetdim2][targetdim1] = val;
			break;
	case 8:
			thres3.init_thres[targetdim4] = val;
			break;
	case 9:
			weights4.init_weights[targetdim4][targetdim3][targetdim2][targetdim1] = val;
			break;
	case 10:
			thres4.init_thres[targetdim4] = val;
			break;
	case 11:
			weights5.init_weights[targetdim4][targetdim3][targetdim2][targetdim1] = val;
			break;
	case 12:
			thres5.init_thres[targetdim4] = val;
			break;
	case 13:
			weights6.init_weights[targetdim4][targetdim3][targetdim2][targetdim1] = val;
			break;
	case 14:
			thres6.init_thres[targetdim4] = val;
			break;


	}

}
void doCompute(ap_uint<64> *in, ap_uint<64> *out){

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
#pragma HLS INTERFACE s_axilite port=numReps bundle=control
// signals to be mapped to the AXI master port (hostmem)
#pragma HLS INTERFACE m_axi offset=slave port=in bundle=hostmem depth=512
#pragma HLS INTERFACE s_axilite port=in bundle=control
#pragma HLS INTERFACE m_axi offset=slave port=out bundle=hostmem depth=16
#pragma HLS INTERFACE s_axilite port=out bundle=control

// partition PE arrays
#pragma HLS ARRAY_PARTITION variable=weights0.m_weights complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs0.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs0.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights1.m_weights complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs1.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs1.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights2.m_weights complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs2.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs2.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights3.m_weights complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs3.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs3.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights4.m_weights complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs4.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs4.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights5.m_weights complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs5.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs5.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights6.m_weights complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs6.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs6.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights7.m_weights complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs7.m_thresholds complete dim=1
#pragma HLS ARRAY_PARTITION variable=threshs7.m_thresholds complete dim=3
#pragma HLS ARRAY_PARTITION variable=weights8.m_weights complete dim=1

  if (doInit) {
    DoMemInit(targetLayer, targetdim4, targetdim3, targetdim2, val);
  } else {
    DoCompute(in, out, numReps);
  }
}
