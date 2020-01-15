#include <string>
#include <iostream>
#include <config>

#define ExtMemWord 36

// binarize an array of floating point values according to their sign and
// pack into a stream of bits
void binarizeAndPack(const vec_t & in, ExtMemWord * out, unsigned int inBufSize) {
  if(in.size() / bitsPerExtMemWord > inBufSize) {
    throw "Not enough space in input buffer";
  }
  // first, fill the target buffer with padding data
  memset(out, FOLDEDMV_INPUT_PADCHAR, inBufSize * sizeof(ExtMemWord));
  // now clear/set each bit position as needed
  for(unsigned int i=0; i < in.size(); i++) {
    if(in[i] >= 0) {
      // set bit
      out[i / bitsPerExtMemWord] |= ((ExtMemWord)1 << (i % bitsPerExtMemWord));
    } else {
      // clear bit
      out[i / bitsPerExtMemWord] &= ~((ExtMemWord)1 << (i % bitsPerExtMemWord));
    }
  }
}
