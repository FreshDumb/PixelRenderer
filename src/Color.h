#include "SDL.h"

/// Color class with methods to convert from Color to Uint32 for convenience
//	contains the getpixel method
class Color {
public:
	char r, g, b, a;
	Color(int _r, int _g, int _b, int _a);
	static Uint32 ColourToUint(int R, int G, int B);
	static SDL_Colour UintToColour(Uint32 color);

	/// Getting the value of a pixel
	//	takes pixel value based on texture coords multiplied with the Texturemap width and height
	static Uint32 getpixel(SDL_Surface *surface, int x, int y);
};

