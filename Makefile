CXXFLAGS = -m32 -Ilinux/include -I/usr/class/cs248/include
LIBS = -Llinux/lib \
	-lsfml-audio \
	-lsfml-network \
	-lsfml-window \
	-lsfml-graphics \
	-lsfml-system \
	-lassimp \
    -lGLU
OBJS = Main.o Shader.o

main: $(OBJS)
	g++ -m32 -o $@ $^ $(LIBS)


run: main
	LD_LIBRARY_PATH=linux/lib ./main

clean:
	rm -rf main *.o
