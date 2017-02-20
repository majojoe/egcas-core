#!/bin/bash
RESSORCE_DIR="../src/res/"
convert -density 200 calculator.svg -resize 32x32 -transparent white "${RESSORCE_DIR}/calculator.png" 
