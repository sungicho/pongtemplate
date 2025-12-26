#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <variant>
// #include <random> 

// Screen sizing
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE 48
#define FRAME_BUFFER 45

using namespace std;
// Make this stationary board first >> then make it move so you can scan the field 
// 32px pieces 
const uint16_t BOARDWIDTH = 12;
const uint16_t BOARDHEIGHT = 12;

class Error {
public:
	// TODO: need to add error type
	string errorS;
	std::vector<std::string> errorList;
	void printErrors(){
		for (int i = 0; i<errorList.size(); i++){
			std::cout<< errorList[i] << "\n";
		}
	}

	// FIX: prevent this from printing out same error multiple times;
	// TODO: need to check for error duplication 
};

bool keyPressBounce(double debounce, KeyboardKey key, double lastKeyPress){
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	double nowDouble = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	double timeDelta = nowDouble - lastKeyPress;
	if (timeDelta> debounce){
		return true;
	}
	else return false;
}
class Ore{
public:
	Texture2D texture; 
	uint16_t x, y; 
	uint16_t size;

void draw(uint16_t x = 0, uint16_t y = 0){
		DrawTexture(texture, x, y, ORANGE);
	}
};

class Piece{
private: 
	std::string error; 
	int texHeight = TILE;
	int texWidth = TILE;

public:
	int cx =0, cy =0;
	// NOTE: all types except King / Queen are lowercase. 
	std::string key;
	std::string type;
	Texture2D texture; 
	uint16_t x, y; 
	Ore oreHeld;
	// NOTE: key = letter showing piece + 4 digit + "-" player letter (w/b)   "p002-w"

	void init(int ix = 0, int iy = 0, std::string itype = "pawn", std::string ikey = "p999-w"){
		x = ix;
		y = iy;
		type = itype;
		key = ikey;
	}

	void draw(int ix = 400, int iy = 400){
		x = ix;
		y = iy;
		if (type == "pawn"){
			texHeight = 48;
			texWidth = 48;
			texture = LoadTexture("../assets/white-pawn48.png");
		}
		if (type == "King") {
			texture = LoadTexture("../assets/white-king64.png");
			texHeight = 64;
			texWidth= 56;
			}
//		if (type == "knight") texture =LoadTexture("../assets/white-knight48.png");
		cx = x * TILE + FRAME_BUFFER - texWidth + TILE;
		cy = (BOARDHEIGHT - y-1) * TILE + FRAME_BUFFER -texHeight + TILE;

	// TODO: convert to CX XY
		DrawTexture(texture, cx, cy, WHITE);
//		//firstPawn.texture = LoadTexture("../assets/white-pawn48.png");
//		firstPawn.draw(FRAME_BUFFER + TILE,FRAME_BUFFER + TILE);
//		firstKing.texture = LoadTexture("../assets/white-king64.png");
//		firstKing.draw(FRAME_BUFFER+ 2*TILE,FRAME_BUFFER + 3*TILE);
//		firstKnight.texture = LoadTexture("../assets/white-knight48.png");
//		firstKnight.draw(FRAME_BUFFER + 4*TILE, FRAME_BUFFER + 4*TILE);
//		firstCopper.texture = LoadTexture("../assets/Orange3Rock.PNG");
	}
	std::vector<std::array<int, 2>> availableMoves(){
		std::array<int, 2> move = {0,0};
		std::vector<std::array<int,2>> available;
		
		if (type == "King"){
			available.push_back({1,0});
			available.push_back({1,1});
			available.push_back({0,1});
			available.push_back({-1,0});
			available.push_back({-1,-1});
			available.push_back({0,-1});
			available.push_back({1,-1});
			available.push_back({-1,1});
		}

		if (type=="pawn"){
			available.push_back({1,0});
			available.push_back({0,1});
			available.push_back({-1,0});
			available.push_back({0,-1});
		}

		if (type=="knight"){
			available.push_back({2,1});
			available.push_back({2,-1});
			available.push_back({1,2});
			available.push_back({-1,2});
			available.push_back({-2,1});
			available.push_back({-2,-1});
			available.push_back({-1,-2});
			available.push_back({1,-2});
		}
	
		return available;
	}

// TODO: add different piece types here; 
	void parse(){
		
	}
	void unload (){
		UnloadTexture(texture);
	}
};

