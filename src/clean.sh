#!/usr/bin/bash

for dir in `ls .`
    do  
        for dir in `ls .`
            do
            if [ -d $dir ]
            then
                echo $dir
                cd $dir
                make clean
                cd ..
            fi
            done 
done