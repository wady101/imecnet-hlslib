#include <ap_int.h>
#include <hls_stream.h>
using namespace hls;

#define MAX(x, y) (((x) > (y)) ? (x) : (y)) /* \brief Maximum value between x and y*/
#define MIN(x, y) (((x) > (y)) ? (y) : (x)) /* !< \brief Minimum value between x and y*/


/**
 * \brief Sliding Window unit that produces output vectors for feeding
 * a Matrix_Vector_Activate_Batch, implementing the im2col algorithm. To be used only if 
 * ConvKernelDim%Stride = 0
 *
 * \tparam ConvKernelDim    Dimension of the convolutional kernel (assumed square)
 * \tparam IFMChannels      Number of Input Feature Maps
 * \tparam Input_precision  Number bits per pixel
 * \tparam IFMDim           Width and Heigth of the Input Feature Map (assumed square)
 * \tparam OFMDim           Width and Heigth of the Output Feature Map (assumed square)
 * \tparam SIMD             Number of input columns computed in parallel
 * \tparam Stride           Stride of the convolutional kernel
 * 
 * \param in                Input stream
 * \param out               Output stream
 * \param numReps           Number of time the function has to be repeatedly executed (e.g. number of images)
 */
template<unsigned int ConvKernelDim, 
		 unsigned int IFMChannels,
		 unsigned int Input_precision,		
		 unsigned int IFMDim, 
		 unsigned int OFMDim,
		 unsigned int SIMD,
		 unsigned int Stride = 1>  
