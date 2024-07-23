# CMake Build Configuration for root of project
cmake_minimum_required(VERSION 3.18)
project(find_the_dot VERSION 1.0 DESCRIPTION "Starter project" LANGUAGES C)

# Compiler options (inherited by sub-folders)
set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -g -Werror -Wpedantic -Wextra)
add_compile_options(-fdiagnostics-color)

# Enable address sanitizer
# (Comment this out to make your code faster)
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

# What folders to build
add_subdirectory(hal)  
add_subdirectory(app)
add_subdirectory(pru-as4)

# Copy the node server folder to specified directory
add_custom_target(copy_pru_files ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/../pru-as4
          $ENV{HOME}/cmpt433/public/myApps/pru-as4-copy
  COMMENT "Copying pru-as4 folder to public NFS directory"
)
