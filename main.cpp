#include <iostream>
#include <raylib.h>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int PADDLE_EDGE_SPACE = 25;
const int PADDLE_WIDTH = 30; 
const int PADDLE_HEIGHT = 150;
int playerspeed = 5; 
int playerStart = SCREEN_HEIGHT/2;
int rightScore = 0;
int leftScore = 0;


class Ball{
public:
	float x, y;
	int speed_x, speed_y; 
	int radius;

	void Draw(){
		DrawCircle(x, y, radius, WHITE);
	}
	void Update(){
		if(x + radius >= SCREEN_WIDTH || x - radius<= 0 )
			{
				if (x-radius<=0){
					leftScore++;
				}
				if (x+radius >= SCREEN_WIDTH) rightScore++;
				speed_x *= -1.16;
			}
        if(y + radius>= SCREEN_HEIGHT || y - radius<= 0)
			{
				speed_y *= -1.16;
			}
		x += speed_x;
        y += speed_y;
	}
};


class Paddle{
public:
	float x, y; 
	float right_x = x+PADDLE_WIDTH;
	float bottom_y = y+PADDLE_HEIGHT;
	int score;
	int speed_y;

	void Draw(Color color){
		// upper left corner draw;
		DrawRectangle(x, y, PADDLE_WIDTH, PADDLE_HEIGHT, color);
	}
	void Update(int Up, int Down){
		if (IsKeyDown(Down) && y<= SCREEN_HEIGHT - PADDLE_HEIGHT){
			y += speed_y;
			std::cout << "down" << "\n";
		}
		if (IsKeyDown(Up) && y >=0){
			y -= speed_y;
			std::cout << "up" << "\n";
		}
	}
};
	


int main () {

	// initilizing ball and players
	Paddle leftPlayer;
	Paddle rightPlayer;
	Ball ball;
	leftPlayer.x = PADDLE_EDGE_SPACE; 
	leftPlayer.y = playerStart;
	rightPlayer.x = SCREEN_WIDTH - PADDLE_WIDTH - PADDLE_EDGE_SPACE;
	rightPlayer.y = playerStart;
	leftPlayer.speed_y = 8;
	rightPlayer.speed_y = 8;
	leftPlayer.score = 0;
	ball.x = 100;
	ball.y = 100;
	ball.speed_x = 5;
	ball.speed_y = 5;
	ball.radius = 15;


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong >> Tetris - Sungi");
	SetTargetFPS(120);
    while (WindowShouldClose() == false){
		BeginDrawing();
            ClearBackground(BLACK);

			// mid goal line 
			DrawLine(SCREEN_WIDTH/2,SCREEN_HEIGHT, SCREEN_WIDTH/2, SCREEN_HEIGHT, GRAY);
			DrawFPS(10,10); // remove if necessary; 
			ball.Draw();
			ball.Update();
			leftPlayer.Draw(WHITE);
			leftPlayer.Update(KEY_W, KEY_S);
			rightPlayer.Draw(BLUE);
			rightPlayer.Update(KEY_UP, KEY_DOWN);
			// add collision detection here 
			// check if vertical
		// check if ball is still within X,
			if (leftPlayer.bottom_y > ball.y +ball.radius/2 && leftPlayer.y<ball.y-ball.radius/2){
				ball.speed_x*=-1.1;
			}
			const char* r = to_string(rightScore).c_str();
			const char* l = to_string(leftScore).c_str();

			// Update Score
			DrawText( r, SCREEN_WIDTH/4, SCREEN_HEIGHT - 90 , 80, WHITE);
			DrawText( l, SCREEN_WIDTH*3/4 , SCREEN_HEIGHT - 90 , 80, RED);

        EndDrawing();
    }

}
