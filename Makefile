CXXFLAGS = -g -Wall -Ilinux/include -I/usr/class/cs248/include
LIBS = -Llinux/lib64 -Llinux/lib \
	-lsfml-network \
	-lsfml-window \
	-lsfml-graphics \
	-lsfml-system \
	-lassimp \
    -lGLU \
    -lGLEW
OBJS = Main.o Shader.o

main: $(OBJS)
	g++ -g -Wall -o $@ $^ $(LIBS)


run: main
	LD_LIBRARY_PATH=/usr/class/cs248/lib:linux/lib64:linux/lib ./main

clean:
	rm -rf main *.o
