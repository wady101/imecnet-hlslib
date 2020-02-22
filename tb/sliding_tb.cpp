#include "wallace_MEC.h"
#include "config_tb.hpp"
#include <ap_int.h>
#include <hls_stream.h>
using namespace hls;
#define MAX_IMAGES 2

void Testbench(stream<ap_uint<IFM_Channels*INPUT_PRECISION> > & in, stream<ap_uint<IFM_Channels*INPUT_PRECISION> > & out, unsigned int numReps);


int main()
{
static	ap_uint<INPUT_PRECISION> INPUT_IMAGES[MAX_IMAGES][IFMDim*IFMDim][IFM_Channels];
stream<ap_uint<IFM_Channels*INPUT_PRECISION> > input_stream("input_stream");
stream<ap_uint<IFM_Channels*INPUT_PRECISION> > output_stream("output_stream");
	unsigned int counter = 0;
	for (unsigned int n_image = 0; n_image < MAX_IMAGES; n_image++) {
		for (unsigned int oy = 0; oy < IFMDim; oy++) {
			for (unsigned int ox = 0; ox < IFMDim; ox++) {
				ap_uint<INPUT_PRECISION*IFM_Channels> input_channel = 0;
				for(unsigned int channel = 0; channel < IFM_Channels; channel++)
				{
					ap_uint<INPUT_PRECISION> input = (ap_uint<INPUT_PRECISION>)(counter);
					INPUT_IMAGES[n_image][oy*IFMDim+ox][channel]= input;
					input_channel = input_channel >> INPUT_PRECISION;
					input_channel(IFM_Channels*INPUT_PRECISION-1,(IFM_Channels-1)*INPUT_PRECISION)=input;
					counter++;

					
				}
				
				for (unsigned int channel = 0; channel < IFM_Channels; channel++)
				std::cout<<INPUT_IMAGES[n_image][oy*IFMDim+ox][channel];
				std::cout<<" Layer" << n_image << " Data "<< input_channel<<std::endl;
				printf("Data channel being sent  %s \n", input_channel.to_string().c_str());
				input_stream.write(input_channel);
			}
		}
	}
	// while(!input_stream.empty()) // check for input_stream stalling
    MEC<KERNEL_DIM, IFM_Channels,INPUT_PRECISION, IFMDim, OFMDim, SIMD>(input_stream, output_stream, MAX_IMAGES);
	unsigned int of_y=0;
	for (unsigned int n_image = 0; n_image < MAX_IMAGES; n_image++) {

					for (unsigned int ky = 0; ky < IFM_Channels*KERNEL_DIM; ky++) {	
						for (unsigned int kx = 0; kx < KERNEL_DIM; kx++) {
							for (unsigned int ox = 0; ox < OFMDim; ox++) {
						unsigned int input_leap = IFMDim*ky;	//Only required because of flattened INPUT_IMAGES[.][FLAT][.]
						unsigned int input_ind = input_leap+kx*STRIDE + ox;
						ap_uint<IFM_Channels*INPUT_PRECISION> outElem = output_stream.read();
						for(unsigned int channel = 0; channel < IFM_Channels; channel++){
							ap_uint<INPUT_PRECISION> out_chan = 0;
							out_chan = outElem(INPUT_PRECISION-1,0);
							if (((INPUT_IMAGES[n_image][input_ind][channel])) != out_chan){
								std::cout << "ERROR: " <<  " Expected " << INPUT_IMAGES[n_image][input_ind][channel] << " actual " <<  out_chan << std::endl;
								std::cout << " ox= " << ox << " kx= " << kx << " ky= " << ky << " of_y= " << of_y << std::endl;
								return 1;
							}
							printf("Data channel recieved  %s \n", out_chan.to_string().c_str());
							outElem = outElem >> INPUT_PRECISION;
						}
			}
		}
		}
		std::cout << "Image # " << n_image << std::endl;
	}
	return 0;

}

