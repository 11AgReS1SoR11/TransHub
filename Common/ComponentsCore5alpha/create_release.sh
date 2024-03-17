#!/bin/bash

version="1.6.0"

OK_STR="\e[1;32mOK\e[0m"
ERR_STR="\e[1;31mERR\e[0m"

curPWD=`pwd`
finishDir="components-core-5-"$version

printf "Start create release distributive...\n\n"

if [ -d $finishDir ]
then
  rm -r $finishDir
fi

mkdir $finishDir
cd $curPWD

# Qt Project:
buffFinishDir=$curPWD/$finishDir
cp *.h $buffFinishDir
cp *.cpp $buffFinishDir
cp *.txt $buffFinishDir
cp *.pro $buffFinishDir
cp ComponentManager $buffFinishDir
cp IComponent $buffFinishDir
cp Invoker $buffFinishDir
cp README.md $buffFinishDir
cp Version $buffFinishDir

# Create finish tar.gz
finishDirTar=$finishDir.tar.gz
tar -cvzf $finishDirTar $finishDir

if [ -d $finishDir ]
then
  rm -r $finishDir
fi

printf "Release distributive: $finishDirTar\n\n"
