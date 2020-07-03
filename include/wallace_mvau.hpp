
#include "hls_stream.h"
#include <iostream> 
using namespace std;
#include "interpret.hpp"
#include "utils.hpp"


/**
 * \brief      Multipliy operation between 2 operands, HLS choose the best resource
 * 
 * The same multiply operation can be implemented using multiple Vivado HLS pragmas to select the 
 * hardware resource to be used:
 * ap_resource_dflt will let HLS choose the best one
 * ap_resource_lut will force HLS to implement the multiplier in LUTs
 * ap_resource_dsp will force HLS to implement the multiplier in DSP48
 *
 * \tparam     TC    First operand datatype (weights)
 * 
 * \tparam     TD    Second operand datatype (input)
 * 
 * \param      c     First operand (array of weights)
 * \param      d     Second operand (array of input activation)
 * \param      r     Resource type for the hardware implementation of the MAC block
 *
 * \return     Result of the multiply operation
 */
template<typename TC, typename TD>
auto mul(TC const &c, TD const &d, ap_resource_dflt const&) -> decltype(c*d) {
#pragma HLS inline
  auto  r = c*d;
  return  r;
}

/**
 * \brief      Multiply operation between 2 operands, implemented in LUT
 * 
 * The same multiply operation can be implemented using multiple Vivado HLS pragmas to select the 
 * hardware resource to be used:
 * ap_resource_dflt will let HLS choose the best one
 * ap_resource_lut will force HLS to implement the multiplier in LUTs
 * ap_resource_dsp will force HLS to implement the multiplier in DSP48
 *
 * \tparam     TC    First operand datatype (weights)
 * \tparam     TD    Second operand datatype (input)
 * 
 * \param      c     First operand (array of weights)
 * \param      d     Second operand (array of input activation)
 * \param      r     Resource type for the hardware implementation of the MAC block
 *
 * \return     Result of the multiply operation
 */
template<typename TC, typename TD>
auto mul(TC const &c, TD const &d) -> decltype(c*d) {
#pragma HLS inline
  decltype(c*d) const  res = c*d;
#pragma HLS RESOURCE variable=res core=Mul_LUT
  return  res;
}




/**
 * \brief      MAC with selectable implementation resource, used by Matrix_Vector_Activate_Batch
 *
 * \tparam     N     Number of MAC to be performed (equals to SIMD in mvau)
 * \tparam     T     Accumulator datatype
 * \tparam     TC    First operand datatype (weights)
 * \tparam     TD    Second operand datatype (input)
 * \tparam     R     Datatype for the resource used for FPGA implementation of the MAC  - safely deducible from the paramaters
 * 
 * \param      a     Initialization value of the accumulation
 * \param      c     First operand (array of weights)
 * \param      d     Second operand (array of input activation)
 * \param      r     Resource type for the hardware implementation of the MAC block
 *
 * \return     Result of the MAC operation
 */
template<unsigned N, typename T, typename TC, typename TD>
T mac(T const &a, TC const &c, TD const &d) {
#pragma HLS inline
  T  res = a;
  for(unsigned  i = 0; i < N; i++) { //SIMD length going in
#pragma HLS unroll
    res += mul(c[i], d[i]);
  }
  return  res;
}
// template<unsigned N, typename T, typename TC, typename TD>
// inline T mac(T const &a, TC const &c, TD const &d) {
// #pragma HLS inline
//   return  mac<N>(a, c, d, ap_resource_dflt());
// }


template<
  unsigned MatrixW, unsigned MatrixH, unsigned SIMD, unsigned PE, unsigned WMEM, unsigned TMEM,
   unsigned Outer,
  typename TI, typename TO, typename TW, typename TA, typename Compare = std::less<TA>
