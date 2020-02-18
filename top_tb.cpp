#include <iostream>
#include <cmath>
#include <ctime>
#include <cstring>
#include <hls_stream.h>
#include <cstdlib>
#define AP_INT_MAX_W 16384
#include "ap_int.h"
// #include "weights.hpp"
#include "bnn-library.h"
// #include "memdata.h"
#include "config_tb.hpp"
#include "activations.hpp"
#include "weights.hpp"
#include "activations.hpp"
#include "interpret.hpp"
// #include "wallace_convlayer.h"
// #include "wallace_GEMM.h"
// #include "wallace_mvau.hpp"
using namespace hls;
using namespace std;

template<int MAX_IMAGE,
	int IFMDim, 
	int OFMDim, 
	int IFMCh, 
	int OFMCh, 
	typename TI, 
	typename TO, 
	typename TW>
	void conv_1x1(TI const img[MAX_IMAGE][IFMDim][IFMDim][IFMCh], TW const weights[OFMCh][IFMCh], TO out[MAX_IMAGE][OFMDim][OFMDim][OFMCh]){
		constexpr int stride= (OFMDim==1)? IFMDim:(IFMDim - 1)/(OFMDim - 1);
		for(int n=0;n<MAX_IMAGE;n++)
			for(int x=0;x<OFMDim;x++)
				for(int y=0;y<OFMDim;y++)
					for(int h=0;h<OFMCh;h++){
						TO tmp = 0;
						for(int w=0;w<IFMCh;w++)
							tmp+=img[n][x*stride][y*stride][w] * weights[h][w];
						out[n][x][y][h] = tmp;

					}
	}

template<int MAX_IMAGE,
	int IFMDim,
	int OFMDim,
	int IFMCh,
	int OFMCh,
	int kernel,
	int stride,
	typename TI,
	typename TO,
	typename TW>
	void conv(TI const img[MAX_IMAGE][IFMDim*IFMDim][IFMCh], TW const weights[OFMCh][kernel][kernel][IFMCh], TO out[MAX_IMAGE][OFMDim][OFMDim][OFMCh]){
		for(int n=0;n<MAX_IMAGE;n++)
			for(int x=0;x<OFMDim;x++)
				for(int y=0;y<OFMDim;y++)
					for(int h=0;h<OFMCh;h++){
						TO tmp = 0;
						for (int ky=0;ky<kernel;ky++)
							for (int kx=0;kx<kernel;kx++)
								for(int w=0;w<IFMCh;w++){
									tmp+=img[n][(y*stride+ky)*IFMDim+x*stride+kx][w] * weights[h][kx][ky][w];
								}
						out[n][x][y][h] = tmp;
					}
	}


void Testbench_conv(stream<ap_uint<IFM_Channels1*INPUT_PRECISION> > & in, stream<ap_uint<OFM_Channels1*ACTIVATION_PRECISION> > & out, unsigned int numReps){
#pragma HLS DATAFLOW
	ConvLayer_Wallace<KERNEL_DIM, IFM_Channels1, IFMDim1, OFM_Channels1, OFMDim1, SIMD1, PE1, Slice<ap_uint<INPUT_PRECISION> >, Slice<ap_int<16> >>(in, out, PARAM::weights, PassThroughActivation<ap_uint<16>>(), numReps);
}
#define MAX_IMAGES 1
// void Testbench_conv(stream<ap_uint<IFM_Channels1*INPUT_PRECISION> > & in, stream<ap_uint<OFM_Channels1*ACTIVATION_PRECISION> > & out, unsigned int numReps);

