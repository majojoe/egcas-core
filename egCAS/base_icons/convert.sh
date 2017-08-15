#!/bin/bash
RESSORCE_DIR="../src/res/"
convert -density 200 calculator.svg -resize 32x32 -transparent white "${RESSORCE_DIR}/calculator.png" 
convert -density 200 egcas.svg -resize 32x32 -transparent white "${RESSORCE_DIR}/egcas.png" 