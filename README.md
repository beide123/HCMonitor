# How to Start HCMonitor
#Requirements
* Kernel version >= 2.6.34, glibc>=2.7.
* > = 32G system memory depending on the application
* DPDK version >=2.2.0
* A Intel network card.

#Usage

# Download DPDK source code from http://http://dpdk.org/download
# cd $(DPDK_PATH)
- To install DPDK and bind the NICs.
# cd $(MONITOR_PATH)
# make
# sh demo_setup.sh 
- to start the programe and see latency CDF in cdf.txt.
# sh kill_monitor monitor 
- to kill the programe

