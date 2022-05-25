CXX := g++
CXXFLAGS:= -O2
CXXINC:= -I ../external/CrHeader/include -I ../include
CXXLIB:= -L ../lib
CXXLNK:= -lOnModel -lstdc++ -lm

sample.out: source.cpp ../lib/libOnModel.a
	$(CC) $(CXXFLAGS) source.cpp -o sample.out $(CXXINC) $(CXXLIB) $(CXXLNK)

.PHONY: clean

clean:
	rm -rf sample.out