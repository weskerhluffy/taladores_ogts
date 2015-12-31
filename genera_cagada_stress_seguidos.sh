#!/bin/sh 
MAX_VALOR=1000000000
MAX_ELEMS=100000
#MAX_ELEMS=100
limite=$((MAX_VALOR - MAX_ELEMS + 2))
saltito=1
salto_inicial=$limite

shit=$(seq -f %1.f -s' ' $salto_inicial $saltito $MAX_VALOR)
echo "$MAX_ELEMS" > /tmp/como.txt
echo "1 $shit" >> /tmp/como.txt

shit=$(seq -f %1.f -s' ' $MAX_VALOR -$saltito $salto_inicial)
echo "$shit 0" >> /tmp/como.txt