void drawCursorBoxes(int x, int y, std::array<int,2> z, Color color = WHITE){
	x += z[0];
	y += z[1];
	
	// check if box is possible
	if (x > BOARDWIDTH-1 || y > BOARDHEIGHT-1 || x<0 || y<0){
		x-= z[0];
		y-= z[1];
	}
	int buff = 4;
	int thickness = 2;
	for (int i:z){
		int cx = x * TILE + FRAME_BUFFER + buff;
		int cy = (BOARDHEIGHT - y-1) * TILE + FRAME_BUFFER + buff;
		float f_x = static_cast<float> (cx);
		float f_y = static_cast<float> (cy);
		float boxwidth = static_cast<float> (TILE - buff*2-1);
		DrawRectangleLinesEx((Rectangle){f_x, f_y, boxwidth, boxwidth}, 5.0f, ORANGE);
			
	}
}

void showMoves(Piece piece){
	std::vector<std::array<int,2>> available = piece.availableMoves();
	for (std::array<int,2> i: available){
		drawCursorBoxes(piece.x, piece.y, i);
	}
}

class Player {
	std::vector<Piece> pieces;
};


class Board{
public: 
	std::array<std::array<std::string, BOARDHEIGHT>, BOARDWIDTH> boardKey = {{}}; 
	std::array<std::array<Piece, BOARDHEIGHT>, BOARDWIDTH> boardClass = {}; 
	std::vector<Piece> activePieces; 
	int pieceCount = 0;
	int oreCount = 0;

	void boardSetup(int pieceCount,
					Piece king, 
					Piece pawn1,
					Piece pawn2){

		
		for (int i= 0; i< BOARDHEIGHT; i++){
			for (int j = 0; j<BOARDWIDTH; j++){
				boardKey[j][i] = "000000";
			}
		}
		// create king in middle and two pawns next to him; 
			// TODO: change this so that the players are added to the player class
			// TODO: change this to the add function;
			
		king.init(BOARDWIDTH /2, 0, "King", "K001-w");
		addPieceToBoard(king);
		pawn1.init(BOARDWIDTH/2+1, 0, "pawn", "p001-w");
		addPieceToBoard(pawn1);
		pawn2.init(BOARDWIDTH/2-1, 0, "pawn", "p002-w");
		addPieceToBoard(pawn2);
	
	/*  ------ this is where I put random generation into the board setup 
		// create two random panws somewhere on the back two rows;	
		// 
		std::random_device rd; 
		std::mt19937 gen(rd()); 
		std::uniform_int_distribution<> distrib(1, 1000);
		int randomValue = distrib(gen);
		std::vector<Pieces> piecesOnBoard= {pieceCount};
		for  (int i = 0; i<pieceCount; i++){
			piecesOnBoard.push_back(
		}
			----- */ 
	}

	void addPieceToBoard(Piece ipiece){
		boardKey[ipiece.x][ipiece.y] = ipiece.key;
		boardClass[ipiece.x][ipiece.y] = ipiece;
		activePieces.push_back(ipiece);
		// TODO: add class function here based on board. 
	}

	void printActivePieces(){
		std::cout << "Number of active pieces: " << activePieces.size() << " ; ---> ";
		for (int i = 0; i< activePieces.size(); i++){
			std::cout<< activePieces[i].key << ", ";
		}
		std::cout << "\n";
	}

