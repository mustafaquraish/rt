#!/bin/bash

if [[ -d frames && $# -eq 0 ]]
then
  echo "'frames' directory exists. Run with -f to overwrite"
  exit 1
fi

for i in `seq -w 00 59`
do
  echo "------ FRAME $i -------"
  ./raytrace 300 $i
  convert output.ppm frames/frame_$i.png
done

convert frames/*.png anim.gif