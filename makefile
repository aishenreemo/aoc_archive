#/bin/bash

INCLUDE_DIR= ./include
CFLAGS= -I$(INCLUDE_DIR)
SRC= $(YEAR)/day_$(shell printf %02d $(DAY))

DAY ?= 1
PART ?= 1
YEAR ?= 2015

MAIN= $(SRC)/part_$(PART).c
SED= "s/.include \"\(\w*\).h\"/\1/g"
GREP= $(shell grep -E "include \"\w+.h\"" $(MAIN) | sed -e $(SED))
INCLUDE= $(strip $(foreach lib,$(GREP),$(shell ls include/$(lib).c 2> /dev/null)))

.PHONY: run

$(SRC)/input.txt:
	./input_dl.sh $(DAY) $(YEAR)

$(SRC)/part_$(PART).out: $(MAIN)
	gcc -o $@ $(MAIN) $(INCLUDE) $(CFLAGS);

run: $(SRC)/part_$(PART).out
	$<;
