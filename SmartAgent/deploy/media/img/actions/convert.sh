#!/bin/sh


echo "Converting Actions"
for f in *jpg
do
    filename="$f"
    echo $filename
    if [ ! -f "min_$filename" ];
        then convert -resize 580x580 $f "min_$filename"
    fi
done