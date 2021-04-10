#! /bin/bash

mkdir build
cd build
# 这一步里设置-o dlog:shared=True是没有用的,因为生成调试工程,没有执行python的build
conan install ../test_package --profile=../profile/iOS -s build_type=Release --build missing
 