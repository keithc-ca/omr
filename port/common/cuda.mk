CXX ?= g++
MAPTOOL := omrcudaerrors
MAPPINGS :=
VERSIONS := 9.2 10.0 10.1 10.2

# parameter 1 : CUDA version number (e.g. 5.5)
CudaHome = $(HOME)/opt/cuda-$1

# parameter 1 : CUDA version number (e.g. 5.5)
define MappingRule
MAPPINGS += mapping-$1.h

all : mapping-$1.h
mapping-$1.h : omrcudaerrors.cpp
	$(CXX) -o $(MAPTOOL) -I../../include_core -I$(call CudaHome,$1)/include omrcudaerrors.cpp
	./$(MAPTOOL) > $$@
	@rm -f $(MAPTOOL)
endef # MappingRule

all : # the default target

clean :
	rm -f $(MAPTOOL) $(MAPPINGS)

$(foreach version, $(VERSIONS), $(eval $(call MappingRule,$(version))))
