#! /bin/bash
# PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
# export PATH

# python3 -V

# pip3 install conan
conan create . daixian/stable -s build_type=Release -o dlog:shared=True --build missing
conan create . daixian/stable -s build_type=Release -o dlog:shared=False --build missing
# conan upload dlog --all -r=xuexue

# mkdir -p build/Linux
# cd build/Linux
# conan install ../.. --build=missing
# cmake -G "Unix Makefiles" "../.." -DBUILD_GMOCK:BOOL="0" -DINSTALL_GTEST:BOOL="0" -DBUILD_TESTING:BOOL="0" -DCMAKE_INSTALL_PREFIX:PATH="./install" -DENABLE_TEST:BOOL="1" -DCMAKE_CONFIGURATION_TYPES:STRING="Release;RelWithDebInfo" -Dgtest_force_shared_crt:BOOL="1"
# make