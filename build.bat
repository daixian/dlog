chcp 65001
REM windows平台编译项目
set CONAN_REVISIONS_ENABLED=1
set CONAN_ARCHIVE_PATH=%~dp0temp/archive
REM conan create . daixian/stable -s compiler.runtime=MT -s arch=x86_64 -s build_type=Release --build missing

REM 这里test_package会失败,如果dlog:shared=True
conan create . daixian/stable -s compiler.runtime=MD -s arch=x86_64 -s build_type=Release -o dlog:shared=False --build missing
conan create . daixian/stable -s compiler.runtime=MD -s arch=x86_64 -s build_type=Release -o dlog:shared=True --build missing
conan create . daixian/stable -s compiler.runtime=MT -s arch=x86_64 -s build_type=Release -o dlog:shared=False --build missing
conan create . daixian/stable -s compiler.runtime=MT -s arch=x86_64 -s build_type=Release -o dlog:shared=True --build missing
conan create . daixian/stable -s compiler.runtime=MD -s arch=x86 -s build_type=Release -o dlog:shared=False --build missing
conan create . daixian/stable -s compiler.runtime=MD -s arch=x86 -s build_type=Release -o dlog:shared=True --build missing
conan create . daixian/stable -s compiler.runtime=MT -s arch=x86 -s build_type=Release -o dlog:shared=False --build missing
conan create . daixian/stable -s compiler.runtime=MT -s arch=x86 -s build_type=Release -o dlog:shared=True --build missing

REM conan upload dlog/2.5.7@daixian/stable --all -r=xuexue
conan upload fmt --all -r=xuexue
conan upload openssl --all -r=xuexue
conan upload poco --all -r=xuexue
conan upload rapidjson --all -r=xuexue
conan upload spdlog --all -r=xuexue
conan upload zlib --all -r=xuexue
