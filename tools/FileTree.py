#!/usr/bin/python
# -*- coding: UTF-8 -*-
import os
import sys
import getopt


def list_files(startpath):
    dircount = 0
    filecount = 0
    for root, dirs, files in os.walk(startpath):
        dircount += len(dirs)
        filecount += len(files)
        level = root.replace(startpath, '').count(os.sep)
        indent = ' ' * 4 * (level)
        print("D "+'{}{}/'.format(indent, os.path.basename(root)))
        subindent = ' ' * 4 * (level + 1)
        for f in files:
            path = os.path.join(root, f)
            size = os.stat(path).st_size
            print("F "+'{}{} ({} bytes)'.format(subindent, f, size))
    print("{} files in {} folders".format(filecount, dircount))


def main(argv):
    dirPath = './'
    try:
        # http://www.runoob.com/python/python-command-line-arguments.html?tdsourcetag=s_pcqq_aiomsg
        # 返回值由两个元素组成：第一个是（选项，值）对的列表;第二个是剥离选项列表后留下的程序参数列表（这是第一个参数的尾部切片）。
        # 返回的每个选项和值对都有选项作为其第一个元素，前缀为连字符（例如，' -  x'），
        # 选项参数作为其第二个元素，如果选项没有参数，则为空字符串。选项以与查找顺序相同的顺序出现在列表中，从而允许多次出现。多头和空头选择可能是混合的。
        #opts, args = getopt.getopt(argv, "hd:", ["dir="])
        opts_args = getopt.getopt(argv, "hd:", ["help","dir="])
        opts = opts_args[0]
    except getopt.GetoptError:
        print('FileTree.py -d <dir>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            print('FileTree.py -d <dir>')
            sys.exit()
        elif opt in ("-d", "--dir"):
            dirPath = arg
    list_files(dirPath)


if __name__ == "__main__":
    main(sys.argv[1:])
