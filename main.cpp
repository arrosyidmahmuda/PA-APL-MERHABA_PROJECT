#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
#include "user.h"
#include "admin.h"

using namespace std;

string roleLogin;
string userLogin;

void clearScreen()
{
    system("cls || clear");
}

void pauseScreen()
{
    cout << "\nTekan ENTER untuk lanjut...";
    cin.get();
}

void AkunHeader()
{
    ifstream file("akun.txt");

    if (!file.good() || file.peek() == EOF)
    {
        file.close();

        ofstream outfile("akun.txt", ios::app);
        if (outfile.is_open())
        {
            outfile << "role|name|password" << endl;
            outfile.close();
        }
    }
}

bool loginUser()
{
    clearScreen();
    AkunHeader();

    string username;
    string password;

    cout << "\n===== LOGIN =====\n";

    cout << "Username : ";
    cin >> username;

    cout << "Password : ";
    cin >> password;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    ifstream file("akun.txt");

    string header;
    getline(file, header);

    string role;
    string user;
    string pass;

    while(getline(file, role, '|'))
    {
        getline(file, user, '|');
        getline(file, pass);

        if(username == user && password == pass)
        {
            roleLogin = role;
            userLogin = user;

            cout << "\nLogin berhasil!\n";

            file.close();

            pauseScreen();

            return true;
        }
    }

    file.close();

    cout << "\nLogin gagal!\n";

    pauseScreen();

    return false;
}

bool cekUsername(string username)
{
    AkunHeader();

    ifstream file("akun.txt");

    string header;
    getline(file, header);

    string role, user, pass;

    while(getline(file, role, '|'))
    {
        getline(file, user, '|');
        getline(file, pass);

        if(user == username)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

bool registerUser()
{
    clearScreen();
    AkunHeader();

    string username;
    string password;

    cout << "\n===== REGISTER =====\n";

    cout << "Username : ";
    cin >> username;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if(cekUsername(username))
    {
        cout << "\nUsername sudah terdaftar!\n";
        pauseScreen();
        return false;
    }

    cout << "Password : ";
    cin >> password;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    ofstream file("akun.txt", ios::app);

    file << "user"
         << "|"
         << username
         << "|"
         << password
         << endl;

    file.close();

    cout << "\nRegister berhasil!\n";
    pauseScreen();

    return loginUser();
}

bool menuAuth()
{
    int pilih;

    do
    {
        clearScreen();

        cout << "\n===== MENU AUTH =====\n";

        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";

        cout << "Pilih : ";
        cin >> pilih;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nInput harus angka!\n";

            pauseScreen();

            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(pilih)
        {
            case 1:
                if(loginUser())
                {
                    return true;
                }
                break;

            case 2:
                if(registerUser())
                {
                    return true;
                }
                break;

            case 3:
                return false;

            default:
                cout << "\nPilihan tidak valid!\n";
                pauseScreen();
        }

    } while(true);
}

int main()
{
    AkunHeader();

    while(menuAuth())
    {
        clearScreen();

        if(roleLogin == "admin")
        {
            menuAdmin();
        }
        else if(roleLogin == "user")
        {
            menuUser();
        }
    }

    clearScreen();
    cout << "\nProgram selesai.\n";

    return 0;
}