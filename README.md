# CHESS-based Resource Strategy Game
Chess as an RTS
Multidimensional Chess - resource vs combat; 
OBJECTIVE: Capture Enemy King

## Pieces:
- Kings and Knights move identically
- Pawns can move backwards and side to side; no longer have first move; can still only attack diagonally (but now also backward)
- Bishops are limted to maximum 6 squares for their movement
- Queens are limted to maximum 8 squares for their movement
- Rooks can move 4 but attack 8; only piece that does not move when attacking
- Only limited to 1 Queen at a time
- Total 16 pc limit

## Tech Tree
- Pawns >> +2 ore +1 turn >> Knight
- Knights >> +1 ore +1 turn >> Bishop
- Pawns >> +1 ore +1 stone + 1 turn >> Bishop
- 2 Pawns >> +2 stone +1 turn >> Rook
- 1 Rook + Bishop >> + 2 ore + 2 turn >> Queen
- 1 Neutral Queen + pawn in the game
- 1 Pawn every 4 turns the queen and king are together;


## Bugg List
- [x] selector squares cant overcast and delte units
- [X] if you selecte a non-eligible square, needs to come off selection
- [X] queens, rooks, bishops cant skip pieces
- [ ] black pawn doesnt show graphic
- [ ] pieces once taken their position doesnt move to the new location;
- [ ] bishop on start (8,8) kicks error 
        - might be a first select on a black piece 
        - or might be an issue wiht the bishop piece itself (check moveAvailble vs attackAvailable cross over)



## PROJECT ROADMAP:
### Gameplay (non-game mechanics)
- [ ] change to correct GH
- [ ] moving camera pan (up and down) for larger maps
- [X] create selector square to show square/piece selected
- [x] selector square to move below pieces 
- [x] selector square shows possible moves (red for attack squares)
    - build this only for selected piece
- [x] selector square mvoes pieces to possible
- [X] create an info box showing piece info
- [ ] create an info box showing node information 
- [x] create an A>Z and a ## 1-20 grid pattern (like standard chess grid)
- [x] create a turn counter to count 
- [ ] create a refactor and make a function to create all of the variables 
    - var: int for number of objects created, type, color/team, 
    - group all textures (change to typedef to separate the different types)
- [ ] create a "next turn" button
- [ ] show "multi turn pathing" 
- [ ] show settings JSON 
- [ ] 

### Game Mechanics 
- [x] create matrix for board
- [x] add ores to board
- [x] add movement to pawn
- [x] add movement to knight
- [ ] create an ability for pawns to hold items. 
- [ ] add pawn upgrade to knight
    - [ ] add a 1turn delay to upgrade
- [ ] add resources randomly in field
- [ ] create timer that shows queen <> king pair
- [ ] create pawn spawn
- [ ] create non-movable squares
- [ ] create a graphic that shows "mining" by pawns;
- [ ] add a way for pieces to attack each other;
- [ ] add an animation for attacking;
- [ ] add animation for mining
- [ ] add maximum move counter (increase)


#### Opponent / AI Mechanics
- [ ] create movement map
- [ ] weight chart for pieces
- [ ] pathing map for each piece


### Controls (input)
- [ ] make mouse movement
- [ ] tab select pieces
- [ ] 


## General Strategy
#### Opening
- Rush to find ore with the Queen 
- Avoid conflict
- Spawn pawns
- upgrade pawns

#### Mid Game
- Try to position against enemy
- send pawns for ore and stone
- force push combination of pawns and other pieces
- try to find stone


#### Late Game
- Start trading pieces
- 


## Game Modes:
- Timed vs Turn based match
- Short Map - 10 ores, 2 stone; 
- Long Map - Lots of resources + start with Knight and Queen neutral queen 
- Campaign - 
     - easy campaign farm (start with pawns)
     - add knights build kingdom 
     - create timed turns;
     - scenario specific turns
        - against only bishops
        - against only knights
        - against queen
    - conquer full kingdoms
    - force choose between Bishops and Knights in campaign 
    - upgrade knights (include forward move)
    - upgrade bishops (more vision)


------- 

## FUTURE ROADMAP
- [ ] create "jumpable" or move cost squares - this may be removable (adds too much complexity - shelved for v2)
- [ ] create a "multijump" queue. i.e target a pawn 8 squares away to move, auto queue its next move (and show the player);
- [ ] multiple different civilizations
    - English Chess - standard
    - Jangi - Chinese Chess
    - Chaturaji - https://en.wikipedia.org/wiki/Chaturaji



## GOOD IDEA FAIRY
 - [ ] create a stationary tower that can attack 
