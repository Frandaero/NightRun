//Autor: Francisco Manuel Diaz
//Proyecto: NightRun
//Comentarios: NightRun es un Breakout-Clone con temática retro.
//             Es mi proyecto final para Laboratorio de Computación II.

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <ctime>

using namespace std;

#include "Game.h"
#include "Ball.h"
#include "Board.h"
#include "Entity.h"
#include "Paddle.h"

int main(int argc, char *argv[])
{
    Game *game = new Game();

    //INICIALIZACION DEL GAME LOOP
    if (game->Init())
    {
        game->Run();
    }

    //LIMPIEZA EN EL PASILLO 3
    delete game;
    return 0;
}
