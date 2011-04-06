CXX=g++
CXXFLAGS=-g $$(sdl-config --cflags --libs) -lSDL_image -lSDL_ttf
SOURCES=BaseEngine.cpp DisplayableObject.cpp FontManager.cpp TileManager.cpp JPGImage.cpp mainfunction.cpp PacmanMain.cpp PacmanTileManager.cpp PacmanObject.cpp PacmanPlayer.cpp PacmanEnemy.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=game

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(EXECUTABLE)
	./$(EXECUTABLE)
