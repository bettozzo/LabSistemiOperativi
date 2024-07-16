# Stampa “T” (per True) o “F” (per False) a seconda che il valore rappresenti un file o una cartella esistente

#!/usr/bin/env bash

DATA="$1";
echo "Checking: $DATA";
([ -e $DATA ] && echo "T") || echo "F";

read