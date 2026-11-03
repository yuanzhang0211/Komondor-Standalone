# Shared build contract for the root entry and Code/main. CLI overrides are explicit.
PYTHON ?= python3
CXX = g++
PROFILE ?= release
ifeq ($(PROFILE),release)
PROFILE_FLAGS := -O2 -g -Wall -std=c++98
else ifeq ($(PROFILE),debug)
PROFILE_FLAGS := -O0 -g3 -Wall -std=c++98
else ifeq ($(PROFILE),sanitize)
PROFILE_FLAGS := -O1 -g -Wall -std=c++98 -fsanitize=address,undefined -fno-omit-frame-pointer
else
$(error Unknown PROFILE '$(PROFILE)'; use release, debug, or sanitize)
endif
CXXFLAGS ?= $(PROFILE_FLAGS)
CPPFLAGS ?=
LDFLAGS ?=
export PYTHON CXX PROFILE CXXFLAGS CPPFLAGS LDFLAGS
export PYTHONDONTWRITEBYTECODE := 1
