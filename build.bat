python -V
python ./tools/DownloadLib.py -d"C:/dxlib/download" -l"C:/dxlib/lib" "concurrentqueue"

mkdir build
cd build
mkdir Win64
cd Win64
conan install ../.. -scompiler.runtime=MT -sarch=x86_64
cmake.exe -G "Visual Studio 15 2017 Win64" "../../" -DBUILD_GMOCK:BOOL="0" -DINSTALL_GTEST:BOOL="0" -DBUILD_TESTING:BOOL="0" -DCMAKE_INSTALL_PREFIX:PATH="./install" -DENABLE_TEST:BOOL="1" -DARCH_X86:BOOL="0" -DCMAKE_CONFIGURATION_TYPES:STRING="Release;RelWithDebInfo" -Dgtest_force_shared_crt:BOOL="1"
::pause
cd ../
mkdir Win32
cd Win32
conan install ../.. -scompiler.runtime=MT -sarch=x86 --build fmt --build spdlog
cmake.exe -G "Visual Studio 15 2017" "../../" -DBUILD_GMOCK:BOOL="0" -DINSTALL_GTEST:BOOL="0" -DBUILD_TESTING:BOOL="0" -DCMAKE_INSTALL_PREFIX:PATH="./install" -DENABLE_TEST:BOOL="1" -DARCH_X86:BOOL="1" -DCMAKE_CONFIGURATION_TYPES:STRING="Release;RelWithDebInfo" -Dgtest_force_shared_crt:BOOL="1"
::pause
