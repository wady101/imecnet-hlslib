#include<ap_int.h>
#include<array>
// if targetdim4 = dim3 = dim2 = 0 
// then make sure we allocate a new buffer 
template<unsigned int DIM4,unsigned int DIM3,unsigned int DIM2,unsigned int DIM1>
class BinaryWeights {
	ap_uint<DIM1>  weights_m[DIM4][DIM3][DIM2];
	
	class TileIndex {
	BinaryWeights const &m_par;
	unsigned      const  m_idx;
	
	public:
	TileIndex(BinaryWeights const &par, unsigned const  idx)
	  : m_par(par), m_idx(idx) {
	#pragma HLS inline
	}
	
	public:
	ap_uint<SIMD> operator[](unsigned const  pe) const {
	#pragma HLS inline
	  return  m_par.weights_m[pe][m_idx];
	}
	};
	
	public:
	TileIndex weights(unsigned const  tile) const {
	#pragma HLS inline
	return  TileIndex(*this, tile);
	}
};

