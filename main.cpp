#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>

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
uint16_t turnCounter = 1;


class TextureHold{
public:
	Texture2D WhiteKingTexture;
	Texture2D WhitePawnTexture;
	Texture2D WhiteKnightTexture;
	Texture2D WhiteBishopTexture;
	Texture2D WhiteQueenTexture;
	Texture2D WhiteRookTexture;
	Texture2D Ore;
	Texture2D BlackKingTexture;
	Texture2D BlackPawnTexture;
	Texture2D BlackKnightTexture;
	Texture2D BlackBishopTexture;
	Texture2D BlackQueenTexture;
	Texture2D BlackRookTexture;
	Texture2D attacking1;
	Texture2D attacking2;
	Texture2D attacking3;
};

class Animation{
public:
	int first, last, cur;
	float duration_left, speed;
};

void animation_update(Animation * self ){
	float dt = GetFrameTime();
	self -> duration_left -=dt;
	if (self->duration_left <=0.0){
		self->duration_left = self->speed;
		self->cur ++;
	}
	if (self->cur > self->last){
		self->cur = self->first;
	}
}

Rectangle buildAnim (Texture2D texture, int x, int y){
	Rectangle result;
	result.height = texture.height;
	result.width = texture.width;
	result.x = x;
	result.y= y;
	return result;
}

Texture2D* TextAnim3(Texture2D *text1, Texture2D* text2, Texture2D* text3, Animation* anim){
	Texture2D resultText;
	if (anim->cur == anim->first) return text1;
	if (anim->cur == anim->last) return text3;
	else return text2;
}


class Error{
};

class Player{
public:
	std::string color;
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
	Player player;

	// NOTE: key = letter showing piece + 4 digit + "-" player letter (w/b)   "p002-w"

	void init(int ix = 0, int iy = 0, std::string itype = "pawn", std::string ikey = "p000-w" , std::string color = "white"){
		x = ix;
		y = iy;
		type = itype;
		key = ikey;
		player.color = color;
	}

	void draw( TextureHold TXH,int ix = 400, int iy = 400, bool elevated = false, bool flying = false){
		x = ix;
		y = iy;
		Texture2D texture;
		
		if (type == "pawn" && player.color == "white"){
			texHeight = 48;
			texWidth = 48;
			texture = TXH.WhitePawnTexture;
		}
		if (type == "Queen" && player.color == "white") {
			texHeight = 64;
			texWidth= 56;
			texture = TXH.WhiteQueenTexture;
		}
		if (type == "King" && player.color == "white") {
			texHeight = 64;
			texWidth= 56;
			texture = TXH.WhiteKingTexture;
		}
		if (type == "knight" && player.color == "white") {
			texHeight = 48;
			texWidth= 48;
			texture = TXH.WhiteKnightTexture;
		}
		if (type == "rook" && player.color == "white") {
			texHeight = 48;
			texWidth= 48;
			texture = TXH.WhiteRookTexture;
		}	
		if (type == "bishop" && player.color == "white") {
			texHeight = 48;
			texWidth= 48;
			texture = TXH.WhiteBishopTexture;
		}

		//Black
		if (type == "pawn" && player.color == "black") {
			texHeight = 48;
			texWidth= 48;
			texture = TXH.BlackBishopTexture;
		}
		if (type == "Queen" && player.color == "black") {
			texHeight = 64;
			texWidth= 56;
			texture = TXH.BlackQueenTexture;
		}
		if (type == "King" && player.color == "black") {
			texHeight = 64;
			texWidth= 56;
			texture = TXH.BlackKingTexture;
		}
		if (type == "knight" && player.color == "black") {
			texHeight = 48;
			texWidth= 48;
			texture = TXH.BlackKnightTexture;
		}
		if (type == "bishop" && player.color == "black") {
			texHeight = 48;
			texWidth= 48;
			texture = TXH.BlackBishopTexture;
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
					Piece* king, 
					Piece* pawn1,
					Piece* pawn2){

		
		for (int i= 0; i< BOARDHEIGHT; i++){
			for (int j = 0; j<BOARDWIDTH; j++){
				boardKey[j][i] = "000000";
			}
		}
		// create king in middle and two pawns next to him; 
			// TODO: change this so that the players are added to the player class
			// TODO: change this to the add function;
			
		king->init(BOARDWIDTH /2, 0, "King", "K001-w");
		addPieceToBoard(king);
		pawn1->init(BOARDWIDTH/2+1, 0, "pawn", "p001-w", "white");
		addPieceToBoard(pawn1);
		pawn2->init(BOARDWIDTH/2-1, 0, "pawn", "p002-w", "white");
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

	std::vector<std::array<int, 2>> availableMoves(Piece piece){
		std::vector<std::array<int,2>> available;
		
		// TODO: run addtional if statements to check if occupied
		if (piece.type == "King"){
			available.push_back({1,0});
			available.push_back({1,1});
			available.push_back({0,1});
			available.push_back({-1,0});
			available.push_back({-1,-1});
			available.push_back({0,-1});
			available.push_back({1,-1});
			available.push_back({-1,1});
		}
		/// TODO: if pawn is right by ore; 
		if (piece.type=="pawn"){
			available.push_back({1,0});
			available.push_back({0,1});
			available.push_back({-1,0});
			available.push_back({0,-1});
		}
		if (piece.type =="knight"){
			available.push_back({2,1});
			available.push_back({2,-1});
			available.push_back({1,2});
			available.push_back({-1,2});
			available.push_back({-2,1});
			available.push_back({-2,-1});
			available.push_back({-1,-2});
			available.push_back({1,-2});
		}

		if (piece.type =="rook"){
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y] == "000000") available.push_back({i,0});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y+i] == "000000") available.push_back({0,i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y] == "000000") available.push_back({-i,0});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y-i] == "000000") available.push_back({0,-i});
				else break;
			}
