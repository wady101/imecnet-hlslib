
#pragma HLS RESOURCE variable=weights0 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=weights0 complete dim=1

#pragma HLS ARRAY_PARTITION variable=weights0 complete dim=2

#pragma HLS RESOURCE variable=threshs0 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=threshs0 complete dim=1

#pragma HLS ARRAY_PARTITION variable=threshs0 complete dim=2

#pragma HLS RESOURCE variable=weights1 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=weights1 complete dim=1

#pragma HLS ARRAY_PARTITION variable=weights1 complete dim=2

#pragma HLS RESOURCE variable=threshs1 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=threshs1 complete dim=1

#pragma HLS ARRAY_PARTITION variable=threshs1 complete dim=2

#pragma HLS RESOURCE variable=weights3 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=weights3 complete dim=1

#pragma HLS ARRAY_PARTITION variable=weights3 complete dim=2

#pragma HLS RESOURCE variable=threshs3 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=threshs3 complete dim=1

#pragma HLS ARRAY_PARTITION variable=threshs3 complete dim=2

#pragma HLS RESOURCE variable=weights4 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=weights4 complete dim=1

#pragma HLS ARRAY_PARTITION variable=weights4 complete dim=2

#pragma HLS RESOURCE variable=threshs4 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=threshs4 complete dim=1

#pragma HLS ARRAY_PARTITION variable=threshs4 complete dim=2

#pragma HLS RESOURCE variable=weights6 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=weights6 complete dim=1

#pragma HLS ARRAY_PARTITION variable=weights6 complete dim=2

#pragma HLS RESOURCE variable=threshs6 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=threshs6 complete dim=1

#pragma HLS ARRAY_PARTITION variable=threshs6 complete dim=2

#pragma HLS RESOURCE variable=weights7 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=weights7 complete dim=1

#pragma HLS ARRAY_PARTITION variable=weights7 complete dim=2

#pragma HLS RESOURCE variable=threshs7 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=threshs7 complete dim=1

#pragma HLS ARRAY_PARTITION variable=threshs7 complete dim=2

#pragma HLS RESOURCE variable=weights8 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=weights8 complete dim=1

#pragma HLS ARRAY_PARTITION variable=weights8 complete dim=2

#pragma HLS RESOURCE variable=threshs8 core=RAM_T2P_BRAM latency=1

#pragma HLS ARRAY_PARTITION variable=threshs8 complete dim=1

#pragma HLS ARRAY_PARTITION variable=threshs8 complete dim=2
