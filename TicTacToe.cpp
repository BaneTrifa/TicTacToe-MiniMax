#include "heleperFunctions.h"

int main() {

    int mod = chooseMode();

    if(mod == 1 ) {
        cout << "\nEnter the name of the X player: "; cin >> firstPlayer;
        cout << "Enter the name of the O player: "; cin >> secondPlayer;
    } else {
        cout << "\nEnter the name of the player: "; cin >> firstPlayer;
        secondPlayer = "computer";
        computer = true;
        }

    system("cls");

    while(true) {
        startGame();
        system("cls");

        string option;

        cout << "--------------------------------------" << endl;
        cout << "                 RESULT             " << endl;
        cout << "--------------------------------------" << endl;
        cout << "         " << firstPlayer << " - " << firstPlayerResult << endl;
        cout << "         " << secondPlayer << " - " << secondPlayerResult << endl;
        cout << "         " << "Tie " << " - " << tieResult << endl;
        cout << "--------------------------------------" << endl;

        do {
            cout << "Do you want to play again? (Y/N): ";

            if(!(cin >> option)) {
                cin.clear();
                cin.ignore(1000,'\n');
            }

        } while(option.compare("Y") != 0 && option.compare("N") != 0);

        if( option.compare("N") == 0) break;

        restartTable();
        }

    return 0;
}


