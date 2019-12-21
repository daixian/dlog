chcp 65001
REM 生成一个开发用的VS工程目录
python -V
python ./tools/DownloadLib.py -d "C:/dxlib/download" -l "C:/dxlib/lib" "concurrentqueue"

mkdir build
cd build
conan install .. -s compiler.runtime=MD -s arch=x86_64 -s build_type=Release --build missing
cmake ../src -G "Visual Studio 15 Win64" -DDLOG_BUILD_SHARED:BOOL="1" -DDLOG_BUILD_TESTS:BOOL="1" -DCMAKE_CONFIGURATION_TYPES:STRING="Release;RelWithDebInfo"