SHELL := /bin/bash
.DEFAULT_GOAL := build
DEST = "NONE"

build:
	@gcc -Wall main.c -o coda;
	@if [[ $(DEST) != "NONE" ]]; then \
		if [[ -d $(DEST) ]]; then \
			cp coda $(DEST); \
			rm coda; \
		else \
			echo "?Error"; \
		fi; \
	fi\