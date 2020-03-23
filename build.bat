chcp 65001
REM windows平台编译项目
set CONAN_REVISIONS_ENABLED=1
set CONAN_ARCHIVE_PATH=%~dp0temp/archive
REM conan create . daixian/stable -s compiler.runtime=MT -s arch=x86_64 -s build_type=Release --build missing
conan create . daixian/stable -s compiler.runtime=MD -s arch=x86_64 -s build_type=Release --build missing
REM conan upload dlog/2.5.0@daixian/stable --all -r=xuexue