#ifndef BOARD_H_
#define BOARD_H_

#include "Entity.h"
#include <cstdlib>
#include <ctime>

//DEFINO LAS DIMENSIONES DE PANEL DE JUEGO Y LA MATRIZ BRICKS PARA POSTERIOR USO
#define BOARD_WIDTH 12
#define BOARD_HEIGHT 12
#define BOARD_BRWIDTH 64
#define BOARD_BRHEIGHT 24

class Brick
{
    public:
        int type;
        bool state;
};

class Board: public Entity
{
    public:
        Board(SDL_Renderer *renderer);
        ~Board();

        void Update(float delta);
        void Render(float delta);
        void CreateLevel_1();
        void CreateLevel_2();

        float brickoffsetx, brickoffsety;

        //DEFINICION DE LA MATRIZ BRICKS Y VISUAL INDICATORS
        Brick bricks[BOARD_WIDTH][BOARD_HEIGHT];
        int levelindicator;
        int lifeindicator;

    private:
        SDL_Texture *bricktexture;
        SDL_Texture *sidetexture;
        SDL_Texture *toptexture;
        SDL_Texture *backgroundtexture;
        SDL_Texture *level1texture;
        SDL_Texture *level2texture;
        SDL_Texture *balls3texture;
        SDL_Texture *balls2texture;
        SDL_Texture *balls1texture;
        SDL_Texture *gameovertexture;
};

#endif
