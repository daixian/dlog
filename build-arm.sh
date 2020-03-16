#! /bin/bash
# PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
# export PATH

# 目前试着安装了gcc-7-arm-linux-gnueabihf
# gcc-8也已经出来了

# conan install .. --profile=../linux86_to_arm --build=missing
conan create . daixian/stable --profile=./linux86_to_arm  -o dlog:shared=True  --build missing
conan upload dlog/2.5.0@daixian/stable --all -r=conan-local

# mkdir -p build/linux/arm
# cd build/linux/arm
# cmake -G "Unix Makefiles" "../.." -DCROSS_COMPILE:BOOL="1" -DINSTALL_GTEST:BOOL="0" -DCMAKE_C_COMPILER:FILEPATH="/usr/bin/arm-linux-gnueabihf-gcc" -DCMAKE_C_COMPILER_AR:FILEPATH="/usr/bin/arm-linux-gnueabihf-ar" -DSPDLOG_BUILD_TESTS:BOOL="0" -DSPDLOG_BUILD_BENCH:BOOL="0" -DBUILD_GMOCK:BOOL="0" -DCMAKE_CXX_FLAGS:STRING="" -DCMAKE_CXX_COMPILER_RANLIB:FILEPATH="/usr/bin/arm-linux-gnueabihf-ranlib" -DCMAKE_BUILD_TYPE:STRING="" -DCMAKE_CXX_COMPILER_AR:FILEPATH="/usr/bin/arm-linux-gnueabihf-ar" -DSPDLOG_BUILD_EXAMPLES:BOOL="0" -DCMAKE_AR:FILEPATH="/usr/bin/arm-linux-gnueabihf-ar" -DCMAKE_C_COMPILER_RANLIB:FILEPATH="/usr/bin/arm-linux-gnueabihf-ranlib" -DCMAKE_INSTALL_PREFIX:PATH="./install" -DCMAKE_CXX_COMPILER:FILEPATH="/usr/bin/arm-linux-gnueabihf-g++" -DCMAKE_LINKER:FILEPATH="/usr/bin/arm-linux-gnueabihf-ld" -DBUILD_TESTING:BOOL="0"
# make
# install
