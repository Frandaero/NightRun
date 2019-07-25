#ifndef BALL_H_
#define BALL_H_

#include "Entity.h"
#include <cmath>

//LA VELOCIDAD DE LA PELOTA EN PIXELES POR SEGUNDO, AUMENTAR PARA MAS DIFICULTAD
const float BALL_SPEED = 1000;

class Ball: public Entity
{
    public:
        Ball(SDL_Renderer *renderer);
        ~Ball();

        void Update(float delta);
        void Render(float delta);

        void SetDirection(float dirx, float diry);

        float dirx, diry;

    private:
        SDL_Texture *texture;
};

#endif
