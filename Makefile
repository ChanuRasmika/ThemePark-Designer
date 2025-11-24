CXX = g++
CXXFLAGS = -Ilib/glad/include -Ilib/GLFW/include -Ilib/ASSIMP/include
LDFLAGS = -Llib/GLFW/lib-mingw-w64 -Llib/ASSIMP/lib
LIBS = -lglfw3 -lassimp -lopengl32 -lgdi32

SOURCES = main_modular.cpp AttractionManager.cpp MerryGoRound.cpp lib/glad/src/glad.c
TARGET = merrygoround.exe

$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS) $(LIBS)

clean:
	del $(TARGET)

.PHONY: clean