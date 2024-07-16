# Stampa “file”, “cartella” o “?” a seconda che il valore rappresenti un file (esistente), una cartella (esistente) o una voce non presente nel file-system


#!/usr/bin/env bash

DATA="$1";
echo "Checking: $DATA";

([ ! -e $DATA ] && echo "?") || ([ -f $DATA ] && echo "file") || ([ -d $DATA ] && echo "cartella");

read