CXX = g++
CXXFLAGS = -g $$(sdl-config --cflags --libs) -lSDL_image -lSDL_ttf
SOURCES = \
		  BaseEngine.cpp \
		  DisplayableObject.cpp \
		  FontManager.cpp \
		  JPGImage.cpp \
		  mainfunction.cpp \
		  PacmanEnemy.cpp \
		  PacmanMain.cpp \
		  PacmanObject.cpp \
		  PacmanPlayer.cpp \
		  PacmanTileManager.cpp \
		  TileManager.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=game

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(EXECUTABLE)
	./$(EXECUTABLE)
