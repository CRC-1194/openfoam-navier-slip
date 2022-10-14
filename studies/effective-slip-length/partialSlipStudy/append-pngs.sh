#!/usr/bin/env bash

for var in *.linear.png; do mv $var ${var:0:4};done

for var in * ; do convert -annotate 0 '%i' -fill 'black' -pointsize 30 -gravity NorthWest $var {$var}.png; done
