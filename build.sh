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
mv voxd31 build/linux64/
tar -czvf "build/voxd31-$TAG-x86_64-linux.tar.gz" build/linux64/*