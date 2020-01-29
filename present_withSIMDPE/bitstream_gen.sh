#!/bin/zsh 

BENNVIVADO="bitstream_gen"
IPFOLDER=$1
echo "=======Vivado HLS======"
if [ -d "HLS" ]; then
  echo "HLS IP generated in ./HLS"
elif [ ! -d "HLS" ]; then 
 vivado_hls -f hls-syn.tcl 
fi

echo "=====Vivado Project====="
rm -rf $BENNVIVADO

vivado -mode batch  -nojournal -notrace -source vivado/make-vivado-proj.tcl -tclargs $IPFOLDER $BENNVIVADO  

cp -f "$BENNVIVADO/$BENNVIVADO.runs/impl_1/procsys_wrapper.bit" ./generated
cp -f "$BENNVIVADO/$BENNVIVADO.srcs/sources_1/bd/procsys/hw_handoff/procsys.hwh" ./generated
cp -f "$BENNVIVADO/procsys.tcl" ./generated
