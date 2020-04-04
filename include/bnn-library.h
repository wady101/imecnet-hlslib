
#include <hls_stream.h>
#include "ap_int.h"
#include <iostream>
#include <string>

using namespace hls;
using namespace std;

#define CASSERT_DATAFLOW(x) {if (!(x)) {std::cout<< "CASSERT_DATAFLOW condition is not met " << endl; exit(-1);	}}
	   
#include "mmv.hpp"
#include "streamtools.h"
#include "dma.h"
// #include "slidingwindow.h"
#include "maxpool.h"
// #include "fclayer.h"
// #include "convlayer.h"


