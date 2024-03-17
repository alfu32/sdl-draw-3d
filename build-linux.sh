#!/bin/bash


TAG=$(git describe --tags --abbrev=0)
COMMIT_HASH=$(git rev-parse HEAD)
DATE=$(date +%Y%m%d)

echo "          TAG: $TAG"
echo "  COMMIT_HASH: $COMMIT_HASH"
echo "         DATE: $DATE"

ls -la build
ls -la build/linux64

rm -f build/linux64/voxd31
rm -f build/voxd31-*
ls -la build
ls -la build/linux64
make
mv voxd31 build/linux64/
tar -czvf "build/voxd31-$TAG-x86_64-linux.tar.gz" build/linux64/*