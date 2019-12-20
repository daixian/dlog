set CONAN_REVISIONS_ENABLED=1
conan create . daixian/stable --profile=./win_to_rpi --build=missing
conan upload dlog/2.4@daixian/stable --all -r=artifactory