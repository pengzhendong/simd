cmake_minimum_required(VERSION 3.14)
project("simd" LANGUAGES C)

option(BUILD_AVX "Enable AVX" OFF)
option(BUILD_SSE "Enable SSE" OFF)

add_executable(main main.c)

if(BUILD_AVX)
  add_compile_definitions(USE_AVX)
  target_compile_options(main PRIVATE -mavx -mfma -mavx2)
elseif(BUILD_SSE)
  add_compile_definitions(USE_SSE)
  target_compile_options(main PRIVATE -msse4.1)
endif()
