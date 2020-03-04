import numpy as np
import os
import sys
import random 
import subprocess


outFileConfig = open("memdata.h" , "wt")

kernel_dim = 1 
stride = 1
input_precision = 1
ifm_channels = 1
ofm_channels = 1
ifm_dimension = 3
ofm_dimension = 3

activation_precision = 1
expand = 1
simd = 1
pe = 1
w_precision = 1
mmv=2

tile = ifm_channels *kernel_dim*kernel_dim * ofm_channels // (simd*pe)

outFileConfig.write("#define KERNEL_DIM %d \n" % kernel_dim)
outFileConfig.write("#define SIMD1 %d \n" % simd)
outFileConfig.write("#define PE1 %d \n" % pe)
outFileConfig.write("#define MMV1 %d \n" % mmv)
outFileConfig.write("#define WIDTH %d \n" % w_precision)

outFileConfig.write("#define IFM_Channels1 %d \n" % ifm_channels)
outFileConfig.write("#define OFM_Channels1 %d \n" % ofm_channels)
outFileConfig.write("#define IFMDim1 %d \n" % ifm_dimension)
outFileConfig.write("#define OFMDim1 %d \n" % ofm_dimension)
outFileConfig.write("#define STRIDE1 %d \n" % stride)
outFileConfig.write("#define INPUT_PRECISION %d \n" % input_precision)
outFileConfig.write("#define WMEM1 %d \n" % tile)
outFileConfig.write("#define TMEM1 %d \n"% (ofm_channels//pe))
outFileConfig.write("#define ACTIVATION_PRECISION %d \n" % activation_precision)

outFileConfig.write("#ifndef PARAMS_HPP\n")
outFileConfig.write("#define PARAMS_HPP\n")

# outFileWeights.write("namespace PARAM{ \n")
if (w_precision == 1):
	outFileConfig.write("static ap_uint<%d> m_weights [%d][%d] = " %(simd,pe,tile) )
	#Order of simdCount and peCount is correct for the above statement
	outFileConfig.write("{")
	for i in range(pe):
		outFileConfig.write("{")
		width = simd*w_precision;
		
		for j in range(tile):
			val = random.randint(0, 1<<width-1)
			outFileConfig.write("%s "%bin(val))#%(j)
			if j!=tile-1:
				outFileConfig.write(",")
		if i!= pe-1: 
			outFileConfig.write("}, ")
		else:
			outFileConfig.write("} ")
	outFileConfig.write("}; \n")

outFileConfig.write("static ap_uint<%d> threshs [%d][%d] = " %(8,pe,ofm_channels//pe))
outFileConfig.write("{")
for i in range(pe):
	outFileConfig.write("{")
	for j in range(ofm_channels//pe):
		val = random.randint(0, 10)
		outFileConfig.write("%d" %val)
		if j!=ofm_channels/pe-1:
				outFileConfig.write(",")
	if i!= pe-1: 
		outFileConfig.write("}, ")
	else:
		outFileConfig.write("} ")
outFileConfig.write("}; \n")	




# outFileConfig.write("}\n};\n } \n")
outFileConfig.write("#endif \n")
# outFileWeights.close()
outFileConfig.close()
