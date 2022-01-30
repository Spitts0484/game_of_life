#include <cstdio>
#include <ncurses.h>
#include <random>
#include <unistd.h>
#include "board.h"

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Public Methods ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
Board::Board(int time_steps){
    this->time_steps = time_steps;
    this->init_board();

    // Initialize the NCurses screen
    initscr();
    this->run();
};

Board::~Board(){
    // Deconstruct the NCurses window
    endwin();
    printf("Destructor Called");
};

void Board::print(){
    char character;
    for(int i = 0; i<this->board_n_rows; i++){
        for(int j = 0; j < this->board_n_cols; j++){
            if(cells[this->current_board][i][j] == 0){
                character = ' ';
            } else {
                character = 'O';
            };
            mvaddch(i,j,character);
            //printf("%i ",cells[this->current_board][i][j]);
        };
        //printf("\n");
    };

    refresh();
};


///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Private Methods ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Board::run(){

    for(int i = 0; i < this->time_steps; i++){

        this->print();
        //getch();
        this->calc_next_board();

        // Switch the current board the updated other board
        if(this->current_board == 0){
            this->current_board =1;
        } else {
            this->current_board = 0;
        };

    usleep(150000); // delay the program so the user can see the screen

    };


};

void Board::init_board(){
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(0.0, 0.54);

    float dummy;

    for(int i = 0; i < this->board_n_rows; i++){
        for(int j = 0; j < this->board_n_cols; j++){
            dummy = distr(eng);
            if(dummy>0.5){
                this->cells[0][i][j] = 1;
            } else {
                this->cells[0][i][j] = 0;
            };
            
            this->cells[1][i][j] = this->cells[0][i][j];
        };
    };
};


void Board::calc_next_board(){
    int next_board; // Variable indicating which board is not the current board
    int sum; // Variable for indicating the state of surrounding cells
    int rows[3]; // rows over which the sum needs to be taken
    int cols[3]; // cols over which the sum nees to be taken

    if(this->current_board == 0){
        next_board = 1;
    } else {
        next_board = 0;
    };

    for(int i = 0; i < this->board_n_rows; i++){
        for(int j = 0; j < this->board_n_cols; j++){
            sum = 0;

            // figure out which positions communicate with the cell of interest
            if (i==0){
                rows[0] = this->board_n_rows-1; rows[1] = i; rows[2] = i+1;
                cols[1] = j;

                if (j == 0){
                    // case the cell is in the top left corner
                    cols[0] = this->board_n_cols-1; cols[2] = j+1;

                } else if ( j == this->board_n_cols-1){
                    // case the cell is in the top right corner
                    cols[0] = j-1; cols[2] = 0;

                } else {
                    // case the cell is in the top row
                    cols[0] = j-1; cols[2] = j+1;
                };
            } else if (j == 0) {
                // case the cell is on the left column
                rows[0] = i-1; rows[1] = i; rows[2] = i+1;
                cols[0] = this->board_n_cols-1; cols[1] = j; cols[2] = j+1;
            } else if (j == this->board_n_cols-1){
                // case the cell is on the right column
                rows[0] = i-1; rows[1] = i; rows[2] = i+1;
                cols[0] = j-1; cols[1] = j; cols[2] = 0;
            } else if (i == this->board_n_rows-1){
                rows[0] = i-1; rows[1] = i; rows[2] = 0;
                if (j == 0){
                    // case the cell is in the bottom left corner
                    cols[0] = this->board_n_cols-1; cols[1] = j; cols[2] = j+1;
                } else if (j == this->board_n_cols-1){
                    // case the cell is in the bottom right corner
                    cols[0] = j-1; cols[1] = j; cols[2] = 0;
                } else {
                    // case the cell is on the bottom row
                    cols[0] = j-1; cols[1] = j; cols[2] = j+1;
                };
            } else {
                // Case the cell is not on the edge
                rows[0] = i-1; rows[1] = i; rows[2] = i+1;
                cols[0] = j-1; cols[1] = j; cols[2] = j+1;
            };


            // Calculate the value of the cells surrounding the current cells
            for(int k=0; k<3; k++){
                for(int l=0; l<3; l++){
                    sum += this->cells[current_board][rows[k]][cols[l]];
                }
            };

            // Decide what the new value of the current cell should be based on
            // the sum
            if(this->cells[current_board][i][j] == 0){
                // what to do if the current cell is dead
                if (sum == 3){
                    this->cells[next_board][i][j] = 1;
                } else {
                    this->cells[next_board][i][j] = 0;
                };
            } else {
               // what to do if the current cell is alive
               // The algorithm counts itself as alive so the sum nees to be
               // reduced by 1
               sum -= 1;

               if ((sum == 2) || (sum == 3)){
                    this->cells[next_board][i][j] = 1;
                } else {
                    this->cells[next_board][i][j] = 0;
                };
            };

        };
    };

};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// Public Attributes ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//////////////////////////// Private Attributes ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
