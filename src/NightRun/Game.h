#ifndef GAME_H_
#define GAME_H_

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <cstdio>

#include "Board.h"
#include "Paddle.h"
#include "Ball.h"

#define FPS_DELAY 500

class Game
{
    public:
        Game();
        ~Game();

        bool Init();
        void Run();

    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;

        //TIMING (UNSIGNED RECOMENDADO DE LAZYFOO)
        unsigned int lasttick;
        unsigned int fpstick;
        unsigned int fps;
        unsigned int framecount;

        //TEST
        float testx, testy;

        Board *board;
        Paddle *paddle;
        Ball *ball;
        bool paddlestick;

        void Clean();
        void Update(float delta);
        void Render(float delta);

        void Level_1();
        void Level_2();
        int levelindex;
        void ResetPaddle();
        void StickBall();

        void SetPaddleX(float x);
        void CheckBoardCollisions();
        float HitTracker(float hitx);
        void CheckPaddleCollisions();
        void CheckBrickCollisions();
        void CheckBrickCollisions2();
        void BallBrickResponse(int dirindex);
        int GetBrickCount();
        int GetLevelCount();
        void SetLevelCount(int lvlindex);
};

#endif
