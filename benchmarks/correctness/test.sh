PATH2LIB="/n/eecs583a/home/erzh/EECS_583/build/datapass/HW2Pass.so"
PASS=BasicPass


clang -emit-llvm -c ${1}.c -Xclang -disable-O0-optnone -o ${1}.bc

opt -load-pass-plugin="${PATH2LIB}" -passes="${PASS}" ${1}.bc -o ${1}.test.bc > /dev/null