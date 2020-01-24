#include <string>
#include <iostream>
#include "config.h"



#include "donutdriver.hpp"

// Needed direct from FINN
typedef unsigned long long ExtMemWord; 
const unsigned int bitsPerExtMemWord = sizeof(ExtMemWord)*8;
const unsigned int bytesPerExtMemWord = sizeof(ExtMemWord)*2;

#ifndef VIRTUAL
  #define INPUT_BUF_ENTRIES     3840000
  #define OUTPUT_BUF_ENTRIES    160000
#else
  #define INPUT_BUF_ENTRIES		8192
  #define OUTPUT_BUF_ENTRIES	1024
#endif

#if defined(INCLUDE_TRANSFER_TIMES_IN_BENCHMARK) || defined(RAWHLS)
#define TRANSFER_EXCL(x) ;
#define TRANSFER_INCL(x) x;
#else
#define TRANSFER_EXCL(x) x;
#define TRANSFER_INCL(x) ;
#endif
/////

DonutDriver * initPlatform(bool cleanSIGINTExit = true);
void deinitPlatform(DonutDriver * driver);
#include <vector>

DonutDriver *thePlatform = 0;
void *accelBufIn, *accelBufOut;
ExtMemWord *bufIn, *bufOut;
void ExecAccel() {
  // invoke accelerator and wait for result
  thePlatform->writeJamRegAddr(0x00, 1);
  while((thePlatform->readJamRegAddr(0x00) & 0x2) == 0);
}

// TODO this variant always assumes an 8 byte val port on the accelerator
void Accel_load(unsigned int targetLayer, unsigned int targetMem, unsigned int targetInd, unsigned int targetThresh, ExtMemWord val) {
// void Accel_load(unsigned int targetLayer, ExtMemWord val) {

  // enable weight loading mode
  thePlatform->writeJamRegAddr(0x28, 1);
  // set up init data
  thePlatform->writeJamRegAddr(0x30, targetLayer);
  thePlatform->writeJamRegAddr(0x38, targetMem);
  thePlatform->writeJamRegAddr(0x40, targetInd);
  thePlatform->writeJamRegAddr(0x48, targetThresh);
  thePlatform->write64BitJamRegAddr(0x50, (AccelDblReg) val);
//   thePlatform->write64BitJamRegAddr(0x38, (AccelDblReg) val);

  // do write
  ExecAccel();
  // disable weight loading mode
  thePlatform->writeJamRegAddr(0x28, 0);
}

void FoldedMVInit(const char * attachName) {
  thePlatform = initPlatform();
  thePlatform->attach(attachName);
  // allocate input/output buffers
  // TODO should be dynamically sized based on the largest I/O
  if (!bufIn) {
    bufIn = new ExtMemWord[INPUT_BUF_ENTRIES];
    if (!bufIn){
      throw "Failed to allocated host buffer";
    }
  }
  if (!bufOut) {
    bufOut = new ExtMemWord[OUTPUT_BUF_ENTRIES];	
    if (!bufOut) {
      throw "Failed to allocated host buffer";			
    }
  }
  if (!accelBufIn) {
    accelBufIn = thePlatform->allocAccelBuffer(INPUT_BUF_ENTRIES * sizeof(ExtMemWord));
    if (!accelBufIn) {
      throw "Failed to allocate accel buffer";
    }
    accelBufOut = thePlatform->allocAccelBuffer(OUTPUT_BUF_ENTRIES * sizeof(ExtMemWord));
    if (!accelBufOut) {
      throw "Failed to allocate accel buffer";
    }
  }
  // set up I/O buffer addresses for the accelerator
  thePlatform->write64BitJamRegAddr(0x10, (AccelDblReg) accelBufIn);
  thePlatform->write64BitJamRegAddr(0x1c, (AccelDblReg) accelBufOut);
  thePlatform->writeJamRegAddr(0x28, 0);
}

void FoldedMVDeinit() {
  delete bufIn;
  delete bufOut;
  bufIn = 0;
  bufOut = 0;
  if (thePlatform && accelBufIn) {
    thePlatform->deallocAccelBuffer(accelBufIn);
  }
  accelBufIn = 0;
  if (thePlatform && accelBufOut) {
    thePlatform->deallocAccelBuffer(accelBufOut);
  }
  accelBufOut = 0;
  deinitPlatform(thePlatform);
  thePlatform = 0;
}

// TODO implement batch execution version
void FoldedMVOffloadBinarized(const ExtMemWord * in, ExtMemWord * out, const unsigned int inBufWords, const unsigned int outBufWords, const unsigned int numImages) {
  TRANSFER_INCL(thePlatform->copyBufferHostToAccel((void *)in, accelBufIn, sizeof(ExtMemWord) * inBufWords));
  // set number of images to stream through
  thePlatform->writeJamRegAddr(0x5C, numImages);
  // launch
  ExecAccel();
  // copy from accelerator output
  TRANSFER_INCL(thePlatform->copyBufferAccelToHost(accelBufOut, (void *)out, sizeof(ExtMemWord) * outBufWords));
}

// binarize an array of floating point values according to their sign and
// pack into a stream of bits
// void binarizeAndPack(const vec_t & in, ExtMemWord * out, unsigned int inBufSize) {
//   if(in.size() / bitsPerExtMemWord > inBufSize) {
//     throw "Not enough space in input buffer";
//   }
//   // first, fill the target buffer with padding data
//   memset(out, FOLDEDMV_INPUT_PADCHAR, inBufSize * sizeof(ExtMemWord));
//   // now clear/set each bit position as needed
//   for(unsigned int i=0; i < in.size(); i++) {
//     if(in[i] >= 0) {
//       // set bit
//       out[i / bitsPerExtMemWord] |= ((ExtMemWord)1 << (i % bitsPerExtMemWord));
//     } else {
//       // clear bit
//       out[i / bitsPerExtMemWord] &= ~((ExtMemWord)1 << (i % bitsPerExtMemWord));
//     }
//   }
// }