	/// print reverse so that array format x, y works and board is perspective from White;
		/// NOTE: this numbering system will have to be different than the actual numbering system displayed in game; starts at 1 not at zero like arrays do. 
	void printBoard(){
			std::cout<< "Board: " << "\n";
			for (int i= BOARDHEIGHT-1; i>=0; i--){
				std::cout << i << ": "; 
				for (int j = 0; j< BOARDWIDTH ; j++){
					std::cout << boardKey[j][i] << ", ";
				}
				std::cout<< "\n";
			}
	}

};

class TileCursor{

private:
	uint16_t blink = 600; // ms of delay in blink
	bool blinkOn = true;
	double lastBlink = 0; 
	uint16_t cursBuff = 8;
	double lastKeyPress = 0;

public:
	int cx = BOARDWIDTH, cy = BOARDHEIGHT;
	Piece piece;
	Ore ore; 
	uint16_t x, y;
	std::string key;
	bool pieceSelected;

	void init(int ix = 0, int iy=0, Color color = PINK){
		x = 4;
		y =4;
		cx = x * TILE + FRAME_BUFFER;
		cy = y * TILE + FRAME_BUFFER;
	}

	// TODO: center up cursor board
	// TODO: buff up cursor square to make it more visible
	void draw(int ix= 0, int iy =0, Color color = PINK, bool selected = false ){
		x = ix;
		y = iy;
		// blinks cursor same time on / off pattern; 
		cx = x * TILE + FRAME_BUFFER + cursBuff;
		cy = (BOARDHEIGHT - y-1) * TILE + FRAME_BUFFER + cursBuff;

// Rectangle {x, y, width, height}, thickness, color
//		if (blinkOn) DrawRectangleLines(cx, cy, TILE - cursBuff, TILE - cursBuff, color);
		int thickness = 5;
		float f_cx = static_cast<float> (cx);
		float f_cy = static_cast<float> (cy);
		float f_x = static_cast<float> (TILE - cursBuff*2-1);
		if (pieceSelected) color = RED;
		else color = WHITE; 
		if (!selected){
			if (blinkOn){	
				DrawRectangleLinesEx((Rectangle){f_cx, f_cy, f_x, f_x}, 5.0f, color);
			}
			// TODO: make the cursor blink for attraction;
			std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
			double nowDouble = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
			double timeDelta = nowDouble - lastBlink;
			if (timeDelta > blink){
				blinkOn = !blinkOn;
				lastBlink = nowDouble;
			}
		}
		else{
			DrawRectangleLinesEx((Rectangle){f_cx, f_cy, f_x, f_x}, 5.0f, color);
		}
	}

	void update(Board board){
		// 
		// TODO: bounce off last key press timer;
		if (IsKeyDown(KEY_D) && x<BOARDWIDTH-1){
			x++;
		}
		if (IsKeyDown(KEY_A) && x>0){
			x--;
		}
		if (IsKeyDown(KEY_S) && y>0){
			y--;
		}
		if (IsKeyDown(KEY_W) && y < BOARDHEIGHT-1){
			y++;
		}
		key = board.boardKey[x][y];

		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		double nowDouble = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		double debounce = 500;
		double delta = nowDouble - lastKeyPress;
		if (IsKeyDown(KEY_SPACE) && delta > debounce){
			pieceSelected = !pieceSelected;
			lastKeyPress = nowDouble;
		}
		if (pieceSelected && key != "000000" ){
			std::string key = "Pressed";
			const char* c_key = key.c_str();
			DrawText(c_key, x+150, y+10, 30, WHITE);
			showMoves(board.boardClass[x][y]);
		}
		
	// ----- required if having cursor errors ------///
		//if (x>BOARDWIDTH-1) x = BOARDWIDTH-1;
		//if (x<=0) x = 0;
		//if (y>BOARDHEIGHT-1) y = BOARDHEIGHT-1;
		//if (y<=0) y = 0;
	}

