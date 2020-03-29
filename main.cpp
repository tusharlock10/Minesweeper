/*

    
___  ____                                                   
|  \/  (_)                                                  
| .  . |_ _ __   ___  _____      _____  ___ _ __   ___ _ __ 
| |\/| | | '_ \ / _ \/ __\ \ /\ / / _ \/ _ \ '_ \ / _ \ '__|
| |  | | | | | |  __/\__ \\ V  V /  __/  __/ |_) |  __/ |   
\_|  |_/_|_| |_|\___||___/ \_/\_/ \___|\___| .__/ \___|_|   
                                           | |              
                                           |_|              
    - by Tushar Jain

*/

#include <iostream>
#include <graphics.h>
#include <string>
#include <sstream>
#include <ctime> 

#define VERTICAL_NUMBER 12
#define HORIZONTAL_NUMBER 12
#define GRID_WIDTH 800
#define GRID_HEIGHT 800
#define WINDOW_WIDTH 810
#define WINDOW_HEIGHT 900
#define MINE_SPAWN_PERCENT 20 // means only 10% block will have mines in it (probably)
#define INNER_BLOCK_PADDING 3;


int BLOCK_HEIGHT = GRID_HEIGHT/VERTICAL_NUMBER;
int BLOCK_WIDTH = GRID_WIDTH/HORIZONTAL_NUMBER;
int MINE_RADIUS = BLOCK_HEIGHT/2-6;

using namespace std;

