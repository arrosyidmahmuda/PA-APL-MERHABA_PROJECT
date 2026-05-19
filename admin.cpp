#include "admin.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <cstdio>

using namespace std;

void clearScreenAdmin()
{
    system("cls || clear");
}

void bersihkanInputAdmin()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pauseScreenAdmin()
{
    cout << "\nTekan ENTER untuk lanjut...";
    cin.get();
}

bool isAngkaAdmin(string teks)
{
    if(teks.empty())
    {
        return false;
    }

    for(int i = 0; i < teks.length(); i++)
    {
        if(!isdigit(teks[i]))
        {
            return false;
        }
    }

    return true;
}

void buatHeaderBookingAdmin()
{
    ifstream cek("booking.txt");

    if(!cek.good() || cek.peek() == ifstream::traits_type::eof())
    {
        ofstream file("booking.txt");
        file << "ID|USER|LAPANGAN|JAM_MULAI|DURASI|JAM_SELESAI|STATUS\n";
        file.close();
    }

    cek.close();
}

void buatHeaderAkunAdmin()
{
    ifstream cek("akun.txt");

    if(!cek.good() || cek.peek() == ifstream::traits_type::eof())
    {
        ofstream file("akun.txt");
        file << "role|name|password\n";
        file.close();
    }

    cek.close();
}

bool usernameSudahAdaAdmin(string username)
{
    buatHeaderAkunAdmin();

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

int ambilJamSekarang()
{
    time_t sekarang = time(0);
    tm *waktu = localtime(&sekarang);

    return waktu->tm_hour;
}

void updateBookingSelesai(int jamSekarang)
{
    buatHeaderBookingAdmin();

    ifstream file("booking.txt");

    if(!file.is_open())
    {
        cout << "\nFile booking.txt tidak bisa dibuka!\n";
        return;
    }

    ofstream temp("temp.txt");

    string header;
    getline(file, header);

    temp << "ID|USER|LAPANGAN|JAM_MULAI|DURASI|JAM_SELESAI|STATUS\n";

    string id, user, lap, jamMulai, durasi, jamSelesai, status;

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jamMulai, '|');
        getline(file, durasi, '|');
        getline(file, jamSelesai, '|');
        getline(file, status);

        if(jamSelesai.length() >= 2 &&
           isdigit(jamSelesai[0]) &&
           isdigit(jamSelesai[1]))
        {
            int selesai = stoi(jamSelesai.substr(0, 2));

            if(status == "acc" && jamSekarang >= selesai)
            {
                status = "done";
            }
        }

        temp << id << "|"
             << user << "|"
             << lap << "|"
             << jamMulai << "|"
             << durasi << "|"
             << jamSelesai << "|"
             << status << endl;
    }

    file.close();
    temp.close();

    std::remove("booking.txt");
    std::rename("temp.txt", "booking.txt");
}

string formatRupiah(int angka)
{
    string hasil = to_string(angka);
    string rupiah = "";

    int hitung = 0;

    for(int i = hasil.length() - 1; i >= 0; i--)
    {
        rupiah = hasil[i] + rupiah;
        hitung++;

        if(hitung == 3 && i != 0)
        {
            rupiah = "." + rupiah;
            hitung = 0;
        }
    }

    return "Rp " + rupiah;
}

void lihatLapangan()
{
    ifstream file("lapangan.txt");

    if(!file.is_open())
    {
        cout << "\nFile lapangan.txt belum ada atau tidak bisa dibuka!\n";
        return;
    }

    string id, nama, harga, status;

    cout << "\n===== DAFTAR LAPANGAN =====\n";

    bool adaData = false;

    while(getline(file, id, '|'))
    {
        getline(file, nama, '|');
        getline(file, harga, '|');
        getline(file, status);

        adaData = true;

        cout << "ID      : " << id << endl;
        cout << "Nama    : " << nama << endl;

        if(isAngkaAdmin(harga))
        {
            cout << "Harga   : " << formatRupiah(stoi(harga)) << endl;
        }
        else
        {
            cout << "Harga   : Data harga tidak valid" << endl;
        }

        cout << "Status  : " << status << endl;
        cout << "----------------------\n";
    }

    file.close();

    if(!adaData)
    {
        cout << "\nBelum ada data lapangan!\n";
    }
}

void tambahLapangan()
{
    lihatLapangan();

    string nama, harga;

    do
    {
        cout << "\nNama Lapangan : ";
        getline(cin >> ws, nama);

        if(nama.empty())
        {
            cout << "Nama lapangan tidak boleh kosong!\n";
        }

    } while(nama.empty());

    do
    {
        cout << "Harga : ";
        getline(cin, harga);

        if(harga.empty())
        {
            cout << "Harga tidak boleh kosong!\n";
        }
        else if(!isAngkaAdmin(harga))
        {
            cout << "Harga hanya boleh angka!\n";
        }

    } while(harga.empty() || !isAngkaAdmin(harga));

    int id = 1;

    ifstream cek("lapangan.txt");

    string temp;

    while(getline(cek, temp))
    {
        id++;
    }

    cek.close();

    ofstream file("lapangan.txt", ios::app);

    if(!file.is_open())
    {
        cout << "\nGagal membuka file lapangan.txt!\n";
        return;
    }

    file << id
         << "|"
         << nama
         << "|"
         << harga
         << "|ready"
         << endl;

    file.close();

    cout << "\nLapangan berhasil ditambahkan!\n";
}

void editLapangan()
{
    lihatLapangan();

    string pilihId;

    do
    {
        cout << "\nMasukkan ID Lapangan yang ingin diedit : ";
        getline(cin >> ws, pilihId);

        if(pilihId.empty())
        {
            cout << "ID Lapangan tidak boleh kosong!\n";
        }
        else if(!isAngkaAdmin(pilihId))
        {
            cout << "ID Lapangan harus angka!\n";
        }

    } while(pilihId.empty() || !isAngkaAdmin(pilihId));

    ifstream file("lapangan.txt");

    if(!file.is_open())
    {
        cout << "\nFile lapangan.txt tidak bisa dibuka!\n";
        return;
    }

    ofstream temp("temp.txt");

    string id, nama, harga, status;

    bool ditemukan = false;

    while(getline(file, id, '|'))
    {
        getline(file, nama, '|');
        getline(file, harga, '|');
        getline(file, status);

        if(id == pilihId)
        {
            ditemukan = true;

            do
            {
                cout << "\nNama Baru : ";
                getline(cin >> ws, nama);

                if(nama.empty())
                {
                    cout << "Nama tidak boleh kosong!\n";
                }

            } while(nama.empty());

            do
            {
                cout << "Harga Baru : ";
                getline(cin, harga);

                if(harga.empty())
                {
                    cout << "Harga tidak boleh kosong!\n";
                }
                else if(!isAngkaAdmin(harga))
                {
                    cout << "Harga hanya boleh angka!\n";
                }

            } while(harga.empty() || !isAngkaAdmin(harga));

            do
            {
                cout << "Status Baru : ";
                getline(cin, status);

                if(status.empty())
                {
                    cout << "Status tidak boleh kosong!\n";
                }

            } while(status.empty());
        }

        temp << id
             << "|"
             << nama
             << "|"
             << harga
             << "|"
             << status
             << endl;
    }

    file.close();
    temp.close();

    std::remove("lapangan.txt");
    std::rename("temp.txt", "lapangan.txt");

    if(ditemukan)
    {
        cout << "\nLapangan berhasil diedit!\n";
    }
    else
    {
        cout << "\nID Lapangan tidak ditemukan!\n";
    }
}

