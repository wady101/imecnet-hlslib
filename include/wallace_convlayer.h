#include <ap_int.h>
#include <hls_stream.h>

#include "streamtools.h"

#include "wallace_GEMM.h"
#include "wallace_mvau.hpp"

template<
		unsigned int ConvKernelDim,		
		unsigned int IFMChannels,		
		unsigned int IFMDim,			
		unsigned int OFMChannels,		
		unsigned int OFMDim,			
		
		unsigned int SIMD, 				// number of SIMD lanes
		unsigned int PE,				// number of PEs
		typename TW, typename TA ,
		int InStreamW, int OutStreamW  // safely deducible (stream width must be int though!)
		
>
void ConvLayer_Wallace(hls::stream<ap_uint<InStreamW>>  &in,
			    hls::stream<ap_uint<OutStreamW>> &out,
			    TW const &weights,
			    TA const &activation,
			    unsigned const   reps
				){
	#pragma HLS INLINE OFF
    unsigned const MatrixW = IFMDim * IFMChannels;
    unsigned const MatrixH = OFMChannels;
    unsigned const InpPerImage = IFMDim*ConvKernelDim*IFMChannels/InStreamW; //Not sure about the division
   WidthAdjustedInputStream <InStreamW, SIMD, InpPerImage>  wa_in (in,  reps);
   WidthAdjustedOutputStream <PE, OutStreamW, OFMDim * OFMDim * (OFMChannels / PE)>  mvOut (out,  reps);
   hls::stream<ap_uint<SIMD>> convInp("StreamingConvLayer_Batch.convInp");
   GEMM<ConvKernelDim, IFMChannels, 1, IFMDim,
			OFMDim, SIMD,1>(wa_in, convInp, reps);
	Wallace_Matrix_Vector_Activate_Batch<MatrixW, MatrixH, SIMD, PE>(static_cast<hls::stream<ap_uint<SIMD>>&>(convInp),
     static_cast<hls::stream<ap_uint<PE>>&>  (mvOut),
     weights, activation, reps* OFMDim * OFMDim);
                }
