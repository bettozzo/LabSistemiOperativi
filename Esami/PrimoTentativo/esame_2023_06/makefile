SHELL := /bin/bash
.DEFAULT_GOAL := build
FILE = "NONE"
DIR = "NONE"
EXE = "NONE"

build:
	@ if [[ -f $(FILE) ]]; then \
		echo $(EXE); \
		mkdir $(DIR); \
		cp $(FILE) $(DIR); \
		gcc main.c -o $(EXE); \
		cp $(EXE) $(DIR); \
		rm $(EXE); \
	else \
		echo "No no!"; \
	fi \
