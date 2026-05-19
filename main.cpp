#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <limits>
#include "user.h"
#include "admin.h"

using namespace std;

string roleLogin;
string userLogin;

void clearScreen()
{
    system("cls");
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
    AkunHeader();
    
    string username;
    string password;
    
    cout << "\n===== LOGIN =====\n";
    
    cout << "Username : ";
    cin >> username;
    
    cout << "Password : ";
    cin >> password;
    
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
        
        if(username == user &&
            password == pass)
            {
                roleLogin = role;
            userLogin = user;
            
            cout << "\nLogin berhasil!\n";
            
            file.close();

            return true;
        }
    }
    
    file.close();
    
    cout << "\nLogin gagal!\n";
    
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
    AkunHeader();

    string username;
    string password;

    cout << "\n===== REGISTER =====\n";

    cout << "Username : ";
    cin >> username;

    if(cekUsername(username))
    {
        cout << "\nUsername sudah terdaftar!\n";
        return false;
    }

    cout << "Password : ";
    cin >> password;

    ofstream file("akun.txt", ios::app);

    file << "user"
         << "|"
         << username
         << "|"
         << password
         << endl;

    file.close();

    cout << "\nRegister berhasil!\n";

    return loginUser();
}

bool menuAuth()
{
    int pilih;

    do
    {
        cout << "\n===== MENU AUTH =====\n";

        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";

        cout << "Pilih : ";
        cin >> pilih;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');

            cout << "\nInput harus angka!\n";
            continue;
        }

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
        }

    } while(true);
}

int main()
{
    AkunHeader();

    while (menuAuth())
    {
        if (roleLogin == "admin")
        {
            menuAdmin();
        }
        else if (roleLogin == "user")
        {
            menuUser();
        }
        
    }
    return 0;
}