# Creare un makefile con una regola help di default che mostri una nota informativa, 
# una regola backup che crei un backup di una cartella appendendo “.bak” al nome 
# e una clear che cancelli il backup. Per definire la cartella 
# sorgente passarne il nome come variabile, ad esempio:
# make -f mf-backup FOLDER=... 

SHELL := /bin/bash
FOLDER="None"

help: 
	@echo "Eseguire target 'backup' per eseguire il backup di una cartella"
	@echo  "	make -f mf-backup backup FOLDER=..."
	@echo ""
	@echo 
	@echo "Eseguire target 'clear' per rimuovere il backup di una cartella"
	@echo  "	make -f mf-backup clear FOLDER=..."


backup:
	@if [[ ! -e $(FOLDER).bak ]]; then\
		echo "Eseguo backup di $$FOLDER";\
		cp -r $(FOLDER) $(FOLDER).bak;\
	else\
		echo "Gia esiste il backup";\
	fi


clear:
	@if [[ -e $(FOLDER).bak ]]; then\
		echo "Rimuovo backup di $$FOLDER";\
		rm -r $(FOLDER).bak;\
	else\
		echo "Non esiste il backup";\
	fi
