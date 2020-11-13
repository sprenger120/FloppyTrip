## Floppy Trip 

[Gameplay](https://www.youtube.com/watch?v=gRv6eYzA1zo)

This is a highschool project for comp. sci. class where we could programm whatever as long as your project was accepted by the teacher. So I chose to copy flappy bird which was popular at the time.   
Written in C++ with SDL, SDL_image, tinyxml2 and BASS for linux (cmake)  and windows (visual studio). Sadly this isn't the final version which had a proper title screen, its own "bird" and a day-night cycling background. I can't seem to find it anywhere.


## Assets

- (Press Start Font)[https://www.fontspace.com/press-start-2p-font-f11591]
- Background by flappy birds original author Dong Nguyen (.Gears Studios)
- Ratherbe (originally done by Clean Bandit) Chiptune Version by (RetusMusic)[https://soundcloud.com/retus]
- Coin, Death, Jump sound from various mario games by Nintendo


## Build and run

For windows just install Visual Studio and compile it.  
For linux install 

- libsdl2-dev 
- libsdl2-image-dev 
- libtinyxml2-dev
- cmake
- build-essential

```
cd FloppyTrip
mkdir build
cmake -S . -B build
cd build
make -j 8
cd ..
build/floppytrip
```