	// NOTE: no needed;
	void correction(){
		if (cx<0) cx=0;
		if (cx>BOARDWIDTH-1) cx = BOARDWIDTH -1;
		if (cy<0) cy=0;
		if (cy>BOARDHEIGHT-1) cx = BOARDHEIGHT -1;
	}
};
class Background{

public:
	Texture2D grass;
	int grassWidth;
	int grassHeight;
	//
	// TODO: create numbering and display it behind (letters for columns and numbers for rows);
	void drawPlayable (){
		int topx = FRAME_BUFFER; 
		int topy = FRAME_BUFFER; 

			// FIX: THIS
		DrawTexture(grass, 1000, 400, ORANGE);
			
		int widthMultiple = grassWidth / TILE;
		int heightMultiple = grassHeight / TILE;
		for (int i = 0; i< BOARDWIDTH / widthMultiple ; i++){
			for (int j = 0; j< BOARDHEIGHT / heightMultiple; j++){
				DrawTexture(grass, i*grassWidth + FRAME_BUFFER, j*grassHeight + FRAME_BUFFER, ORANGE);
			}
		}
		DrawLine(20,20, FRAME_BUFFER + BOARDWIDTH* TILE, topy + TILE, WHITE);
		//rows
		for (int i = 0; i<= BOARDHEIGHT; i++){
			DrawLine(topx, topy + i*TILE, FRAME_BUFFER + BOARDWIDTH* TILE, topy + i*TILE, WHITE);
		}
		//columnns
		for (int i = 0; i<=BOARDWIDTH; i++){
			DrawLine(topx + i*TILE, topy, topx + i*TILE, FRAME_BUFFER + BOARDHEIGHT*TILE, WHITE); 
		}

		
		const char* score = "SCORE";
		DrawText( score, SCREEN_WIDTH*3/4 - 80*3/2, SCREEN_HEIGHT/10 , 80, WHITE);
	}
};

class Tooltip{
private:
	float x = BOARDWIDTH * TILE + FRAME_BUFFER*2;
	float y = (BOARDHEIGHT*TILE) / 2;  
	float width = SCREEN_WIDTH - (BOARDWIDTH*TILE + FRAME_BUFFER *3);
	float height = SCREEN_HEIGHT /2;
public: 
	int pos_x, pos_y;
	std::string name; 

	void assignPiece(Piece piece){
		pos_x = piece.x;
		pos_y = piece.y;
		if (piece.type == "pawn"){
			name = "PAWN";
		}
		if (piece.type == "King"){
			name = "KING";
		}
		if (piece.type == "knight"){
			name = "KNIGHT";
		}
	}


	void displayCursor(Board board, TileCursor curs){
		Piece piece = board.boardClass[curs.x][curs.y];
		assignPiece(piece);
		// TODO: need to fix this for 2 digits or bigger boards;
		const char* c_name = name.c_str();
		DrawText(c_name, x+50, y+10, 30, WHITE);
		std::string key = board.boardKey[curs.x][curs.y];
		const char* c_key = key.c_str();
		DrawText(c_key, x+150, y+10, 30, WHITE);
		string s_pos_x = std::to_string(curs.x);
		const char* c_pos_x = s_pos_x.c_str();
		string s_pos_y = std::to_string(curs.y);
		const char* c_pos_y = s_pos_y.c_str();
		std::string strcurs = "Cursor: ";
		const char* c_curs = strcurs.c_str();
		DrawText(c_curs, x+20, y+50 , 20, WHITE);
		DrawText(c_pos_x, x+120, y+50, 20, WHITE);
		std::string x_str = "X: ";
		const char* c_x_str = x_str.c_str();
		DrawText(c_x_str, x+100, y+50, 20, WHITE);
		std::string y_str = "Y: ";
		const char* c_y_str = y_str.c_str();
		DrawText(c_y_str, x+150, y+50, 20, WHITE);
		DrawText(c_pos_y, x+170, y+50, 20, WHITE);
	}

