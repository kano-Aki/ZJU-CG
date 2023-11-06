CC=g++
CPP_SRC=$(wildcard *.cpp)
OBJECT=$(patsubst %.cpp,%.o,$(CPP_SRC))
INCLUDE=-I C:\D\ZJU\learn\31\cg\lab\include
LIB=-L C:\D\ZJU\learn\31\cg\lab\lib
OPENGL=-lopengl32 -lgl C:\D\ZJU\learn\31\cg\lab\Debug\glfw3.dll

run: C:\D\ZJU\learn\31\cg\lab\Debug\flag.exe
	$<

C:\D\ZJU\learn\31\cg\lab\Debug\flag.exe:$(OBJECT)
	$(CC) $^ -o $@ $(INCLUDE) $(LIB) $(OPENGL)

%.o:%.cpp
	$(CC) -c $< $(INCLUDE) 

clean:
	powershell rm *.o 
	powershell rm C:\D\ZJU\learn\31\cg\lab\Debug\flag.exe