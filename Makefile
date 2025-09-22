# SHELL=cmd.exe
USE_DEBUG = NO
USE_64BIT = NO
USE_LEGACY = YES

ifeq ($(USE_64BIT),YES)
TOOLS=d:\tdm64\bin
else
TOOLS=d:\tdm32\bin
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
IFLAGS += -Ider_libs

# This is required for *some* versions of makedepend
IFLAGS += -DNOMAKEDEPEND

ifeq ($(USE_LEGACY),YES)
CFLAGS += -DLEGACY_QUALIFY
endif

CPPSRC=wd_info.cpp wd_parse.cpp nsort.cpp \
der_libs\common_funcs.cpp 

ifeq ($(USE_LEGACY),YES)
CPPSRC+=der_libs\qualify_orig.cpp 
else
CPPSRC+=der_libs\qualify.cpp 
endif

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

LINTFILES=lintdefs.cpp lintdefs.ref.h 

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
	rm -f $(OBJS) *.exe *~ *.zip

dist:
	rm -f wdparse.zip
	zip wdparse.zip $(BIN) Readme.md

wc:
	wc -l $(CPPSRC)

check:
	cmd /C "d:\clang\bin\clang-tidy.exe $(CHFLAGS) $(CPPSRC) $(CHTAIL)"

lint:
	cmd /C "c:\lint9\lint-nt +v -width(160,4) $(LiFLAGS) -ic:\lint9 mingw.lnt -os(_lint.tmp) $(LINTFILES) $(CPPSRC)"

depend: 
	makedepend $(IFLAGS) $(CPPSRC)

$(BIN): $(OBJS)
	$(TOOLS)\g++ $(OBJS) $(LFLAGS) -o $(BIN) $(LIBS) 

# DO NOT DELETE

wd_info.o: der_libs/common.h wd_info.h der_libs/qualify.h
wd_parse.o: der_libs/common.h wd_info.h
nsort.o: der_libs/common.h wd_info.h
der_libs\common_funcs.o: der_libs/common.h
der_libs\qualify_orig.o: der_libs/common.h der_libs/qualify.h
