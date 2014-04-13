CXXFLAGS := -I$(PWD) -Wall -Werror -O2 --std=c++0x
LDFLAGS  := -lrt

SRCFILES := 
OBJFILES := $(SRCFILES:.cc=.o)

PROGS 	 := bench

all: $(PROGS)

%.o: %.cc 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PROGS): %: %.o $(OBJFILES)
	$(CXX) $< -o $@ $(OBJFILES) $(LDFLAGS)

DEPFILES := $(wildcard *.d converters/*.d tools/*.d)
ifneq ($(DEPFILES),)
-include $(DEPFILES)
endif

.PHONY: clean
clean:
	rm -f *.o *.d $(PROGS)
