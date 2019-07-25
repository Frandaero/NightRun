#include "Board.h"

Board::Board(SDL_Renderer* renderer): Entity(renderer)
{
    SDL_Surface* surface = IMG_Load("gamebackground.png");
    backgroundtexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("bricks.png");
    bricktexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("side.png");
    sidetexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("top.png");
    toptexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("level1.png");
    level1texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("level2.png");
    level2texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("3balls.png");
    balls3texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("2balls.png");
    balls2texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("lastball.png");
    balls1texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("gameover.png");
    gameovertexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    srand(time(0));

    x = 16;
    y = 16;
    width = 768;
    height = 590;

    brickoffsetx = 0;
    brickoffsety = 0;
}

Board::~Board()
{
    //LIMPIEZA DE RECURSOS
    SDL_DestroyTexture(bricktexture);
    SDL_DestroyTexture(sidetexture);
    SDL_DestroyTexture(toptexture);
    SDL_DestroyTexture(backgroundtexture);
    SDL_DestroyTexture(level1texture);
    SDL_DestroyTexture(level2texture);
    SDL_DestroyTexture(balls3texture);
    SDL_DestroyTexture(balls2texture);
    SDL_DestroyTexture(balls1texture);
    SDL_DestroyTexture(gameovertexture);
}

void Board::Update(float delta)
{

}

void Board::Render(float delta)
{
    SDL_Rect dstrect;

    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = 800;
    dstrect.h = 600;
    SDL_RenderCopy(renderer, backgroundtexture, 0, &dstrect);

    //RENDERIZO LADRILLOS
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = bricks[i][j];

            //CHEQUEO SI EL BRICK EXISTE
            if (!brick.state)
                continue;

            SDL_Rect srcrect;

            srcrect.x = (brick.type % 2) * BOARD_BRWIDTH;
            srcrect.y = (brick.type / 2) * BOARD_BRHEIGHT;
            srcrect.w = BOARD_BRWIDTH;
            srcrect.h = BOARD_BRHEIGHT;

            SDL_Rect dstrect;
            dstrect.x = brickoffsetx + x + i * BOARD_BRWIDTH;
            dstrect.y = brickoffsety + y + j * BOARD_BRHEIGHT;
            dstrect.w = BOARD_BRWIDTH;
            dstrect.h = BOARD_BRHEIGHT;

            SDL_RenderCopy(renderer, bricktexture, &srcrect, &dstrect);
        }
    }

    //RENDERIZO COSTADOS

    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = 16;
    dstrect.h = 600;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);

    dstrect.x = 800 - 16;
    dstrect.y = 0;
    dstrect.w = 16;
    dstrect.h = 600;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);

    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = 800;
    dstrect.h = 16;
    SDL_RenderCopy(renderer, toptexture, 0, &dstrect);

    //INDICADORES DE LEVEL Y BOLAS DISPONIBLES
    if(levelindicator == 1)
    {
        dstrect.x = 300;
        dstrect.y = 620;
        dstrect.w = 200;
        dstrect.h = 80;
        SDL_RenderCopy(renderer, level1texture, 0, &dstrect);
    }

    if(levelindicator == 2)
    {
        dstrect.x = 300;
        dstrect.y = 620;
        dstrect.w = 200;
        dstrect.h = 80;
        SDL_RenderCopy(renderer, level2texture, 0, &dstrect);
    }

    if(lifeindicator == 3)
    {
        dstrect.x = 0;
        dstrect.y = 620;
        dstrect.w = 200;
        dstrect.h = 80;
        SDL_RenderCopy(renderer, balls3texture, 0, &dstrect);
    }

    if(lifeindicator == 2)
    {
        dstrect.x = 0;
        dstrect.y = 620;
        dstrect.w = 200;
        dstrect.h = 80;
        SDL_RenderCopy(renderer, balls2texture, 0, &dstrect);
    }

    if(lifeindicator == 1)
    {
        dstrect.x = 0;
        dstrect.y = 620;
        dstrect.w = 200;
        dstrect.h = 80;
        SDL_RenderCopy(renderer, balls1texture, 0, &dstrect);
    }

    if(lifeindicator == 0)
    {
        dstrect.x = 250;
        dstrect.y = 400;
        dstrect.w = 300;
        dstrect.h = 100;
        SDL_RenderCopy(renderer, gameovertexture, 0, &dstrect);
    }
}

void Board::CreateLevel_1()
{
    levelindicator = 1;
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick;

            brick.type = rand() % 4;    //COLOR RANDOM
            if (i == 0 || i == 2 || i == 4 || i == 7 ||  i == 9 || i == 11)
            {
            brick.state = true;        //BRICK PRESENTE
            }
            else
            {
                brick.state = false;
            }
            bricks[i][j] = brick;
        }
    }
}

void Board::CreateLevel_2()
{
    levelindicator = 2;
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick;

            brick.type = rand() % 4;       //COLOR RANDOM
            if (((i >= 1) && (i <= 10)) && ((j >= 1) && (j <= 10)))
            {
                brick.state = true;        //BRICK PRESENTE
            }
            else
            {
                brick.state = false;
            }
            bricks[i][j] = brick;
        }
    }
}
