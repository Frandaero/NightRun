#include "Game.h"

Game::Game()
{
    window = 0;
    renderer = 0;
}

Game::~Game()
{

}

bool Game::Init()
{
    SDL_Init(SDL_INIT_VIDEO);

    //CREACION VENTANA
    window = SDL_CreateWindow("NightRun", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 700, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == 0)
    {
        std::cout << "Error creating window:" << SDL_GetError() << std::endl;
        return false;
    }

    //CREACION RENDERER
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == 0)
    {
        std::cout << "Error creating renderer:" << SDL_GetError() << std::endl;
        return false;
    }

    //TEST RECURSOS
    SDL_Surface *surface = IMG_Load("test.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    //INICIALIZACION DE TIMING
    lasttick = SDL_GetTicks();
    fpstick = lasttick;
    fps = 0;
    framecount = 0;

    //ELEMENTOS DE TESTEO
    testx = 0;
    testy = 0;

    return true;
}

void Game::Clean()
{
    //LIMPIEZA DE TEXTURA
    SDL_DestroyTexture(texture);

    //LIMPIEZA DE RENDERER Y VENTANA
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Game::Run()
{
    //CREACION DE NUEVAS ENTIDADES (DE TIPO SDL_RENDERER)
    board = new Board(renderer);
    paddle = new Paddle(renderer);
    ball = new Ball(renderer);

    //NUEVO JUEGO
    Level_1();
    board->lifeindicator = 3;

    //LOOP PRINCIPAL, PROHIBIDO HACERSE EL CAPO ACÁ
    while (true)
    {
        //EVENTOS DE INPUT DE USUARIO
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                return;
            }
        }

        //CALCULO DE VALOR DELTA Y FPS, BASTANTE TEDIOSO ENTENDERLO POR COMPLETO (ESTUDIAR MAS DE SECCION TIMING DE LAZYFOO!)
        //DELTA: VALOR PARA ACTUALIZAR ESCENARIO BASADA EN EL TIEMPO ELAPSADO DESDE QUE EL JUEGO FUE POR ULTIMA VEZ ACTUALIZADO.
        unsigned int curtick = SDL_GetTicks();
        float delta = (curtick - lasttick) / 1000.0f;
        if (curtick - fpstick >= FPS_DELAY)
        {
            fps = framecount * (1000.0f / (curtick - fpstick));
            fpstick = curtick;
            framecount = 0;

            //PARA VER FPS EN CONSOLA:
            //std::cout << "FPS: " << fps << std::endl;

            char buf[100];
            snprintf(buf, 100, "NightRun", fps);
            SDL_SetWindowTitle(window, buf);
        }
        else
        {
            framecount++;
        }
        lasttick = curtick;

        //ACTUALIZO Y RENDERIZO EL JUEGO
        Update(delta);
        Render(delta);

        //SI PIERDO LAS 3 BOLAS, GAME OVER
        if(board->lifeindicator == 0)
        {
            SDL_Delay(3000);
            return;
        }
    }

    //SI ME SALGO DEL LOOP PRINCIPAL (GAME OVER)
    delete board;
    delete paddle;
    delete ball;
    Clean();
    SDL_Quit();
}

void Game::Level_1()
{
    SetLevelCount(1);
    board->CreateLevel_1();
    ResetPaddle();
}

void Game::Level_2()
{
    SetLevelCount(2);
    board->CreateLevel_2();
    ResetPaddle();
}

void Game::ResetPaddle()
{
    paddlestick = true;
    StickBall();
}

void Game::StickBall()
{
    //PARA PEGAR LA BOLITA AL PADDLE
    ball->x = paddle->x + paddle->width/2 - ball->width/2;
    ball->y = paddle->y - ball->height;
}

void Game::Update(float delta)
{
    //LOGICA DE JUEGO

    //INPUT DE MOUSE
    int mx, my;
    Uint8 mstate = SDL_GetMouseState(&mx, &my);
    SetPaddleX(mx - paddle->width/2.0f);

    if (mstate&SDL_BUTTON(1))
    {
        if (paddlestick)
        {
            paddlestick = false;
            ball->SetDirection(1, 1);
        }
    }

    if (paddlestick)
    {
        StickBall();
    }

    CheckBoardCollisions();
    CheckPaddleCollisions();
    CheckBrickCollisions2();

    if (GetBrickCount() == 0)
    {
        if(GetLevelCount() == 1)
        {
            Level_2();
        }
    }

    board->Update(delta);
    paddle->Update(delta);

    if (!paddlestick)
    {
        ball->Update(delta);
    }
}

void Game::SetPaddleX(float x)
{
    float newx;
    if (x < board->x)
    {
        //LIMITE SUPERIOR
        newx = board->x;
    }
    else if (x + paddle->width > board->x + board->width)
    {
        //LIMITE INFERIOR
        newx = board->x + board->width - paddle->width;
    }
    else
    {
        newx = x;
    }
    paddle->x = newx;
}

