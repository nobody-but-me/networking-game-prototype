
#!/usr/bin/env bash

echo "[CONFIG]: Are you generating ninja files? (y or n)(DEFAULT: n)"
read -p " > " build
echo "[CONFIG]: Do you want to run the project right after building it? (y or n)(DEFAULT: y)"
read -p " > " run

echo "[INFO]: creating build folder..."
mkdir -p ./build && cd ./build

if [ "$build" = "y" ]; then
    echo "[INFO]: generating ninja files..."
    cmake .. -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
			-DGLFW_LIB=/usr/local/lib64/libglfw3.a							\
			-DENET_LIB=/usr/local/lib/libenet.a								
fi

echo "[INFO]: building..."
ninja

cd ..

if  [ "$run" != "n" ]; then
    echo "\n--------------------------------------------------\n"
    ./run.sh
fi

