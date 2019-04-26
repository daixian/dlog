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
        print("D"+'{}{}/'.format(indent, os.path.basename(root)))
        subindent = ' ' * 4 * (level + 1)
        for f in files:
            path = os.path.join(root, f)
            size = os.stat(path).st_size
            print("F"+'{}{} ({} bytes)'.format(subindent, f, size))
    print("{} files in {} folders".format(filecount, dircount))


def main(argv):
    dirPath = './'
    try:
        #opts, args = getopt.getopt(argv, "hd:", ["dir="])
        opts_args = getopt.getopt(argv, "hd:", ["dir="])
    except getopt.GetoptError:
        print('FileTree.py -d <dir>')
        sys.exit(2)
    for opt, arg in opts_args[0]:
        if opt == '-h':
            print('FileTree.py -d <dir>')
            sys.exit()
        elif opt in ("-d", "--dir"):
            dirPath = arg
    list_files(dirPath)


if __name__ == "__main__":
    main(sys.argv[1:])
