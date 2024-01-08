#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include<limits>

using namespace std;

// TicTacToe table
extern char table[11][11];

// Variables
extern bool computer;
extern string firstPlayer;
extern string secondPlayer;
extern int firstPlayerResult; 
extern int secondPlayerResult;
extern int tieResult;
extern int positionForPlaying[9][2]; //fields on the board (matrix) where characters are placed, i.e. their grouped coordinates

// Functions
int chooseMode();

void startGame();
int theBestMoves();
int MiniMax(bool isMaximizing); // isMaximizing tells us whether it is the Maximizing cycle or the Minimizing cycle

bool checkTie();
bool checkValidation(int player, int option);
int checkWinner();

void restartTable();

void startingCountdown();
void pause();

void printTable();


#endif