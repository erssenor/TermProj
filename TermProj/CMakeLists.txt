cmake_minimum_required(VERSION 3.15)
project(TermProj)

set(CMAKE_CXX_STANDARD 14)

add_executable(TermProj main.cpp VDIFile.cpp VDIFile.h VDIheader.h Partition.h Partition.cpp ext2.h ext2.cpp dir.h)