void MEC(
		stream<ap_uint<SIMD*Input_precision> > & in,
		stream<ap_uint<SIMD*Input_precision> > & out,
		const unsigned int numReps ) {
  const unsigned int multiplying_factor = IFMChannels/SIMD;
  const unsigned int number_blocks = ConvKernelDim/Stride + 1 ;
  ap_uint<SIMD*Input_precision> inputBuf[number_blocks][Stride * IFMDim * multiplying_factor];

#pragma HLS ARRAY_PARTITION variable=inputBuf complete dim=1
#pragma HLS RESOURCE variable inputBuf core=RAM_2P
  const unsigned int cycles_write_block = (OFMDim * ConvKernelDim * ConvKernelDim * multiplying_factor);
  const unsigned int cycles_read_block = Stride * IFMDim * multiplying_factor;
  const unsigned int max_cycles = MAX(cycles_write_block,cycles_read_block);
  const unsigned int baseIter = IFMDim*IFMDim * ConvKernelDim * multiplying_factor// Initial buffer
			                  + OFMDim * MAX(cycles_write_block,cycles_read_block);
  unsigned int counter_internal_block = 0;
  unsigned int current_block_write = 0;
  unsigned int next_block_write = 0;	
  unsigned int current_line = 0;
  unsigned int read_block = 0; 
  unsigned int inp = 0, ofm_y = 0, oo_x = 0, k_y = 0, o_x = 0, count_simd =0;
#pragma HLS reset variable=inp
  for (unsigned int count_image = 0; count_image < numReps; count_image++) {
    for (unsigned int i = 0; i < baseIter; i++) {
#pragma HLS PIPELINE II=1
      if (inp < IFMDim * ConvKernelDim*multiplying_factor) {// Initial buffer of ConvKernelDim lines	
        ap_uint<SIMD*Input_precision> inElem;
        inElem = in.read();
        printf("DEBUG Input is  %s \n", inElem.to_string().c_str());
        inputBuf[current_block_write][current_line] = inElem; //buffer size is row=IFMDim col=ConvKernelDim/Stride
        current_line++;
        inp++;
        if (current_line == Stride * IFMDim * multiplying_factor ) { // complete x-axis of I
          current_line = 0;
          current_block_write++;
          // std::cout<<"DEBUG = "<<count_image<< " Block_write_1 " << current_block_write<<std::endl;
          if (current_block_write == number_blocks) { //complete y-axis of I till kernel dim
            current_block_write=0;
          }
          read_block++;
          counter_internal_block = 0;
        }
      } else {
        if (counter_internal_block < cycles_write_block-1) { // We are writing output, MMV IFMChan per cycle
          unsigned int current_block_read = (current_block_write + 1 + k_y / Stride);
          // std::cout<<"DEBUG = "<<count_image<< " before" << k_y<<std::endl;
          if (current_block_read >= number_blocks) {
            current_block_read-= number_blocks;
		  }
          unsigned int current_line_in_block = (oo_x*Stride + o_x)*multiplying_factor + count_simd;
          ap_uint<SIMD*Input_precision> outElem = inputBuf[current_block_read][(current_line_in_block)];
          // std::cout<<"DEBUG = "<<count_image<< " " << current_block_read<<std::endl;
          out.write(outElem);
          printf("outELem  %s \n", outElem.to_string().c_str());
        
          

          count_simd++;
          if (count_simd == multiplying_factor) {
            count_simd=0;					
            o_x++; // iteration of each row
            if (o_x == OFMDim) {
              o_x = 0;
              oo_x++;   //right increment of row
              if (oo_x == ConvKernelDim) {
                oo_x = 0;
                k_y ++; //y axis down
              if (k_y == ConvKernelDim) {
                k_y = 0;
                inp = 0;
                ofm_y ++; //y axis down
              }
              
              }
            }
          }
        }
    // Commented this entire if block -> because I am missing an entire write cycle when I am using this i.e. outELem is printed out thrice for Input1 to be inputted thrice and Input2 once. If u r using this block change baseItr 
    //line to = IFMDim * ConvKernelDim * multiplying_factor// Initial buffer 
    // + rest of max_cycles
//         if ((counter_internal_block < cycles_read_block-1) && (read_block<(IFMDim- ConvKernelDim)/Stride )) { // In parallel we write in the buffer, in the current block write if we still need to
//           ap_uint<SIMD*Input_precision> inElem;
//           inElem = in.read();
//           inputBuf[current_block_write][current_line] = inElem;
//            printf("DEBUG Input2 is  %s \n", inElem.to_string().c_str());
       
// #pragma AP dependence variable=inputBuf intra false
// #pragma AP dependence variable=inputBuf inter false
//           current_line++;
//          // inp++;
//           if (current_line == Stride * IFMDim * multiplying_factor) {// We read the whole block, we change the next block in which we want to we
//             // We filled up a block, let's not read until
//             current_line = 0;
//             read_block++;
//             current_block_write++;
//             // std::cout<<"DEBUG = "<<count_image<< " Block_write " << current_block_write<<std::endl;
          
//             if (current_block_write == number_blocks) {
//               current_block_write=0;
//               #pragma AP dependence variable=current_block_write intra false	
// 			}
//           }
//         }
        counter_internal_block++; // = (counter_internal_block +1) % max_cycles;
        if (counter_internal_block == (max_cycles-1)) { 
          counter_internal_block = 0;
        }
      }
    } // End base_iter
	read_block = 0;
  // inp = 0; // Pipeline will stall becauze of this
  } // End count_image
} // End generator


// I had a small question in the implementation of the sliding window unit (slidingwindow.h).
//  In line https://github.com/Xilinx/finn-hlslib/blob/8b7f5f5dcd4e3dd17eddc41253040931af28179a/slidingwindow.h#L107 Why can't we only use the below if-else  block for reading ? 

// https://github.com/Xilinx/finn-hlslib/blob/8b7f5f5dcd4e3dd17eddc41253040931af28179a/slidingwindow.h#L153 
// Authors/Fellow members a real quick question here again.  Why are there 2 read blocks writing to *inputBuf* buffer in https://github.com/Xilinx/finn-hlslib/blob/master/slidingwindow.h ? I do understand the *inp* variable read block is used to 'kick-start' the writing to the buffer *inputBuf* (so that , but then why is there another concurrent read_block writing to *inputBuf*? For reference blocks - **Block1--** https://github.com/Xilinx/finn-hlslib/blob/d657d4dd36e19e488eeead67ee67c7b8acd33369/slidingwindow.h#L107 and  **Block2--** https://github.com/Xilinx/finn-hlslib/blob/d657d4dd36e19e488eeead67ee67c7b8acd33369/slidingwindow.h#L153 