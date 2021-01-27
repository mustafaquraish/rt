CC       = g++
INCLUDE  = src
OBJ      = build
SRC      = src
SRCS     = $(wildcard $(SRC)/**/*.cc) src/main.cc
OBJS     = $(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(SRCS))
EXE      = raytrace
CFLAGS   = -g -I$(INCLUDE) -std=c++17
LDLIBS   = -lm

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

$(EXE): $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%.cc | $(OBJ)
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ):
	mkdir -p $@

# No compiler optimizations enabled, use default flags.
debug: clean $(EXE)

# Optimize in release mode
release: CFLAGS += -O3 -ffast-math $(OPENMP) -ftree-vectorize -msse2 -mfpmath=sse -flto=full -march=native
release: LDLIBS += $(OPENMP)
release: $(EXE)

clean:
	rm -rf $(OBJ) $(EXE)

