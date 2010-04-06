GCC = g++
PLUGIN_DIR = "/Library/Application Support/Virage/"
PLUGIN_DIR_JAMOMA = "/Library/Application Support/Jamoma"

all: libController

libController: PluginFactories.o Device.o Controller.o main.o Minuit.dylib OSC.dylib

	#$(GCC) PluginFactories.o Device.o Controller.o main.o -o  testController -ldl 
	test -d ./Plugins || mkdir ./Plugins
	mv *.dylib ./Plugins
	ar -cr libController.a PluginFactories.o Device.o Controller.o
	rm -f *~ *.o *.dylib 

testController: PluginFactories.o Device.o Controller.o main.o
	$(GCC) PluginFactories.o Device.o Controller.o main.o -o  testController -ldl 




PluginFactories.o: PluginFactories.cpp PluginFactories.h 
	$(GCC) -fPIC  -c PluginFactories.cpp 

Device.o: Device.cpp Device.h
	$(GCC) -c Device.cpp 

Controller.o: Controller.cpp Controller.h
	$(GCC) -c Controller.cpp 	



main.o: main.cpp
	$(GCC) -c main.cpp 



Minuit.dylib: Minuit.cpp 
	$(GCC) -dynamiclib -undefined dynamic_lookup Minuit.cpp Minuit/*.o NetworkCommunication/osc/*.o NetworkCommunication/ip/*.o Device.o -o Minuit.dylib

OSC.dylib: OSC.cpp
	$(GCC) -dynamiclib -undefined dynamic_lookup OSC.cpp Minuit/*.o NetworkCommunication/osc/*.o NetworkCommunication/ip/*.o Device.o -o OSC.dylib



cleanall:
	rm -f *~ *.o *.dylib ./Plugins/*.dylib testController

clean:
	rm -f *~ *.o *.dylib testController libController.a

install:
	test -d $(PLUGIN_DIR) || mkdir $(PLUGIN_DIR)
	cp ./Plugins/*.dylib $(PLUGIN_DIR)
	rm -f *~ *.o *.dylib

install_jamoma:
	test -d $(PLUGIN_DIR_JAMOMA) || mkdir $(PLUGIN_DIR_JAMOMA)
	cp ./Plugins/*.dylib $(PLUGIN_DIR_JAMOMA)
	rm -f *~ *.o *.dylib

plugins: 
	$(GCC) -dynamiclib -undefined dynamic_lookup Minuit.cpp Minuit/*.o NetworkCommunication/osc/*.o NetworkCommunication/ip/*.o Device.o -o Minuit.dylib
	$(GCC) -dynamiclib -undefined dynamic_lookup OSC.cpp Minuit/*.o NetworkCommunication/osc/*.o NetworkCommunication/ip/*.o Device.o -o OSC.dylib
	mv *.dylib ./Plugins
	rm -f *~ *.o *.dylib

test: testController
