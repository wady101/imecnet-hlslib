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
		//  unsigned int Input_precision,		
		 unsigned int IFMDim, 
		 unsigned int OFMDim,
		 unsigned int SIMD,
		 unsigned int Stride = 1>  
void ConvolutionInputGenerator(
		stream<ap_uint<SIMD> > & in,
		stream<ap_uint<SIMD> > & out,
		const unsigned int numReps = 1) {
        const unsigned int multiplying_factor = IFMChannels/SIMD;
  const unsigned int number_blocks = OFDim ;
  // rows*col
  // ap_uint<SIMD> I[IFMDim][IFMDim * multiplying_factor];
  // ap_uint<SIMD> W[IFMDim][ConvKernelDim * multiplying_factor];
  ap_uint<SIMD> L[OFMDim][IFMDim * ConvKernelDim * multiplying_factor];

#pragma HLS ARRAY_PARTITION variable=inputBuf complete dim=1
//#pragma HLS RESOURCE variable inputBuf core=RAM_2P_LUTRAM
#pragma HLS RESOURCE variable inputBuf core=RAM_2P_BRAM
  const unsigned int cycles_write_block = (OFMDim * ConvKernelDim * IFMDim * multiplying_factor);
  const unsigned int cycles_read_block = Stride * IFMDim * multiplying_factor;
  const unsigned int max_cycles = MAX(cycles_write_block,cycles_read_block);
  const unsigned int baseIter = IFMDim * ConvKernelDim * multiplying_factor// Initial buffer
			                  + OFMDim * MAX(cycles_write_block,cycles_read_block);
  unsigned int counter_internal_block = 0;
  unsigned int current_block_write = 0;
  unsigned int next_block_write = 0;	
  unsigned int current_line = 0;
  unsigned int read_block = 0; 
  unsigned int inp = 0, ofm_y = 0, ofm_x = 0, k_y = 0, k_x = 0, count_simd =0;
#pragma HLS reset variable=inp
  for (unsigned int count_image = 0; count_image < numReps; count_image++) {
    for (unsigned int i = 0; i < baseIter; i++) {
#pragma HLS PIPELINE II=1
      if (inp < IFMDim * ConvKernelDim*multiplying_factor) {// Initial buffer of ConvKernelDim lines	
        ap_uint<SIMD> inElem;
        inElem = in.read();
        L[current_block_write][current_line] = inElem;
        current_line++;
        inp++;
        if (current_line == IFMDim * ConvKernelDim * multiplying_factor ) {
          current_line = 0;
          current_block_write++;
          if (current_block_write == number_blocks) {
            current_block_write=0;
          }
          read_block++;
          counter_internal_block = 0;
        }
      } else {
        if (counter_internal_block < cycles_write_block-1) { // We are writing output, MMV IFMChan per cycle
          unsigned int current_block_read = (current_block_write + 1 + k_y / Stride);
          if (current_block_read >= number_blocks) {
            current_block_read-= number_blocks;
		  }
          unsigned int current_line_in_block = ((k_y%Stride) * IFMDim + ofm_x*Stride + k_x)*multiplying_factor + count_simd;
          ap_uint<SIMD> outElem = inputBuf[current_block_read][(current_line_in_block)];
          out.write(outElem);
          count_simd++;
          if (count_simd == multiplying_factor) { //LSB
            count_simd=0;					
            k_x++;
            if (k_x == ConvKernelDim) {
              k_x = 0;
              k_y++;
              if (k_y == ConvKernelDim) {
                k_y = 0;
                ofm_x ++;
                if (ofm_y == OFMDim) { //MSB
                  ofm_x = 0;
                  inp = 0;
                }
              }
            }
          }
        }
        if ((counter_internal_block < cycles_read_block-1) && (read_block<IFMDim/Stride)) { // In parallel we write in the buffer, in the current block write if we still need to
          ap_uint<SIMD> inElem;
          inElem = in.read();
           I[current_block_write][current_line] = inElem;
#pragma AP dependence variable=I intra false
#pragma AP dependence variable=I inter false
          current_line++;
          if (current_line == IFMDim *ConvKernelDim *  multiplying_factor) {// We read the whole block, we change the next block in which we want to we
            // We filled up a block, let's not read until
            current_line = 0;
            read_block++;
            current_block_write++;
            if (current_block_write == number_blocks) {
              current_block_write=0;
			}
#pragma AP dependence variable=current_block_write intra false	
          }
        }
        counter_internal_block++; // = (counter_internal_block +1) % max_cycles;
        if (counter_internal_block == (max_cycles-1)) {
          counter_internal_block = 0;
        }
      }
    } // End base_iter
	read_block = 0;
  } // End count_image

        }