//"THIS IS WHERE THE FUN BEGINS"
//                  - Anakin Skywalker (Revenge of the Sith)
//https://www.youtube.com/watch?v=KAHLwAxS7FI

void Game::CheckBoardCollisions()
{
    //COLISIONES SUPERIOR E INFERIOR
    if (ball->y < board->y)
    {
        //PARED SUPERIOR
        //MANTENER LA BOLITA DENTRO DEL PANEL Y REFLEJAR LA DIRECCION EN EJE Y
        ball->y = board->y;
        ball->diry *= -1;
    }
    else if (ball->y + ball->height > board->y + board->height)
    {
        //PARED INFERIOR
        //PELOTA PERDIDA
        ResetPaddle();
        board->lifeindicator--;
    }

    //COLISIONES IZQUIERDA Y DERECHA
    if (ball->x <= board->x)
    {
        //PARED IZQUIERDA
        //MANTENER LA BOLITA DENTRO DEL PANEL Y REFLEJAR LA DIRECCION EN EJE X
        ball->x = board->x;
        ball->dirx *= -1;
    }
    else if (ball->x + ball->width >= board->x + board->width)
    {
        //PARED DERECHA
        //MANTENER LA BOLITA DENTRO DEL PANEL Y REFLEJAR LA DIRECCION EN EJE X
        ball->x = board->x + board->width - ball->width;
        ball->dirx *= -1;
    }
}

float Game::HitTracker(float hitx)
{
    //ASEGURATE QUE hitx SE ENCUENTRA DENTRO DEL RANGO DE ANCHO DEL PADDLE
    if (hitx < 0)
    {
        hitx = 0;
    }
    else if (hitx > paddle->width)
    {
        hitx = paddle->width;
    }

    //TODO LO QUE GOLPEA A LA IZQUIERDA DEL PADDLE ES REFLEJADO HACIA LA IZQUIERDA
    //TODO LO QUE GOLPEA A LA DERECHA DEL PADDLE ES REFLEJADO HACIA LA DERECHA
    hitx -= paddle->width / 2.0f;

    //ESCALA REFLEJO, RANGO -2.0f a 2.0f
    return 2.0f * (hitx / (paddle->width / 2.0f));
}


void Game::CheckPaddleCollisions()
{
    //ADQUIERO EL CENTRO EJE X DE LA BOLA
    float ballcenterx = ball->x + ball->width / 2.0f;

    //CHEQUEO SI HAY COLISION CON EL PADDLE
    if (ball->Collides(paddle))
    {
        ball->y = paddle->y - ball->height;
        ball->SetDirection(HitTracker(ballcenterx - paddle->x), -1);
    }
}

void Game::CheckBrickCollisions()
{
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = board->bricks[i][j];

            //CHEQUEO SI EL BRICK ESTA PRESENTE
            if (brick.state)
            {
                //COORDENADAS EJE X Y EJE Y DEL BRICK
                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;

                //CHEQUEO COLISION ENTRE BRICK Y BOLA
                //DETERMINO LA COLISION USANDO LA MITAD DE LOS ANCHOS DE LOS RECTANGULOS
                float w = 0.5f * (ball->width + BOARD_BRWIDTH);
                float h = 0.5f * (ball->height + BOARD_BRHEIGHT);
                float dx = (ball->x + 0.5f*ball->width) - (brickx + 0.5f*BOARD_BRWIDTH);
                float dy = (ball->y + 0.5f*ball->height) - (bricky + 0.5f*BOARD_BRHEIGHT);

                //LA FUNCION fabs DEVUELVE EL VALOR ABSOLUTO DEL ARGUMENTO, PENSALO COMO MODULO MATEMATICO (ESTUDIAR MAS DE LAZYFOO!)
                if (fabs(dx) <= w && fabs(dy) <= h)
                {
                    //COLISION DETECTADA
                    board->bricks[i][j].state = false;

                    float wy = w * dy;
                    float hx = h * dx;

                    if (wy > hx)
                    {
                        if (wy > -hx)
                        {
                            //COLISION CON LADO INFERIOR DEL BRICK (FLIP DE DIRECCION Y)
                            BallBrickResponse(3);
                        }
                        else
                        {
                            //COLISION CON LADO IZQUIERDO
                            BallBrickResponse(0);
                        }
                    }
                    else
                    {
                        if (wy > -hx)
                        {
                            //COLISION CON LADO DERECHO
                            BallBrickResponse(2);
                        }
                        else
                        {
                            //COLISION CON LADO SUPERIOR DEL BRICK (FLIP DE DIRECCION Y)
                            BallBrickResponse(1);
                        }
                    }
                    return;
                }
            }
        }
    }
}

