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

set (OUTPUT init)

# Build
add_library (${OUTPUT} SHARED init.cpp)
target_link_libraries (${OUTPUT} snappy)
target_link_libraries (${OUTPUT} watershed)
