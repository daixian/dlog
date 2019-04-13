#! /bin/bash
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

__INTERACTIVE=""
if [ -t 1 ] ; then
    __INTERACTIVE="1"
fi

__green(){
    if [ "$__INTERACTIVE" ] ; then
        printf '\033[1;31;32m'
    fi
    printf -- "$1"
    if [ "$__INTERACTIVE" ] ; then
        printf '\033[0m'
    fi
}

__red(){
    if [ "$__INTERACTIVE" ] ; then
        printf '\033[1;31;40m'
    fi
    printf -- "$1"
    if [ "$__INTERACTIVE" ] ; then
        printf '\033[0m'
    fi
}

__yellow(){
    if [ "$__INTERACTIVE" ] ; then
        printf '\033[1;31;33m'
    fi
    printf -- "$1"
    if [ "$__INTERACTIVE" ] ; then
        printf '\033[0m'
    fi
}

#Current folder
cur_dir=`pwd`
cd $cur_dir
#lib_dir="$cur_dir/lib"

# Download spd
function download_spdlog(){
    spdlog_version='1.3.1'
    spdlog_file="v$spdlog_version.tar.gz"
    if [ -f $spdlog_file ];then
        echo -e "$spdlog_file [$(__green "found")]"
    else
        if ! wget --no-check-certificate https://github.com/gabime/spdlog/archive/$spdlog_file;then
            echo "Failed to download $spdlog_file"
            exit 1
        fi
    fi
    tar xzf $spdlog_file
    if [ $? -eq 0 ];then
        # 把文件夹改名成spdlog
        rm -rf "$cur_dir/spdlog"
        mv "$cur_dir/spdlog-$spdlog_version" "$cur_dir/spdlog"
    else
        echo ""
        echo "Unzip $spdlog_file failed!"
        exit 1
    fi
}

# Download gtest
function download_gtest(){
    gtest_version='1.8.1'
    gtest_file="release-$gtest_version.tar.gz"
    if [ -f $gtest_file ];then
        echo -e "$gtest_file [$(__green "found")]"
    else
        if ! wget --no-check-certificate https://github.com/google/googletest/archive/$gtest_file;then
            echo "Failed to download $gtest_file"
            exit 1
        fi
    fi
    tar xzf $gtest_file
    if [ $? -eq 0 ];then
        # 把文件夹改名成spdlog
        rm -rf "$cur_dir/gtest"
        mv "$cur_dir/googletest-release-$gtest_version" "$cur_dir/gtest"

    else
        echo ""
        echo "Unzip $gtest_file failed!"
        exit 1
    fi
}

# Download concurrentqueue
function download_concurrentqueue(){
concurrentqueue_dir="$cur_dir/concurrentqueue"
rm -rf "$concurrentqueue_dir"
mkdir "$concurrentqueue_dir"
if ! wget --no-check-certificate -P "$concurrentqueue_dir" https://github.com/cameron314/concurrentqueue/raw/master/blockingconcurrentqueue.h;then
    echo "Failed to download concurrentqueue"
    exit 1
fi
if ! wget --no-check-certificate -P "$concurrentqueue_dir" https://github.com/cameron314/concurrentqueue/raw/master/concurrentqueue.h;then
    echo "Failed to download concurrentqueue"
    exit 1
fi
}

download_gtest
download_spdlog
download_concurrentqueue