#!/bin/bash

cd  ../apps/examples/
for example in $( ls )
do
echo "-----------------------------------------------------------------"
echo "building " $example
cd $example/
./openFrameworksDebug.app/Contents/MacOS/openFrameworksDebug
cd ../
echo "-----------------------------------------------------------------"
echo ""
done
