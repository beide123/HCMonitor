HCMonitor is a high performance monitor system developed on user-level.
1. Prerequisites
We require the following libraries to run HCMonitor.
    * libdpdk (Intel's DPDK package*, DPDK-19.08 best) 
    * libnuma
    * libconfig
    * mysql
    * mysqlclient
    * python2.7
2. Requires headers.
    * mysql-devel
    * python-devel
3. Included directories
    HCMonitor: source code directory

4. Install guide
    HCMonitor can be installed as below.

    DPDK INSTALL
    Download DPDK(Best for DPDK-19.08).
    Setup DPDK.

    cd <path to DPDK>/usertools 
    ./dpdk-setup.sh
    Press [39] x86_64-native-linux-gcc

    Press [43] Insert IGB UIO module

    Press [47] Setup hugepage mappings for NUMA systems 
    (set 2560 for each node 2MB hugepages )

    Press [49] Bind Ethernet/Baseband/Crypto device to IGB UIO module 

    Press [60] Exit Script

    Only those devices will work with DPDK drivers that are listed on this page: http://dpdk.org/doc/nics. Please make sure that your NIC is compatible before moving on to the next step.
5.  Setup HCMonitor library:
cd <path to HCMonitor> 
vim Makefile
Add two configurations at the beginning as below
RTE_SDK=`echo $PWD`/dpdk
RTE_TARGET=x86_64-native-linuxapp-gcc
make
User guides
