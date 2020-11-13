#include "Game.h"
#include "SDL.h"
#include "Pipe.h"
#include <iostream>
#include <math.h>
#include "bass.h"
#include <SDL_image.h>
#include "Texture.h"
#include "Animation.h"
#include "Text.h"
#include <sstream>
using std::cout;

Game::Game() {
	f_initSuccessful = false;
	veloGravity.set(0,650);
	WindowHeight = 500;
	WindowWidth = 500;
	startPosX = 230;
	startPosY = 250;
	pipeWidth = 50;
	pipeSpace = 120;
	playerWidth = 20;
	playerHeight = 20;
	pipeAcc = pipeWidth/20;
	PipeCount = int(WindowWidth/(pipeWidth+pipeSpace))+2;
	pPipe = new Pipe[PipeCount];
	srand(1337);
}

Game::~Game() {
	if (f_initSuccessful) {
		SDL_DestroyRenderer(p_sdlRenderer);
		SDL_DestroyWindow(p_sdlWindow);

	}
	SDL_Quit();
	BASS_Free();
	delete [] pPipe;
}


void Game::run() {
	f_initSuccessful = initRender();
	if (!f_initSuccessful) {
		throw std::runtime_error("initRender failed");
	}

	f_initSuccessful = initAudio();
	if (!f_initSuccessful) {
        throw std::runtime_error("initRender failed");
	}

	Texture texP("./Resources/Textures/pipe2.png",p_sdlRenderer);
	Texture texPH("./Resources/Textures/pipe_head.png",p_sdlRenderer);
	Texture texPV("./Resources/Textures/pipe_valve4.png",p_sdlRenderer);
	Texture texBack("./Resources/Textures/background.png",p_sdlRenderer);
	Texture texMenue("./Resources/Textures/startScreen.png",p_sdlRenderer);

	Animation animPlr(8,21,0,0,5,"./Resources/Textures/mario_sprite.png",p_sdlRenderer);

	Text txSmall("./Resources/Font/font_small.png","./Resources/Font/font_small.xml",p_sdlRenderer);
	Text txPoints("./Resources/Font/font2.png","./Resources/Font/font2.xml",p_sdlRenderer);
	Text txPixel("./Resources/Font/pixelFont.png","./Resources/Font/pixelFont.xml",p_sdlRenderer);

	texPipe = &texP;
	texPipeHead = &texPH;
	texPipeValve = &texPV;
	texBackground = &texBack;
	texMenueScreen = &texMenue; 
	animPlayer = &animPlr;
	fontPoints = &txPoints;
	fontSmall = &txSmall;
	fontPixelated = &txPixel;


	restartGame();

	//S Taste f�r Sound aus
	//Game Loop
	//int FadingTickLength = 50;
	//SDL_Texture* tex2 = SDL_CreateTexture(p_sdlRenderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_STREAMING,WindowHeight,WindowWidth);

	//int* pixels = new int[WindowWidth*WindowHeight];
	//int pxcap = WindowWidth*WindowHeight-1;
	//
	//
	//	_asm {
	//		mov edx,4
	//		mov eax,0
	//		mov ebx,pixels
	//loops:
	//		mov DWORD PTR[ebx+eax*4],0x000000aa
	//		add eax,1
	//	
	//		cmp eax,pxcap
	//		je ende
	//		jne loops
	//
	//	ende:
	//
	//	};
	//memset(pixels,0xFF,WindowWidth*WindowHeight*4);

	//Animation anim2(2,70,0,0,50,"./Resources/Textures/spacebar.png",p_sdlRenderer);

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	int lastFPSUpdate = SDL_GetTicks();
	SDL_Event e;

	bool fPressing = false;
	bool fInPipe = false;
	double tickLength = 1000.0/60.0;
	int tick = 1;
	int startTick = 0;
	int lastFPSUpdateTick = 0;
	while (1) {
		startTick = SDL_GetTicks();

		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT){
			break;
		}
		if (keys[SDL_SCANCODE_SPACE]){
			fPressing = true;
		}else{
			//Jump
			if (fPressing) {
				fPressing = false;

				if (!fLost) {
					if (!fGameStarted) {
						QWORD Pos = BASS_ChannelGetPosition(audioSongRadio,BASS_POS_BYTE);

						BASS_ChannelPause(audioSongRadio);
						BASS_ChannelSetPosition(audioSongRadio,0L,BASS_POS_BYTE);

						BASS_ChannelPlay(audioSong,true);
						BASS_ChannelSetPosition(audioSong,Pos,BASS_POS_BYTE);
					}


					veloPlayer.set(0,-250);
					fGameStarted = true; 
					BASS_ChannelPlay(audioJump,false);
				}else{
					restartGame();
				}
			}
		}
		
		
		renderBackground();
		if (!fGameStarted) {
			renderMenueScreen();
		}else{
			renderGame();
		}
		if (fLost) {
			renderLooseScreen();
		}
		SDL_Rect rect = {0,0,70,13};
		//SDL_RenderCopy(p_sdlRenderer,anim2.getTx(),anim2.getRect(),&rect);
		
		SDL_RenderPresent(p_sdlRenderer);

		
		//Loose Sound
		if (fLost && fGameStarted && ! fDeathPlayed) {
			BASS_ChannelPlay(audioDeath,false);
			fDeathPlayed = true;
		}

		//Regulate fps
		double ackTickLen = SDL_GetTicks() - startTick;
		if (ackTickLen < tickLength) {
			SDL_Delay(int(tickLength-ackTickLen));
		}

		if (SDL_GetTicks() - lastFPSUpdate >= 1000) {
			FPS = (tick - lastFPSUpdateTick);
			lastFPSUpdateTick = tick;
			lastFPSUpdate = SDL_GetTicks();
		}


		//Game logic & physics
		double deltaTime = (double) (SDL_GetTicks() - startTick) / 1000;
		if (fGameStarted) {
			posPlayer = posPlayer + veloPlayer * deltaTime;
			veloPlayer = veloPlayer + veloGravity * deltaTime ;

			if (posPlayer.getY() < 0 || posPlayer.getY() > WindowHeight) {
				fLost = true;
				continue;
			}


			//New Pipes
			if (tick % 75  == 0 && !fLost) {
				for (int i=0;i<PipeCount;i++) {
					if (pPipe[i].Free) {
						pPipe[i].X = WindowWidth;
						pPipe[i].Y = 100 + (rand()%5)*50;	
						pPipe[i].Free = false;
						break;
					}
				}
			}

			//Clear Pipes + Collision
			fInPipe = false;
			for (int i=0;i<PipeCount;i++) {
				if (!fLost) {
					if (!pPipe[i].Free) {
						pPipe[i].X -= pipeAcc;
					}
					if (pPipe[i].X+pipeWidth < 0) {
						pPipe[i].Free = true;
					}

					if (posPlayer.getX() >= pPipe[i].X && posPlayer.getX() <= pPipe[i].X+pipeWidth && !pPipe[i].Free) {
						fInPipe = true;
					}

				}
				if (isColliding(pPipe[i]) && !pPipe[i].Free) {
					fLost = true;
					continue;
				}
			}
		}

		if (fInPipe && !fEnteredPipe) {
			fEnteredPipe = true;
			Points++;
			BASS_ChannelSetPosition(audioCoin,0L,BASS_POS_BYTE);
			BASS_ChannelPlay(audioCoin,false);
		}
		if (!fInPipe && fEnteredPipe) {
			fEnteredPipe = false;
		}



		//Music Loops
		if (!fGameStarted) {
			if (BASS_ChannelGetLength(audioSongRadio,BASS_POS_BYTE) == BASS_ChannelGetPosition(audioSongRadio,BASS_POS_BYTE)){
				BASS_ChannelSetPosition(audioSongRadio,0L,BASS_POS_BYTE);
				BASS_ChannelPlay(audioSongRadio,false);
			}
		}else{
			if (BASS_ChannelGetLength(audioSong,BASS_POS_BYTE) == BASS_ChannelGetPosition(audioSong,BASS_POS_BYTE)){
				BASS_ChannelSetPosition(audioSong,0L,BASS_POS_BYTE);
				BASS_ChannelPlay(audioSong,false);
			}
		}

		tick++;
	}
}


