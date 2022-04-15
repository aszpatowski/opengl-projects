name = 4zadanie

${name}.exe: ${name}.cpp
	g++ -I. $< -o $@ AGL3Window.cpp objects/sphere/Sphere.cpp objects/tetrahedron/Tetrahedron.cpp objects/cube/Cube.cpp Camera.cpp TetrahedronSphereDetection.cpp -lepoxy -lGL -lglfw
clean:
	rm ${name}.exe
