IEX: main.cpp sha_256.cpp
	g++ -o build/IEX main.cpp sha_256.cpp

clean: 
	rm -rf build/*
