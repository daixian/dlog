#! /bin/bash
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

# 目前试着安装了gcc-7-arm-linux-gnueabihf
# gcc-8也已经出来了
python3 -V
python3 ./tools/DownloadLib.py -p "linux" "concurrentqueue" "spdlog" "gtest"
mkdir -p build/linux/arm
cd build/linux/arm
cmake -G "Unix Makefiles" "../../../" -DINSTALL_GTEST:BOOL="0" -DCMAKE_C_COMPILER:FILEPATH="/usr/bin/arm-linux-gnueabihf-gcc-7" -DCMAKE_C_COMPILER_AR:FILEPATH="/usr/bin/arm-linux-gnueabihf-gcc-ar-7" -DSPDLOG_BUILD_TESTS:BOOL="0" -DSPDLOG_BUILD_BENCH:BOOL="0" -DBUILD_GMOCK:BOOL="0" -DCMAKE_CXX_FLAGS:STRING="" -DCMAKE_CXX_COMPILER_RANLIB:FILEPATH="/usr/bin/arm-linux-gnueabihf-gcc-ranlib-7" -DCMAKE_BUILD_TYPE:STRING="" -DCMAKE_CXX_COMPILER_AR:FILEPATH="/usr/bin/arm-linux-gnueabihf-gcc-7" -DSPDLOG_BUILD_EXAMPLES:BOOL="0" -DCMAKE_AR:FILEPATH="/usr/bin/arm-linux-gnueabihf-ar" -DCMAKE_C_COMPILER_RANLIB:FILEPATH="/usr/bin/arm-linux-gnueabihf-gcc-ranlib-7" -DCMAKE_INSTALL_PREFIX:PATH="./install" -DCMAKE_CXX_COMPILER:FILEPATH="/usr/bin/arm-linux-gnueabihf-g++-7" -DCMAKE_LINKER:FILEPATH="/usr/bin/arm-linux-gnueabihf-ld" -DBUILD_TESTING:BOOL="0"
make
# install