int main()
{
  static	ap_uint<INPUT_PRECISION> IMAGE[MAX_IMAGES][IFMDim1*IFMDim1][IFM_Channels1];
	static	ap_int<ACTIVATION_PRECISION> TEST[MAX_IMAGES][OFMDim1][OFMDim1][OFM_Channels1];
	stream<ap_uint<IFM_Channels1*INPUT_PRECISION> > input_stream("input_stream");
	stream<ap_uint<OFM_Channels1*ACTIVATION_PRECISION> > output_stream("output_stream");
  unsigned int counter = 0;
  for (unsigned int n_image = 0; n_image < MAX_IMAGES; n_image++) {
		for (unsigned int oy = 0; oy < IFMDim1; oy++) {
			for (unsigned int ox = 0; ox < IFMDim1; ox++) {
				ap_uint<INPUT_PRECISION*IFM_Channels1> input_channel = 0;
				for(unsigned int channel = 0; channel < IFM_Channels1; channel++)
				{
          ap_uint<INPUT_PRECISION> input = (ap_uint<INPUT_PRECISION>)(counter);
					IMAGE[n_image][oy*IFMDim1+ox][channel]= input;
					input_channel = input_channel >> INPUT_PRECISION;
					input_channel(IFM_Channels1*INPUT_PRECISION-1,(IFM_Channels1-1)*INPUT_PRECISION)=input;

					counter++;
				}
				input_stream.write(input_channel);
			}
		}
	}
  	static	ap_int<4> W1[OFM_Channels1][KERNEL_DIM][KERNEL_DIM][IFM_Channels1];
	// initialize the weights
	constexpr int TX = (IFM_Channels1*KERNEL_DIM*KERNEL_DIM) / SIMD1;
	constexpr int TY = OFM_Channels1 / PE1;
	unsigned int kx=0;
	unsigned int ky=0;
	unsigned int chan_count=0;
	unsigned int out_chan_count=0;
	for (unsigned int oy = 0; oy < TY; oy++) {
		for (unsigned int ox = 0; ox <TX; ox++) {
			for(int pe=0;pe <PE1;pe++){
				for(int simd=0;simd<SIMD1;simd++){
					std::cout << " Value: "  << PARAM::weights.weights(oy*TX + ox)[pe][simd] <<std::endl;
					std::cout << "W1[" << out_chan_count << "][" << kx << "][" << ky << "][" << chan_count << "]"<< std::endl;
					W1[out_chan_count][kx][ky][chan_count] = PARAM::weights.weights(oy*TX + ox)[pe][simd];
					kx++;
					if (kx==KERNEL_DIM){
						kx=0;
					    ky++;
					    if (ky==KERNEL_DIM){
					    	ky=0;
					    	chan_count++;
						    if (chan_count==IFM_Channels1){
						    	chan_count=0;
						    	out_chan_count++;
							    if (out_chan_count==OFM_Channels1){
							    	out_chan_count=0;
							    }
						    }
					    }
					}
				}
			}
		}
	}
  conv<MAX_IMAGES,IFMDim1,OFMDim1,IFM_Channels1,OFM_Channels1, KERNEL_DIM, 1, ap_uint<INPUT_PRECISION> >(IMAGE, W1, TEST);
	Testbench_conv(input_stream, output_stream, MAX_IMAGES);
	int err_counter = 0, err_perimage=0;
	ap_int<ACTIVATION_PRECISION> out_chan;
	for (unsigned int n_image = 0; n_image < MAX_IMAGES; n_image++) {
		for (unsigned int oy = 0; oy < OFMDim1; oy++) {
			for (unsigned int ox = 0; ox < OFMDim1; ox++) {
				for(int e=0;e<1;e++){
					ap_uint<OFM_Channels1*ACTIVATION_PRECISION> outElem = output_stream.read();
					for(unsigned int channel = 0; channel < OFM_Channels1; channel++){
						ap_int<ACTIVATION_PRECISION> EXP = TEST[n_image][ox][oy][channel + e * OFM_Channels1];
						out_chan(ACTIVATION_PRECISION-1,0) = outElem((channel + 1)*ACTIVATION_PRECISION-1,channel*ACTIVATION_PRECISION);

						if (EXP != out_chan){
							std::cout << "ERROR: Expected["<<oy <<"]["<<ox<<"]["<<channel<<"]=" << EXP << " actual " <<  out_chan << std::endl;
							//return 1;
							err_counter ++;
							err_perimage++;
							//if(err_counter>10)
								//return 1;
						}
					}
				}
			}
		}
		if(err_perimage == 0){
			std::cout << "Image # " << n_image << " passed the testing."<< std::endl;
		}
		else{
			err_perimage=0;
			std::cout << "Image # " << n_image << " failed the testing."<< std::endl;
		}
	}
	if(err_counter == 0){
		return 0;
	}
	else{
		return 1;
	}
}