bool Game::initRender(){
	cout<<"Init SDL...";
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cout<<"Error ("<<SDL_GetError()<<")\n";
		return false;
	}
	cout<<"Done\n";


	cout<<"Init SDL_img...";
	int init = IMG_Init(IMG_INIT_PNG);
	if (! (IMG_Init(IMG_INIT_PNG) &IMG_INIT_PNG)) {
		cout<<"Error\n";
		return false;
	}
	cout<<"Done\n"; 


	cout<<"Create Window...";
	p_sdlWindow = SDL_CreateWindow("Flappy Trip Alpha",100,100,WindowHeight,WindowWidth,SDL_WINDOW_SHOWN);
	if (p_sdlWindow == nullptr) {
		cout<<"Error ("<<SDL_GetError()<<")\n";
		return false;
	}
	cout<<"Done\n";

	cout<<"Create Renderer...";
	p_sdlRenderer = SDL_CreateRenderer(p_sdlWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (p_sdlRenderer == nullptr) {
		cout<<"Error ("<<SDL_GetError()<<")\n";
		return false;
	}
	cout<<"Done\n"; 


	return true;
}

bool Game::isColliding(Pipe& rPipe) {
	double heighgLowerPipe = double(WindowHeight) - rPipe.Y - pipeSpace; 
	if (isRectPointCollision(rPipe.X,0,pipeWidth,rPipe.Y, posPlayer.getX(),posPlayer.getY()) || 
		isRectPointCollision(rPipe.X,0,pipeWidth,rPipe.Y, posPlayer.getX()+playerWidth,posPlayer.getY()) || 
		isRectPointCollision(rPipe.X,0,pipeWidth,rPipe.Y, posPlayer.getX(),posPlayer.getY()+playerHeight) || 
		isRectPointCollision(rPipe.X,0,pipeWidth,rPipe.Y, posPlayer.getX()+playerWidth,posPlayer.getY()+playerHeight) ||

		isRectPointCollision(rPipe.X,rPipe.Y+pipeSpace,pipeWidth,heighgLowerPipe, posPlayer.getX(),posPlayer.getY()) || 
		isRectPointCollision(rPipe.X,rPipe.Y+pipeSpace,pipeWidth,heighgLowerPipe, posPlayer.getX()+playerWidth,posPlayer.getY()) || 
		isRectPointCollision(rPipe.X,rPipe.Y+pipeSpace,pipeWidth,heighgLowerPipe, posPlayer.getX(),posPlayer.getY()+playerHeight) || 
		isRectPointCollision(rPipe.X,rPipe.Y+pipeSpace,pipeWidth,heighgLowerPipe, posPlayer.getX()+playerWidth,posPlayer.getY()+playerHeight)
		)
	{
		return true;
	}
	return false;
}

