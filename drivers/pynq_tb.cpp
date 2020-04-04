#include <iostream>
#include <string.h>
#include <chrono>
#include <algorithm>
#include "tiny_cnn/tiny_cnn.h"
#include "tiny_cnn/util/util.h"
#include "ap_int.h"

using namespace std;
using namespace tiny_cnn;

typedef unsigned long long ExtMemWord;

const unsigned int bitsPerExtMemWord = sizeof(ExtMemWord)*8;

#define FOLDEDMV_INPUT_PADCHAR  0

void BlackBoxJam(ap_uint<64> * in, ap_uint<64> * out, bool doInit, unsigned int targetLayer, unsigned int targetMem, unsigned int targetInd, unsigned int targetThresh, ap_uint<64> val, unsigned int numReps);

#if defined(RAWHLS) && defined(OFFLOAD)

void FoldedMVOffloadBinarized(const ExtMemWord * in, ExtMemWord * out, const unsigned int inBufWords, const unsigned int outBufWords, const unsigned int numImages) {
  // call the accelerator in compute mode
  BlackBoxJam((ap_uint<64> *)in, (ap_uint<64> *)out, true, 0, 0, 0, 0, 0, numImages);
}

#endif

// return in padded to a multiple of padTo
unsigned int paddedSize(unsigned int in, unsigned int padTo) {
  if(in % padTo == 0) {
    return in;
  } else {
    return in + padTo - (in % padTo);
  }
}

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

// unpack a stream of bit and debinarize them into -1 and +1 floating point
// values (where a 0 bit is -1 and 1 bit is +1)
void unpackAndDebinarize(const ExtMemWord * in, vec_t &out) {
  for(unsigned int i = 0; i < out.size(); i++) {
    if((in[i / bitsPerExtMemWord] >> (i % bitsPerExtMemWord)) & 0x1) {
      out[i] = 1;
    } else {
      out[i] = -1;
    }
  }
}


std::vector<int> testPrebinarized_nolabel(std::vector<vec_t> & imgs, const unsigned int labelBits, float &usecPerImage) {
  // TODO support labelBits > bitsPerExtMemWord
  if(labelBits > bitsPerExtMemWord) {
    throw "labelBits > bitsPerExtMemWord not yet supported";
  }
  const unsigned int count = 1;
  cout << "Running prebinarized test for " << count << " images..." << endl;
  // compute the number of words needed to store the each img and label in binarized form
  const unsigned int psi = paddedSize(imgs[0].size(), bitsPerExtMemWord) / bitsPerExtMemWord;
  const unsigned int psl = paddedSize(labelBits, bitsPerExtMemWord) / bitsPerExtMemWord;
  // allocate buffers for binarized input and output data
  ExtMemWord * binImages = new ExtMemWord[(count * psi)];

  // binarize each image and label
  for(unsigned int i = 0; i < count; i++) {
    binarizeAndPack(imgs[i], &binImages[i * psi], psi);
  }
  unsigned int r = 0;

  // recognize
  unsigned int ok = 0, failed = 0;
  ExtMemWord * outLabel = new ExtMemWord[(count * psl)];
  // TRANSFER_EXCL(thePlatform->copyBufferHostToAccel((void *)binImages, accelBufIn, sizeof(ExtMemWord) * count * psi));
  
  auto t1 = chrono::high_resolution_clock::now();
  FoldedMVOffloadBinarized(binImages, outLabel, count * psi, count * psl, count);
  auto t2 = chrono::high_resolution_clock::now();
  
  // TRANSFER_EXCL(thePlatform->copyBufferAccelToHost(accelBufOut, (void *)outLabel, sizeof(ExtMemWord) * count * psl));
  // compare against labels
  uint64_t mask_output = 0xFFFFFFFFFFFFFFFF;
  mask_output = mask_output >> (64 - labelBits);
  for(unsigned int i = 0; i < count; i++) {
    outLabel[i * psl] = outLabel[i * psl] & mask_output;
  }
  auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
  usecPerImage = (float)duration / (count);
  cout << "Inference took " << duration << " microseconds, " << usecPerImage << " usec per image" << endl;
  cout << "Classification rate: " << 1000000.0 / usecPerImage << " images per second" << endl;
  std::vector<int> result;
  double internal_result;
  if (outLabel[0] == 0) {
    internal_result = 0;
  } else {
    internal_result = round(log2((double) outLabel[0]));
  }
  for (unsigned int i = 0 ; i < 64; i++) {
    if (i!=(unsigned int) internal_result) {
      result.push_back(0);
    } else { 
      result.push_back(1);
    }
  }
  delete[] outLabel;
  delete[] binImages;
  return result;
}

extern "C" int* inference(const char* path, int results[64], int number_class, float *usecPerImage, unsigned int enable_detail = 0) {
   std::vector<label_t> test_labels;
  std::vector<vec_t> test_images;
  std::vector<int> all_result;
  float usecPerImage_int;
  vec_t image;
  int * result;

//   FoldedMVInit("lfcW1A1-pynq");
//   network<mse, adagrad> nn;
//   makeNetwork(nn);	
  parse_cifar10(path, &test_images, &test_labels, -1.0, 1.0, 0, 0);
  image = test_images[0];
  all_result=testPrebinarized_nolabel(test_images, number_class, usecPerImage_int);

  if(results) {
    std::copy(all_result.begin(),all_result.end(), results);
  } 
  if (usecPerImage) {
    *usecPerImage = usecPerImage_int;
  }
  result = new int [all_result.size()];
  std::copy(all_result.begin(),all_result.end(), result);
  return result;
}

extern "C" void free_results(int * result) {
  delete[] result;
}

extern "C" int main(int argc, char** argv) {
  if (argc != 4) {
    cout << "3 parameters are needed: " << endl;
    cout << "1 - path to image to be classified" << endl;
    cout << "2 - number of classes in the dataset" << endl;
    cout << "3 - expected result" << endl;
    return 1;
  }

  float execution_time = 0;
  int * class_inference ;
  class_inference = 0;
  int scores[64];

  class_inference = inference(argv[1], scores, atol(argv[2]), &execution_time);	

  cout << "Detected class " << class_inference << " in " << execution_time << " microseconds" << endl;	
  if (*class_inference != atol(argv[3])) {
    return 1;
  } else {
    return 0;
  }
}