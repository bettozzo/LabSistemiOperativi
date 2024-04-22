# Scrivere uno script che mostri il contenuto della cartella corrente in ordine inverso rispetto all’output generato da “ls”.
# Per semplicità, assumere che tutti i file e le cartelle non abbiano spazi nel nome.

#!/usr/bin/env bash

DATA=$( ls );
echo "NORMAL ORDER:";
for el in $DATA; do
    echo $el;
done

echo ;
echo ;
echo ;

echo "REVERSE ORDER:";
bash ./2.1-reverse_args.sh $DATA;
