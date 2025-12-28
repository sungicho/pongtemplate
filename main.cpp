#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <variant>
#include<unordered_map>

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


class TextureHold{
public:
	Texture2D KingTexture;
	Texture2D PawnTexture;
	Texture2D KnightTexture;
	Texture2D BishopTexture;
	Texture2D QueenTexture;
	Texture2D RookTexture;
	Texture2D Ore;

};


class Error{
};

class Ore{
public:
	Texture2D texture; 
	uint16_t x, y; 
	uint16_t size;
	int texWidth = 48;
	int texHeight = 48;


	void draw(uint16_t x = 0, uint16_t y = 0){
		int cx = x * TILE + FRAME_BUFFER - texWidth + TILE;
		int cy = (BOARDHEIGHT - y-1) * TILE + FRAME_BUFFER -texHeight + TILE;
		DrawTexture(texture, cx, cy, ORANGE);
	}
};

class Piece{
private: 
	std::string error; 
	int texHeight = TILE;
	int texWidth = TILE;

public:
	int cx =0, cy =0;
	std::string key;
	std::string type;
	Texture2D texture; 
	uint16_t x, y; 
	Ore oreHeld;
	// NOTE: key = letter showing piece + 4 digit + "-" player letter (w/b)   "p002-w"

	void init(int ix = 0, int iy = 0, std::string itype = "pawn", std::string ikey = "p000-w"){
		x = ix;
		y = iy;
		type = itype;
		key = ikey;
	}

	void draw( TextureHold TXH,int ix = 400, int iy = 400, bool elevated = false, bool flying = false){
		x = ix;
		y = iy;
		Texture2D texture;
		
		if (type == "pawn"){
			texHeight = 48;
			texWidth = 48;
			texture = TXH.PawnTexture;
		}
		if (type == "Queen") {
			texHeight = 64;
			texWidth= 56;
			texture = TXH.QueenTexture;
		}
		if (type == "King") {
			texHeight = 64;
			texWidth= 56;
			texture = TXH.KingTexture;
		}
		if (type == "knight") {
			texHeight = 48;
			texWidth= 48;
			texture = TXH.KnightTexture;
		}
		if (type == "rook") {
			texHeight = 48;
			texWidth= 48;
			texture = TXH.RookTexture;
		}	
		if (type == "bishop") {
			texHeight = 48;
			texWidth= 48;
			texture = TXH.BishopTexture;
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
		//UnloadTexture(texture)	kk;
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
		if (type =="rook"){
			for (int i = 0; i<=8; i++){
				available.push_back({i,0});
				available.push_back({0,i});
				available.push_back({-i,0});
				available.push_back({0,-i});
			}
		}	
		if (type =="bishop"){
			for (int i = 0; i<=8; i++){
				available.push_back({i,i});
				available.push_back({-i,i});
				available.push_back({i,-i});
				available.push_back({-i,-i});
			}
		}	
		if (type =="Queen"){
			for (int i = 0; i<=8; i++){
				available.push_back({i,i});
				available.push_back({-i,i});
				available.push_back({i,-i});
				available.push_back({-i,-i});
				available.push_back({i,0});
				available.push_back({0,i});
				available.push_back({-i,0});
				available.push_back({0,-i});
			}
		}
	
		return available;
	}
};


void drawCursorBoxes(int x, int y, std::array<int,2> z = {0,0}, Color color = WHITE){
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
		DrawRectangleLinesEx((Rectangle){f_x, f_y, boxwidth, boxwidth}, 5.0f, color);
			
	}
}

