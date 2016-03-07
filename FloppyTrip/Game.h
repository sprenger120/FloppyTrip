#ifndef HEADER_GAME
#define HEADER_GAME	
#include "Vector.h"
#include "bass.h"
#include <sstream>
#include "SDL_rect.h"

struct SDL_Window;
struct SDL_Renderer;
struct Pipe;
class Texture;
class Animation;
class Text;

class Game {
private:
	//Render
	SDL_Window* p_sdlWindow;
	SDL_Renderer* p_sdlRenderer;
	std::stringstream strStrm;
	bool f_initSuccessful;
	int WindowHeight;
	int WindowWidth;
	int FadingTargetTick;
	int FPS;
	
	//Textures
	Texture* texPipe;
	Texture* texPipeHead;
	Texture* texPipeValve;
	Texture* texBackground;
	Texture* texMenueScreen;
	Animation* animPlayer;
	Text* fontPoints;
	Text* fontSmall;
	Text* fontPixelated;

	//Physic
	Vector posPlayer;
	Vector veloPlayer;
	Vector veloGravity;
	double startPosX;
	double startPosY;
	double playerWidth;
	double playerHeight;


	//Game Logic
	int PipeCount;
	Pipe* pPipe;
	
	double pipeWidth;
	double pipeSpace; // Hole between pipes
	double pipeAcc;

	bool fLost;
	bool fGameStarted;

	int Points;
	bool fEnteredPipe;

	//Audio
	HSTREAM audioCoin;
	HSTREAM audioDeath;
	HSTREAM audioJump;
	HSTREAM audioSongRadio;
	HSTREAM audioSong;
	bool fDeathPlayed;
public: 
	Game();
	~Game();

	void run();
private:
	bool initRender();
	bool initAudio();

	bool isColliding(Pipe&);
	bool isRectPointCollision(double,double,double,double,double,double);
	void restartGame();
	void renderGame();
	void renderLooseScreen();
	void renderBackground();
	void renderMenueScreen();
};
#endif