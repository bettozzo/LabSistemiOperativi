SHELL := /bin/bash
.DEFAULT_GOAL := build
FILE = "NONE"

build:
	clear;
	@if [[ -f $(FILE) ]]; then \
		gcc -Wall main.c -o SignalProxy; \
	else \
		gcc main.c -o SignalProxy; \
		touch $(FILE); \
		echo "start" > $(FILE);\
	fi;

clean:
	@if [[ -f $(FILE) ]]; then \
		rm SignalProxy; \
		rm $(FILE);\
	fi;