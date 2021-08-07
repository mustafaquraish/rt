CC        = g++
INCLUDE   = src
OBJ       = build
SRC       = src
SRCS      = $(SRC)/main.cc
SRCS     += $(wildcard $(SRC)/*/*.cc)
SRCS     += $(wildcard $(SRC)/*/*/*.cc)
DEPDIR   := $(OBJ)/deps
DEPFILES := $(patsubst $(SRC)/%.cc,$(DEPDIR)/%.d,$(SRCS))
OBJS      = $(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(SRCS))
DEPFLAGS  = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
EXE       = raytrace
CFLAGS    = -g -I$(INCLUDE) -std=c++17
LDLIBS    = -lm -lpng
MODE      = "Debug"

.PHONY: all run clean release debug

all: release

ifneq (, $(shell which ccache))
 CC = ccache g++
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	OPENMP += -fopenmp
endif
ifeq ($(UNAME_S),Darwin)
	OPENMP += -Xpreprocessor -fopenmp
	LDLIBS += -lomp
endif

$(DEPFILES):
	@mkdir -p "$(@D)"
	
$(EXE): $(OBJS) | $(BIN)
	@echo "($(MODE)) Building final executable $@"
	@$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o : $(SRC)/%.cc $(DEPDIR)/%.d | $(DEPDIR)
	@echo "($(MODE)) Compiling $@"
	@mkdir -p "$(@D)"
	@$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@ 

$(OBJ):
	@mkdir -p $@

$(DEPDIR): 
	@mkdir -p $@


# No compiler optimizations enabled, use default flags.
debug: $(EXE)

# Optimize in release mode
release: MODE = "Release"
release: CFLAGS += -O3 -ffast-math $(OPENMP) -ftree-vectorize -msse2 -mfpmath=sse -flto=full -march=native
release: LDLIBS += $(OPENMP)
release: $(EXE)

clean:
	rm -rf $(OBJ) $(EXE)

include $(wildcard $(DEPFILES))