CXX := g++
CXXFLAGS:= -O2
CXXINC:= -I ./external/CrHeader/include -I ./include -I ./_private/include
CXXLIB:= 
CXXLNK:=

lib/libOnModel.a:\
	lib/tmp/OnModel_standard.o lib/tmp/OnModel_updating.o lib/tmp/OnModel_datacollection.o lib/tmp/OnModel_debugging.o lib/tmp/OnModel_calculation.o # lib/tmp/OnModel_IO.o
	@mkdir -p lib
	ar rvs lib/libOnModel.a lib/tmp/OnModel_standard.o lib/tmp/OnModel_updating.o lib/tmp/OnModel_datacollection.o lib/tmp/OnModel_debugging.o lib/tmp/OnModel_calculation.o # lib/tmp/OnModel_IO.o

lib/tmp/OnModel_standard.o:\
	src/OnModel_standard.cpp include/OnModel_standard.hpp include/OnModel_types.hpp include/OnModel_error.hpp _private/include/_OnModel_types.hpp
	@mkdir -p lib/tmp
	$(CXX) $(CXXFLAGS) -o lib/tmp/OnModel_standard.o -c src/OnModel_standard.cpp $(CXXINC) $(CXXLIB) $(CXXLNK)

lib/tmp/OnModel_updating.o:\
	src/OnModel_updating.cpp include/OnModel_updating.hpp include/OnModel_types.hpp include/OnModel_error.hpp  _private/include/_OnModel_types.hpp
	@mkdir -p lib/tmp
	$(CXX) $(CXXFLAGS) -o lib/tmp/OnModel_updating.o -c src/OnModel_updating.cpp $(CXXINC) $(CXXLIB) $(CXXLNK)

lib/tmp/OnModel_datacollection.o:\
	src/OnModel_datacollection.cpp include/OnModel_datacollection.hpp include/OnModel_types.hpp include/OnModel_error.hpp  _private/include/_OnModel_types.hpp
	@mkdir -p lib/tmp
	$(CXX) $(CXXFLAGS) -o lib/tmp/OnModel_datacollection.o -c src/OnModel_datacollection.cpp $(CXXINC) $(CXXLIB) $(CXXLNK)

lib/tmp/OnModel_debugging.o:\
	src/OnModel_debugging.cpp include/OnModel_debugging.hpp include/OnModel_types.hpp include/OnModel_error.hpp  _private/include/_OnModel_types.hpp
	@mkdir -p lib/tmp
	$(CXX) $(CXXFLAGS) -o lib/tmp/OnModel_debugging.o -c src/OnModel_debugging.cpp $(CXXINC) $(CXXLIB) $(CXXLNK)

lib/tmp/OnModel_calculation.o:\
	src/OnModel_calculation.cpp include/OnModel_calculation.hpp include/OnModel_error.hpp
	@mkdir -p lib/tmp
	$(CXX) $(CXXFLAGS) -o lib/tmp/OnModel_calculation.o -c src/OnModel_calculation.cpp $(CXXINC) $(CXXLIB) $(CXXLNK)


.PHONY: clean

clean:
	rm -rf lib

	