	void displayPiece(Piece piece){
		// TODO: need to fix this for 2 digits or bigger boards;
		string s_pos_x = std::to_string(pos_x);
		const char* c_pos_x = s_pos_x.c_str();
		string s_pos_y = std::to_string(pos_y);
		const char* c_pos_y = s_pos_y.c_str();
		DrawRectangleLinesEx({x, y, width,height}, 3.0f, WHITE);
		DrawText(c_pos_x, x+100, y+100, 20, WHITE);
		std::string x_str = "X: ";
		const char* c_x_str = x_str.c_str();
		DrawText(c_x_str, x+80, y+100, 20, WHITE);
		std::string y_str = "Y: ";
		const char* c_y_str = y_str.c_str();
		DrawText(c_y_str, x+130, y+100, 20, WHITE);
		DrawText(c_pos_y, x+150, y+100, 20, WHITE);
	}
};

// NOTE: note used







// BUG: is not completely unloaded the texture. 
void unloadPieces(Board board){
	for (Piece i: board.activePieces){
		UnloadTexture(i.texture);
		std::cout<< i.key << " unloaded \n";
	}
}
void drawPieces(Board board){
	for (Piece i: board.activePieces){
			i.draw(i.x, i.y);
	}
}

int main () {
	bool startupInit = false; 
	Error currentError; 
	Tooltip tt;

	// initilizing ball and players
	Board board; 
	TileCursor testCursor;
	TileCursor myCursor;
	Piece firstPawn;
	Piece secondPawn;
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
		//DrawFPS(20,20); // remove if necessary; 
		bg.grass = LoadTexture("../assets/grass.png"); 
		bg.grassWidth = 96;
		bg.grassHeight = 96;
		bg.drawPlayable();
	
		while (!startupInit){
			myCursor.init();
			board.boardSetup(3, firstKing, firstPawn, secondPawn );
			startupInit = !startupInit;
		}
		
		// Background 
		// TODO: make it patterned - dirt / grass / dirt / grass (to show white v black square) 
		testCursor.draw(2,2, PINK);

		// Playable Chars
		// TODO: transfer draw textures to board class function 
		// firstPawn.texture = LoadTexture("../assets/white-pawn48.png");
		firstPawn.draw(2, 2);
		firstPawn.type = "pawn";
		firstPawn.x = 2;
		firstPawn.y = 2;
//		firstKnight.texture = LoadTexture("../assets/white-knight48.png");
//		firstKnight.draw(FRAME_BUFFER + 4*TILE, FRAME_BUFFER + 4*TILE);
		firstCopper.texture = LoadTexture("../assets/Orange3Rock.PNG");
		firstCopper.draw(FRAME_BUFFER + 5*TILE, FRAME_BUFFER + 5*TILE);
		myCursor.update(board);

//		showMoves(board.activePieces[0]);
		myCursor.draw(myCursor.x, myCursor.y, BLUE);
		drawPieces(board);

//		Piece test =  board.activePieces[0];
//		test.draw(4,4);
//		std::cout << test.key << "-- > x: "<< test.x << "; y: " << test.y << "\n";
//		std::cout << test.key << "-- > cx: "<< test.cx << "; cy: " << test.cy << "\n";
		board.printBoard();
		board.printActivePieces();
		tt.displayPiece(firstPawn);
		tt.displayCursor(board, myCursor);
		
		
		currentError.printErrors();
		std::cout<< "CURSOR: X:" << myCursor.x << ", Y:" << myCursor.y << "\n";
		//myCursor.correction();
		// NOTE: see if this correction statement above is still neeeded;

        EndDrawing();
			//
		// TODO: transfer unloadtexture functions to a class board function;
		board.activePieces[0].unload();
		

		UnloadTexture(board.activePieces[0].texture);
		UnloadTexture(board.activePieces[1].texture);
		UnloadTexture(board.activePieces[2].texture);

		UnloadTexture(firstPawn.texture);
		UnloadTexture(firstCopper.texture);
		UnloadTexture(bg.grass);
		//unloadPieces(board);
		firstPawn.unload();
    }

}
