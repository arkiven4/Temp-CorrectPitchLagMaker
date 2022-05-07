#
# Makefile for commands
#

TARGET		= CorrectPitchLagMaker
SRCS		= $(TARGET).cc
OBJS		= $(SRCS:.cc=.o)

PREFIX		= /usr/local/SPTK
BIN		= $(PREFIX)/bin

CC		= g++
RM		= rm -rf
INSTALL		= install -cs

INCDIR		= -I/usr/local/SPTK/include
LIBDIR		= -L/usr/local/SPTK/lib
CFLAGS		= -O2 $(INCDIR)
LDFLAGS		= $(LIBDIR)
LIBS		= -lSPTK -lm


.c.o:
	$(CC) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) 
#	mv $(TARGET) ../../

install: $(TARGET)
	$(INSTALL) $(TARGET) $(BIN)

clean:
	$(RM) $(TARGET) $(OBJS) \#* *~

veryclean: clean
	$(RM) $(BIN)/$(TARGET)

