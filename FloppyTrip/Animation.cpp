#include "Animation.h"


Animation::Animation(int frameCount,int FrameWidth,int froX,int froY,int frameTime,const char* filename,SDL_Renderer* rend) :
Texture(filename,rend)
{
	iFrame = 0;
	iFrameCount = frameCount;
	Rect.w = FrameWidth;
	Rect.y = froY;
	iFrameOffsetX = froX;
	iFrameTimeProgress = 0;
	iFrameTime = frameTime;
}

SDL_Rect* Animation::getRect() {
	Rect.x = iFrame * Rect.w + iFrameOffsetX;
	
	iFrameTimeProgress++;
	
	if (iFrameTimeProgress == iFrameTime-1) {
		iFrameTimeProgress = 0;
		iFrame++;
		if (iFrame == iFrameCount) {
			iFrame = 0;
		}
	}
	return &Rect;
}
