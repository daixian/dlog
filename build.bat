chcp 65001
REM windows平台编译项目
set CONAN_REVISIONS_ENABLED=1
conan create . daixian/stable -s compiler.runtime=MT -s arch=x86_64 -s build_type=Release --build missing
conan create . daixian/stable -s compiler.runtime=MD -s arch=x86_64 -s build_type=Release --build missing
conan upload dlog/2.4.0@daixian/stable --all -r=conan-local

REM mkdir build
REM cd build
REM mkdir Win64
REM cd Win64
REM conan install ../.. -scompiler.runtime=MT -sarch=x86_64
REM cmake.exe -G "Visual Studio 15 2017 Win64" "../../" -DBUILD_GMOCK:BOOL="0" -DINSTALL_GTEST:BOOL="0" -DBUILD_TESTING:BOOL="0" -DCMAKE_INSTALL_PREFIX:PATH="./install" -DENABLE_TEST:BOOL="1" -DARCH_X86:BOOL="0" -DCMAKE_CONFIGURATION_TYPES:STRING="Release;RelWithDebInfo" -Dgtest_force_shared_crt:BOOL="1"
REM ::pause
REM cd ../
REM mkdir Win32
REM cd Win32
REM conan install ../.. -scompiler.runtime=MT -sarch=x86 --build fmt --build spdlog
REM cmake.exe -G "Visual Studio 15 2017" "../../" -DBUILD_GMOCK:BOOL="0" -DINSTALL_GTEST:BOOL="0" -DBUILD_TESTING:BOOL="0" -DCMAKE_INSTALL_PREFIX:PATH="./install" -DENABLE_TEST:BOOL="1" -DARCH_X86:BOOL="1" -DCMAKE_CONFIGURATION_TYPES:STRING="Release;RelWithDebInfo" -Dgtest_force_shared_crt:BOOL="1"
::pause
