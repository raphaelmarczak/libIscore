all:
	@(cd Release && $(MAKE))

clean:
	@(cd Release && $(MAKE) $@)
	rm -f Engines.o
	
install:
	cp Release/libIscore.a /usr/local/lib/
	mkdir -p /usr/local/include/IScore
	cp -R Includes/* /usr/local/include/IScore/
	#sh installPlugins.sh