class Board{
public: 
	std::array<std::array<std::string, BOARDHEIGHT>, BOARDWIDTH> boardKey = {{}}; 
	std::unordered_map<std::string, Piece> activePieces;
	std::vector<std::string> keys;
	Piece fluffPiece;

	
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
	}
	
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
	void showMoves(int x, int y){
		std::string key = boardKey[x][y];
		Piece activePiece = activePieces.find(key)->second;
		// TODO: need to stop showing if there is a piece there. 
		std::vector<std::array<int,2>> available = activePiece.availableMoves();
		for (std::array<int,2> i: available){
			if (boardKey[x+i[0]][y+i[1]] == "000000"){
				drawCursorBoxes(activePiece.x, activePiece.y, i, ORANGE);
			}
			if (boardKey[x+i[0]][y+i[1]] != "000000"){
				drawCursorBoxes(activePiece.x, activePiece.y, i, RED);
			}
		}
	}
	
	// TODO: need to validate enemy position
	bool isValidMove(string key, int tx, int ty){
		Piece activePiece = activePieces.find(key)->second;
		std::vector<std::array<int,2>> available = activePiece.availableMoves();
		for (std::array<int,2> i: available){
			if (activePiece.x+i[0]== tx && activePiece.y+i[1]== ty && boardKey[tx][ty] == "000000"){
				return true;
			}
		}
		return false;
	}

	void updateBoardKey(Piece piece, int oldx, int oldy){
		activePieces[piece.key] = piece;
		boardKey[oldx][oldy] = "000000";
		boardKey[piece.x][piece.y] = piece.key;

	}

	void addPieceToBoard(Piece ipiece){
		boardKey[ipiece.x][ipiece.y] = ipiece.key;
		activePieces.insert({ipiece.key,ipiece});
		keys.push_back(ipiece.key);
		// TODO: add class function here based on board. 
	}

	void printActivePieces(){
		
		std::cout << "Number of active pieces: " << activePieces.size() << " ; ---> ";
		std::vector<std::string> keyList;
		for (auto& key: activePieces){
			keyList.push_back(key.first);
		}
		for (int i=0; i<keyList.size(); i++){ 
			std::cout << keyList[i] << ", ";
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

//	void removePiece (int x, int y){
//		boardKey[x][y] = "000000";
//		fluffPiece.init(0,0 ,"pawn", "000000");
//
//		boardClass[x][y] = fluffPiece;
//		fluffPiece.unload();
//	}

};

class TileCursor{

private:
	uint16_t blink = 600; // ms of delay in blink
	bool blinkOn = true;
	double lastBlink = 0; 
	uint16_t cursBuff = 8;
	double lastKeyPress = 0;
	double lastCursorPress = 0;
	Piece selectedPiece, buffPiece;
	std::string selectedKey;
	std::string key;
	bool pieceSelected ;
	std::string tf ="";

public:
	int cx = BOARDWIDTH, cy = BOARDHEIGHT;
	Piece piece;
	Ore ore; 
	uint16_t x, y;
	int selectedX=0, selectedY=0;
	bool master = false;
	int Up_Key = KEY_W, Down_Key = KEY_S, Right_Key = KEY_D, Left_Key = KEY_A;

	void init(int ix = 0, int iy=0, Color color = PINK){
		x = 4;
		y =4;
		cx = x * TILE + FRAME_BUFFER;
		cy = y * TILE + FRAME_BUFFER;
	}

	// TODO: center up cursor board
	// TODO: buff up cursor square to make it more visible
	void draw(int ix= 0, int iy =0, Color color = PINK ){
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
		//else color = WHITE; 
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
		//else{
		//	DrawRectangleLinesEx((Rectangle){f_cx, f_cy, f_x, f_x}, 5.0f, color);
		//}
	}

	void update(Board *board){
		// TODO: bounce off last key press timer;
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		double nowDouble = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		double cursorDebounce = 80;
		double delta = nowDouble - lastCursorPress;

		if (IsKeyDown(Right_Key) && x<BOARDWIDTH-1 && delta>cursorDebounce){
			x++;
			lastCursorPress = nowDouble;
		}
		if (IsKeyDown(Left_Key) && x>0 && delta>cursorDebounce){
			x--;
			lastCursorPress = nowDouble;
		}
		if (IsKeyDown(KEY_S) && y>0 && delta>cursorDebounce){
			y--;
			lastCursorPress = nowDouble;
		}
		if (IsKeyDown(KEY_W) && y < BOARDHEIGHT-1 && delta>cursorDebounce){
			y++;
			lastCursorPress = nowDouble;
		}
		key = board->boardKey[x][y];

		now = std::chrono::system_clock::now();
		nowDouble = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		double debounce = 300;
		delta = nowDouble - lastKeyPress;
		// TODO: need to change this to off-select the cursor once plyaer hits space a second time

		if (IsKeyDown(KEY_SPACE) && delta > debounce && key != "000000" && !pieceSelected){
			pieceSelected = true;
			lastKeyPress = nowDouble;
			selectedX = x; 
			selectedY = y;
			buffPiece = board->activePieces[key];
			selectedKey = buffPiece.key;
		}
		if (pieceSelected) tf = "TRUE";
		else tf = "FALSE";
		std::cout<< "SELECTION: "<<tf << "\n";

		std::cout<< "Selected KEY: " << selectedKey<< ";  S-X: "<< buffPiece.x << ";  Y: " << buffPiece.y <<"\n";
		std::cout<< "SelectedX: " << selectedX<< " ; SELECTED Y:  "<< selectedY << "\n";
		std::cout<< "X: " << x << " ;  Y:" << y << "\n";
		std::cout<< "Delta: " << delta <<" Now: " << nowDouble << " ;  lastKeyPress:" << lastKeyPress << "\n";
		now = std::chrono::system_clock::now();
		nowDouble = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		debounce = 500;
		delta = nowDouble - lastKeyPress;

		if (IsKeyDown(KEY_SPACE) && delta > debounce && pieceSelected && board->isValidMove(selectedKey,x ,y)){
			pieceSelected = !pieceSelected;
			lastKeyPress = nowDouble;
			board->boardKey[selectedX][selectedY] = "000000";
			board->boardKey[x][y] = selectedKey;
			board->activePieces[selectedKey].x = x;
			board->activePieces[selectedKey].y = y;
			std::string key = selectedKey;
			const char* c_key = key.c_str();
			DrawText(c_key, 250, 10, 30, WHITE);		
		}

		if (pieceSelected){
			// TODO: need to have this persist for the last one since last clicked
			std::string key = "Pressed";
			const char* c_key = key.c_str();
			DrawText(c_key, 150, 10, 30, WHITE);
			board->showMoves(selectedX, selectedY);
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

class Tooltip{
private:
	float x = BOARDWIDTH * TILE + FRAME_BUFFER*2;
	float y = (BOARDHEIGHT*TILE) / 2;  
	float width = SCREEN_WIDTH - (BOARDWIDTH*TILE + FRAME_BUFFER *3);
	float height = SCREEN_HEIGHT /2;
public: 
	int pos_x = 5, pos_y =5;
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
		if (piece.type == "bishop"){
			name = "BISHOP";
		}
		if (piece.type == "rook"){
			name = "ROOK";
		}
		if (piece.type == "Queen"){
			name = "QUEEN";
		}
	}


	void displayCursor(Board board, TileCursor curs){
		std::string pieceKey = board.boardKey[curs.x][curs.y];
		if (pieceKey != "000000"){
	//		Piece piece = board.activePieces.find(pieceKey$ 
			std::cout<< "cursor piece: X: " << curs.x << ";  Y: " << curs.y  << "\n";
			Piece piece = board.activePieces.find(pieceKey)->second;
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

		int widthMultiple = grassWidth / TILE;
		int heightMultiple = grassHeight / TILE;
		for (int i = 0; i< BOARDWIDTH / widthMultiple ; i++){
			for (int j = 0; j< BOARDHEIGHT / heightMultiple; j++){
				DrawTexture(grass, i*grassWidth + FRAME_BUFFER, j*grassHeight + FRAME_BUFFER, ORANGE);
			}
		}
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

void drawPieces(TextureHold txh, Board board){
	for (int i=0; i<board.keys.size(); i++){ 
		Piece piece = board.activePieces[board.keys[i]];
		//Piece piece = board.activePieces.find(board.keys[i])->second;
		piece.draw(txh, piece.x, piece.y);
		//UnloadTexture(piece.texture);
	}
}

//------------- MAIN ------------//
int main () {
	bool startupInit = false; 
	Error currentError; 
	bool startupTestPawn = false;
	TextureHold textures;

	TileCursor myCursor;
	Background bg;
	Board board;
	double lastKeyPress= 0;
	// initilizing ball and players
	Piece firstPawn;
	Piece testPawn;
	Piece secondPawn;
	Piece firstKing;
	Piece firstRook;
	Piece firstQueen;
	Piece firstKnight;
	Piece firstBishop;
	Ore firstCopper;
	Tooltip tt;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
	SetTargetFPS(60);

	bool testInit = false;

    while (WindowShouldClose() == false){
		BeginDrawing();
		ClearBackground(BLACK);
		DrawFPS(20,20); // remove if necessary; 
		textures.PawnTexture = LoadTexture("../assets/white-pawn48.png");
		textures.KingTexture = LoadTexture("../assets/white-king64.png");
		textures.KnightTexture = LoadTexture("../assets/white-knight48.png");
		textures.BishopTexture = LoadTexture("../assets/white-bishop48.png");
		textures.RookTexture = LoadTexture("../assets/white-rook48.png");
		textures.QueenTexture = LoadTexture("../assets/white-queen64.png");
		textures.Ore = LoadTexture("../assets/Orange3Rock.PNG");
		firstCopper.texture = textures.Ore;
		while (!startupInit){
			myCursor.init();
			board.boardSetup(3, firstKing, firstPawn, secondPawn );
			startupInit = !startupInit;
		}
		while(!testInit){
			firstKnight.init(3,4,"knight", "n001-w");
			board.addPieceToBoard(firstKnight);
			firstBishop.init(8,8,"bishop", "b001-w");
			firstQueen.init(9,9,"Queen", "Q001-w");
			firstRook.init(11,10,"rook", "r001-w");
			board.addPieceToBoard(firstQueen);
			board.addPieceToBoard(firstRook);
			board.addPieceToBoard(firstBishop);
			testInit = !testInit;
		}

		bg.grass = LoadTexture("../assets/grass.png"); 
		bg.grassWidth = 96;
		bg.grassHeight = 96;
		bg.drawPlayable();
//		board.boardSetup(3, firstKing, firstPawn, secondPawn);
		board.printBoard();
		myCursor.draw(myCursor.x, myCursor.y, BLUE);

		// NOTE: need to check the correct pointer and values are passed through
		Board* boardPointer = &board;
		board = *boardPointer;

		myCursor.update(boardPointer);
		std::cout<< "KEYS: ";
		for (int i = 0; i< board.keys.size(); i++){
			std::cout <<  board.keys[i] << ", ";
		}
		std::cout << "\n";
		std::cout << "StartupInit: " << startupInit << " ;  testInit:" << testInit << "\n";
		std::cout<<  "Knight- X:" << board.activePieces["n001-w"].x << " ;  Y: " << board.activePieces["n001-w"].y << "\n";

//		int nx = board.activePieces["n001-w"].x;
//		int ny = board.activePieces["n001-w"].y;
//		boardPointer->boardKey[nx][ny] = "0x0x0x";
//		boardPointer->activePieces["n001-w"].x= 10;
//		boardPointer->activePieces["n001-w"].y= 10;
//		boardPointer->boardKey[10][10] = "n001-w";
		drawPieces(textures, *boardPointer);
		firstCopper.draw(6,6);
		board.printActivePieces();
		tt.displayPiece(firstPawn);
		tt.displayCursor(board, myCursor);
		EndDrawing();


		UnloadTexture(textures.PawnTexture);
		UnloadTexture(textures.KingTexture);
		UnloadTexture(textures.QueenTexture);
		UnloadTexture(textures.RookTexture);
		UnloadTexture(textures.KnightTexture);
		UnloadTexture(textures.BishopTexture);
		UnloadTexture(bg.grass);
		UnloadTexture(textures.Ore);
	}
		

}

