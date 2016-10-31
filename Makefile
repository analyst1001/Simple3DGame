all:	
	g++ main.cpp imageloader.cpp vec3f.cpp md2model.cpp -lGL -lglut -lGLU

clean:
	rm a.out
