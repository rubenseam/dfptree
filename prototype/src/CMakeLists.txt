# ----------------------------------------------------------------- #
#                                                                   #
#                                                                   #
#           Universidade Federal de Minas Gerais - UFMG             #
#               Departamento de Ciência da Computação               #
#           -------------------------------------------             #
#                   Rubens Emilio Alves Moreira                     #
#                       [rubens@dcc.ufmg.br]                        #
#                                                                   #
#                                                                   #
# ----------------------------------------------------------------- #

cmake_minimum_required (VERSION 2.6)
project (DFPTree CXX)

# Compiler
set (CMAKE_CXX_COMPILER "mpic++")
set (CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Flags
set (CMAKE_CXX_FLAGS "-std=c++0x -O3 -fPIC")

link_directories ("$ENV{SNAPPY}/lib")
link_directories ("$ENV{WATERSHED}/lib")
include_directories ("$ENV{SNAPPY}")
include_directories ("$ENV{XERCES}/include")
include_directories ("$ENV{WATERSHED}/include")

# Paths
add_subdirectory (process)
add_subdirectory (init)
add_subdirectory (tree)
add_subdirectory (sum)
add_subdirectory (merge)
add_subdirectory (reduce)
