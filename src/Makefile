CXX=g++
CXXFLAGS=-Wall -pedantic-errors -g $$(sdl-config --cflags --libs) -lSDL_image -lSDL_ttf
SOURCES=BaseEngine.cpp BouncingBall.cpp BouncingBallMain.cpp DisplayableObject.cpp FontManager.cpp JPGImage.cpp mainfunction.cpp MyProjectMain.cpp TileManager.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=game

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
