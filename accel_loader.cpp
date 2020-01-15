#include "accel_loader.h"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <array>
#include <fstream>
#include <algorithm>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
using namespace std;
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
int char_freq (std::string str)
{ int count = 0;
      for (int i = 0; i < str.size(); i++)
    {
        if (str[i] ==  ',')
        {
            ++ count;
        }
    }
return count;
}
std::vector<unsigned char> readFile(const char* filename)
{
    // open the file:
    std::streampos fileSize;
    std::ifstream file(filename, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    unsigned char e = 0;
    for (unsigned int line = 0; line <= fileSize; line++)
      {file.read((char *)&e, sizeof(unsigned char));
      unsigned int asfjfalfj = e; 
      printf("%u",e);}
    std::vector<unsigned char> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}
//ConvLayer_6_Bram_15_Dimension_torch.Size([192, 192, 3, 3]).bin

// Order has to remain -> BatchNorm followed Conv
void BinOpener(std::string dir)
{
  std::vector<std::string> paths; 
  std::vector<int> BNw_dim1;
  std::vector<int> BNt_dim1;
  std::vector<int> CNw_dim1;
  std::vector<int> CNw_dim2;
  std::vector<int> CNw_dim3;
  std::vector<int> CNw_dim4;

      for ( int i=0; i<=(sizeof(TOTALLAYERS)/sizeof(*TOTALLAYERS))-1; i++)
        {
          //while(bnl_flag & bnl_flag != 1)
          for (const auto & entry : fs::directory_iterator(dir))
          {
        // std::cout << entry.path() << std::endl;
        std::string str = entry.path();
        
        //std::cout << str.find("/HLS_BNLayer");
    
         
        if (str.find("/HLS_BNLayer"+to_string(TOTALLAYERS[i]))!=std::string::npos)
          {
          // std::string s2(std::find(str.begin(), str.end(), '[')+1,(str.end() - 6));
          // std::string s3(s2.begin(),std::find(s2.begin(),s2.end(), ','));
          // std::cout << s3 <<endl;
          int freq=char_freq(str);
           //For 1d tensor below 
           std::string s2(std::find(str.begin(), str.end(), '[')+1,(str.end() - 6));
           int dim1 = stoi(s2);
           // File reading
           const char *filename = str.c_str();
          std::vector<unsigned char> fileData = readFile(filename);
          int poke = 0;
          for(std::vector<unsigned char>::iterator it = fileData.begin(); it != fileData.end(); ++it) {
              poke++;
              //std::cout << *it;
             }
             cout<<endl;
           BNw_dim1.push_back(dim1);          
          
           }
        if (str.find("/HLS_BNThreshold"+to_string(TOTALLAYERS[i]))!=std::string::npos)
        {
          //For 1d tensor below 
          std::string s2(std::find(str.begin(), str.end(), '[')+1,(str.end() - 6));
          int dim1=stoi(s2);
          BNt_dim1.push_back(dim1);
          
        }
        if ((str.find("/HLS_ConvLayer"+to_string(TOTALLAYERS[i]))!=std::string::npos) ) // && (bnt_flag & bnl_flag == 1))
         {
           // For 4-d tensor parser below
           
           std::string s2(std::find(str.begin(), str.end(), '[')+1,(str.end() - 6));
           auto firstcomma = std::find(s2.begin(),s2.end(), ',');
           auto secondcomma = std::find(firstcomma+1,s2.end(), ',');
           auto thirdcomma = std::find(secondcomma+1,s2.end(), ',');
           std::string s1d(s2.begin(),firstcomma);
           std::string s2d(firstcomma+1,secondcomma);
           std::string s3d(secondcomma+1,thirdcomma);
           std::string s4d(thirdcomma+1,s2.end());  
           int dim1 = stoi(s1d);
           int dim2 = stoi(s2d);
           int dim3 = stoi(s3d);
           int dim4 = stoi(s4d);

           CNw_dim1.push_back(dim1);
           CNw_dim2.push_back(dim2);
           CNw_dim3.push_back(dim3);
           CNw_dim4.push_back(dim4);
           //////////////////////////////
           
         } 
        
        } // end of for loop

        }
  // 
  // {
  //   ifstream wtfile(dir+"/HLS_BNLayer"+to_string(TOTALLAYERS[i]+"_Dimension_torch.Size([") )
  // }
}
int main()
{
  BinOpener("../HLSgenerated/");
  return 0;
}