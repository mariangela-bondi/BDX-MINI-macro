CC = g++
LD = $(CC)
CFLAGS = -c -Wall -Os
#IFLAGS =  -I./ -I$(BDXRECO_ROOT)/src/libraries -I$(BDXRECO_ROOT)/src/external/jana_0.7.7p1/src
ROOTINC := $(shell root-config --cflags)
ROOTLIB := $(shell root-config --glibs)
#LIBS = -L$(BDXRECO_ROOT)/lib -lbdxReco -lbdxRecoExt -lJANA -lProof
#LFLAGS = -Wl


SRC = $(wildcard *.C)

TARGET = CheckCosmic

OBJECTS = $(patsubst %.C, %.o, $(wildcard *.C))

all:   dict $(TARGET)

dict: CheckCosmic_selector.h
	@echo "Generating dictionary $@..."
	@rootcint -v -f CheckCosmic_selector_Dict.C -c -p CheckCosmic_selector.h CheckCosmic_selector_LinkDef.h

$(TARGET): $(OBJECTS)
	$(LD) -shared -fPIC -o libCheckCosmic_selector.so $(ROOTLIB) $^
	$(LD) -o $@ $^ $(ROOTLIB)

%.o: %.C
	$(CC) -fPIC -g $(CFLAGS) $(ROOTINC) $^ -o $@

clean:
	rm $(TARGET) $(OBJECTS)
	rm libCheckCosmic_selector.so
