rm -rf build 
cmake -S test -B build/ \
          -G "Unix Makefiles" \
          -DCMAKE_BUILD_TYPE=Debug \
          -DBUILD_CLONE_SUBMODULES=ON \
          -DUNITTEST=1 \
          -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Wsign-compare -DLIBRARY_LOG_LEVEL=LOG_DEBUG'
make -C build all
#cd build && ctest -E system --output-on-failure --verbose
cd build && ctest
make coverage
cd ..

#cd build && ctest -E system --output-on-failure --verbose