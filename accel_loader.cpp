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
// Below snippet of code is for reading files. Can we multi-thread each datastream such that we have 3 threads running co-currently? PYNQ should stall but?
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
    std::vector<unsigned char> fileData(fileSize);
    
    for (unsigned int line = 0; line <= fileSize; line++)
      {file.read((char *)&e, sizeof(unsigned char));
      //cout wont work - printf("%u",e);
      fileData[line]=e;
    
      }
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
  std::vector<unsigned char> CN_w;
  std::vector<unsigned char> BN_w;
  std::vector<unsigned char> BN_t;
  int CN_dim1;
  int CN_dim2;
  int CN_dim3;
  int CN_dim4;
  int BN_dim1,BT_dim1;


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
           int BN_dim1 = stoi(s2);
           // File reading
           const char *filename = str.c_str();
           BN_w = readFile(filename);
          // for(std::vector<unsigned char>::iterator it = fileData.begin(); it != fileData.end(); ++it) {
          //     printf("%u",*it);
          //    }
          //    cout<<endl;
           BNw_dim1.push_back(BN_dim1);          
          
           }
        if (str.find("/HLS_BNThreshold"+to_string(TOTALLAYERS[i]))!=std::string::npos)
        {
          //For 1d tensor below 
          std::string s2(std::find(str.begin(), str.end(), '[')+1,(str.end() - 6));
          BT_dim1=stoi(s2);
          /// REDO FUNCTION FOR FLOATING POINT THRESHOLD - THIS WILL NOT WORK. 
          const char *filename = str.c_str();
          BN_t = readFile(filename);
          BNt_dim1.push_back(BT_dim1);
          
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
           CN_dim1 = stoi(s1d);
           CN_dim2 = stoi(s2d);
           CN_dim3 = stoi(s3d);
           CN_dim4 = stoi(s4d);

           const char *filename = str.c_str();
            CN_w = readFile(filename);
           CNw_dim1.push_back(CN_dim1);
           CNw_dim2.push_back(CN_dim2);
           CNw_dim3.push_back(CN_dim3);
           CNw_dim4.push_back(CN_dim4);
           //////////////////////////////
           
         }
        } // End of for loop thru directories
        // TODO: Support for padding the AXI bus -> 
        //      MSB | ---x62 | LSB
        //      CN_w| -- |BN_w|BN_t
        int longest = std::max({CN_w.size(),BN_w.size(),BN_t.size()});
        // cout<<(float)longest/64<<endl;


        /// Conv Weight Loader 
        int padding = bitsPerExtMemWord - (CN_w.size()%bitsPerExtMemWord);
        if(padding==64) padding=0;
        int missing = CN_w.size()%bitsPerExtMemWord;
        for (int poke = 0; poke <= padding-1; poke++)
        {
            CN_w.push_back(0); //Initialize 1 for padding to match 64-bits.  
        }
        for (int ind=0; ind<= CN_w.size()+padding-1; ind+=sizeof(ExtMemWord))
        {
          //unsigned int *CN_w_arr[sizeof(ExtMemWord)-1];
          ExtMemWord CN_w_arr;
          //CN_w_arr = new ExtMemWord;
          for (int j=ind; j<= ind+sizeof(ExtMemWord)-1; j++)
            CN_w_arr = CN_w_arr << 1 | CN_w[j];
          // printf("%u\n",CN_w_arr);
          // HAvig more AXI ports slows the processor? 
          // Implementaiton of dimensions = {i,CN_dim1,CN_dim2,CN_dim3, CN_dim4};
          // std::string temp = (to_string(i)+to_string(CN_dim1)+to_string(CN_dim2)+to_string(CN_dim3)+to_string(CN_dim4));
          // int dimensions = stoi(temp);
          std::string temp = (to_string(CN_dim3)+to_string(CN_dim4));
          int dimensions = stoi(temp);
          Accel_load(i,CN_dim1,CN_dim2,dimensions, CN_w_arr);
        }


        /// BN Weight Loader
        padding = bitsPerExtMemWord - (BN_w.size()%bitsPerExtMemWord);
        if(padding==64) padding=0;
         for (int poke = 0; poke <= padding-1; poke++)
        {
            BN_w.push_back(0); //Initialize 1 for padding to match 64-bits. 
        }
        // cout<<(BN_w.size()%bitsPerExtMemWord)<<endl<<BN_w.size()<<endl<<padding<<endl;
        missing = BN_w.size()%bitsPerExtMemWord;
        for (int ind=0; ind<= BN_w.size()+padding-1; ind+=sizeof(ExtMemWord))
        {
          //unsigned int *CN_w_arr[sizeof(ExtMemWord)-1];
          ExtMemWord CN_w_arr;
          //CN_w_arr = new ExtMemWord;
          for (int j=ind; j<= ind+sizeof(ExtMemWord)-1; j++)
            CN_w_arr = CN_w_arr << 1 | BN_w[j];
          int BN_dim2=0;
          int BN_dim3=0;
          int BN_dim4=0;
          std::string temp = (to_string(BN_dim3)+to_string(BN_dim4));
          int dimensions = stoi(temp);
          Accel_load(i,BN_dim1,BN_dim2,dimensions, CN_w_arr);
        }
        
        /// BN Threshold Loader
        padding = bitsPerExtMemWord - (BN_t.size()%bitsPerExtMemWord);
        if(padding==64) padding=0;
        {
            BN_t.push_back(0); //Initialize 1 for padding to match 64-bits. 
        }
        missing = BN_t.size()%bitsPerExtMemWord;
        for (int ind=0; ind<= BN_t.size()+padding-1; ind+=sizeof(ExtMemWord))
        {
          //unsigned int *CN_w_arr[sizeof(ExtMemWord)-1];
          ExtMemWord CN_w_arr;
          //CN_w_arr = new ExtMemWord;
          for (int j=ind; j<= ind+sizeof(ExtMemWord)-1; j++)
            CN_w_arr = CN_w_arr << 1 | BN_t[j];
          int BN_dim2=0;
          int BN_dim3=0;
          int BN_dim4=0;
          std::string temp = (to_string(BN_dim3)+to_string(BN_dim4));
          int dimensions = stoi(temp);
          Accel_load(i,BT_dim1,BN_dim2,dimensions, CN_w_arr);
        }
          
        // cout<<longest<<endl;

          
        
        } // end of for loop
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