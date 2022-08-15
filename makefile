#/bin/bash

INCLUDE_DIR= ./include
CFLAGS= -I$(INCLUDE_DIR)
SRC= $(YEAR)/day_$(shell printf %02d $(DAY))

DAY ?= 1
PART ?= 1
YEAR ?= 2015
ADD_ONS ?=

.PHONY: run

$(SRC)/input.txt:
	./input_dl.sh $(DAY) $(YEAR)

$(SRC)/part_$(PART).out: $(SRC)/part_$(PART).c $(ADD_ONS)
	gcc -o $@ $^ $(CFLAGS)

run: $(SRC)/part_$(PART).out
	$(SRC)/part_$(PART).out

