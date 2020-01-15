#include <accel_loader.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include <filesystem>
namespace fs = std::filesystem;
// std::vector<int> testPrebinarized_nolabel(std::vector<vec_t> & imgs, const unsigned int labelBits, float &usecPerImage) {
//   if(labelBits > bitsPerExtMemWord) {
//     throw "labelBits > bitsPerExtMemWord not yet supported";
//   }
//   const unsigned int count = 1;
//   cout << "Running prebinarized test for " << count << " images..." << endl;
//   // compute the number of words needed to store the each img and label in binarized form
//   const unsigned int psi = paddedSize(imgs[0].size(), bitsPerExtMemWord) / bitsPerExtMemWord;
//   const unsigned int psl = paddedSize(labelBits, bitsPerExtMemWord) / bitsPerExtMemWord;
//   // allocate buffers for binarized input and output data
//   ExtMemWord * binImages = new ExtMemWord[(count * psi)];

//   // binarize each image and label
//   for(unsigned int i = 0; i < count; i++) {
//     binarizeAndPack(imgs[i], &binImages[i * psi], psi);
//   }
//   unsigned int r = 0;

//   // recognize
//   unsigned int ok = 0, failed = 0;
//   ExtMemWord * outLabel = new ExtMemWord[(count * psl)];
//   TRANSFER_EXCL(thePlatform->copyBufferHostToAccel((void *)binImages, accelBufIn, sizeof(ExtMemWord) * count * psi));
  
//   auto t1 = chrono::high_resolution_clock::now();
//   FoldedMVOffloadBinarized(binImages, outLabel, count * psi, count * psl, count);
//   auto t2 = chrono::high_resolution_clock::now();
  
//   TRANSFER_EXCL(thePlatform->copyBufferAccelToHost(accelBufOut, (void *)outLabel, sizeof(ExtMemWord) * count * psl));
//   // compare against labels
//   uint64_t mask_output = 0xFFFFFFFFFFFFFFFF;
//   mask_output = mask_output >> (64 - labelBits);
//   for(unsigned int i = 0; i < count; i++) {
//     outLabel[i * psl] = outLabel[i * psl] & mask_output;
//   }
//   auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
//   usecPerImage = (float)duration / (count);
//   cout << "Inference took " << duration << " microseconds, " << usecPerImage << " usec per image" << endl;
//   cout << "Classification rate: " << 1000000.0 / usecPerImage << " images per second" << endl;
//   std::vector<int> result;
//   double internal_result;
//   if (outLabel[0] == 0) {
//     internal_result = 0;
//   } else {
//     internal_result = round(log2((double) outLabel[0]));
//   }
//   for (unsigned int i = 0 ; i < 64; i++) {
//     if (i!=(unsigned int) internal_result) {
//       result.push_back(0);
//     } else { 
//       result.push_back(1);
//     }
//   }
//   delete[] outLabel;
//   delete[] binImages;
//   return result;
// }

// std::vector<int> testPrebinarized_nolabel_multiple_images(std::vector<vec_t> & imgs, const unsigned int labelBits, float &usecPerImage) {
//   // TODO support labelBits > bitsPerExtMemWord
//   if(labelBits > bitsPerExtMemWord) {
//     throw "labelBits > bitsPerExtMemWord not yet supported";
//   }
//   const unsigned int count = imgs.size();
//   cout << "Running prebinarized test for " << count << " images..." << endl;
//   // compute the number of words needed to store the each img and label in binarized form
//   const unsigned int psi = paddedSize(imgs[0].size(), bitsPerExtMemWord) / bitsPerExtMemWord;
//   const unsigned int psl = paddedSize(labelBits, bitsPerExtMemWord) / bitsPerExtMemWord;
//   // allocate buffers for binarized input and output data
//   ExtMemWord * binImages = new ExtMemWord[(count * psi)];

//   // binarize each image and label
//   for(unsigned int i = 0; i < count; i++) {
//     binarizeAndPack(imgs[i], &binImages[i * psi], psi);
//   }
//   unsigned int r = 0;

//   // recognize
//   unsigned int ok = 0, failed = 0;
//   ExtMemWord * outLabel = new ExtMemWord[(count * psl)];
//   TRANSFER_EXCL(thePlatform->copyBufferHostToAccel((void *)binImages, accelBufIn, sizeof(ExtMemWord) * count * psi));
  
//   auto t1 = chrono::high_resolution_clock::now();
//   FoldedMVOffloadBinarized(binImages, outLabel, count * psi, count * psl, count);
//   auto t2 = chrono::high_resolution_clock::now();
  
