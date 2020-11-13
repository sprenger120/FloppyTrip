#ifndef HEADER_TEXTURE
#define HEADER_TEXTURE
#include <SDL_rect.h>

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;

class Texture {
	SDL_Surface* p_Surface; 
protected:
	SDL_Rect Rect;
	SDL_Texture* p_Texture;
	SDL_Renderer* p_Renderer;
public:
	Texture(const char*,SDL_Renderer*);
	Texture();
	~Texture();

	SDL_Texture* getTx();
	SDL_Rect* getRect();
};

#endif