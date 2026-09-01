USE_DEBUG = NO
USE_64BIT = NO
USE_UNICODE = NO
USE_CLANG = NO
# sadly, cygwin mingw does not support gdiplus...
USE_CYGWIN = NO
#  Why am I using this??
#  Because the new <string> version of qualify is Unicode only
USE_LEGACY = YES

include der_libs\tool_select.mak

ifeq ($(USE_DEBUG),YES)
CFLAGS = -Wall -g -c
LFLAGS = -g
else
CFLAGS = -Wall -O3 -c
LFLAGS = -s -O3
endif
CFLAGS += -Weffc++
CFLAGS += -Wno-write-strings

LIBS=-lshlwapi

LiFLAGS += -Ider_libs
CFLAGS += -Ider_libs
IFLAGS += -Ider_libs

# This is required for *some* versions of makedepend
IFLAGS += -DNOMAKEDEPEND

ifeq ($(USE_LEGACY),YES)
# this flag is used by qualify.h
CFLAGS += -DLEGACY_QUALIFY
endif

CPPSRC=wd_info.cpp wd_parse.cpp nsort.cpp \
der_libs/common_funcs.cpp 

ifeq ($(USE_LEGACY),YES)
CPPSRC+=der_libs/qualify_orig.cpp 
else
CPPSRC+=der_libs/qualify.cpp 
endif

BASE := wdparse
BIN = $(BASE).exe

OBJS = $(CPPSRC:.cpp=.o)

# Automatically parse the latest version block
VERSION := $(shell grep -oE '\[[0-9]+\.[0-9]+\]' CHANGELOG.md | head -n 1 | tr -d '[]')
DIST_ZIP := $(BASE)V$(VERSION).zip

# Force these action-only targets to always run
.PHONY: dist release update

#**************************************************************************
%.o: %.cpp
	$(TOOLS)\$(GNAME) $(CFLAGS) $< -o $@

all: $(BIN)

clean:
	rm -f $(OBJS) *.exe *~ *.zip

dist:
	rm -f *.zip
	zip $(DIST_ZIP) $(BIN) Readme.md LICENSE.txt CHANGELOG.md

# Your new automated release workflow
release: dist
	@cmd /C "@echo Preparing GitHub release for v$(VERSION)..."
	sed -n '/## \['$(VERSION)'\]/,/## \[/p' CHANGELOG.md | sed '$$d' > temp_notes.md
	gh release create v$(VERSION) ./$(DIST_ZIP) ./CHANGELOG.md --notes-file temp_notes.md
	rm temp_notes.md
	@cmd /C "@echo Release v$(VERSION) successfully uploaded to GitHub!"
	
# Your corrected, bulletproof update-in-place pipeline
update: dist
	@cmd /C "@echo Updating assets for existing release v$(VERSION)..."
	gh release upload v$(VERSION) ./$(DIST_ZIP) ./CHANGELOG.md --clobber
	@cmd /C "@echo Release v$(VERSION) assets successfully updated on GitHub!"

wc:
	wc -l $(CPPSRC)

clint:
	cmd /C "python ..\ClaudeLint.py --exclude der_libs"
	
check:
	cmd /C "d:\llvm\bin\clang-tidy.exe $(CPPSRC)"

cppc:
	cmd /C "cppcheck --project=compile_commands.json --std=c++14 --suppressions-list=./.suppress.cppcheck"

depend: 
	makedepend $(IFLAGS) $(CPPSRC)

$(BIN): $(OBJS)
	$(TOOLS)/$(GNAME) $(OBJS) $(LFLAGS) -o $(BIN) $(LIBS) 

# DO NOT DELETE

wd_info.o: der_libs/common.h wd_info.h der_libs/qualify.h
wd_parse.o: der_libs/common.h wd_info.h
nsort.o: der_libs/common.h wd_info.h
der_libs/common_funcs.o: der_libs/common.h
der_libs/qualify_orig.o: der_libs/common.h der_libs/qualify.h
