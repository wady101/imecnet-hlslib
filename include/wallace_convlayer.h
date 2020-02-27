#include <ap_int.h>
#include <hls_stream.h>

#include "streamtools.h"

#include "wallace_MEC.h"
#include "wallace_mvau.hpp"

template<
		unsigned int ConvKernelDim,		
		unsigned int IFMChannels,		
		unsigned int IFMDim,			
		unsigned int OFMChannels,		
		unsigned int OFMDim,			
		
		unsigned int SIMD, 				// number of SIMD lanes
		unsigned int PE,				// number of PEs
		unsigned int WMEM,
		unsigned int TMEM,
		typename TW, typename TA ,
		int InStreamW, int OutStreamW  // safely deducible (stream width must be int though!)
		
>
void ConvLayer_Wallace(hls::stream<ap_uint<InStreamW>>  &in,
			    hls::stream<ap_uint<OutStreamW>> &out,
			    TW  weights[][WMEM],
			    TA  activation[][TMEM],
			    unsigned const   reps
				){
	#pragma HLS INLINE OFF
	#pragma HLS DATAFLOW //remove this
    unsigned const MatrixW = ConvKernelDim* ConvKernelDim * IFMChannels;
	unsigned const Outer = OFMDim*IFMChannels;
    unsigned const MatrixH = OFMChannels;
    unsigned const InpPerImage = ConvKernelDim* IFMDim*IFMDim*IFMChannels/InStreamW; //Dimension of entire image * Kernel because of repepting of conv window
	
   WidthAdjustedInputStream <InStreamW, SIMD, InpPerImage>  wa_in (in,  reps);
   WidthAdjustedOutputStream <PE, OutStreamW, OFMDim * OFMDim * (OFMChannels / PE)>  mvOut (out,  reps);
   hls::stream<ap_uint<SIMD>> convInp("StreamingConvLayer_Batch.convInp");
   MEC<ConvKernelDim, IFMChannels, 1, IFMDim,
			OFMDim, SIMD,1>(wa_in, convInp, reps);
	std::cout<<"GEMM performed" << std::endl;
	Wallace_Matrix_Vector_Activate_Batch<MatrixW, MatrixH, SIMD, PE, WMEM, TMEM, Outer>(static_cast<hls::stream<ap_uint<SIMD>>&>(convInp),
     static_cast<hls::stream<ap_uint<PE>>&>  (mvOut),
     weights, activation, reps* OFMDim * OFMDim);
	 std::cout<<"MVAU performed" << std::endl;
                }
	
