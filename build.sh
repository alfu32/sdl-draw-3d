#!/bin/bash

TAG=$1
if [[ "$TAG" == "" ]];then
    TAG="$(git describe --tags --abbrev=0)"
fi

if [[ "$TAG" == "" ]];then
    TAG=$(git rev-parse HEAD)
fi

DATE=$(date +%Y%m%d)

echo "          TAG: $TAG"
echo "         DATE: $DATE"

ls -la build
ls -la build/linux64

rm -f build/linux64/voxd31
rm -f build/voxd31-*-linux.tar.gz
ls -la build
ls -la build/linux64
make
# gcc -o voxd31 main.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s -O1 -D_DEFAULT_SOURCE -I/usr/local/include -isystem. -isystem/home/alfu64/Development/src -isystem/home/alfu64/Development/release/include -isystem/home/alfu64/Development/src/external -L. -L/usr/local/lib -L/home/alfu64/Development/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -DPLATFORM_DESKTOP
mv voxd31 build/linux64/
tar -czvf "build/voxd31-$TAG-x86_64-linux.tar.gz" build/linux64/*