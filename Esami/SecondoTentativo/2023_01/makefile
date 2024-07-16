SHELL := /bin/bash 
.DEFAULT_GOAL := build 
FILE="NONE"

build:
	@if [[ ! -e $(FILE) ]]; then \
		touch $(FILE); \
	fi; \
	gcc main.c -o SignalProxy; \

clean:
	@if [[ -e SignalProxy ]]; then \
		@rm SignalProxy; \
	fi; \
	if [[ -e $(FILE) ]]; then \
		rm $(FILE); \
	fi \
