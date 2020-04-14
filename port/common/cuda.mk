CXX ?= g++
MAPTOOL := omrcudaerrors
MAPPINGS :=
CUDA_HOME_PREFIX := $(HOME)/opt/cuda

# parameter 1 : CUDA version number (e.g. 5.5)
define MappingRule
MAPPINGS += mapping-$1.h

all : mapping-$1.h
mapping-$1.h : omrcudaerrors.cpp
	$(CXX) -o $(MAPTOOL) -I../../include_core -I$(CUDA_HOME_PREFIX)-$1/include omrcudaerrors.cpp
	./$(MAPTOOL) > $$@
	@rm -f $(MAPTOOL)
endef # MappingRule

all : # the default target

clean :
	rm -f $(MAPTOOL) $(MAPPINGS)

VERSIONS := $(patsubst $(CUDA_HOME_PREFIX)-%,%,$(wildcard $(CUDA_HOME_PREFIX)-*))

$(foreach version, $(VERSIONS), $(eval $(call MappingRule,$(version))))
