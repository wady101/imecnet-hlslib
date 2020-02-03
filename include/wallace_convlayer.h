#include <ap_int.h>
#include <hls_stream.h>

#include "streamtools.h"
#include "mvau.hpp"
template<
		unsigned int ConvKernelDim,		
		unsigned int IFMChannels,		
		unsigned int IFMDim,			
		unsigned int OFMChannels,		
		unsigned int OFMDim,			
		
		unsigned int SIMD, 				// number of SIMD lanes
		unsigned int PE,				// number of PEs
		int InStreamW, int OutStreamW,  // safely deducible (stream width must be int though!)
>
void ConvLayer_Wallace(hls::stream<ap_uint<InStreamW>>  &in,
			    hls::stream<ap_uint<OutStreamW>> &out,
			    TW const        &weights,
			    TA const        &activation,
			    unsigned const   reps,
				R const &r){
    unsigned const MatrixW = ConvKernelDim * ConvKernelDim * IFMChannels;
    unsigned const MatrixH = OFMChannels;
    unsigned const InpPerImage = IFMDim*ConvKernelDim*IFMChannels/InStreamW; //Not sure about the division
   WidthAdjustedInputStream <InStreamW, SIMD*TSrcI::width, InpPerImage>  wa_in (in,  reps);
   WidthAdjustedOutputStream <PE*TDstI::width, OutStreamW, OFMDim * OFMDim * (OFMChannels / PE)>  mvOut (out,  reps);
   hls::stream<ap_unint<SIMD>> convInp("StreamingConvLayer_Batch.convInp");
   
    
                }
