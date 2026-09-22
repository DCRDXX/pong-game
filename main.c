#include <stdio.h>
#include <time.h>
#include <SDL3/SDL.h>

static int player1_move_up = 0;
static int player1_move_down = 0;
static int player2_move_up = 0;
static int player2_move_down = 0;

typedef struct Speed
{
    int x;
    int y;
} Speed;

void move_rect(SDL_Surface *surface, SDL_Rect *rect, Speed *speed)
{
    SDL_FillSurfaceRect(surface, rect, 0x00000000);
    if (rect->y + speed->y >= 0 && rect->y + speed->y + rect->h <= surface->h)
    {
	rect->y += speed->y;
    }
    if (rect->x + speed->x >= 0 && rect->x + speed->x + rect->w <= surface->w)
    {
	rect->x += speed->x;
    }

    SDL_FillSurfaceRect(surface, rect, 0xffffffff);
}

void move_player(SDL_Surface *surface, SDL_Rect *rect, Speed *speed)
{
    if (rect->y + speed->y < 20 || rect->y + speed->y + rect->h > surface->h-20)
    {
	return;
    }
    move_rect(surface, rect, speed);
}

void move_ball(SDL_Surface *surface, SDL_Rect *player1, SDL_Rect *player2, SDL_Rect *ball, Speed *speed)
{
    if (ball->x < player1->x + player1->w &&
    ball->x + ball->w > player1->x &&
    ball->y < player1->y + player1->h &&
    ball->y + ball->h > player1->y)
    {
	if (speed->y >= 0)
	{
	    if (ball->y + ball->h - player1->y <= 10)
	    {
		SDL_FillSurfaceRect(surface, ball, 0x00000000);	
		ball->y = player1->y - ball->h ;
		speed->y = -speed->y;
	    }
	}
	else if (speed->y < 0)
	{
	    if (player1->y + player1->h - ball->y <= 10)
	    {
		SDL_FillSurfaceRect(surface, ball, 0x00000000);	
		ball->y = player1->y + player1->h;
		speed->y = -speed->y;
	    }
	}
	speed->x = -speed->x;
    }

    if (ball->x < player2->x + player2->w &&
    ball->x + ball->w > player2->x &&
    ball->y < player2->y + player2->h &&
    ball->y + ball->h > player2->y)
    {
	if (speed->y >= 0)
	{
	    if (ball->y + ball->h - player2->y <= 10)
	    {
		SDL_FillSurfaceRect(surface, ball, 0x00000000);	
		ball->y = player2->y - ball->h;
		speed->y = -speed->y;
	    }
	}
	else if (speed->y < 0)
	{
	    if (player2->y + player2->h - ball->y <= 10)
	    {
		SDL_FillSurfaceRect(surface, ball, 0x00000000);	
		ball->y = player2->y + player2->h;
		speed->y = -speed->y;
	    }
	}
	
	speed->x = -speed->x;
    }
    
	
    if (ball->y <= 26)
    {
	speed->y = -speed->y;
    }

    if (ball->y + ball->h >= 613)
    {
	speed->y = -speed->y;
    }
    
    move_rect(surface, ball, speed);
}


int main()
{
    srand(time(NULL));
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Pong Game", 860, 640, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Rect player1 = (SDL_Rect){40, 300, 20, 100};
    SDL_FillSurfaceRect(surface, &player1, 0xffffffff);

    int score1 = 0;
    int score2 = 0;
    SDL_Rect top = (SDL_Rect){0, 0, 860, 20};
    SDL_FillSurfaceRect(surface, &top, 0xffffffff);

    SDL_Rect bottom = (SDL_Rect){0, 620, 860, 20};
    SDL_FillSurfaceRect(surface, &bottom, 0xffffffff);
    
    SDL_Rect player2 = {780, 300, 20, 100};
    SDL_FillSurfaceRect(surface, &player2, 0xffffffff);

    SDL_Rect ball = {420, 310, 20, 20};
    SDL_FillSurfaceRect(surface, &ball, 0xffffffff);

    SDL_Rect border = {429, 0, 1, 640};
    SDL_FillSurfaceRect(surface, &border, 0xffffffff);

    int dir1 = rand() % 2 == 0 ? 1 : -1;
    int dir2 = rand() % 2 == 0 ? 1 : -1;
    Speed ball_speed = {dir1 * (rand() % 3 + 2), dir2 * (rand() % 3 + 4)};
    
    SDL_UpdateWindowSurface(window);
    int running = 1;
    SDL_Event event;
    while(running)
    {
	SDL_PollEvent(&event);
	if (event.type == SDL_EVENT_QUIT)
	{
	    running = 0;
	}
	Speed speed1 = {0, (player1_move_up + player1_move_down)*3};
	Speed speed2 = {0, (player2_move_up + player2_move_down)*3};
	move_player(surface, &player1, &speed1);
	move_player(surface, &player2, &speed2);
	
	if (event.type == SDL_EVENT_KEY_DOWN)
	{
	    if (event.key.key == SDLK_UP)
	    {
		player2_move_up = -1;
	    }
	    if (event.key.key == SDLK_DOWN)
	    {
		player2_move_down = 1;
	    }
	    if (event.key.key == SDLK_W)
	    {
		player1_move_up = -1;
	    }
	    if (event.key.key == SDLK_S)
	    {
		player1_move_down = 1;
	    }
	}
        if (event.type == SDL_EVENT_KEY_UP) 
        {
	    if (event.key.key == SDLK_UP)
	    {
		player2_move_up = 0;
	    }
	    if (event.key.key == SDLK_DOWN)
	    {
		player2_move_down = 0;
	    }
	    if (event.key.key == SDLK_W)
	    {
		player1_move_up = 0;
	    }
	    if (event.key.key == SDLK_S)
	    {
		player1_move_down = 0;
	    }
        }
	move_ball(surface, &player1, &player2, &ball, &ball_speed);
	if (ball.x == 0)
	{
	    score2++;
	    SDL_FillSurfaceRect(surface, &ball, 0x00000000);	
	    ball.x = 420;
	    ball.y = 310;
	    dir1 = rand() % 2 == 0 ? 1 : -1;
	    dir2 = rand() % 2 == 0 ? 1 : -1;
	    ball_speed.x = dir1 * (rand() % 3 + 2);
	    ball_speed.y = dir2 * (rand() % 3 + 4);
	    printf("player1 score:%d, player2 score:%d\n", score1, score2);
	}
	if (ball.x == 840)
	{
	    score1++;
	    SDL_FillSurfaceRect(surface, &ball, 0x00000000);
	    ball.x = 420;
	    ball.y = 310;
	    dir1 = rand() % 2 == 0 ? 1 : -1;
	    dir2 = rand() % 2 == 0 ? 1 : -1;
	    ball_speed.x = dir1 * (rand() % 3 + 2);
	    ball_speed.y = dir2 * (rand() % 3 + 4);
	    printf("player1 score:%d, player2 score:%d\n", score1, score2);
	}
	SDL_FillSurfaceRect(surface, &top, 0xffffffff);
	SDL_FillSurfaceRect(surface, &ball, 0xffffffff);
	SDL_FillSurfaceRect(surface, &border, 0xffffffff);	
	SDL_UpdateWindowSurface(window);
	SDL_Delay(10);
    }
    return 0;
}

//test
