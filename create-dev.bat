chcp 65001
REM 生成一个开发用的VS工程目录
python -V
REM python ./tools/DownloadLib.py -d "C:/dxlib/download" -l "C:/dxlib/lib" "concurrentqueue"

mkdir build
cd build
conan install ../test_package -s compiler.runtime=MD -s arch=x86_64 -s build_type=Release --build missing
REM cmake ../src -G "Visual Studio 14 Win64" -DDLOG_BUILD_SHARED:BOOL="1" -DDLOG_BUILD_TESTS:BOOL="1" -DCMAKE_CONFIGURATION_TYPES:STRING="Debug;Release;RelWithDebInfo"

REM 返回上级
cd ..
cmake.exe --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -H./ -B./build -G "Visual Studio 16 2019"
