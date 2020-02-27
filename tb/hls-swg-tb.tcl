open_project hls-syn-swg
set config_proj_part "xc7z020clg400-3" 
add_files input_sliding_tb.cpp -cflags "-std=c++0x -I../include/" 
add_files -tb sliding_tb.cpp -cflags "-std=c++0x -I../include/" 
set_top Testbench
open_solution sol1
set_part $config_proj_part
create_clock -period 5 -name default
csim_design
csynth_design
cosim_design
exit
