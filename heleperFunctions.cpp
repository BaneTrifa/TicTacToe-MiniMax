#include "heleperFunctions.h"

// TicTacToe table
char table[11][11] ={   {' ',' ',' ','|',' ',' ',' ','|',' ',' ',' '},
                        {' ',' ',' ','|',' ',' ',' ','|',' ',' ',' '},
                        {' ',' ',' ','|',' ',' ',' ','|',' ',' ',' '},
                        {'-','-','-','-','-','-','-','-','-','-','-'},
                        {' ',' ',' ','|',' ',' ',' ','|',' ',' ',' '},
                        {' ',' ',' ','|',' ',' ',' ','|',' ',' ',' '},
                        {' ',' ',' ','|',' ',' ',' ','|',' ',' ',' '},
                        {'-','-','-','-','-','-','-','-','-','-','-'},
                        {' ',' ',' ','|',' ',' ',' ','|',' ',' ',' '},
                        {' ',' ',' ','|',' ',' ',' ','|',' ',' ',' '},
                        {' ',' ',' ','|',' ',' ',' ','|',' ',' ',' '}  };


// Variables
bool computer = false;
string firstPlayer = "";
string secondPlayer = "";
int firstPlayerResult = 0;
int secondPlayerResult = 0;
int tieResult = 0;
int positionForPlaying[9][2] = { {1,1}, {1,5}, {1,9},
                                {5,1}, {5,5}, {5,9},
                                {9,1}, {9,5}, {9,9} };

// Methods
int chooseMode() {
    int mod;

    cout << "---------------------------" << endl;
    cout << "     Select a game mode    " << endl;
    cout << "---------------------------" << endl;
    cout << "       1 - 1v1             " << endl;
    cout << "       2 - vs computer     " << endl;
    cout << "---------------------------" << endl;

    do {
        cout << "Your option: ";

        if(!(cin >> mod)) {
            cin.clear();
            cin.ignore(1000,'\n');
        }

    } while(mod>2 || mod<1);

    return mod;

}


void startGame() {
    startingCountdown();
    system("cls");

    int flag = 2; // Used to determine which player is on the move, if the flag is an even number, the first player is on the move, otherwise the second player is on the move
    int option; 

    while(true) {
        cout << "\n";
        printTable();

        // The first player has a turn
        if(flag % 2 == 0) {

            do {
                 cout << "\nX player " << firstPlayer << " turn:  ";

                if(!(cin >> option)) {
                cin.clear();
                cin.ignore(1000,'\n');
                            }

            } while(option > 9 || option < 1 || !checkValidation(1, option));

            flag = 3;

        // The second player has a turn
        } else {

            if(!computer) { // second player is not computer

                do {
                     cout << "\nO player " << secondPlayer << " turn:  ";

                    if(!(cin >> option)) {
                    cin.clear();
                    cin.ignore(1000,'\n');
                                }

                   } while(option>9 || option<1 || !checkValidation(2,option));


            } else { // second player is computer
                    pause();

                    option = theBestMoves();
                    checkValidation(2,option);
            }

            flag = 2;
        }

        system("cls"); cout << "\n";

        printTable();

        // We are checking to see if we have a winner
        switch ( checkWinner() ) {
            case 1:
                cout << "\nThe winner is X player " << firstPlayer << endl;
                firstPlayerResult++;

                pause();pause();

                return;

            case 2:
                if(!computer) cout << "\nThe winner is O player " << secondPlayer << endl;
                else cout << "\nThe winner is computer!" << endl;

                secondPlayerResult++;

                pause();pause();

                return;

            case 3: break;
        }


        // We check if all fields are filled
        if( checkTie() ) {
            cout << "\nTIE!!!\n" << endl;
            tieResult++;

            pause();pause();

            return;
        }

        system("cls");

    }

}

// MINIMAX ALGORITHM
int theBestMoves() {
    int bestScore = -INT32_MAX;
    int option = -1; // pozicija na koja je najoptimalnija za odigravanje

    for(int i = 0; i < 9; i++) {
            if(table[positionForPlaying[i][0]][positionForPlaying[i][1]] == ' ') {

                table[positionForPlaying[i][0]][positionForPlaying[i][1]] = 'O';

                int score = MiniMax(false);

                table[positionForPlaying[i][0]][positionForPlaying[i][1]] = ' ';

                if (score > bestScore) {
                      bestScore = score;
                      option = i;
                    }
                }
        }

    return option+1;
}