//   TRANSFER_EXCL(thePlatform->copyBufferAccelToHost(accelBufOut, (void *)outLabel, sizeof(ExtMemWord) * count * psl));
//   // compare against labels
//   uint64_t mask_output=0xFFFFFFFFFFFFFFFF; // = uint64_t(uint64_t(1<<labelBits)-1);
//   mask_output = mask_output>> (64-labelBits);
//   for(unsigned int i = 0; i < count; i++) {
//     outLabel[i * psl] = outLabel[i * psl] & mask_output;
//   }
//   auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
//   usecPerImage = (float)duration / (count);
//   cout << "Inference took " << duration << " microseconds, " << usecPerImage << " usec per image" << endl;
//   cout << "Classification rate: " << 1000000.0 / usecPerImage << " images per second" << endl;
//   std::vector<int> result;
//   double internal_result;
//   for(unsigned int j = 0; j < count; j++) {
//     if (outLabel[j] == 0) {
//       internal_result = 0;
//     } else {
//       internal_result = log2((double) outLabel[j]);
//     }
//     result.push_back((unsigned int) internal_result);
//   }
//   delete[] outLabel;
//   delete[] binImages;
//   return result;
// }


// void testPrebinarized(std::vector<vec_t> & imgs, std::vector<label_t> & labels, const unsigned int labelBits) {
//   // TODO support labelBits > bitsPerExtMemWord
//   if(labelBits > bitsPerExtMemWord) {
//     throw "labelBits > bitsPerExtMemWord not yet supported";
//   }
//   const unsigned int count = imgs.size();
//   cout << "Running prebinarized test for " << count << " images..." << endl;
//   // compute the number of words needed to store the each img and label in binarized form
//   const unsigned int psi = paddedSize(imgs[0].size(), bitsPerExtMemWord) / bitsPerExtMemWord;
//   const unsigned int psl = paddedSize(labelBits, bitsPerExtMemWord) / bitsPerExtMemWord;
//   // allocate buffers for binarized input and output data
//   ExtMemWord * binImages = new ExtMemWord[(count * psi)];
//   ExtMemWord * binLabels = new ExtMemWord[(count * psl)];
//   // binarize each image and label
//   for(unsigned int i = 0; i < count; i++) {
//     binarizeAndPack(imgs[i], &binImages[i * psi], psi);
//     memset(&binLabels[i * psl], 0, sizeof(ExtMemWord) * psl);
//     binLabels[i * psl] = 1 << labels[i];
//   }
//   unsigned int r = 0;
//   cout << "Enter number of times to repeat test: " << endl;
//   cin >> r;
//   // recognize
//   unsigned int ok = 0, failed = 0;
//   ExtMemWord * outLabel = new ExtMemWord[(count * psl)];
//   TRANSFER_EXCL(thePlatform->copyBufferHostToAccel((void *)binImages, accelBufIn, sizeof(ExtMemWord) * count * psi));
//   auto t1 = chrono::high_resolution_clock::now();
//   for(unsigned int y = 0; y < r; y++) {
//     FoldedMVOffloadBinarized(binImages, outLabel, count * psi, count * psl, count);
//   }
//   auto t2 = chrono::high_resolution_clock::now();
//   TRANSFER_EXCL(thePlatform->copyBufferAccelToHost(accelBufOut, (void *)outLabel, sizeof(ExtMemWord) * count * psl));
//   // compare against labels
//   uint64_t mask_output = 0xFFFFFFFFFFFFFFFF; // = uint64_t(uint64_t(1<<labelBits)-1);
//   mask_output = mask_output>> (64-labelBits);
//   for(unsigned int i = 0; i < count; i++) {
//     outLabel[i * psl] = outLabel[i * psl] & mask_output;
//     if(memcmp(&outLabel[i * psl], &binLabels[i * psl], psl * sizeof(ExtMemWord)) == 0) {
//       ok++;
//     } else {
//       failed++;
//     }
//   }
//   cout << "Succeeded " << ok << " failed " << failed << " accuracy " << 100.0*(float)ok / count << "%" << endl;
//   auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
//   float usecPerImage = (float)duration / (count*r);
//   cout << "Inference took " << duration << " microseconds, " << usecPerImage << " usec per image" << endl;
//   cout << "Classification rate: " << 1000000.0 / usecPerImage << " images per second" << endl;
//   delete[] outLabel;
//   delete[] binImages;
//   delete[] binLabels;
// }

//ConvLayer_6_Bram_15_Dimension_torch.Size([192, 192, 3, 3]).bin

// Order has to remain -> BatchNorm followed Conv
void BinOpener(std::string dir)
{
  for (const auto & entry : fs::directory_iterator(dir))
        std::cout << entry.dir() << std::endl;
  for (unsigned int i=0, i<=TOTALLAYERS.size()-1, i++)
  {
    ifstream wtfile(dir+"/HLS_BNLayer"+to_string(TOTALLAYERS[i]+"_Dimension_torch.Size([") )
  }
}