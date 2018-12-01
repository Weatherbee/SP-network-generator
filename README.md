# SP-network-generator
(1) C++ software that parametrically generates arbitrary ANSI C code for a given width substitution-permutation network (SP-network)
(2) associated shell script that runs multiple iterations of this C++ software and optimizes for maximal length SP-network sequences

The resulting code can be used as a fast (one word per cycle in hardware) linear-feedback shift register (LFSR) substitute or for other data scrambling applications. The generated ANSI C code SP-networks can be compiled for CPU execution or will synthesize without changes via high-level synthesis for FPGA or ASIC implementation.  The SP-network is constructed out of arbitrary non-uniform width S-boxes up to a settable maximum width.  Both the SP-network and its inverse are generated.
