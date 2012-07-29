#!/bin/bash
echo "Printing PDF file from PS file"
START=`date +%s%N`
#startnum=${#deb}
ps2pdf -sPAPERSIZE=a4 tetrapharmakos.ps
END=`date +%s%N`
#endnum=${#end}
TIME=$(( ($END-$START)/1000000 ))
echo "Done, in ~"$TIME "µs."
