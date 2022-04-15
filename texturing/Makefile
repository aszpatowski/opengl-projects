name = 7zadanie

${name}.exe: ${name}.cpp
	g++ -I. $< -o $@ AGL3Window.cpp Mesh.cpp Camera.cpp -lepoxy -lGL -lglfw
clean:
	rm ${name}.exe
