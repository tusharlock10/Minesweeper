#include <iostream>
#include <graphics.h>
#include <string>
#include <sstream>

#define VERTICAL_NUMBER 10
#define HORIZONTAL_NUMBER 10
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define MINE_SPAWN_PERCENT 10 // means only 10% block will have mines in it (probably)
#define MINE_RADIUS 30


int BLOCK_HEIGHT = WINDOW_HEIGHT/VERTICAL_NUMBER;
int BLOCK_WIDTH = WINDOW_WIDTH/HORIZONTAL_NUMBER;

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
        int gridArray[10][10];

        void drawBlock(int x, int y){
            // draws a rectangle/square shape at the appropriate x,y indices
            // x, y are not coordinates, but indices of gridArray

            // pointing out the corners of this block
            int x_topLeft = x*BLOCK_WIDTH;  // x coord of topLeft point of the block
            int y_topLeft = y*BLOCK_HEIGHT; // y coord of topLeft point of the block


            // drawing the rectangle in dark gray border
            setcolor(DARKGRAY);
            setlinestyle(SOLID_LINE, 1, 3);
            rectangle(x_topLeft, y_topLeft, BLOCK_WIDTH, BLOCK_HEIGHT);
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

        void drawMine(int x, int y){
            // draws a mine at the specified indices
            // x, y are indices here not coordinates

            int x_center = x*BLOCK_WIDTH  + (BLOCK_WIDTH/2);  // x coord of topLeft point of the block
            int y_center = y*BLOCK_HEIGHT + (BLOCK_HEIGHT/2); // y coord of topLeft point of the block

            setcolor(RED);
            setlinestyle(SOLID_LINE, 1, 4);
            circle(x_center, y_center, MINE_RADIUS);
        }

    public:
        void initializeGrid(){
            for(int y=0; y<VERTICAL_NUMBER; y++){
                for (int x=0; x<HORIZONTAL_NUMBER; x++){
                    // 0 means unplayed blank block
                    // 1 means played safe block
                    // 2 means unplayed block with mine -> user touchs this block, he's dead
                    gridArray[x][y] = placeMine();
                    cout<<gridArray[x][y]<<" ";
                }
                cout<<endl;
            }
            // after the grid is initialized, randomly spawn mines
        }

        void drawGrid(){
            for(int y=0; y<VERTICAL_NUMBER; y++){
                for (int x=0; x<HORIZONTAL_NUMBER; x++){
                    if (gridArray[x][y]==1){
                        // means block is played
                    } else if(gridArray[x][y]==2){
                        // means its a mine
                        drawMine(x, y);
                    } else{
                        // else is an unplayed block
                        drawBlock(x, y);
                    }
                }
            }
        }

        POINT convertCoordinatesToIndices(POINT mouse_coord){
            POINT indices;
            indices.x = mouse_coord.x/BLOCK_WIDTH;
            indices.y = mouse_coord.y/BLOCK_HEIGHT;
            return indices;
        }


};

int main(){
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
        cout<<"Rnadom number : "<<rand();

    }
    
    getch();
    return 0;
}


// void drawBlock(int x, int y){
//             // draws a rectangle/square shape at the appropriate x,y indices
//             // x, y are not coordinates, but indices of gridArray

//             // pointing out the corners of this block
//             int x_topLeft = x*BLOCK_WIDTH;  // x coord of topLeft point of the block
//             int y_topLeft = y*BLOCK_HEIGHT; // y coord of topLeft point of the block

//             int x_topRight = (x+1)*BLOCK_WIDTH;
//             int y_topRight = y*BLOCK_HEIGHT;

//             int x_bottomLeft = x*BLOCK_WIDTH;
//             int y_bottomLeft = (y+1)*BLOCK_HEIGHT;

//             int x_bottomRight = (x+1)*BLOCK_WIDTH;
//             int y_bottomRight = (y+1)*BLOCK_HEIGHT;

//             line(x_topLeft, y_topLeft, x_topRight, y_topRight);  // line for top
//             line(x_bottomLeft, y_bottomLeft, x_bottomRight, y_bottomRight);  // line for bottom
//             line(x_topRight, y_topRight, x_bottomRight, y_bottomRight);  // line for right
//             line(x_topLeft, y_topLeft, x_bottomLeft, y_bottomLeft);  // line for left
//         }