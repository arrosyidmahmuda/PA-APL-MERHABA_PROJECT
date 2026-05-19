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

void hapusLapangan()
{
    lihatLapangan();

    string pilihId;

    do
    {
        cout << "\nMasukkan ID Lapangan yang ingin dihapus : ";
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

        if(id != pilihId)
        {
            temp << id
                 << "|"
                 << nama
                 << "|"
                 << harga
                 << "|"
                 << status
                 << endl;
        }
        else
        {
            ditemukan = true;
        }
    }

    file.close();
    temp.close();

    std::remove("lapangan.txt");
    std::rename("temp.txt", "lapangan.txt");

    if(ditemukan)
    {
        cout << "\nLapangan berhasil dihapus!\n";
    }
    else
    {
        cout << "\nID Lapangan tidak ditemukan!\n";
    }
}

void tambahAdmin()
{
    buatHeaderAkunAdmin();

    string username, password;

    do
    {
        cout << "\nUsername Admin : ";
        getline(cin >> ws, username);

        if(username.empty())
        {
            cout << "Username tidak boleh kosong!\n";
        }
        else if(usernameSudahAdaAdmin(username))
        {
            cout << "Username sudah terdaftar!\n";
        }

    } while(username.empty() || usernameSudahAdaAdmin(username));

    do
    {
        cout << "Password : ";
        getline(cin, password);

        if(password.empty())
        {
            cout << "Password tidak boleh kosong!\n";
        }

    } while(password.empty());

    ofstream file("akun.txt", ios::app);

    if(!file.is_open())
    {
        cout << "\nGagal membuka file akun.txt!\n";
        return;
    }

    file << "admin"
         << "|"
         << username
         << "|"
         << password
         << endl;

    file.close();

    cout << "\nAdmin berhasil ditambahkan!\n";
}

void approveBooking()
{
    buatHeaderBookingAdmin();

    ifstream file("booking.txt");

    if(!file.is_open())
    {
        cout << "\nFile booking.txt tidak bisa dibuka!\n";
        return;
    }

    string header;
    getline(file, header);

    string id, user, lap, jamMulai, durasi, jamSelesai, status;

    cout << "\n===== BOOKING PENDING =====\n";

    bool adaPending = false;

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jamMulai, '|');
        getline(file, durasi, '|');
        getline(file, jamSelesai, '|');
        getline(file, status);

        if(status == "pending")
        {
            adaPending = true;

            int hargaLapangan = 0;

            ifstream lapanganFile("lapangan.txt");

            string idLap, namaLap, hargaLap, statusLap;

            while(getline(lapanganFile, idLap, '|'))
            {
                getline(lapanganFile, namaLap, '|');
                getline(lapanganFile, hargaLap, '|');
                getline(lapanganFile, statusLap);

                if(idLap == lap && isAngkaAdmin(hargaLap))
                {
                    hargaLapangan = stoi(hargaLap);
                    break;
                }
            }

            lapanganFile.close();

            int totalHarga = 0;

            if(isAngkaAdmin(durasi))
            {
                totalHarga = hargaLapangan * stoi(durasi);
            }

            cout << "ID        : " << id << endl;
            cout << "User      : " << user << endl;
            cout << "Lapangan  : " << lap << endl;
            cout << "Jam       : " << jamMulai << " - " << jamSelesai << endl;
            cout << "Durasi    : " << durasi << " jam" << endl;
            cout << "Harga/Jam : " << formatRupiah(hargaLapangan) << endl;
            cout << "Total     : " << formatRupiah(totalHarga) << endl;
            cout << "Status    : " << status << endl;
            cout << "----------------------\n";
        }
    }

    file.close();

    if(!adaPending)
    {
        cout << "\nBelum ada booking berstatus pending!\n";
        return;
    }

    string pilihId, aksi;

    do
    {
        cout << "\nMasukkan ID Booking : ";
        getline(cin >> ws, pilihId);

        if(pilihId.empty())
        {
            cout << "ID Booking tidak boleh kosong!\n";
        }
        else if(!isAngkaAdmin(pilihId))
        {
            cout << "ID Booking harus angka!\n";
        }

    } while(pilihId.empty() || !isAngkaAdmin(pilihId));

    do
    {
        cout << "Status (acc/decline) : ";
        getline(cin, aksi);

        if(aksi != "acc" && aksi != "decline")
        {
            cout << "Status hanya boleh acc atau decline!\n";
        }

    } while(aksi != "acc" && aksi != "decline");

    ifstream baca("booking.txt");
    ofstream temp("temp.txt");

    getline(baca, header);

    temp << "ID|USER|LAPANGAN|JAM_MULAI|DURASI|JAM_SELESAI|STATUS\n";

    bool ditemukan = false;

    while(getline(baca, id, '|'))
    {
        getline(baca, user, '|');
        getline(baca, lap, '|');
        getline(baca, jamMulai, '|');
        getline(baca, durasi, '|');
        getline(baca, jamSelesai, '|');
        getline(baca, status);

        if(id == pilihId && status == "pending")
        {
            status = aksi;
            ditemukan = true;
        }

        temp << id << "|"
             << user << "|"
             << lap << "|"
             << jamMulai << "|"
             << durasi << "|"
             << jamSelesai << "|"
             << status << endl;
    }

    baca.close();
    temp.close();

    std::remove("booking.txt");
    std::rename("temp.txt", "booking.txt");

    if(ditemukan)
    {
        cout << "\nStatus booking berhasil diubah!\n";
    }
    else
    {
        cout << "\nID booking tidak ditemukan atau status bukan pending!\n";
    }
}

