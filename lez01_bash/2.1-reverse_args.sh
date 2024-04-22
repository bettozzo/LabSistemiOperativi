#Scrivere uno script che dato un qualunque numero di argomenti li restituisca in output in ordine inverso.

#!/usr/bin/env bash
for ((i = $#; i > 0; i--)); do
    echo ${!i}
done

read 