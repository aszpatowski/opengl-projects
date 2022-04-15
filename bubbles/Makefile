name = 5zadanie

${name}.exe: ${name}.cpp
	g++ -I. $< -o $@ AGL3Window.cpp objects/sphere/Sphere.cpp objects/cube/Cube.cpp Camera.cpp -lepoxy -lGL -lglfw
clean:
	rm ${name}.exe
