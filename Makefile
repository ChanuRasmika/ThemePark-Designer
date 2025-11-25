CXX = g++
CXXFLAGS = -Iinclude -Ilib/glad/include -Ilib/GLFW/include -Ilib/ASSIMP/include
LDFLAGS = -Llib/GLFW/lib-mingw-w64 -Llib/ASSIMP/lib
LIBS = -lglfw3 -lassimp -lopengl32 -lgdi32

SRCDIR = src
BUILDDIR = build
SOURCES = $(SRCDIR)/core/main_modular.cpp $(SRCDIR)/managers/AttractionManager.cpp $(SRCDIR)/managers/RoadManager.cpp $(SRCDIR)/attractions/MerryGoRound.cpp $(SRCDIR)/attractions/FoodStall.cpp $(SRCDIR)/attractions/GameStall.cpp $(SRCDIR)/attractions/Road.cpp $(SRCDIR)/utils/RoadGenerator.cpp lib/glad/src/glad.c
TARGET = $(BUILDDIR)/merrygoround.exe

$(TARGET): $(SOURCES) | $(BUILDDIR)
	$(CXX) $(SOURCES) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS) $(LIBS)

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean:
	if exist $(BUILDDIR) rmdir /s /q $(BUILDDIR)

.PHONY: clean