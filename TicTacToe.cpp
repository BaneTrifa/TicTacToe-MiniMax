#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <stdlib.h>

using namespace std;

char tabla[11][11] ={   {' ',' ',' ','|',' ',' ',' ','|',' ',' ',' '},
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

bool komp = false;
string prviIgrac = "", drugiIgrac = "";
int prviRez = 0, drugiRez = 0, neresenoRez = 0;
int pozicijeZaIgranje[9][2] = { {1,1}, {1,5}, {1,9},
                                {5,1}, {5,5}, {5,9},
                                {9,1}, {9,5}, {9,9} };   //polja na tabli na kojima je dozvoljeno stavljanje znakova tj. njihove grupisane koordinate


///METODE///
int OdabirModa();

void WhilePetlja();
int NajboljiPotez();
int MiniMax(bool isMaximizing); // isMaximizing nam pokazuje da li je na redu Maximizing ciklus ili Minimizing ciklus

bool ProveriNereseno();
bool ProveriValidnost(int igrac, int opcija);
int ProveriPobednika();

void RestartujPolja();

void PocetakIgre();
void Pauza();

void IspisiTablu();
/////////////



int main() {

    int mod = OdabirModa();

    if(mod == 1 ) {
        cout << "\nUnesite ime X igraca: "; cin >> prviIgrac;
        cout << "Unesite ime O igraca: "; cin >> drugiIgrac;
    } else {
        cout << "\nUnesite ime igraca: "; cin >> prviIgrac;
        drugiIgrac = "Kompijuter";
        komp = true;
        }

    system("cls");

    while(true) {
        WhilePetlja();
        system("cls");

        string opcija;

        cout << "--------------------------------------" << endl;
        cout << "                 REZULTAT             " << endl;
        cout << "--------------------------------------" << endl;
        cout << "         " << prviIgrac << " - " << prviRez << endl;
        cout << "         " << drugiIgrac << " - " << drugiRez << endl;
        cout << "         " << "Nereseno " << " - " << neresenoRez << endl;
        cout << "--------------------------------------" << endl;

        do {
            cout << "Da li zelite ponovo da igrate? (Da/Ne): ";

            if(!(cin >> opcija)) {
                cin.clear();
                cin.ignore(1000,'\n');
            }

        } while(opcija.compare("Da") != 0 && opcija.compare("Ne") != 0);

        if( opcija.compare("Ne") == 0) break;

        RestartujPolja();
        }

    return 0;
}


int OdabirModa() {
    int mod;

    cout << "---------------------------" << endl;
    cout << "     Izaberite mod igre    " << endl;
    cout << "---------------------------" << endl;
    cout << "       1 - 1v1             " << endl;
    cout << "       2 - vs computer     " << endl;
    cout << "---------------------------" << endl;

    do {
        cout << "TVOJA OPCIJA: ";

        if(!(cin >> mod)) {
            cin.clear();
            cin.ignore(1000,'\n');
        }

    } while(mod>2 || mod<1);

    return mod;

}


void WhilePetlja() {
    PocetakIgre();
    system("cls");

    int flag = 2, opcija; // Sluzi da utvrdimo koji igrac je na potezu, ako je flag paran broj prvi igrac je na potezu u suprotnom drugi igrac je na potezu

    while(true) {
        cout << "\n";
        IspisiTablu();

        // PRVI IGRAC je na potezu
        if(flag % 2 == 0) {

            do {
                 cout << "\nX igrac " << prviIgrac << " je na potezu:  ";

                if(!(cin >> opcija)) {
                cin.clear();
                cin.ignore(1000,'\n');
                            }

            } while(opcija>9 || opcija<1 || !ProveriValidnost(1,opcija));

            flag = 3;

        // DRUGI IGRAC je na potezu
        } else {

            if(!komp) { //drugi igrac nije kompijuter

                do {
                     cout << "\nO igrac " << drugiIgrac << " je na potezu:  ";

                    if(!(cin >> opcija)) {
                    cin.clear();
                    cin.ignore(1000,'\n');
                                }

                   } while(opcija>9 || opcija<1 || !ProveriValidnost(2,opcija));


            } else { // drugi igrac je kompijuter
                    Pauza();

                    opcija = NajboljiPotez();
                    ProveriValidnost(2,opcija);
            }

            flag = 2;
        }

        //PROVERAVAMO da li imamo pobednika
        system("cls"); cout << "\n";
        IspisiTablu();
        switch ( ProveriPobednika() ) {
            case 1:
                cout << "\nPobednik je X igrac " << prviIgrac << endl;
                prviRez++;

                Pauza();Pauza();
                return;
            case 2:
                if(!komp) cout << "\nPobednik je O igrac " << drugiIgrac << endl;
                else cout << "\nPobednik je Kompijuter" << endl;

                drugiRez++;
                Pauza();Pauza();
                return;
            case 3: break;
        }


        //PROVERAVAMO da li su popunjena sva polja
        if( ProveriNereseno() ) {
            cout << "\nNERESENO!!!\n" << endl;
            neresenoRez++;
            Pauza();Pauza();
            return;
        }

        system("cls");

    }//kraj while-a

}

///MINIMAX ALGORITAM///
int NajboljiPotez() {
    int bestScore = -999;
    int opcija = -1; // pozicija na koja je najoptimalnija za odigravanje

    for(int i=0;i<9;i++) {
            if(tabla[pozicijeZaIgranje[i][0]][pozicijeZaIgranje[i][1]] == ' ') {
                tabla[pozicijeZaIgranje[i][0]][pozicijeZaIgranje[i][1]] = 'O';
                int score = MiniMax(false);
                tabla[pozicijeZaIgranje[i][0]][pozicijeZaIgranje[i][1]] = ' ';
                if (score > bestScore) {
                      bestScore = score;
                      opcija = i;
                    }
                }
        }

    return opcija+1;
}
int MiniMax(bool isMaximizing) {
    //da li je kraj igre, BASE CASE
    if(ProveriPobednika() == 1 ) return -100; // vracamo -100 kada je pobednik iks(X) igrac (moze i -10 i -1 ali onda i u sledecem redu mora isto toliko u plusu)
    if(ProveriPobednika() == 2 ) return 100; //vracamo 100 kada je pobednik oks(O) igrac,
    if(ProveriNereseno()) return 0; //vracamo 0 ako je nereseno


    if (isMaximizing) {
        int bestScore = -999;
        for (int i=0;i<9;i++) {
            if (tabla[pozicijeZaIgranje[i][0]][pozicijeZaIgranje[i][1]] == ' ') {  // Da li je polje prazno
              tabla[pozicijeZaIgranje[i][0]][pozicijeZaIgranje[i][1]] = 'O';
              int score = MiniMax(false);
              tabla[pozicijeZaIgranje[i][0]][pozicijeZaIgranje[i][1]] = ' ';
              if (score > bestScore) bestScore = score;
            }
        }
        return bestScore;
  }else {
        int bestScore = 999;
        for (int i=0;i<9;i++) {
            if (tabla[pozicijeZaIgranje[i][0]][pozicijeZaIgranje[i][1]] == ' ') {  // Da li je polje prazno
              tabla[pozicijeZaIgranje[i][0]][pozicijeZaIgranje[i][1]] = 'X';
              int score = MiniMax(true);
              tabla[pozicijeZaIgranje[i][0]][pozicijeZaIgranje[i][1]] = ' ';
              if (score < bestScore) bestScore = score;
            }
        }
        return bestScore;
  }
}
///////////////////////


bool ProveriNereseno() {
    if( tabla[1][1] != ' ' && tabla[1][5] != ' ' && tabla[1][9] != ' ' &&
        tabla[5][1] != ' ' && tabla[5][5] != ' ' && tabla[5][9] != ' ' &&
        tabla[9][1] != ' ' && tabla[9][5] != ' ' && tabla[9][9] != ' ' ) return true;
    else return false;
}
bool ProveriValidnost(int igrac, int opcija) {
    switch (opcija) {
        case 1:
            if(tabla[1][1] != ' ') return false;
            if(igrac == 1 ) tabla[1][1] = 'X';
            else tabla [1][1] = 'O';
            break;
        case 2:
            if(tabla[1][5] != ' ') return false;
            if(igrac == 1 ) tabla[1][5] = 'X';
            else tabla [1][5] = 'O';
            break;
        case 3:
            if(tabla[1][9] != ' ') return false;
            if(igrac == 1 ) tabla[1][9] = 'X';
            else tabla[1][9] = 'O';
            break;
        case 4:
            if(tabla[5][1] != ' ') return false;
            if(igrac == 1 ) tabla[5][1] = 'X';
            else tabla [5][1] = 'O';
            break;
        case 5:
            if(tabla[5][5] != ' ') return false;
            if(igrac == 1 ) tabla[5][5] = 'X';
            else tabla[5][5] = 'O';
            break;
        case 6:
            if(tabla[5][9] != ' ') return false;
            if(igrac == 1 ) tabla[5][9] = 'X';
            else tabla[5][9] = 'O';
            break;
        case 7:
            if(tabla[9][1] != ' ') return false;
            if(igrac == 1 ) tabla[9][1] = 'X';
            else tabla[9][1] = 'O';
            break;
        case 8:
            if(tabla[9][5] != ' ') return false;
            if(igrac == 1 ) tabla[9][5] = 'X';
            else tabla[9][5] = 'O';
            break;
        case 9:
            if(tabla[9][9] != ' ') return false;
            if(igrac == 1 ) tabla[9][9] = 'X';
            else tabla[9][9] = 'O';
            break;
        default: return false;
    }

    return true;
}
int ProveriPobednika() {

    if( tabla[1][1]==tabla[1][5] && tabla[1][5]==tabla[1][9]) {
        if(tabla[1][1] == 'X') return 1;
        else if(tabla[1][1] == 'O') return 2;
    }
    else if( tabla[5][1]==tabla[5][5] && tabla[5][5]==tabla[5][9]) {
        if(tabla[5][1] == 'X') return 1;
        else if(tabla[5][1] == 'O') return 2;
    }
    else if( tabla[9][1]==tabla[9][5] && tabla[9][5]==tabla[9][9]) {
        if(tabla[9][1] == 'X') return 1;
        else if(tabla[9][1] == 'O') return 2;
    }
    else if( tabla[1][1]==tabla[5][1] && tabla[5][1]==tabla[9][1]) {
        if(tabla[1][1] == 'X') return 1;
        else if(tabla[1][1] == 'O') return 2;
    }
    else if( tabla[1][5]==tabla[5][5] && tabla[5][5]==tabla[9][5]) {
        if(tabla[1][5] == 'X') return 1;
        else if(tabla[1][5] == 'O') return 2;
    }
    else if( tabla[1][9]==tabla[5][9] && tabla[5][9]==tabla[9][9]) {
        if(tabla[1][9] == 'X') return 1;
        else if(tabla[1][9] == 'O') return 2;
    }
    else if( tabla[1][1]==tabla[5][5] && tabla[5][5]==tabla[9][9]) {
        if(tabla[1][1] == 'X') return 1;
        else if(tabla[1][1] == 'O') return 2;
    }
    else if( tabla[1][9]==tabla[5][5] && tabla[5][5]==tabla[9][1]) {
        if(tabla[1][9] == 'X') return 1;
        else if(tabla[1][9] == 'O') return 2;
    }
    else return 3;
}


void RestartujPolja() {

    for(int i=0;i<9;i++) {
        tabla[pozicijeZaIgranje[i][0]][pozicijeZaIgranje[i][1]] = ' ';
    }
}


void PocetakIgre(){
    using namespace std::this_thread;
    using namespace std::chrono;

    cout << "\nIgra pocinje za ..." << endl;

    for(int i =3;i>0;i--) {
    cout << i << " " ;
    sleep_for(nanoseconds(10));
    sleep_until(system_clock::now() + seconds(1));
    }

    cout << "\n" << endl;

}
void Pauza() {
    using namespace std::this_thread;
    using namespace std::chrono;

    sleep_for(nanoseconds(10));
    sleep_until(system_clock::now() + seconds(1));
}


void IspisiTablu() {
    for(int i=0;i<11;i++) {
        for(int j=0;j<11;j++) {
            cout << tabla[i][j];
        }
        cout << "\n";
    }
}


