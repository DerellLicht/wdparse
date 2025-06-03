# makefile for media_list app
SHELL=cmd.exe
USE_DEBUG = NO
USE_64BIT = NO

ifeq ($(USE_64BIT),YES)
TOOLS=d:\tdm64\bin
else
TOOLS=c:\tdm32\bin
endif

ifeq ($(USE_DEBUG),YES)
CFLAGS = -Wall -g -c
LFLAGS = -g
else
CFLAGS = -Wall -s -O3 -c
LFLAGS = -s -O3
endif
CFLAGS += -Weffc++
CFLAGS += -Wno-write-strings
ifeq ($(USE_64BIT),YES)
CFLAGS += -DUSE_64BIT
endif

LIBS=-lshlwapi

LiFLAGS += -Ider_libs
CFLAGS += -Ider_libs

CPPSRC=wd_info.cpp wd_parse.cpp nsort.cpp \
der_libs\common_funcs.cpp \
der_libs\qualify.cpp 

#  clang-tidy options
CHFLAGS = -header-filter=.*
CHTAIL = --
CHTAIL += -Ider_libs
ifeq ($(USE_64BIT),YES)
CHTAIL += -DUSE_64BIT
endif
ifeq ($(USE_UNICODE),YES)
CHTAIL += -DUNICODE -D_UNICODE
endif


OBJS = $(CPPSRC:.cpp=.o)

#**************************************************************************
%.o: %.cpp
	$(TOOLS)\g++ $(CFLAGS) -c $< -o $@

ifeq ($(USE_64BIT),NO)
BIN = wdparse.exe
else
BIN = wdparse64.exe
endif

all: $(BIN)

clean:
	rm -f *.o *.exe *~ *.zip

dist:
	rm -f wdparse.zip
	zip wdparse.zip $(BIN) Readme.md

wc:
	wc -l $(CPPSRC)

check:
	cmd /C "d:\clang\bin\clang-tidy.exe $(CHFLAGS) $(CPPSRC) $(CHTAIL)"

lint:
	cmd /C "c:\lint9\lint-nt +v -width(160,4) $(LiFLAGS) -ic:\lint9 mingw.lnt -os(_lint.tmp) lintdefs.cpp $(CPPSRC)"

depend: 
	makedepend $(CPPSRC)

$(BIN): $(OBJS)
	$(TOOLS)\g++ $(OBJS) $(LFLAGS) -o $(BIN) $(LIBS) 

# DO NOT DELETE

wd_info.o: wd_info.h
wd_parse.o: wd_info.h
nsort.o: wd_info.h
