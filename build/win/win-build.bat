@ECHO OFF

set arg1=%1

lua ../auto-increment.lua ../../engine/include/k_version.hpp %arg1% 1

mkdir "../temp"
cd "../temp"

cmake ../.. -DCMAKE_BUILD_TYPE=%arg1%
cmake --build . --config %arg1%

cd ..
rmdir /s /q temp
cd win