//			
		}	
		if (piece.type =="bishop"){
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y+i] == "000000") available.push_back({i,i});
				else break;
			}	
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y+i] == "000000") available.push_back({-i, i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y-i] == "000000") available.push_back({i,-i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y-i] == "000000") available.push_back({-i,-i});
				else break;
			}
		}

		if (piece.type =="Queen"){
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y+i] == "000000") available.push_back({i,i});
				else break;
			}	
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y+i] == "000000") available.push_back({-i, i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y-i] == "000000") available.push_back({i,-i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y-i] == "000000") available.push_back({-i,-i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y] == "000000") available.push_back({i,0});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y+i] == "000000") available.push_back({0,i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y] == "000000") available.push_back({-i,0});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y-i] == "000000") available.push_back({0,-i});
				else break;
			}
			
		}
		return available;
	}


	std::vector<std::array<int, 2>> availableAttack(Piece piece){
		std::vector<std::array<int, 2>> available;
		
		if (piece.type == "Bishop"){
			for (int i = 1; i<=8; i++){
				if (activePieces.find(boardKey[piece.x+i][piece.y])->second.player.color
					!= piece.player.color) available.push_back({0,-i});
			}
		}
		return available;
	}

	void showMoves(int x, int y){
		std::string key = boardKey[x][y];
		Piece activePiece = activePieces.find(key)->second;
		std::vector<std::array<int,2>> available = availableMoves(activePiece);
		for (std::array<int,2> i: available){
			if (boardKey[x+i[0]][y+i[1]] == "000000"){
				drawCursorBoxes(activePiece.x, activePiece.y, i, ORANGE);
			}
			if (boardKey[x+i[0]][y+i[1]] != "000000"){
				drawCursorBoxes(activePiece.x, activePiece.y, i, RED);
			}
		}
	}
	
	bool isValidMove(string key, int tx, int ty){
		Piece activePiece = activePieces.find(key)->second;
		std::vector<std::array<int,2>> available = availableMoves(activePiece);
		for (std::array<int,2> i: available){
			if (activePiece.x+i[0]== tx && activePiece.y+i[1]== ty && boardKey[tx][ty] == "000000"){
				return true;
			}
		}
		return false;
	}

	void addPieceToBoard(Piece* ipiece){

		std::string key_str = ipiece -> key;
		boardKey[ipiece->x][ipiece->y] = ipiece->key;
		//*activePieces[ipiece->key] = &ipiece;
		activePieces.emplace(key_str, *ipiece);
		keys.push_back(ipiece->key);
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
			// TODO: make the cursor blink for less than shown (more on than off)
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
			turnCounter ++;
		}

		if ((IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_ESCAPE)) && delta > debounce && pieceSelected && !board->isValidMove(selectedKey,x ,y)){
			pieceSelected = !pieceSelected;
			lastKeyPress = nowDouble;
			x = selectedX;
			y = selectedY;
		}

		if (pieceSelected){
			// TODO: need to have this persist for the last one since last clicked
			std::string key = "Pressed";
			const char* c_key = key.c_str();
			DrawText(c_key, 150, 10, 30, WHITE);
			board->showMoves(selectedX, selectedY);
		}

		std::string tc_string = std::to_string(turnCounter); 
		const char* tC_char = tc_string.c_str();
		DrawText(tC_char, 100, 10, 30, WHITE);		

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

	void displayPiece(Board board, Piece piece){
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
		std::string piece_player = piece.player.color;
		const char* c_piece_player = piece_player.c_str();
		DrawText(c_piece_player, x+220, y+100, 20, WHITE);
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

void drawPieces(TextureHold txh, Board* board){
	for (int i=0; i<board->keys.size(); i++){ 
		Piece piece = board->activePieces[board->keys[i]];
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

	Player white;
	Player black;
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
	Piece blackPawn;
	Piece blackPawn2;
	Piece blackKing;
	Piece blackRoo;
	Piece blackQueen;
	Piece blackKnight;
	Piece blackBishop;

	Ore firstCopper;
	Tooltip tt;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
	SetTargetFPS(60);

	bool testInit = false;

	Animation attackAnim = (Animation){
		.first = 1,
		.last = 3, 
		.cur = 1,
		.duration_left = .5, 
		.speed = .3
	};

    while (WindowShouldClose() == false){

		SetExitKey(KEY_NULL);
		BeginDrawing();
		ClearBackground(BLACK);
		DrawFPS(20,20); // remove if necessary; 
		//
		// TODO: make a fucntion with allof the textures and pointer declarations
		// TODO: need to make another function to create all of the classes
		textures.WhitePawnTexture = LoadTexture("../assets/white-pawn48.png");
		textures.WhiteKingTexture = LoadTexture("../assets/white-king64.png");
		textures.WhiteKnightTexture = LoadTexture("../assets/white-knight48.png");
		textures.WhiteBishopTexture = LoadTexture("../assets/white-bishop48.png");
		textures.WhiteRookTexture = LoadTexture("../assets/white-rook48.png");
		textures.WhiteQueenTexture = LoadTexture("../assets/white-queen64.png");
		textures.Ore = LoadTexture("../assets/Orange3Rock.PNG");
		textures.BlackPawnTexture = LoadTexture("../assets/black-pawn48.png");
		textures.BlackKingTexture = LoadTexture("../assets/black-king64.png");
		textures.BlackKnightTexture = LoadTexture("../assets/black-knight48.png");
		textures.BlackBishopTexture = LoadTexture("../assets/black-bishop48.png");
		textures.BlackRookTexture = LoadTexture("../assets/black-rook48.png");
		textures.BlackQueenTexture = LoadTexture("../assets/black-queen48.png");
		textures.attacking1 = LoadTexture("../assets/FX/FX036_01.png");
		textures.attacking2 = LoadTexture("../assets/FX/FX036_02.png");
		textures.attacking3 = LoadTexture("../assets/FX/FX036_03.png");
		Rectangle attRect = buildAnim(textures.attacking1, 10, 10);

		firstCopper.texture = textures.Ore;



		while (!startupInit){
			myCursor.init();
			board.boardSetup(3, &firstKing, &firstPawn, &secondPawn );
			startupInit = !startupInit;
		}
		while(!testInit){
			firstKnight.init(10,4,"knight", "n001-w", "white");
			board.addPieceToBoard(&firstKnight);
			firstBishop.init(8,8,"bishop", "b001-w");
			firstQueen.init(9,9,"Queen", "Q001-w");
			firstRook.init(11,10,"rook", "r001-w");
			board.addPieceToBoard(&firstQueen);
			board.addPieceToBoard(&firstRook);
			board.addPieceToBoard(&firstBishop);
			testInit = !testInit;
			blackKnight.init(2,1,"knight", "n001-b", "black");
			blackPawn.init(3,3, "pawn", "p001-b", "black");
			board.addPieceToBoard(&blackKnight);
			board.addPieceToBoard(&blackPawn);
			blackKing.init(3,4, "King", "K001-b", "black");
			board.addPieceToBoard(&blackKing);
		}
		

		bg.grass = LoadTexture("../assets/grass.png"); 
		bg.grassWidth = 96;
		bg.grassHeight = 96;
		bg.drawPlayable();
//		board.boardSetup(3, firstKing, firstPawn, secondPawn);
		board.printBoard();
		myCursor.draw(myCursor.x, myCursor.y, BLUE);

		// NOTE: need to check the correct pointer and values are passed through

		myCursor.update(&board);
		std::cout<< "KEYS: ";
		for (int i = 0; i< board.keys.size(); i++){
			std::cout <<  board.keys[i] << ", ";
		}
		std::cout << "\n";

//		int nx = board.activePieces["n001-w"].x;
//		int ny = board.activePieces["n001-w"].y;
//		boardPointer->boardKey[nx][ny] = "0x0x0x";
//		boardPointer->activePieces["n001-w"].x= 10;
//		boardPointer->activePieces["n001-w"].y= 10;
//		boardPointer->boardKey[10][10] = "n001-w";
		drawPieces(textures, &board);
		firstCopper.draw(6,6);
		board.printActivePieces();

		tt.displayPiece(board, firstPawn);
		tt.displayCursor(board, myCursor);
		std::cout<< "Player cursor: " << firstPawn.player.color << "\n"; 
		std::cout<< "COLOR: " << board.activePieces["p001-b"].player.color<< "\n";

		Texture2D *buffText = TextAnim3(&textures.attacking1, &textures.attacking2, &textures.attacking3, &attackAnim);
		animation_update(&attackAnim);
		DrawTexturePro(*buffText, attRect, attRect, {0,0}, 0, WHITE);



		EndDrawing();


		UnloadTexture(textures.WhitePawnTexture);
		UnloadTexture(textures.WhiteKingTexture);
		UnloadTexture(textures.WhiteQueenTexture);
		UnloadTexture(textures.WhiteRookTexture);
		UnloadTexture(textures.WhiteKnightTexture);
		UnloadTexture(textures.WhiteBishopTexture);
		UnloadTexture(bg.grass);
		UnloadTexture(textures.BlackPawnTexture);
		UnloadTexture(textures.BlackKingTexture);
		UnloadTexture(textures.BlackQueenTexture);
		UnloadTexture(textures.BlackRookTexture);
		UnloadTexture(textures.BlackKnightTexture);
		UnloadTexture(textures.BlackBishopTexture);

		UnloadTexture(textures.Ore);
	}
		

}

