SHELL := /bin/bash
include ../../build.mk
COST_WRAPPER := ../COST/cxx
BIN := komondor_main
GENERATED := komondor_main.cxx
# COST expands includes into generated C++; every source/header must trigger regeneration.
SOURCES := $(shell find .. -type f \( -name '*.h' -o -name '*.cc' -o -name '*.cpp' -o -name '*.l' \) ! -name 'compcxx_*')
.PHONY: all clean FORCE
all: $(BIN)
$(BIN): $(GENERATED) .build-config
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $(GENERATED) $(LDFLAGS)
.build-config: FORCE
	@$(PYTHON) ../../scripts/build_environment.py --stamp $@
FORCE:
$(GENERATED): $(SOURCES) $(COST_WRAPPER) Makefile ../../build.mk
	rm -f $@ compcxx_komondor_main.h
	$(COST_WRAPPER) komondor_main.cc
clean:
	rm -f $(BIN) $(GENERATED) compcxx_komondor_main.h .build-config .build-config.tmp
