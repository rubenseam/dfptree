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

set (OUTPUT reduce)

# Build
add_library (${OUTPUT} SHARED reduce.cpp)
target_link_libraries (${OUTPUT} snappy)
target_link_libraries (${OUTPUT} watershed)
