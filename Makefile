# Makefile for CS 453 HW 8

SRC =  stereo2.cpp sgm.cpp crosscheck.cpp
BIN =  stereo2

IMGLIB = /home/schar/cs453/imageLib
# IMGLIB = ../imageLib
ARCH := $(shell arch)

DBG = -g
CC = g++
CPPFLAGS = -O2 -W -Wall -I$(IMGLIB)
LDLIBS = -L$(IMGLIB) -lImg.$(ARCH)$(DBG) -lpng -lz
OBJ = $(SRC:.cpp=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDLIBS)

clean: 
	rm -f $(BIN) $(OBJ)
