REM 简单下载conan里没有的库
python -V
python ./tools/DownloadLib.py -d"C:/dxlib/download" -l"C:/dxlib/lib" "concurrentqueue"

pip3 install conan
REM 添加一个源
conan remote add artifactory  http://mr.xuexuesoft.com:8081/artifactory/api/conan/conan
REM 添加自己的上传账号
conan user -p 15200abc -r artifactory daixian
REM 搜索一下玩玩
conan search "*" -rartifactory