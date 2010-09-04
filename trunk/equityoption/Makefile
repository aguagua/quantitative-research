# Makefile
# 
#
# Created by William Wang on 01/09/2010.
# Copyright 2010 __MyCompanyName__. All rights reserved.

all: compile run

compile:
	g++ CBadRecord.cpp COptionPrice.cpp COptionRecord.cpp main.cpp -o bnpp -lQuantLib

run:
	./bnpp OptionData.txt