int MiniMax(bool isMaximizing) {

    // is it game over, BASE CASE
    if(checkWinner() == 1 ) return -100; // we return -100 when the winner is X player (it can be -10 and -1, but then the next row must be in plus as well)
    if(checkWinner() == 2 ) return 100; // we return 100 when the winner is O player (AI),
    if(checkTie()) return 0;            // we return 0 if it is tied

    int bestScore;
    if (isMaximizing) { // AI turn

        bestScore = -INT32_MAX;

        for (int i=0;i<9;i++) {
            if (table[positionForPlaying[i][0]][positionForPlaying[i][1]] == ' ') {  // is field empty

                table[positionForPlaying[i][0]][positionForPlaying[i][1]] = 'O';

                int score = MiniMax(false);

                table[positionForPlaying[i][0]][positionForPlaying[i][1]] = ' ';

                if (score > bestScore) 
                    bestScore = score;
            }
        }

    } else { // Player turn
            bestScore = INT32_MAX;

            for (int i=0;i<9;i++) {

                if (table[positionForPlaying[i][0]][positionForPlaying[i][1]] == ' ') {  // is field empty

                    table[positionForPlaying[i][0]][positionForPlaying[i][1]] = 'X';

                    int score = MiniMax(true);

                    table[positionForPlaying[i][0]][positionForPlaying[i][1]] = ' ';

                    if (score < bestScore) 
                        bestScore = score;
                
                }
            }
        }
    
    return bestScore;
}



bool checkTie() {
    if( table[1][1] != ' ' && table[1][5] != ' ' && table[1][9] != ' ' &&
        table[5][1] != ' ' && table[5][5] != ' ' && table[5][9] != ' ' &&
        table[9][1] != ' ' && table[9][5] != ' ' && table[9][9] != ' ' ) return true;
    else return false;
}

bool checkValidation(int player, int option) { // Checks whether the selected field is already filled
    switch (option) {
        case 1:
            if(table[1][1] != ' ') return false;
            if(player == 1 ) table[1][1] = 'X';
            else table [1][1] = 'O';
            break;
        case 2:
            if(table[1][5] != ' ') return false;
            if(player == 1 ) table[1][5] = 'X';
            else table [1][5] = 'O';
            break;
        case 3:
            if(table[1][9] != ' ') return false;
            if(player == 1 ) table[1][9] = 'X';
            else table[1][9] = 'O';
            break;
        case 4:
            if(table[5][1] != ' ') return false;
            if(player == 1 ) table[5][1] = 'X';
            else table [5][1] = 'O';
            break;
        case 5:
            if(table[5][5] != ' ') return false;
            if(player == 1 ) table[5][5] = 'X';
            else table[5][5] = 'O';
            break;
        case 6:
            if(table[5][9] != ' ') return false;
            if(player == 1 ) table[5][9] = 'X';
            else table[5][9] = 'O';
            break;
        case 7:
            if(table[9][1] != ' ') return false;
            if(player == 1 ) table[9][1] = 'X';
            else table[9][1] = 'O';
            break;
        case 8:
            if(table[9][5] != ' ') return false;
            if(player == 1 ) table[9][5] = 'X';
            else table[9][5] = 'O';
            break;
        case 9:
            if(table[9][9] != ' ') return false;
            if(player == 1 ) table[9][9] = 'X';
            else table[9][9] = 'O';
            break;
        default: return false;
    }

    return true;
}

int checkWinner() {

    if( table[1][1]==table[1][5] && table[1][5]==table[1][9]) {
        if(table[1][1] == 'X') return 1;
        else if(table[1][1] == 'O') return 2;
    }
    else if( table[5][1]==table[5][5] && table[5][5]==table[5][9]) {
        if(table[5][1] == 'X') return 1;
        else if(table[5][1] == 'O') return 2;
    }
    else if( table[9][1]==table[9][5] && table[9][5]==table[9][9]) {
        if(table[9][1] == 'X') return 1;
        else if(table[9][1] == 'O') return 2;
    }
    else if( table[1][1]==table[5][1] && table[5][1]==table[9][1]) {
        if(table[1][1] == 'X') return 1;
        else if(table[1][1] == 'O') return 2;
    }
    else if( table[1][5]==table[5][5] && table[5][5]==table[9][5]) {
        if(table[1][5] == 'X') return 1;
        else if(table[1][5] == 'O') return 2;
    }
    else if( table[1][9]==table[5][9] && table[5][9]==table[9][9]) {
        if(table[1][9] == 'X') return 1;
        else if(table[1][9] == 'O') return 2;
    }
    else if( table[1][1]==table[5][5] && table[5][5]==table[9][9]) {
        if(table[1][1] == 'X') return 1;
        else if(table[1][1] == 'O') return 2;
    }
    else if( table[1][9]==table[5][5] && table[5][5]==table[9][1]) {
        if(table[1][9] == 'X') return 1;
        else if(table[1][9] == 'O') return 2;
    }
    
    return 0;
}


void restartTable() {

    for(int i=0;i<9;i++) {
        table[positionForPlaying[i][0]][positionForPlaying[i][1]] = ' ';
    }
}


void startingCountdown(){
    using namespace std::this_thread;
    using namespace std::chrono;

    cout << "\nThe game starts for ..." << endl;

    for(int i =3;i>0;i--) {
    cout << i << " " ;
    sleep_for(nanoseconds(10));
    sleep_until(system_clock::now() + seconds(1));
    }

    cout << "\n" << endl;

}
void pause() {
    using namespace std::this_thread;
    using namespace std::chrono;

    sleep_for(nanoseconds(10));
    sleep_until(system_clock::now() + seconds(1));
}


void printTable() {
    for(int i=0;i<11;i++) {
        for(int j=0;j<11;j++) {
            cout << table[i][j];
        }
        cout << "\n";
    }
}