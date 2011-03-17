CXX=g++
CXXFLAGS=-g $$(sdl-config --cflags --libs) -lSDL_image -lSDL_ttf
SOURCES=BaseEngine.cpp BouncingBall.cpp BouncingBallMain.cpp DisplayableObject.cpp FontManager.cpp JPGImage.cpp mainfunction.cpp MyProjectMain.cpp TileManager.cpp MyTileManager.cpp Demo3Main.cpp Demo3Object.cpp Demo3ObjectFinished.cpp Demo3TileManager.cpp


#Demo2aMain.cpp Demo2Main.cpp Demo2Object.cpp Demo2TileManager.cpp Demo3Main.cpp Demo3Object.cpp Demo3ObjectFinished.cpp Demo3TileManager.cpp Demo3TileManager.cpp

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
