default:
	@echo "Building for Windows x64"
	g++ main.cpp -o pmfow.exe -Wall

x86:
	@echo "Building for Windows x86"
	g++32 main.cpp -o pmfow.exe -Wall

linux-x64:
	@echo "Building for Windows x64 from Linux"
	x86_64-w64-mingw32-g++ main.cpp -o pmfow.exe -Wall

linux-x86:
	@echo "Building for Windows x86 from Linux"
	i686-w64-mingw32-g++ main.cpp -o pmfow.exe -Wall

clean:
	@echo "Cleaning up"
	rm -f pmfow.exe