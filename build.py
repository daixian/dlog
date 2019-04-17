# coding: utf-8
import urllib
from urllib import request
import os

#下载
dirConcur = "./concurrentqueue"
if not os.path.exists(dirConcur):
    os.makedirs(dirConcur)

print("下载 concurrentqueue ...")
url = 'https://github.com/cameron314/concurrentqueue/raw/master/blockingconcurrentqueue.h'
request.urlretrieve(url, dirConcur + "/blockingconcurrentqueue.h")
url = "https://github.com/cameron314/concurrentqueue/raw/master/concurrentqueue.h"
request.urlretrieve(url, dirConcur + "/concurrentqueue.h")
print("下载完成")