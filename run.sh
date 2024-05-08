cmake -B build && cmake --build build
cmake -B build_avx -DBUILD_AVX=ON && cmake --build build_avx
cmake -B build_sse -DBUILD_SSE=ON && cmake --build build_sse

cmake -B build_release -DCMAKE_BUILD_TYPE=Release && cmake --build build_release
cmake -B build_release_avx -DCMAKE_BUILD_TYPE=Release -DBUILD_AVX=ON && cmake --build build_release_avx
cmake -B build_release_sse -DCMAKE_BUILD_TYPE=Release -DBUILD_SSE=ON && cmake --build build_release_sse

echo "========== Debug mode =========="
./build/main
./build_avx/main
./build_sse/main

echo "========== Release mode =========="
./build_release/main
./build_release_avx/main
./build_release_sse/main
