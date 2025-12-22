#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <variant>

// Screen sizing
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE 48
#define FRAME_BUFFER 15

using namespace std;

// Make this stationary board first >> then make it move so you can scan the field 
// 32px pieces 
const int BOARDWIDTH = 8;
const int BOARDHEIGHT = 12;


class Ore{
public:
	Texture2D texture; 
	int x, y; 
void draw(int x = 100, int y = 100){
		DrawTexture(texture, x, y, ORANGE);
	}
};

class Piece{
public:
	Texture2D texture; 
	int x, y; 
	int itemHold; // NOTE: need to bring ore back or not? 
void draw(int x = 100, int y = 100){
		DrawTexture(texture, x, y, WHITE);
	}
};

class TileCursor{
private:
	uint16_t blink = 600; // ms of delay in blink
	bool blinkOn = true;
	double lastBlink = 0; 
public:
	int x, y;
	Piece piece;
	Ore ore; 

void draw(int x= 0, int y =0, Color color = PINK ){
	if (blinkOn) DrawRectangleLines(x, y, TILE-4, TILE-4, PINK);
		// TODO: make the cursor blink for attraction;
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		double nowDouble = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		nowDouble = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		double timeDelta = nowDouble - lastBlink;
		if (timeDelta > blink){
			blinkOn = !blinkOn;
			lastBlink = nowDouble;
		}
}
};

class Board{
public: 

std::array<std::array<std::string, BOARDWIDTH>, BOARDHEIGHT> boardKey = {{}}; 
std::array<std::array<std::variant<Piece, Ore, int>, BOARDWIDTH>, BOARDHEIGHT> boardClass = {}; 
	int pieceCount = 0;
	int oreCount = 0;

void boardSetup(int pieceCount =2){
	
		for (int i= 0; i< BOARDHEIGHT; i++){
			for (int j = 0; j<BOARDWIDTH; j++){
				boardKey[j][i] = "0";
			}
		}
	}

void printBoard(){
		std::cout<< "Board: " << "\n";
		for (int i= 0; i< BOARDHEIGHT; i++){
			for (int j = 0; j< BOARDWIDTH ; j++){
				std::cout << boardKey[j][i];
			}
			std::cout<< "\n";
		}
	}
	
};

class Background{
public:
	Texture2D grass;

void drawPlayable (){
	int topx = FRAME_BUFFER; 
	int topy = FRAME_BUFFER; 
	DrawTexture(grass, 400, 500, ORANGE);
//rows
	for (int i = 0; i<= BOARDHEIGHT; i++){
		DrawLine(topx, topy + i*TILE, FRAME_BUFFER + BOARDWIDTH* TILE, topy + i*TILE, WHITE);
	}
	//columnns
	for (int i = 0; i<=BOARDWIDTH; i++){
 		DrawLine(topx + i*TILE, topy, topx + i*TILE, FRAME_BUFFER + BOARDHEIGHT*TILE, WHITE); 
	}
}
};

int main () {

	// initilizing ball and players
	Board board; 
	TileCursor myCursor;
	Piece firstPawn;
	Piece firstKing;
	Piece firstRook;
	Piece firstQueen;
	Piece firstKnight;
	Piece firstBishop;
	Ore firstCopper;
	Background bg;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
	SetTargetFPS(60);

    while (WindowShouldClose() == false){
		BeginDrawing();
		ClearBackground(BLACK);
		DrawFPS(20,20); // remove if necessary; 
		
		// Background 
		bg.grass = LoadTexture("../assets/grass.png"); 
		bg.drawPlayable();
		myCursor.draw(100,100);

		// Playable Chars
		firstPawn.texture = LoadTexture("../assets/white-pawn48.png");
		firstPawn.draw(FRAME_BUFFER + TILE,FRAME_BUFFER + TILE);
		firstKing.texture = LoadTexture("../assets/white-king64.png");
		firstKing.draw(FRAME_BUFFER+ 2*TILE,FRAME_BUFFER + 3*TILE);
		firstKnight.texture = LoadTexture("../assets/white-knight48.png");
		firstKnight.draw(FRAME_BUFFER + 4*TILE, FRAME_BUFFER + 4*TILE);
		firstCopper.texture = LoadTexture("../assets/Orange3Rock.PNG");
		firstCopper.draw(FRAME_BUFFER + 8*TILE, FRAME_BUFFER + 8*TILE);
		board.boardSetup();
		board.printBoard();



		// Will implement these later
//		firstBishop.texture = LoadTexture("../assets/white-bishop48.png");
//		firstBishop.draw(250,250);
//		firstQueen.texture = LoadTexture("../assets/white-queen64.png");
//		firstQueen.draw(300,300);
//		firstRook.texture = LoadTexture("../assets/white-rook48.png");
//		firstRook.draw(350,350);
        EndDrawing();

		UnloadTexture(firstPawn.texture);
		UnloadTexture(firstKing.texture);
		UnloadTexture(firstKnight.texture);
		UnloadTexture(bg.grass);
    }
}