void Game::CheckBrickCollisions2()
{
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = board->bricks[i][j];

            //CHEQUEO SI EL BRICK ESTA PRESENTE
            if (brick.state)
            {
                //COORDINADAS X E Y DE BRICK
                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;

                //COORDENADAS X E Y DEL CENTRO DE LA BOLA
                float ballcenterx = ball->x + 0.5f*ball->width;
                float ballcentery = ball->y + 0.5f*ball->height;

                //COORDENADAS X E Y DEL CENTRO DEL BRICK
                float brickcenterx = brickx + 0.5f*BOARD_BRWIDTH;
                float brickcentery = bricky + 0.5f*BOARD_BRHEIGHT;

                if (ball->x <= brickx + BOARD_BRWIDTH && ball->x+ball->width >= brickx && ball->y <= bricky + BOARD_BRHEIGHT && ball->y + ball->height >= bricky)
                {
                    //COLISION DETECTADA, REMUEVO EL BRICK
                    board->bricks[i][j].state = false;

                    //ASUMIR QUE LA PELOTA VA SUFICIENTEMENTE LENTO COMO PARA NO SALTEARSE BRICKS, NO?

                    //CALCULO TAMAÑO Y
                    float ymin = 0;
                    if (bricky > ball->y)
                    {
                        ymin = bricky;
                    }
                    else
                    {
                        ymin = ball->y;
                    }

                    float ymax = 0;
                    if (bricky+BOARD_BRHEIGHT < ball->y+ball->height)
                    {
                        ymax = bricky+BOARD_BRHEIGHT;
                    }
                    else
                    {
                        ymax = ball->y+ball->height;
                    }

                    float ysize = ymax - ymin;

                    //CALCULO TAMAÑO X
                    float xmin = 0;
                    if (brickx > ball->x)
                    {
                        xmin = brickx;
                    }
                    else
                    {
                        xmin = ball->x;
                    }

                    float xmax = 0;
                    if (brickx+BOARD_BRWIDTH < ball->x+ball->width)
                    {
                        xmax = brickx+BOARD_BRWIDTH;
                    }
                    else
                    {
                        xmax = ball->x+ball->width;
                    }

                    float xsize = xmax - xmin;

                    //ORIGEN EN LA ESQUINA SUPERIOR IZQUIERDA DE LA PANTALLA
                    //RESPUESTA DE COLISION:
                    if (xsize > ysize)
                    {
                        if (ballcentery > brickcentery)
                        {
                            //INFERIOR
                            ball->y += ysize + 0.01f; //ME MUEVO FUERA DE COLISION
                            BallBrickResponse(3);
                        }
                        else
                        {
                            //SUPERIOR
                            ball->y -= ysize + 0.01f; //ME MUEVO FUERA DE COLISION
                            BallBrickResponse(1);
                        }
                    }
                    else
                    {
                        if (ballcenterx < brickcenterx)
                        {
                            //IZQUIERDA
                            ball->x -= xsize + 0.01f; //ME MUEVO FUERA DE COLISION
                            BallBrickResponse(0);
                        }
                        else
                        {
                            //DERECHA
                            ball->x += xsize + 0.01f; //ME MUEVO FUERA DE COLISION
                            BallBrickResponse(2);
                        }
                    }
                    return;
                }
            }
        }
    }
}

void Game::BallBrickResponse(int dirindex)
{
    //dirindex 0: IZQ, 1: SUP, 2: DER, 3: INF

    //FACTORES DE DIRECCION
    int mulx = 1;
    int muly = 1;

    if (ball->dirx > 0)
    {
        //DIRECCION DE BOLA X POSITIVA
        if (ball->diry > 0)
        {
            //DIRECCION DE BOLA Y POSITIVA
            //+1 +1
            if (dirindex == 0 || dirindex == 3)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
        else if (ball->diry < 0)
        {
            //DIRECCION DE BOLA Y NEGATIVA
            //+1 -1
            if (dirindex == 0 || dirindex == 1)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
    }
    else if (ball->dirx < 0)
    {
        //DIRECCION DE BOLA X NEGATIVA
        if (ball->diry > 0)
        {
            //DIRECCION DE BOLA Y POSITIVA
            //-1 +1
            if (dirindex == 2 || dirindex == 3)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
        else if (ball->diry < 0)
        {
            //DIRECCION DE BOLA Y NEGATIVA
            //-1 -1
            if (dirindex == 1 || dirindex == 2)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
    }
    //SETEO LA NUEVA DIRECCION DE LA BOLA, MULTIPLICANDO LA VIEJA DIRECCION
    //CON LOS FACTORES DE DIRECCION DETERMINADOS PREVIAMENTE
    ball->SetDirection(mulx * ball->dirx, muly * ball->diry);
}

int Game::GetBrickCount()
{
    int brickcount = 0;
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = board->bricks[i][j];
            if (brick.state)
            {
                brickcount++;
            }
        }
    }
    return brickcount;
}

void Game::Render(float delta)
{
    SDL_RenderClear(renderer);

    board->Render(delta);
    paddle->Render(delta);
    ball->Render(delta);

    SDL_RenderPresent(renderer);
}

int Game::GetLevelCount()
{
    return levelindex;
}

void Game::SetLevelCount(int lvlindex)
{
    levelindex = lvlindex;
}
