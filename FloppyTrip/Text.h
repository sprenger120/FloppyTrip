#ifndef HEADER_TEXT	
#define HEADER_TEXT
#include "Texture.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class Text : private Texture {
	std::vector<SDL_Rect> vCharSet;
public:
	Text(const char*,const char*,SDL_Renderer*);

	//Returns the length of the rendered text
	//@1 Position
	//@2 Font size
	//@3 Text
	//@4 True to render the text
	//@5 Letter offset (will be added to width of every letter)
	//@6 true: move lower case letters closer to upper case ones
	//@7 offset that will be subtracted (related to @5)
	int write(SDL_Rect,string,bool = true,int = 0,bool = false,int = 2);
};
#endif