void Game::restartGame() {
	BASS_ChannelPause(audioSongRadio);
	BASS_ChannelSetPosition(audioSongRadio,0L,BASS_POS_BYTE);
	BASS_ChannelPause(audioSong);
	BASS_ChannelSetPosition(audioSong,0L,BASS_POS_BYTE);


	veloPlayer.set(0,0);
	posPlayer.set(startPosX,startPosY);

	for (int i=0;i<PipeCount;i++) {
		pPipe[i].Free = true;
	}
	fLost = false;
	fGameStarted = false;
	FadingTargetTick = 0;
	Points = 0;
	fEnteredPipe = false;
	fDeathPlayed = false;

	BASS_ChannelPlay(audioSongRadio,false);
}

bool Game::initAudio() {
	cout<<"Bass version...";
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		cout<<"Error (Incorrect BASS version)\n";
		return false;
	}
	cout<<"Done\n";


	cout<<"Bass version...";
	if (!BASS_Init(-1,44100,0,NULL,NULL)) {
		cout<<"Error (Unable to initialize device)\n";
		return false;
	}
	cout<<"Done\n";

	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM,2000);	

	cout<<"Sound: coin...";
	if ((audioCoin = BASS_StreamCreateFile(FALSE, "./Resources/Sound/CoinSound.wav", 0, 0, 0)) == NULL) {
		cout<<"Error\n";
		return false;
	}
	cout<<"Done\n";

	cout<<"Sound: death...";
	if ((audioDeath = BASS_StreamCreateFile(FALSE, "./Resources/Sound/DeathSound.wav", 0, 0, 0)) == NULL) {
		cout<<"Error\n";
		return false;
	}
	cout<<"Done\n";

	cout<<"Sound: jump...";
	if ((audioJump = BASS_StreamCreateFile(FALSE, "./Resources/Sound/JumpSound.wav", 0, 0, 0)) == NULL) {
		cout<<"Error\n";
		return false;
	}
	cout<<"Done\n";

	cout<<"Sound: song...";
	if ((audioSong = BASS_StreamCreateFile(FALSE, "./Resources/Sound/RatherBeChiptune.wav", 0, 0, 0)) == NULL) {
		cout<<"Error\n";
		return false;
	}
	cout<<"Done\n";


	cout<<"Sound: song_radio...";
	if ((audioSongRadio = BASS_StreamCreateFile(FALSE, "./Resources/Sound/RatherBeChiptune_radio.wav", 0, 0, 0)) == NULL) {
		cout<<"Error\n";
		return false;
	}
	cout<<"Done\n";


	return true;
}

bool Game::isRectPointCollision(double rX,double rY,double rW,double rH,double pX,double pY) {
	if (pX >= rX && pX <= rX+rW && pY >= rY && pY <= rY + rH) {
		return true;
	}

	return false;
}

