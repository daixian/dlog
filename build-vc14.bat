chcp 65001
REM windows平台编译项目
set CONAN_REVISIONS_ENABLED=1
conan create . daixian/stable -s compiler.version=14 -s compiler.runtime=MT -s arch=x86_64 -s build_type=Release --build missing
conan create . daixian/stable -s compiler.version=14 -s compiler.runtime=MD -s arch=x86_64 -s build_type=Release --build missing
conan upload dlog/2.5.0@daixian/stable --all -r=xuexue