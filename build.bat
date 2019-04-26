python -V
python ./tools/build.py
mkdir build
cd build
mkdir Win64
cd Win64
cmake.exe -G "Visual Studio 15 2017 Win64" "../../" -DBUILD_GMOCK:BOOL="0" -DINSTALL_GTEST:BOOL="0" -DBUILD_TESTING:BOOL="0" -DCMAKE_INSTALL_PREFIX:PATH="./install" -DENABLE_TEST:BOOL="1" -DCMAKE_CONFIGURATION_TYPES:STRING="Release;RelWithDebInfo" -Dgtest_force_shared_crt:BOOL="1"
::pause
cd ../
mkdir Win32
cd Win32
cmake.exe -G "Visual Studio 15 2017" "../../" -DBUILD_GMOCK:BOOL="0" -DINSTALL_GTEST:BOOL="0" -DBUILD_TESTING:BOOL="0" -DCMAKE_INSTALL_PREFIX:PATH="./install" -DENABLE_TEST:BOOL="1" -DCMAKE_CONFIGURATION_TYPES:STRING="Release;RelWithDebInfo" -Dgtest_force_shared_crt:BOOL="1"
::pause
