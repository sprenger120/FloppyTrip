#include "Text.h"
#include "SDL_render.h"
#include "tinyxml2.h"
#include <iostream>
#include <string>
using namespace tinyxml2;

Text::Text(const char* path,const char* xml,SDL_Renderer* pRend) : 
	Texture(path,pRend)
{
	Rect.h = 16;
	Rect.w = 16;
	Rect.x = 0;
	Rect.y = 0;

	XMLDocument doc;
	doc.LoadFile(xml);

	XMLElement* root = doc.FirstChildElement("fontMetrics");
	XMLElement* para = NULL;

	if (root == NULL) {
		throw std::exception("invalid xml file");
	}

	for (auto charE = root->FirstChildElement("character");charE!=NULL;charE = charE->NextSiblingElement("character")) {
		if (charE->NoChildren()) {
			throw std::exception("invalid xml file");
		}
		
		SDL_Rect c;

		para = charE->FirstChildElement("x");
		if (para==NULL || para->QueryIntText(&c.x) != XML_SUCCESS) {
			throw std::exception("invalid xml file");
		}

		para = charE->FirstChildElement("y");
		if (para==NULL || para->QueryIntText(&c.y) != XML_SUCCESS) {
			throw std::exception("invalid xml file");
		}

		para = charE->FirstChildElement("width");
		if (para==NULL || para->QueryIntText(&c.w) != XML_SUCCESS) {
			throw std::exception("invalid xml file");
		}

		para = charE->FirstChildElement("height");
		if (para==NULL || para->QueryIntText(&c.h) != XML_SUCCESS) {
			throw std::exception("invalid xml file");
		}
		vCharSet.push_back(c);
	}
}

int Text::write(SDL_Rect CoordRect,string str,bool fRender,int offset,bool fShiftCloser,int shiftOffset) {
	const char * pstring = str.c_str();
	int len = str.length();
	SDL_Rect* renderRect;
	int renderedLength = 0;

	for (int i = 0;i<len;i++) {
		if (pstring[i]>=32 && pstring[i] <= 127 &&  pstring[i]-32 <= vCharSet.size()) {
			renderRect = &(vCharSet[pstring[i]-32]);

			CoordRect.h = renderRect->h;
			CoordRect.w = renderRect->w;

			if (fRender) {
				SDL_RenderCopy( p_Renderer, p_Texture, renderRect,&CoordRect);
			}
			
			CoordRect.x += renderRect->w + offset;
			renderedLength += renderRect->w + offset;
			
			if (fShiftCloser && (i + 1 < len) && pstring[i] >= 65 && pstring[i] <= 90 && pstring[i+1] >= 97 && pstring[i+1] <= 122) {
				CoordRect.x -= shiftOffset;
				renderedLength -= shiftOffset;
			} 
		}
	}
	return renderedLength;
}