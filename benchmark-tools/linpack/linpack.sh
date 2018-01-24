#!/bin/bash
sudo wget http://registrationcenter-download.intel.com/akdlm/irc_nas/9752/l_mklb_p_2018.0.006.tgz -P /tmp
sudo tar -xzf /tmp/l_mklb_p_2018.0.006.tgz -C /tmp/
sudo cp -a /tmp/l_mklb_p_2018.0.006/benchmarks_2018/linux/mkl/benchmarks/linpack ~/.
sudo ln -sf ~/linpack/runme_xeon64 /usr/sbin/
sudo ln -sf ~/linpack/xlinpack_xeon64 /usr/sbin/
sudo runme_xeon64
exit 0