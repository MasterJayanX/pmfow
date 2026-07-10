default:
	@echo Building for Windows x64
	g++ main.cpp -o pmfow.exe -Wall -lwininet

x86:
	@echo Building for Windows x86 (Warning: this assumes that you have a 32-bit version of g++ installed called g++32.exe)
	g++32 main.cpp -o pmfow.exe -Wall -lwininet

linux-x64:
	@echo Building for Windows x64 from Linux
	x86_64-w64-mingw32-g++ main.cpp -o pmfow.exe -Wall -lwininet

linux-x86:
	@echo Building for Windows x86 from Linux
	i686-w64-mingw32-g++ main.cpp -o pmfow.exe -Wall -lwininet

clean:
	@echo Cleaning up
	del pmfow.exe

clean-linux:
	@echo Cleaning up
	rm -f pmfow.exe