void lihatPendapatan()
{
    buatHeaderBookingAdmin();

    updateBookingSelesai(ambilJamSekarang());

    ifstream booking("booking.txt");

    if(!booking.is_open())
    {
        cout << "\nFile booking.txt tidak bisa dibuka!\n";
        return;
    }

    string header;
    getline(booking, header);

    string id, user, lap, jamMulai, durasi, jamSelesai, status;

    int total = 0;

    while(getline(booking, id, '|'))
    {
        getline(booking, user, '|');
        getline(booking, lap, '|');
        getline(booking, jamMulai, '|');
        getline(booking, durasi, '|');
        getline(booking, jamSelesai, '|');
        getline(booking, status);

        if(status == "acc" || status == "done")
        {
            ifstream lapangan("lapangan.txt");

            string idLap, nama, harga, statusLap;

            while(getline(lapangan, idLap, '|'))
            {
                getline(lapangan, nama, '|');
                getline(lapangan, harga, '|');
                getline(lapangan, statusLap);

                if(idLap == lap && isAngkaAdmin(harga) && isAngkaAdmin(durasi))
                {
                    total += stoi(harga) * stoi(durasi);
                    break;
                }
            }

            lapangan.close();
        }
    }

    booking.close();

    cout << "\nTotal Pendapatan : " << formatRupiah(total) << endl;
}

void menuEditLapanganAdmin()
{
    int pilihanEdit;

    cout << "\n===== MENU LAPANGAN =====\n";
    cout << "1. Edit Lapangan\n";
    cout << "2. Hapus Lapangan\n";
    cout << "3. Kembali\n";

    cout << "Pilih : ";
    cin >> pilihanEdit;

    if(cin.fail())
    {
        cin.clear();
        bersihkanInputAdmin();

        cout << "\nInput harus angka!\n";
        return;
    }

    bersihkanInputAdmin();

    switch(pilihanEdit)
    {
        case 1:
            editLapangan();
            break;

        case 2:
            hapusLapangan();
            break;

        case 3:
            cout << "\nKembali ke menu admin...\n";
            break;

        default:
            cout << "\nPilihan tidak valid!\n";
    }
}

void menuAdmin()
{
    int pilih;

    do
    {
        clearScreenAdmin();

        cout << "\n===== MENU ADMIN =====\n";

        cout << "1. Lihat Lapangan\n";
        cout << "2. Tambah Lapangan\n";
        cout << "3. Edit/Hapus Lapangan\n";
        cout << "4. Approve Booking\n";
        cout << "5. Tambah Admin\n";
        cout << "6. Lihat Pendapatan\n";
        cout << "7. Exit\n";

        cout << "Pilih : ";
        cin >> pilih;

        if(cin.fail())
        {
            cin.clear();
            bersihkanInputAdmin();

            cout << "\nInput Hanya Bisa Angka!\n";
            pauseScreenAdmin();

            continue;
        }

        bersihkanInputAdmin();

        switch(pilih)
        {
            case 1:
                clearScreenAdmin();
                lihatLapangan();
                pauseScreenAdmin();
                break;

            case 2:
                clearScreenAdmin();
                tambahLapangan();
                pauseScreenAdmin();
                break;

            case 3:
                clearScreenAdmin();
                menuEditLapanganAdmin();
                pauseScreenAdmin();
                break;

            case 4:
            {
                clearScreenAdmin();

                updateBookingSelesai(ambilJamSekarang());

                approveBooking();

                updateBookingSelesai(ambilJamSekarang());

                // int jamTes;
                //
                // cout << "\nMasukkan jam sekarang untuk test status selesai (0-23): ";
                // cin >> jamTes;
                //
                // if(cin.fail())
                // {
                //     cin.clear();
                //     bersihkanInputAdmin();
                //
                //     cout << "\nInput jam harus angka!\n";
                //     pauseScreenAdmin();
                //     break;
                // }
                //
                // bersihkanInputAdmin();
                //
                // if(jamTes < 0 || jamTes > 23)
                // {
                //     cout << "\nJam harus antara 0 sampai 23!\n";
                // }
                // else
                // {
                //     updateBookingSelesai(jamTes);
                //     cout << "\nUpdate status berdasarkan jam test berhasil!\n";
                // }

                pauseScreenAdmin();
                break;
            }

            case 5:
                clearScreenAdmin();
                tambahAdmin();
                pauseScreenAdmin();
                break;

            case 6:
                clearScreenAdmin();
                lihatPendapatan();
                pauseScreenAdmin();
                break;

            case 7:
                cout << "\nLogout...\n";
                pauseScreenAdmin();
                return;

            default:
                cout << "\nPilihan tidak valid!\n";
                pauseScreenAdmin();
        }

    } while(true);
}