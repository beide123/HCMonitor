# How to Start HCMonitor
# Requirements
* Kernel version >= 2.6.34, glibc>=2.7
* More than 32G system memory depending on the application
* DPDK version >=2.2.0
* A Intel network card.

# Usage
- Install DPDK and bind the NICs.

#Download DPDK source code from http://http://dpdk.org/download
- Compile the HCMonitor code

#cd $(MONITOR_PATH)

#make
- Start the programe and see latency CDF in cdf.txt

#sh demo_setup.sh 

#tail -f cdf.txt
- Kill the programe

#sh kill_monitor monitor 

