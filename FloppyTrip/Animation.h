#ifndef HEADER_ANIMATION
#define HEADER_ANIMATION
#include "Texture.h"

class Animation : public Texture {
	int iFrame;
	int iFrameCount; 
	int iFrameOffsetX;
	int iFrameTime;
	int iFrameTimeProgress;
public:
	/*
	@1 Frame Count
	@2 Frame Width
	@3 Frame offsetX
	@4 Frame offsetY
	@5 Frame time
	@6 Filename
	@7: Renderere
	*/
	Animation(int,int,int,int,int,const char*,SDL_Renderer*);
	virtual SDL_Rect* getRect();
};

#endif