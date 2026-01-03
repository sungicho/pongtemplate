#include "nlohmann/json_fwd.hpp"
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


// Screen sizing
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE 48
#define FRAME_BUFFER 45

//namespace std;
// Make this stationary board first >> then make it move so you can scan the field 
// 32px pieces 
const uint16_t BOARDWIDTH = 16;
const uint16_t BOARDHEIGHT = 12;
uint16_t turnCounter = 1;
const std::string empty = "000000";

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
	Player capturedPlayer;

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

		// FIX: change back from ORANGE
		DrawTexture(texture, cx, cy, ORANGE);
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
	std::unordered_map<std::string, Piece> capturedPieces;
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
				boardKey[j][i] = empty;
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
				if (boardKey[piece.x+i][piece.y] == empty) available.push_back({i,0});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y+i] == empty) available.push_back({0,i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y] == empty) available.push_back({-i,0});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y-i] == empty) available.push_back({0,-i});
				else break;
			}
//			
		}	
		if (piece.type =="bishop"){
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y+i] == empty) available.push_back({i,i});
				else break;
			}	
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y+i] == empty) available.push_back({-i, i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y-i] == empty) available.push_back({i,-i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y-i] == empty) available.push_back({-i,-i});
				else break;
			}
		}

		if (piece.type =="Queen"){
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y+i] == empty) available.push_back({i,i});
				else break;
			}	
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y+i] == empty) available.push_back({-i, i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y-i] == empty) available.push_back({i,-i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y-i] == empty) available.push_back({-i,-i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y] == empty) available.push_back({i,0});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y+i] == empty) available.push_back({0,i});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y] == empty) available.push_back({-i,0});
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y-i] == empty) available.push_back({0,-i});
				else break;
			}
			
		}
		return available;
	}

	std::vector<std::array<int, 2>> availableAttack(Piece piece){
		std::vector<std::array<int,2>> available;
		
		// TODO: run addtional if statements to check if occupied
		if (piece.type == "King"){
			if (piece.x+1< BOARDWIDTH) available.push_back({1,0});
			if (piece.x+1< BOARDWIDTH && piece.y+1 < BOARDHEIGHT) available.push_back({1,1});
			if (piece.y+1 < BOARDHEIGHT) available.push_back({0,1});
			if (piece.x-1>= 0 ) available.push_back({-1,0});
			if (piece.x-1>= 0 && piece.y-1 >=0) available.push_back({-1,-1});
			if (piece.y-1 >=0) available.push_back({0,-1});
			if (piece.x+1< BOARDWIDTH && piece.y-1>= 0) available.push_back({1,-1});
			if (piece.x-1>= 0 && piece.y+1 < BOARDHEIGHT) available.push_back({-1,1});
					}
		/// TODO: if pawn is right by ore; 
		if (piece.type=="pawn"){
			if (piece.x+1< BOARDWIDTH && piece.y+1 < BOARDHEIGHT) available.push_back({1,1});
			if (piece.x-1>= 0 && piece.y+1 < BOARDHEIGHT) available.push_back({-1,1});
			if (piece.x+1< BOARDWIDTH && piece.y-1>= 0) available.push_back({1,-1});
			if (piece.x-1>= 0 && piece.y-1 >=0) available.push_back({-1,-1});
		}
		if (piece.type =="knight"){
			if (piece.x+2< BOARDWIDTH && piece.y+1 < BOARDHEIGHT) available.push_back({2,1});
			if (piece.x+2< BOARDWIDTH && piece.y-1>= 0) available.push_back({2,-1});
			if (piece.x+1< BOARDWIDTH && piece.y+2 < BOARDHEIGHT) available.push_back({1,2});
			if (piece.x-1>= 0 && piece.y+2 < BOARDHEIGHT) available.push_back({-1,2});
			if (piece.x-2>= 0 && piece.y+1 < BOARDHEIGHT) available.push_back({-2,1});
			if (piece.x-2>= 0 && piece.y-1 >=0) available.push_back({-2,-1});
			if (piece.x-1>= 0 && piece.y-2 >=0) available.push_back({-1,-2});
			if (piece.x+1< BOARDWIDTH && piece.y-2 >=0) available.push_back({1,-2});
		}

		if (piece.type =="rook"){
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y] == empty) available.push_back({i,0});
				else if (piece.x +i < BOARDWIDTH){
					available.push_back({i,0});
					break;
				}
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y] == empty) available.push_back({-i,0});
				else if (piece.x -i >= 0){
					available.push_back({-i,0});
					break;
				}
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y+i] == empty) available.push_back({0,i});
				else if (piece.y +i < BOARDHEIGHT){
					available.push_back({0,i});
					break;
				}
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y-i] == empty) available.push_back({0,-i});
				else if (piece.y -i >= 0){
					available.push_back({0,-i});
					break;
				}
				else break;
			}
		}

		if (piece.type =="bishop"){
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y+i] == empty) available.push_back({i,i});
				else if (piece.x +i < BOARDWIDTH && piece.y+i < BOARDHEIGHT){
					available.push_back({i,i});
					break;
				}	
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y+i] == empty) available.push_back({i,-i});
				else if (piece.x +i < BOARDWIDTH && piece.y-i >=0){
					available.push_back({i,-i});
					break;
				}
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y-i] == empty) available.push_back({-i,-i});
				else if (piece.x -i >= 0 && piece.y-i >= 0){
					available.push_back({-i,-i});
					break;
				}
				else break;
			}
		}

		if (piece.type =="Queen"){
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y] == empty) available.push_back({i,0});
				else if (piece.x +i < BOARDWIDTH){
					available.push_back({i,0});
					break;
				}
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y] == "000000") available.push_back({-i,0});
				else if (piece.x -i >= 0){
					available.push_back({-i,0});
					break;
				}
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y+i] == "000000") available.push_back({0,i});
				else if (piece.y +i < BOARDHEIGHT){
					available.push_back({0,i});
					break;
				}
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x][piece.y-i] == "000000") available.push_back({0,-i});
				else if (piece.y -i >= 0){
					available.push_back({0,-i});
					break;
				}
				else break;
			}

			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y+i] == "000000") available.push_back({i,i});
				else if (piece.x +i < BOARDWIDTH && piece.y+i < BOARDHEIGHT){
					available.push_back({i,i});
					break;
				}	
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y+i] == "000000") available.push_back({-i, i});

				else if (piece.x -i >=0 && piece.y+i < BOARDHEIGHT){
					available.push_back({-i,i});
					break;
				}
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x+i][piece.y-i] == "000000") available.push_back({i,-i});
				else if (piece.x +i < BOARDWIDTH && piece.y-i >=0){
					available.push_back({i,-i});
					break;
				}
				else break;
			}
			for (int i = 1; i<=8; i++){
				if (boardKey[piece.x-i][piece.y-i] == "000000") available.push_back({-i,-i});
				else if (piece.x -i >= 0 && piece.y-i >= 0){
					available.push_back({-i,-i});
					break;
				}
				else break;
			}
		}
		return available;
	}

