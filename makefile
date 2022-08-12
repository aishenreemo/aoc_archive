#/bin/bash

DAY ?= 1
PART ?= 1
YEAR ?= 2015

INCLUDE_DIR= ./include
CFLAGS= -I$(INCLUDE_DIR)
SRC= $(YEAR)/day_$(shell printf %02d $(DAY))

.PHONY: run

$(SRC)/input.txt:
	./input_dl.sh $(DAY) $(YEAR)

$(SRC)/part_$(PART).out: $(SRC)/part_$(PART).c
	gcc -o $@ $< $(CFLAGS)

run: $(SRC)/part_$(PART).out
	$(SRC)/part_$(PART).out
