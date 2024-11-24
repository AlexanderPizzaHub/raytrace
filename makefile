CC = g++
PROM = raytracer.exe
OBJ = raytracing.o raytracer/ray.o raytracer/tracer.o geometry/mesh.o geometry/objects.o geometry/marchingcube.o
CFLAGS = -Wall -std=c++11

$(PROM): $(OBJ)
	$(CC) $(CFLAGS) -o $(PROM) $(OBJ) 

raytracing.o: raytracing.cpp constants.hpp
	$(CC) $(CFLAGS) -c raytracing.cpp

ray.o: ray/ray.cpp ray/ray.hpp constants.hpp
	$(CC) $(CFLAGS) -c ray/ray.cpp 

tracer.o: raytracer/tracer.cpp raytracer/tracer.hpp constants.hpp 
	$(CC) $(CFLAGS) -c raytracer/tracer.cpp

mesh.o: geometry/mesh.cpp geometry/mesh.hpp constants.hpp
	$(CC) $(CFLAGS) -c geometry/mesh.cpp 

objects.o: geometry/objects.cpp geometry/objects.hpp constants.hpp
	$(CC) $(CFLAGS) -c geometry/objects.cpp

marchingcube.o: geometry/marchingcube.cpp geometry/marchingcube.hpp constants.hpp
	$(CC) $(CFLAGS) -c geometry/marchingcube.cpp

clean:
	rm -f $(PROM) $(OBJ)