#/bin/bash

INCLUDE_DIR= ./include
CFLAGS= -I$(INCLUDE_DIR)
SRC= $(YEAR)/day_$(shell printf %02d $(DAY))

DAY ?= 1
PART ?= 1
YEAR ?= 2015

.PHONY: run

$(SRC)/input.txt:
	./input_dl.sh $(DAY) $(YEAR)

$(SRC)/part_$(PART).out: $(SRC)/part_$(PART).c
	incl="";\
	grep -E "#include \"\w+.h\"" $< | while read -r line; do\
		tmp=$$(echo $$line | sed -e "s/#include \"\(\w*\).h\"/include\/\1.c/");\
		[ -d $$tmp ] && incl=$$incl $$tmp;\
	done;\
	gcc -o $@ $< $$incl $(CFLAGS);

run: $(SRC)/part_$(PART).out
	$(SRC)/part_$(PART).out

