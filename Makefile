CXXFLAGS = -Ilinux/include
LIBS = -Llinux/lib \
	-lsfml-audio \
	-lsfml-network \
	-lsfml-window \
	-lsfml-graphics \
	-lsfml-system \
	-lassimp
OBJS = Main.o Shader.o

main: $(OBJS)
	g++ -o $@ $^ $(LIBS)


run: main
	LD_LIBRARY_PATH=linux/lib ./main

clean:
	rm -rf main *.o