void initializeGraphics(){
    initwindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

int getRandomNumber(){
    // generates random number from 0-100
    return rand()%100;
}

POINT handleMouseInput(){
    POINT mouse_coord;
    int x, y; 
    getmouseclick(WM_LBUTTONDOWN,x,y);
    if (x!=-1){
        // means we have a mouse click
        mouse_coord.x = mousex();
        mouse_coord.y = mousey();
    }
    else{
        mouse_coord.x=-1;
        mouse_coord.y=-1;
    }
    return mouse_coord;
}

class Grid {
    private:
        int gridArray[100][100];
        int gameOver = 0; // means game is not obver initially

        void drawBlock(int x, int y){
            // draws a rectangle/square shape at the appropriate x,y indices
            // x, y are not coordinates, but indices of gridArray

            // pointing out the corners of this block
            int x_topLeft = x*BLOCK_WIDTH;  // x coord of topLeft point of the block
            int y_topLeft = y*BLOCK_HEIGHT; // y coord of topLeft point of the block

            int x_topRight = (x+1)*BLOCK_WIDTH;
            int y_topRight = y*BLOCK_HEIGHT;

            int x_bottomLeft = x*BLOCK_WIDTH;
            int y_bottomLeft = (y+1)*BLOCK_HEIGHT;

            int x_bottomRight = (x+1)*BLOCK_WIDTH;
            int y_bottomRight = (y+1)*BLOCK_HEIGHT;

            setcolor(DARKGRAY);
            setlinestyle(SOLID_LINE, 1, 2);

            line(x_topLeft, y_topLeft, x_topRight, y_topRight);  // line for top
            line(x_bottomLeft, y_bottomLeft, x_bottomRight, y_bottomRight);  // line for bottom
            line(x_topRight, y_topRight, x_bottomRight, y_bottomRight);  // line for right
            line(x_topLeft, y_topLeft, x_bottomLeft, y_bottomLeft);  // line for left
        }

        void drawUnplayedBlock(int x, int y){

            int x_topLeft = x*BLOCK_WIDTH  + INNER_BLOCK_PADDING;
            int y_topLeft = y*BLOCK_HEIGHT + INNER_BLOCK_PADDING;

            int x_topRight = (x+1)*BLOCK_WIDTH - INNER_BLOCK_PADDING;
            int y_topRight = y*BLOCK_HEIGHT + INNER_BLOCK_PADDING;

            int x_bottomLeft = x*BLOCK_WIDTH + INNER_BLOCK_PADDING;
            int y_bottomLeft = (y+1)*BLOCK_HEIGHT - INNER_BLOCK_PADDING;

            int x_bottomRight = (x+1)*BLOCK_WIDTH - INNER_BLOCK_PADDING;
            int y_bottomRight = (y+1)*BLOCK_HEIGHT - INNER_BLOCK_PADDING;

            setcolor(LIGHTGRAY);
            setlinestyle(SOLID_LINE, 1, 2);
            setfillstyle(SOLID_FILL,LIGHTGRAY);
            line(x_topLeft, y_topLeft, x_topRight, y_topRight);  // line for top
            line(x_bottomLeft, y_bottomLeft, x_bottomRight, y_bottomRight);  // line for bottom
            line(x_topRight, y_topRight, x_bottomRight, y_bottomRight);  // line for right
            line(x_topLeft, y_topLeft, x_bottomLeft, y_bottomLeft);  // line for left

            floodfill(x_topLeft+5, y_topLeft+5, LIGHTGRAY);
        }

        void drawPlayedBlock(int x, int y){

            int x_topLeft = x*BLOCK_WIDTH  + INNER_BLOCK_PADDING;
            int y_topLeft = y*BLOCK_HEIGHT + INNER_BLOCK_PADDING;

            int x_topRight = (x+1)*BLOCK_WIDTH - INNER_BLOCK_PADDING;
            int y_topRight = y*BLOCK_HEIGHT + INNER_BLOCK_PADDING;

            int x_bottomLeft = x*BLOCK_WIDTH + INNER_BLOCK_PADDING;
            int y_bottomLeft = (y+1)*BLOCK_HEIGHT - INNER_BLOCK_PADDING;

            int x_bottomRight = (x+1)*BLOCK_WIDTH - INNER_BLOCK_PADDING;
            int y_bottomRight = (y+1)*BLOCK_HEIGHT - INNER_BLOCK_PADDING;

            setcolor(BLACK);
            setlinestyle(SOLID_LINE, 1, 3);
            setfillstyle(SOLID_FILL, BLACK);
            line(x_topLeft, y_topLeft, x_topRight, y_topRight);  // line for top
            line(x_bottomLeft, y_bottomLeft, x_bottomRight, y_bottomRight);  // line for bottom
            line(x_topRight, y_topRight, x_bottomRight, y_bottomRight);  // line for right
            line(x_topLeft, y_topLeft, x_bottomLeft, y_bottomLeft);  // line for left

            floodfill(x_topLeft+5, y_topLeft+5, BLACK);
        }

        int placeMine(){
            // a function which tells randomly if a mine should be placed or not
            int num = getRandomNumber();
            if (num<MINE_SPAWN_PERCENT){
                // then we will spawn a mine
                return 2;
            }
            else{
                // we will leave the block empty and it will be safe to play
                return 0;
            }
        }

        void checkWin(){
            // player wins when there is no empty place left
            // and he didn't touch a mine

            for(int y=0; y<VERTICAL_NUMBER; y++){
                for (int x=0; x<HORIZONTAL_NUMBER; x++){
                    if (gridArray[x][y]==0){
                        // means player has not won as there is a block to play
                        return;
                    }
                }
            }

            gameOver=true;
            displayYouWon();
        }

        void drawMine(int x, int y){
            // draws a mine at the specified indices
            // x, y are indices here not coordinates

            int x_center = x*BLOCK_WIDTH  + (BLOCK_WIDTH/2);  // x coord of topLeft point of the block
            int y_center = y*BLOCK_HEIGHT + (BLOCK_HEIGHT/2); // y coord of topLeft point of the block

            drawPlayedBlock(x, y);
            setcolor(RED);
            setlinestyle(SOLID_LINE, 1, 4);
            circle(x_center, y_center, MINE_RADIUS);
        }

        void displayGameOver(){
            int x_start = 10;
            int y_start = WINDOW_HEIGHT-(WINDOW_HEIGHT-GRID_HEIGHT)/2 -20;

            setcolor(RED);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
            outtextxy(x_start, y_start, "GAME OVER");
        }

        void displayYouWon(){
            int x_start = 10;
            int y_start = WINDOW_HEIGHT-(WINDOW_HEIGHT-GRID_HEIGHT)/2 -20;

            setcolor(GREEN);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
            outtextxy(x_start, y_start, "YOU WON");
        }

        void clearAreaHelper(int x, int y){
            // give coordinates to it, this fucntion will clear that block
            if (gridArray[x][y]==2){
                drawMine(x, y);
            }
            else{
                gridArray[x][y] = 1;
                drawPlayedBlock(x, y);
            }

        }

        void clearSurroundingArea(int x, int y){
            clearAreaHelper(x+1, y+1);
            clearAreaHelper(x-1, y-1);
            clearAreaHelper(x+1, y-1);
            clearAreaHelper(x-1, y+1);
            clearAreaHelper(x+1, y);
            clearAreaHelper(x-1, y);
            clearAreaHelper(x, y-1);
            clearAreaHelper(x, y+1);

        }

    public:
        void initializeGrid(){
            for(int y=0; y<VERTICAL_NUMBER; y++){
                for (int x=0; x<HORIZONTAL_NUMBER; x++){
                    // 0 means unplayed blank block
                    // 1 means played safe block
                    // 2 means unplayed block with mine -> user touchs this block, he's dead
                    gridArray[x][y] = placeMine();
                }
            }
            // after the grid is initialized, randomly spawn mines
        }

        void drawGrid(){
            for(int y=0; y<VERTICAL_NUMBER; y++){
                for (int x=0; x<HORIZONTAL_NUMBER; x++){
                    drawBlock(x, y);
                    drawUnplayedBlock(x, y);
                }
            }
        }

        POINT convertCoordinatesToIndices(POINT mouse_coord){
            POINT indices;
            indices.x = mouse_coord.x/BLOCK_WIDTH;
            indices.y = mouse_coord.y/BLOCK_HEIGHT;
            return indices;
        }

        void handleUserClick(int x, int y){
            if (gameOver){
                return;
            }
            if (gridArray[x][y]==2){
                gameOver=true;
                displayGameOver();
                // revailing all mines as game is now over
                for(int j=0; j<VERTICAL_NUMBER; j++){
                    for (int i=0; i<HORIZONTAL_NUMBER; i++){
                        if (gridArray[i][j]==2){
                            drawMine(i, j);
                        }

                    }
                }
            }
            else{
                gridArray[x][y] = 1;
                drawPlayedBlock(x, y);
                clearSurroundingArea(x, y);
                checkWin();
            }
        }


};

int main(){
    srand((unsigned)time(0));
    POINT mouse_coord, indices;

    initializeGraphics();
    // make an object for the grid;
    Grid GridObject;
    GridObject.initializeGrid();
    GridObject.drawGrid();


    while(1){
        delay(100);
        mouse_coord = handleMouseInput();
        if (mouse_coord.x==-1){
            // means no input detected, we can skip this loop
            continue;
        }

        indices = GridObject.convertCoordinatesToIndices(mouse_coord);
        // handle user click
        GridObject.handleUserClick(indices.x, indices.y);
    }
    
    getch();
    return 0;
}

// STRATEGY -> 
// 0 -> unplayed plank block
// 1 -> played empty block
// 2 -> unplayed block with a mine 


// Players clicks on a block
// block can be 0, 1, 2
// if block is 1, ignore the click as it is already a played block
// if block is 2, uncover all the mines, game is over
// if block is 0, clear the block and the areas surrounding it
// i.e. top, bottom, left, right and diagnally