void Game::renderGame() {
	//Render 
	SDL_Rect playerRect = {0,0,(int)playerWidth,(int)playerHeight};
	SDL_Rect pipeRect = {0,0,0,0};
	SDL_Rect FPSRect = {5,5,0,0};
	SDL_Rect PointsRect = {0,50,0,0};

	
	//SDL_UpdateTexture(tex2, NULL, pixels, WindowWidth * sizeof (Uint32));
	//SDL_RenderCopy( p_sdlRenderer,tex2, NULL,NULL );

	for (int i=0;i<PipeCount;i++) {
		if (!pPipe[i].Free) {
			pipeRect.x = (int)pPipe[i].X;
			pipeRect.y = 0;
			pipeRect.h = (int)pPipe[i].Y-100;
			pipeRect.w = (int)pipeWidth;

			//Pipe
			SDL_RenderCopyEx( p_sdlRenderer, texPipe->getTx(), texPipe->getRect(), &pipeRect,0,NULL,SDL_FLIP_VERTICAL);

			//PipeHead
			pipeRect.y = pipeRect.h;
			pipeRect.h = 70;
			SDL_RenderCopyEx( p_sdlRenderer, texPipeHead->getTx(), texPipeHead->getRect(), &pipeRect,0,NULL,SDL_FLIP_VERTICAL);

			//Pipe Valve
			pipeRect.y += 70;
			pipeRect.h = 30;
			pipeRect.w = 60;
			pipeRect.x -=5;
			SDL_RenderCopyEx( p_sdlRenderer, texPipeValve->getTx(), texPipeValve->getRect(), &pipeRect,0,NULL,SDL_FLIP_VERTICAL);

			//Lower one 
			//Pipe
			pipeRect.x = (int)pPipe[i].X;
			pipeRect.w = (int)pipeWidth;
			pipeRect.y = (int)(pPipe[i].Y+pipeSpace)+100;
			pipeRect.h = WindowHeight-pipeRect.y;
			if (pipeRect.y < WindowHeight) {
				SDL_RenderCopy( p_sdlRenderer, texPipe->getTx(), texPipe->getRect(), &pipeRect );
			}
			//Pipe Head
			pipeRect.y = (int)(pPipe[i].Y+pipeSpace)+30;
			if (WindowHeight - pipeRect.y > 70) {
				pipeRect.h = 70;
			}else{
				pipeRect.h = WindowHeight - pipeRect.y;
			}
			SDL_RenderCopy( p_sdlRenderer, texPipeHead->getTx(), texPipeHead->getRect(), &pipeRect);

			//Pipe Valve
			pipeRect.y -= 30;
			pipeRect.h = 30;
			pipeRect.w = 60;
			pipeRect.x -=5;
			SDL_RenderCopy( p_sdlRenderer, texPipeValve->getTx(), texPipeValve->getRect(), &pipeRect);
		}
	}

	posPlayer.transferInRect(&playerRect);

	double angle = 0;
	//if (veloPlayer.getY() < 0) {
	//	angle = veloPlayer.getY() / 5;
	//}else{
	//	angle = veloPlayer.getY() / 3.5;
	//}
	SDL_RenderCopyEx(p_sdlRenderer, animPlayer->getTx(), animPlayer->getRect(), &playerRect,angle,NULL, SDL_FLIP_HORIZONTAL);


	//Render points
	strStrm<<Points;
	int len = fontPoints->write(PointsRect,strStrm.str(),false); 
	PointsRect.x = WindowWidth / 2 - (len / 2);
	fontPoints->write(PointsRect,strStrm.str()); 
	strStrm.str(string());
}

void Game::renderLooseScreen() {
	SDL_Rect gameOver = {250,250,0,0};

}


void Game::renderMenueScreen() {
		SDL_Rect rectMenue = {100,150,300,200};
		SDL_RenderCopy(p_sdlRenderer,texMenueScreen->getTx(),texMenueScreen->getRect(),&rectMenue);
		
		SDL_Rect rectText1 = {137,200,300,200};
		fontPixelated->write(rectText1,"Press SPACE",true,-9,true,1);

		SDL_Rect rectText2 = {170,250,300,200};
		fontPixelated->write(rectText2,"to start!",true,-9,true,1);
}

void Game::renderBackground() {
		//SDL_SetRenderDrawColor(p_sdlRenderer,255,255,255,255);
		//SDL_RenderClear(p_sdlRenderer);

		SDL_Rect rectBackground = {0,0,500,500};
		SDL_RenderCopy(p_sdlRenderer,texBackground->getTx(),texBackground->getRect(),&rectBackground);
}