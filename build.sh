#! /bin/bash
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

python3 -V
python3 ./tools/DownloadLib.py -d"$HOME/dxlib/download" -l"$HOME/dxlib/lib" -p"linux" "concurrentqueue"

pip3 install conan

mkdir -p build/linux/x64
cd build/linux/x64
cmake -G "Unix Makefiles" "../../../" -DBUILD_GMOCK:BOOL="0" -DINSTALL_GTEST:BOOL="0" -DBUILD_TESTING:BOOL="0" -DCMAKE_INSTALL_PREFIX:PATH="./install" -DENABLE_TEST:BOOL="1" -DCMAKE_CONFIGURATION_TYPES:STRING="Release;RelWithDebInfo" -Dgtest_force_shared_crt:BOOL="1"
make