// NOTE: needs to get refactored so validMove and validAttack aren't repeating the same lines of code. 
	void showMoves(int x, int y){
		std::string key = boardKey[x][y];
		Piece activePiece = activePieces.find(key)->second;
		std::vector<std::array<int,2>> available = availableMoves(activePiece);
		for (std::array<int,2> i: available){
			if (boardKey[x+i[0]][y+i[1]] == "000000"){
				drawCursorBoxes(activePiece.x, activePiece.y, i, ORANGE);
			}
		}
		std::vector<std::array<int,2>> availAttack = availableAttack(activePiece);
		std:: string findkey;
		for (std::array<int,2> i: availAttack){
			if (boardKey[x+i[0]][y+i[1]] != "000000"){
				findkey =activePieces.find(boardKey[x+i[0]][y+i[1]])->second.player.color;
				if (activePieces.find(boardKey[x+i[0]][y+i[1]])->second.player.color !=activePiece.player.color){
					drawCursorBoxes(activePiece.x, activePiece.y, i, PINK);
				}
			}
		}
	}
	
	bool isValidMove(std::string key, int tx, int ty){
		Piece activePiece = activePieces.find(key)->second;
		std::vector<std::array<int,2>> available = availableMoves(activePiece);
		for (std::array<int,2> i: available){
			if (activePiece.x+i[0]== tx && activePiece.y+i[1]== ty && boardKey[tx][ty] == "000000"){
				return true;
			}
		}
		return false;
	}

	bool isValidAttack(Piece selectedPiece, Piece targetPiece, int tx, int ty){
		std::vector<std::array<int,2>> availAttack = availableAttack(selectedPiece);
		int spx = selectedPiece.x;
		int spy = selectedPiece.y;
		for (std::array<int,2> i: availAttack){
			if (tx == spx+i[0] && ty==spy+i[1] && selectedPiece.player.color!= targetPiece.player.color){
				const char* c_key = "VALID ATTACK";
				DrawText(c_key, 500, 10, 30, WHITE);
				return true;
			}
	}
	return false;
	}

	void attack(Piece selectedPiece, Piece targetPiece, int tx, int ty, int sx, int sy){
		boardKey[tx][ty] = selectedPiece.key;
		boardKey[sx][sy] = empty;
		boardKey[targetPiece.x][targetPiece.y] = selectedPiece.key;
		selectedPiece.x = tx;
		selectedPiece.y = ty;
		std::erase(keys, targetPiece.key);
////		boardKey[selectedPiece.x][selectedPiece.y] = "000000";
		capturedPieces.emplace(targetPiece.key, targetPiece);
		activePieces.erase(targetPiece.key);
}

	void addPieceToBoard(Piece* ipiece){

		std::string key_str = ipiece -> key;
		boardKey[ipiece->x][ipiece->y] = ipiece->key;
		//*activePieces[ipiece->key] = &ipiece;
		activePieces.emplace(key_str, *ipiece);
		keys.push_back(ipiece->key);
		// TODO: add class function here based on board. 
	}

	void printActiveCapturedPieces(){
		
		std::cout << "Number of active pieces: " << activePieces.size() << " ; ---> ";
		std::vector<std::string> keyList;
		for (auto& key: activePieces){
			keyList.push_back(key.first);
		}
		for (int i=0; i<keyList.size(); i++){ 
			std::cout << keyList[i] << ", ";
		}
		std::cout << "\n";
		std::cout << "Keys size: " << keys.size() << "\n";
		std::cout << "Number of captured pieces: " << capturedPieces.size() << " ; ---> ";
		std::vector<std::string> capturedList;
		for (auto& key: capturedPieces){
			capturedList.push_back(key.first);
		}
		for (int i=0; i<capturedList.size(); i++){ 
			std::cout << capturedList[i] << ", ";
		}
		std::cout << "\n";}
	

	/// print reverse so that array format x, y works and board is perspective from White;
		/// NOTE: this numbering system will have to be different than the actual numbering system displayed in game; starts at 1 not at zero like arrays do. 
	void printBoard(){
		std::cout << "\n";
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
	Piece selectedPiece, targetPiece;
	std::string selectedKey;
	std::string key;
	bool isPieceSelected ;
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
		if (isPieceSelected) color = RED;
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

		if (IsKeyDown(KEY_SPACE) && delta > debounce && key != "000000" && !isPieceSelected && !board->isValidAttack(selectedPiece, targetPiece, x, y)){
			isPieceSelected = true;
			lastKeyPress = nowDouble;
			selectedX = x; 
			selectedY = y;
			selectedPiece = board->activePieces[key];
			selectedKey = selectedPiece.key;
		}
		
		now = std::chrono::system_clock::now();
		nowDouble = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		debounce = 500;
		delta = nowDouble - lastKeyPress;

		if (IsKeyDown(KEY_SPACE) && delta > debounce && isPieceSelected && board->isValidMove(selectedKey,x ,y)){
			isPieceSelected = !isPieceSelected;
			lastKeyPress = nowDouble;
			board->boardKey[selectedX][selectedY] = "000000";
			board->boardKey[x][y] = selectedKey;
			board->activePieces[selectedKey].x = x;
			board->activePieces[selectedKey].y = y;
			std::string skey = selectedKey;
			const char* c_key = skey.c_str();
			DrawText(c_key, 250, 10, 30, WHITE);		
			turnCounter ++;
		}
		
		now = std::chrono::system_clock::now();
		nowDouble = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		debounce = 500;
		delta = nowDouble - lastKeyPress;
		int tx = x;
		int ty = y;
		int sx = selectedPiece.x;
		int sy = selectedPiece.y;
		std::cout << "TEST: Selected X: "<< sx << ";  Y: " << sy << "\n";
		// NOTE: atack update here
		if (IsKeyDown(KEY_SPACE) &&  
			delta > debounce && isPieceSelected && board->isValidAttack(selectedPiece, targetPiece, tx, ty)){
			//board->boardKey[selectedX][selectedY] = "000000";
			//board->boardKey[x][y] = selectedKey;
			//board->activePieces[selectedKey].x = x;
			//board->activePieces[selectedKey].y = y;
//			std::cout << "TEST: Selected X: "<< sx << ";  Y: " << sy << "\n";
//			std::cout << "TEST: Target Piece: " << targetPiece.key << "\n";
//			std::cout << "TEST: Selected Piece: " << selectedPiece.key << "\n";
			board->attack(selectedPiece, targetPiece, x, y, sx, sy);
			isPieceSelected = !isPieceSelected;
			selectedX = x;
			selectedY = y;
		}
		key = board->boardKey[x][y];

// TODO: need to change escape so that it remains on the current cursor;
// TODO: need to check that isValidMove and isValidAttack dont have overlap.
		// FIX: need to add is !isValidAttack (not) 
		//
		if (IsKeyDown(KEY_SPACE) && delta > debounce && isPieceSelected && !board->isValidMove(selectedKey,x ,y)){
			isPieceSelected = !isPieceSelected;
			lastKeyPress = nowDouble;
		}
		if ( IsKeyDown(KEY_ESCAPE) && delta > debounce && isPieceSelected ){
			isPieceSelected = !isPieceSelected;
			lastKeyPress = nowDouble;
		}

		if (isPieceSelected){
			// TODO: need to have this persist for the last one since last clicked
			std::string skey = "Pressed";
			const char* c_key = skey.c_str();
			DrawText(c_key, 150, 10, 30, WHITE);
			board->showMoves(selectedX, selectedY);

			key = board->boardKey[x][y];
			if (board->activePieces.contains(key)){
				targetPiece = board->activePieces.find(key)->second;
				if (board->isValidAttack(selectedPiece, targetPiece, x, y)){
					const char* c_key = "VALID ATTACK";
					DrawText(c_key, 500, 10, 30, WHITE);
				}
			 }
		}
		std::cout << "THIS PART RUNS" << "\n";


		if (board->capturedPieces.contains(key)){
			std::cout << "Target Piece: " << targetPiece.key << "\n";
			std::cout << "Captured Key: " << board->capturedPieces.find(key)->second.key << "\n";
		}
			std::cout << "Target Piece: " << targetPiece.key << "\n";
//		if (board != nullptr){
//			delete board;
//			board = nullptr;
//		}
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
		if (pieceKey != empty){
	//		Piece piece = board.activePieces.find(pieceKey$ 
//			std::cout<< "cursor piece: X: " << curs.x << ";  Y: " << curs.y  << "\n";
			Piece piece = board.activePieces.find(pieceKey)->second;
			assignPiece(piece);
		
			// TODO: need to fix this for 2 digits or bigger boards;
			const char* c_name = name.c_str();
			DrawText(c_name, x+50, y+10, 30, WHITE);
			std::string key = board.boardKey[curs.x][curs.y];
			const char* c_key = key.c_str();
			DrawText(c_key, x+150, y+10, 30, WHITE);
			std::string s_pos_x = std::to_string(curs.x);
			const char* c_pos_x = s_pos_x.c_str();
			std:: string s_pos_y = std::to_string(curs.y);
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
		std::string s_pos_x = std::to_string(pos_x);
		const char* c_pos_x = s_pos_x.c_str();
		std::string s_pos_y = std::to_string(pos_y);
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

	//NOTE: shows turn location cursor location in toooltip
	void displayTurnCounter(){
		std::string tc_string = std::to_string(turnCounter); 
		const char* tc_char = tc_string.c_str();
		DrawText(tc_char, 100, 10, 30, WHITE);		
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

		// TODO: convert this to a function
		const std::vector<std::string> abc = {
			"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
			"AA","AB","AC","AD","AE","AF","AG","AH","AI","AJ","AK","AL","AM","AN","AO","AP","AQ","AR","AS","AT","AU","AV","AW","AX","AY","AZ",
			"BA","BB","BC","BD","BE","BF","BG","BH","BI","BJ","BK","BL","BM","BN","BO","BP","BQ","BR","BS","BT","BU","BV","BW","BX","BY","BZ",
			"CA","CB","CC","CD","CE","CF","CG","CH","CI","CJ","CK","CL","CM","CN","CO","CP","CQ","CR","CS","CT","CU","CV","CW","CX","CY","CZ",
			"DA","DB","DC","DD","DE","DF","DG","DH","DI","DJ","DK","DL","DM","DN","DO","DP","DQ","DR","DS","DT","DU","DV","DW","DX","DY","DZ",
			"EA","EB","EC","ED","EE","EF","EG","EH","EI","EJ","EK","EL","EM","EN","EO","EP","EQ","ER","ES","ET","EU","EV","EW","EX","EY","EZ",
			"FA","FB","FC","FD","FE","FF","FG","FH","FI","FJ","FK","FL","FM","FN","FO","FP","FQ","FR","FS","FT","FU","FV","FW","FX","FY","FZ",
			"GA","GB","GC","GD","GE","GF","GG","GH","GI","GJ","GK","GL","GM","GN","GO","GP","GQ","GR","GS","GT","GU","GV","GW","GX","GY","GZ",
			"HA","HB","HC","HD","HE","HF","HG","HH","HI","HJ","HK","HL","HM","HN","HO","HP","HQ","HR","HS","HT","HU","HV","HW","HX","HY","HZ",
			"IA","IB","IC","ID","IE","IF","IG","IH","II","IJ","IK","IL","IM","IN","IO","IP","IQ","IR","IS","IT","IU","IV","IW","IX","IY","IZ",
			"JA","JB","JC","JD","JE","JF","JG","JH","JI","JJ","JK","JL","JM","JN","JO","JP","JQ","JR","JS","JT","JU","JV","JW","JX","JY","JZ",
			"KA","KB","KC","KD","KE","KF","KG","KH","KI","KJ","KK","KL","KM","KN","KO","KP","KQ","KR","KS","KT","KU","KV","KW","KX","KY","KZ",
			"LA","LB","LC","LD","LE","LF","LG","LH","LI","LJ","LK","LL","LM","LN","LO","LP","LQ","LR","LS","LT","LU","LV","LW","LX","LY","LZ",
			"MA","MB","MC","MD","ME","MF","MG","MH","MI","MJ","MK","ML","MM","MN","MO","MP","MQ","MR","MS","MT","MU","MV","MW","MX","MY","MZ",
			"NA","NB","NC","ND","NE","NF","NG","NH","NI","NJ","NK","NL","NM","NN","NO","NP","NQ","NR","NS","NT","NU","NV","NW","NX","NY","NZ",
			"OA","OB","OC","OD","OE","OF","OG","OH","OI","OJ","OK","OL","OM","ON","OO","OP","OQ","OR","OS","OT","OU","OV","OW","OX","OY","OZ",
			"PA","PB","PC","PD","PE","PF","PG","PH","PI","PJ","PK","PL","PM","PN","PO","PP","PQ","PR","PS","PT","PU","PV","PW","PX","PY","PZ",
			"QA","QB","QC","QD","QE","QF","QG","QH","QI","QJ","QK","QL","QM","QN","QO","QP","QQ","QR","QS","QT","QU","QV","QW","QX","QY","QZ",
			"RA","RB","RC","RD","RE","RF","RG","RH","RI","RJ","RK","RL","RM","RN","RO","RP","RQ","RR","RS","RT","RU","RV","RW","RX","RY","RZ",
			"SA","SB","SC","SD","SE","SF","SG","SH","SI","SJ","SK","SL","SM","SN","SO","SP","SQ","SR","SS","ST","SU","SV","SW","SX","SY","SZ",
			"TA","TB","TC","TD","TE","TF","TG","TH","TI","TJ","TK","TL","TM","TN","TO","TP","TQ","TR","TS","TT","TU","TV","TW","TX","TY","TZ",
			"UA","UB","UC","UD","UE","UF","UG","UH","UI","UJ","UK","UL","UM","UN","UO","UP","UQ","UR","US","UT","UU","UV","UW","UX","UY","UZ",
			"VA","VB","VC","VD","VE","VF","VG","VH","VI","VJ","VK","VL","VM","VN","VO","VP","VQ","VR","VS","VT","VU","VV","VW","VX","VY","VZ",
			"WA","WB","WC","WD","WE","WF","WG","WH","WI","WJ","WK","WL","WM","WN","WO","WP","WQ","WR","WS","WT","WU","WV","WW","WX","WY","WZ",
			"XA","XB","XC","XD","XE","XF","XG","XH","XI","XJ","XK","XL","XM","XN","XO","XP","XQ","XR","XS","XT","XU","XV","XW","XX","XY","XZ",
			"YA","YB","YC","YD","YE","YF","YG","YH","YI","YJ","YK","YL","YM","YN","YO","YP","YQ","YR","YS","YT","YU","YV","YW","YX","YY","YZ",
			"ZA","ZB","ZC","ZD","ZE","ZF","ZG","ZH","ZI","ZJ","ZK","ZL","ZM","ZN","ZO","ZP","ZQ","ZR","ZS","ZT","ZU","ZV","ZW","ZX","ZY","ZZ",
			"AAA","AAB","AAC","AAD","AAE","AAF","AAG","AAH","AAI","AAJ","AAK","AAL","AAM","AAN","AAO","AAP","AAQ","AAR","AAS","AAT","AAU","AAV","AAW","AAX","AAY","AAZ",
			"ABA","ABB","ABC","ABD","ABE","ABF","ABG","ABH","ABI","ABJ","ABK","ABL","ABM","ABN","ABO","ABP","ABQ","ABR","ABS","ABT","ABU","ABV","ABW","ABX","ABY","ABZ",
			"ACA","ACB","ACC","ACD","ACE","ACF","ACG","ACH","ACI","ACJ","ACK","ACL","ACM","ACN","ACO","ACP","ACQ","ACR","ACS","ACT","ACU","ACV","ACW","ACX","ACY","ACZ",
			"ADA","ADB","ADC","ADD","ADE","ADF","ADG","ADH","ADI","ADJ","ADK","ADL","ADM","ADN","ADO","ADP","ADQ","ADR","ADS","ADT","ADU","ADV","ADW","ADX","ADY","ADZ"
		};

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

		// letters on rows
		int letterHeight = 20;	
		for (int i = 0; i<BOARDWIDTH; i++){
			const char* x_char = abc[i].c_str();
			int py = FRAME_BUFFER + TILE * BOARDHEIGHT + letterHeight;
			int px = FRAME_BUFFER + TILE * i + TILE/4; // TODO: Need to find out the "halfway" of the character string;
			DrawText(x_char, px, py, letterHeight, WHITE);
		}

		for (int i = 1; i<=BOARDHEIGHT; i++){
			std::string y_str = std::to_string(i);
			const char* y_char = y_str.c_str();
			int px = FRAME_BUFFER / 2;
			int py = FRAME_BUFFER + BOARDHEIGHT*TILE - TILE*i + TILE/4;
			DrawText(y_char, px, py, letterHeight, WHITE);
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
		Rectangle attRect = buildAnim(textures.attacking1, 0, 0);

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
		board.printActiveCapturedPieces();

		myCursor.update(&board);

		tt.displayPiece(board, firstPawn);
		tt.displayCursor(board, myCursor);
		tt.displayTurnCounter();

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
		UnloadTexture(textures.attacking1);
		UnloadTexture(textures.attacking2);
		UnloadTexture(textures.attacking3);

		UnloadTexture(textures.Ore);
	}
		

}

