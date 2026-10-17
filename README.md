# imecnet-hlslib

Vivado HLS research implementation of **IMECNet**, a streaming binary-neural-network accelerator built around Inverse Memory Efficient Convolution (IMEC). This repository contains the core hardware artifact developed for the 2020 thesis *IMECBinNets: A Framework for Deploying Neural Networks on FPGAs*.

> **Research artifact:** the implementation targets a legacy Xilinx toolchain and a fixed CIFAR-10 Network-in-Network configuration. It is published for study and reproduction, not as a current drop-in replacement for FINN.

## Highlights

- `include/wallace_MEC.h` implements the IMEC sliding-window data generator.
- `include/wallace_convlayer.h` composes IMEC with the matrix-vector activation unit.
- `include/wallace_mvau.hpp` contains the binary matrix-vector processing engine.
- `top.cpp` assembles the thesis Network-in-Network dataflow pipeline.
- `tb/` provides focused convolution and sliding-window HLS testbenches.
- `drivers/pynq_tb.cpp` provides the legacy PYNQ-side test driver.

## Relationship to the other repositories

| Repository | Purpose |
| --- | --- |
| [`imecnet-hlslib`](https://github.com/wady101/imecnet-hlslib) | This repository: IMEC and the thesis HLS pipeline. |
| [`imecnet-training-thesis`](https://github.com/wady101/imecnet-training-thesis) | Experimental model training and checkpoint-to-HLS parameter generation. |
| [`BNN-upstream`](https://github.com/wady101/BNN-upstream) | Legacy BNN-PYNQ integration baseline used during development. |

This code was derived from the Xilinx [finn-hlslib](https://github.com/Xilinx/finn-hlslib) and [BNN-PYNQ](https://github.com/Xilinx/BNN-PYNQ) ecosystem, then reorganized for the IMEC experiments. It is kept standalone because its thesis-specific layout is not a drop-in copy of upstream `finn-hlslib`.

## Requirements

The original environment used:

- Xilinx Vivado HLS 2018.2
- PYNQ-Z1/PYNQ-Z2 target part `xc7z020clg400-3`
- A compiler supported by that Vivado HLS release
- The `xilinx-tiny-cnn` submodule for the PYNQ test driver

Newer Vitis HLS releases may require source and Tcl changes.

## Clone

```bash
git clone --recursive https://github.com/wady101/imecnet-hlslib.git
cd imecnet-hlslib
```

If the repository was cloned without `--recursive`:

```bash
git submodule update --init --recursive
```

## HLS simulation and synthesis

Run the focused IMEC sliding-window testbench:

```bash
cd tb
vivado_hls -f hls-swg-tb.tcl
```

Run the convolution-layer testbench:

```bash
cd tb
vivado_hls -f hls-tb.tcl
```

Build the full fixed pipeline from the repository root:

```bash
vivado_hls -f hls-syn.tcl
```

The full script runs C simulation, HLS synthesis, and IP-catalog export. Network dimensions and generated parameters are fixed in `include/config.h`, `include/weights.h`, and `include/weights_pragmas.h`.

## Training and parameter generation

The companion `imecnet-training-thesis` repository contains the experimental PyTorch code and `cifar/parser_gen.py`. That parser generated the HLS configuration, packed weights, and thresholds used here. The committed generated files are retained to make the hardware artifact inspectable without reproducing the legacy training environment first.

## Citation

```bibtex
@mastersthesis{wadhwa2020imecbinnets,
  author  = {Wadhwa, Eashan},
  title   = {{IMECBinNets}: A Framework for Deploying Neural Networks on FPGAs},
  school  = {Trinity College Dublin},
  year    = {2020},
  month   = apr,
  doi     = {10.13140/RG.2.2.13870.73287}
}
```

Thesis: [doi:10.13140/RG.2.2.13870.73287](https://doi.org/10.13140/RG.2.2.13870.73287)

## Acknowledgements

IMECNet builds on FINN, BNN-PYNQ, and finn-hlslib. Retain the upstream notices embedded in inherited source files and cite the FINN authors when using those components.

## Reuse

This archive does not currently include a repository-wide license. Inherited source files retain their upstream notices; review those terms and obtain any required permission before redistribution or reuse.

## Contact

wady101 — [omegangregovitch@gmail.com](mailto:omegangregovitch@gmail.com)