>
void Wallace_Matrix_Vector_Activate_Batch(hls::stream<TI> &in,
				  hls::stream<TO> &out,
				  TW  const weights[][WMEM],
				  TA  const activation[][TMEM],
				  int const  reps
          ) {


  // k_n / PE or OFMChannels / PE 
  // number of vertical matrix chunks of O
  unsigned const  NF = MatrixH / PE; 

  // k_w*k_w*i_c / SIMD or 
  // number of horizontal matrix chunks of K and (2nd+4th dim of L)
  unsigned const  SF = MatrixW / SIMD;

  // input vector buffers
  TI  inputBuf[SF];
  cout<<"SF "<<SF<<endl;
#pragma HLS ARRAY_PARTITION variable=inputBuf complete dim=0

  // decltype(activation.init(0,0))  accu[MMV][PE];
  ap_uint<SIMD> accu[Outer][PE];
#pragma HLS ARRAY_PARTITION variable=accu complete dim=0

  unsigned  nf   = 0;
  unsigned  sf   = 0;
  unsigned  tile = 0; // invariant: tile = nf*SF + sf
  unsigned tmem = 0;

  // everything merged into a common iteration space (one "big" loop instead
  // of smaller nested loops) to get the pipelinening the way we want
  unsigned const TOTAL_FOLD = NF * SF;
  for(unsigned  i = 0; i < reps * TOTAL_FOLD; i++) {
#pragma HLS PIPELINE II=1
for (unsigned of = 0; of<Outer; of++){
    TI  inElem;
    if(nf == 0) {
      // read input from stream
      inElem = in.read();
      // store in appropriate buffer for reuse
      inputBuf[sf] = inElem;
    }
    else {
      // reuse buffered input
      inElem = inputBuf[sf];
    }
    // Initialisation for accu[of][pe]
    if(sf == 0) {
     for(unsigned of=0; of<Outer; of++){ 
       #pragma HLS UNROLL
      for(unsigned  pe = 0; pe < PE; pe++) {
#pragma HLS UNROLL
          accu[of][pe] = 0;
      }
     }
    }

    // compute matrix-vector product for each processing element
    //for(unsigned of=0;of<Outer; of++) {  // This is just keeping accu const to same value for Outer iterations as inElem is not updating . also ineffeicient
      //#pragma HLS UNROLL
    for(unsigned  pe = 0; pe < PE; pe++) {
#pragma HLS UNROLL
        accu[of][pe] = mac<SIMD>(accu[of][pe], weights[pe][tile], inElem);
        // printf("input %s weights %s accu  %s \n", inElem.to_string().c_str(),weights[pe][tile].to_string().c_str(),accu[of][pe].to_string().c_str());
        

    }
    //}
  } // EOL
  // keep track of which line we are processing. tile indicates next line for weights 
    ++tile; // tile = WMEM
    if(++sf == SF) {
      // produce output and clear accumulators
      // auto  outElem = Identity().template operator()<TO>();
      TO outElem;
      for (unsigned of=0; of<Outer; of++) {
        #pragma HLS UNROLL
      for (unsigned  pe = 0; pe < PE; pe++) {
#pragma HLS UNROLL
          // outElem(pe,0) = Compare()(activation[pe][nf], accu[pe]) << pe;
          // outElem(of*PE+pe,of*PE) = accu[of][pe];//activation[pe][tmem] >= accu[of][pe]? 1:0; 
     
          // outElem(of*SIMD+pe,of*SIMD) =  accu[of][pe];//activation[pe][tmem] >= accu[of][pe]? 1:0; 
          
          //This has to return a single bit value, else outElem is only going to take single-bit input. 
          outElem(pe,0) =  accu[of][pe];//activation[pe][tmem] >= accu[of][pe]? 1:0; 
          

          // outElem(pe,1) = accu[pe];
          
          // outElem(pe,1) = activation.activate(nf, pe, accu[pe]);
      }
      //  printf("outELem  %s \n", outElem.to_string().c_str());
      out.write(outElem);
      }
      // next folded neuron or image
      sf = 0;
      if(++nf == NF) {
	    nf   = 0;
	    tile = 0;
      }
      if(++tmem == TMEM) {
        tmem = 0;
      }
    }
  }
}