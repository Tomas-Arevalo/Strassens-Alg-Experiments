all: strassen triangles

strassen: strassen.cpp
	g++ -std=c++11 -O3 -o strassen strassen.cpp

triangles: triangles.cpp
	g++ -std=c++11 -O3 -o triangles triangles.cpp	