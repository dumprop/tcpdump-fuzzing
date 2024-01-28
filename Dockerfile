FROM ubuntu:22.04

RUN apt-get update && DEBIAN_FRONTEND="noninteractive" TZ="Russia/Moscow" apt-get install -y tzdata

# AFL++ packages
RUN apt-get install -y build-essential python3-dev automake cmake git flex bison libglib2.0-dev libpixman-1-dev python3-setuptools cargo libgtk-3-dev
RUN apt-get install -y lld llvm llvm-dev clang
RUN apt-get install -y gcc-$(gcc --version|head -n1|sed 's/\..*//'|sed 's/.* //')-plugin-dev libstdc++-$(gcc --version|head -n1|sed 's/\..*//'|sed 's/.* //')-dev

# coverage packages
RUN apt-get install -y lcov

# common software packages
RUN apt-get install -y wget

# AFL++ installing
WORKDIR /afl
RUN wget "https://github.com/AFLplusplus/AFLplusplus/archive/refs/tags/v4.09c.tar.gz"
RUN tar xf v4.09c.tar.gz
WORKDIR /afl/AFLplusplus-4.09c
RUN make source-only
RUN make install 

# set AFL variables and exit condition (30min~1800sec since last path)
ENV AFL_USE_ASAN=1 
ENV CC=afl-cc 
ENV AFL_EXIT_ON_TIME=1800


# download tcpdump 
WORKDIR /tcpdump
RUN wget "https://github.com/the-tcpdump-group/tcpdump/archive/refs/tags/tcpdump-4.9.1.tar.gz"
RUN tar xf tcpdump-4.9.1.tar.gz
WORKDIR /tcpdump/tcpdump-tcpdump-4.9.1

# download latest libpcap because tcpdump require that library
RUN wget "https://github.com/the-tcpdump-group/libpcap/archive/refs/tags/libpcap-1.10.1.tar.gz"
RUN tar xf libpcap-1.10.1.tar.gz
RUN mv libpcap-libpcap-1.10.1 libpcap
WORKDIR /tcpdump/tcpdump-tcpdump-4.9.1/libpcap
RUN ./configure --enable-shared=no
RUN make

# build tcpdump
WORKDIR /tcpdump/tcpdump-tcpdump-4.9.1/
RUN ./configure
RUN make

# prepare folder for AFL++ results
RUN mkdir afl_out
