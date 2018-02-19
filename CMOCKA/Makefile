CC = cc
CFLAGS = -std=c99 -Wall -g -O0

#
# build third-party libraries
#
SUBDIRS = 3rd-party

ifdef SUBDIRS
.PHONY : $(SUBDIRS)
$(SUBDIRS) : 
	@if [ -d $@ ]; then \
		$(MAKE) --no-print-directory --directory=$@ \
			CC=$(CC) CFLAGS="$(CFLAGS)" $(MAKECMDGOALS); \
	fi	
endif


THIRD_PARTY_DIR = 3rd-party
CMOCKA_INCLUDE_DIR = $(THIRD_PARTY_DIR)/build-Debug/include
CMOCKA_LIBRARY = $(THIRD_PARTY_DIR)/build-Debug/lib/libcmocka.a

cmocka : $(SUBDIRS)

CFLAGS += -I $(CMOCKA_INCLUDE_DIR)

#
# quick and dirty build rules to demonstrate testing, not good build
# system practices.
#
#
# library
#
#LIB_SRCS =\

 

#LIB_OBJS = $(LIB_SRCS:%.c=%.o)

#LIB_HEADERS = $(LIB_SRCS:%.c=%.h)

LIB = libutils.a

#
# main executable
#
#EXE_SRCS = main.c
#EXE_OBJS = $(EXE_SRCS:%.c=%.o)
#EXE = test_main.out

#
# tests
#
TEST_SRCS = main.c 

TEST_OBJS = $(TEST_SRCS:%.c=%.o)

TEST_EXE = test_main.out



#
# Generic rule to generate objects
#
%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB) : $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

all : $(LIB) $(EXE)

$(EXE) : $(EXE_OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ -lm

test : $(TEST_EXE)
	./$(TEST_EXE)

$(TEST_EXE) : $(TEST_SRCS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(CMOCKA_LIBRARY) -lm


.PHONY : clean
clean : FORCE
	-rm -rf *~ *.o $(LIB) $(EXE) $(TEST_EXE) *.dSYM/

.PHONY : clobber
clobber : $(SUBDIRS) clean


FORCE :
