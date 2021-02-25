CC        = g++
INCLUDE   = src
OBJ       = build
SRC       = src
SRCS      = $(wildcard $(SRC)/**/*.cc) src/main.cc
DEPDIR   := $(OBJ)/deps
DEPFILES := $(patsubst $(SRC)/%.cc,$(DEPDIR)/%.d,$(SRCS))
OBJS      = $(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(SRCS))
DEPFLAGS  = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
EXE       = raytrace
CFLAGS    = -g -I$(INCLUDE) -std=c++17
LDLIBS    = -lm

.PHONY: all run clean release debug

all: release

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
	@echo "Building final executable $@"
	@$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o : $(SRC)/%.cc $(DEPDIR)/%.d | $(DEPDIR)
	@echo "Compiling $@"
	@mkdir -p "$(@D)"
	@$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@ 

$(OBJ):
	@mkdir -p $@

$(DEPDIR): 
	@mkdir -p $@


# No compiler optimizations enabled, use default flags.
debug: $(EXE)

# Optimize in release mode
release: CFLAGS += -O3 -ffast-math $(OPENMP) -ftree-vectorize -msse2 -mfpmath=sse -flto=full -march=native
release: LDLIBS += $(OPENMP)
release: $(EXE)

clean:
	rm -rf $(OBJ) $(EXE)

include $(wildcard $(DEPFILES))