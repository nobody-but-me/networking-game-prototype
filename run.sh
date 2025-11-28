
#!/usr/bin/env bash

# if [ "$(type -t ./build/bin/mol)" = "file" ]; then
if [ -f ./build/bin/prototype ]; then
    echo "[INFO]: running...\n"
    
    cd ./build/bin/
    
    ./prototype $1
    
    cd ..
else
    echo "[ERROR]: could not run engine: executable does not exist or has some error. \n"
fi
