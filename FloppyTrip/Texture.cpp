#include "Texture.h"
#include <SDL_image.h>
#include <string>

Texture::Texture(const char* pFilename,SDL_Renderer* pRend) {
	//250 32 180 color key
	//FA 20 B4
	p_Surface = IMG_Load(pFilename);
	if (p_Surface == NULL) {
		throw std::string("Unable to load file");
	}

	SDL_SetColorKey( p_Surface, SDL_TRUE, SDL_MapRGB( p_Surface->format, 0xFA, 0x20, 0xb4 ) );
	//SDL_SetColorKey( p_Surface, SDL_TRUE, SDL_MapRGB( p_Surface->format, 0xFF, 0xFF, 0xFF ) );

	p_Texture = SDL_CreateTextureFromSurface(pRend,p_Surface);
	if (p_Texture == NULL) {
		throw std::string("Unable to create texture");
	}
	Rect.h = p_Surface->h;
	Rect.w = p_Surface->w;
	Rect.y = 0;
	Rect.x = 0;
	p_Renderer = pRend;
}


Texture::~Texture() {
	SDL_FreeSurface(p_Surface);
	SDL_DestroyTexture(p_Texture);
}


SDL_Texture* Texture::getTx() {
	return p_Texture;
}

SDL_Rect* Texture::getRect() {
	return &Rect;
}
