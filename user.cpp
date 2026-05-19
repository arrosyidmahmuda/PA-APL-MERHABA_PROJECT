#include "user.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

extern string userLogin;

struct Lapangan
{
    string id;
    string nama;
    int harga;
    string status;
};

void buatHeaderBooking()
{
    ifstream cek("booking.txt");
    if(!cek.good() || cek.peek() == ifstream::traits_type::eof())
    {
        ofstream file("booking.txt");
        file << "ID|USER|LAPANGAN|JAM_MULAI|DURASI|JAM_SELESAI|STATUS\n";
    }
}

string formatRupiahUser(int angka)
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

string formatJam(int jam)
{
    if(jam < 10)
        return "0" + to_string(jam) + ":00";

    return to_string(jam) + ":00";
}

int ambilAngkaJam(string jam)
{
    return stoi(jam.substr(0, 2));
}

bool cekRentangTersediaKecualiId(string lapangan, int mulaiBaru, int selesaiBaru, string idKecuali)
{
    buatHeaderBooking();

    ifstream file("booking.txt");

    string header;
    getline(file, header);

    string id, user, lap, jamMulai, durasi, jamSelesai, status;

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jamMulai, '|');
        getline(file, durasi, '|');
        getline(file, jamSelesai, '|');
        getline(file, status);

        if(id == idKecuali)
        {
            continue;
        }

        if(lap == lapangan && (status == "pending" || status == "acc"))
        {
            int mulaiLama = ambilAngkaJam(jamMulai);
            int selesaiLama = ambilAngkaJam(jamSelesai);

            if(mulaiBaru < selesaiLama && selesaiBaru > mulaiLama)
            {
                file.close();
                return false;
            }
        }
    }

    file.close();
    return true;
}

bool cekRentangTersedia(string lapangan, int mulaiBaru, int selesaiBaru)
{
    return cekRentangTersediaKecualiId(lapangan, mulaiBaru, selesaiBaru, "");
}

void bubbleSortHarga(Lapangan data[], int jumlah, bool ascending)
{
    for(int i = 0; i < jumlah - 1; i++)
    {
        for(int j = 0; j < jumlah - i - 1; j++)
        {
            bool swapNeeded = (ascending)
                ? (data[j].harga > data[j + 1].harga)
                : (data[j].harga < data[j + 1].harga);

            if(swapNeeded)
                swap(data[j], data[j + 1]);
        }
    }
}

void tampilLapangan()
{
    ifstream file("lapangan.txt");

    Lapangan data[100];
    int jumlah = 0;

    string id, nama, harga, status;

    while(getline(file, id, '|'))
    {
        getline(file, nama, '|');
        getline(file, harga, '|');
        getline(file, status);

        if(status == "ready")
        {
            data[jumlah].id = id;
            data[jumlah].nama = nama;
            data[jumlah].harga = stoi(harga);
            data[jumlah].status = status;
            jumlah++;
        }
    }

    string pilihSortInput;

    cout << "\nUrutkan harga:\n";
    cout << "1. Termurah ke Termahal\n";
    cout << "2. Termahal ke Termurah\n";
    cout << "Pilih : ";
    cin >> pilihSortInput;

    if(pilihSortInput != "1" && pilihSortInput != "2")
    {
        cout << "\nInput tidak valid!\n";
        throw runtime_error("Input sort tidak valid");
    }

    int pilihSort = stoi(pilihSortInput);

    if(pilihSort == 1)
        bubbleSortHarga(data, jumlah, true);
    else if(pilihSort == 2)
        bubbleSortHarga(data, jumlah, false);

    cout << "\n===== DAFTAR LAPANGAN =====\n";

    for(int i = 0; i < jumlah; i++)
    {
        cout << "ID      : " << data[i].id << endl;
        cout << "Nama    : " << data[i].nama << endl;
        cout << "Harga   : " << formatRupiahUser(data[i].harga) << endl;
        cout << "----------------------\n";
    }
}

void bookingLapangan()
{
    buatHeaderBooking();

    try
    {
        tampilLapangan();
    }
    catch(...)
    {
        return;
    }

    string lapangan;

    cout << "\nNama : " << userLogin << endl;
    cin.ignore();

    cout << "Pilih ID Lapangan : ";
    getline(cin, lapangan);

    if(cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');

        cout << "\nInput lapangan tidak valid!\n";
        return;
    }

    ifstream fileValidasi("lapangan.txt");

    string idFile;
    string namaFile;
    string hargaFile;
    string statusFile;

    bool lapanganDitemukan = false;
    int hargaLapangan = 0;

    while(getline(fileValidasi, idFile, '|'))
    {
        getline(fileValidasi, namaFile, '|');
        getline(fileValidasi, hargaFile, '|');
        getline(fileValidasi, statusFile);

        if(idFile == lapangan && statusFile == "ready")
        {
            lapanganDitemukan = true;
            hargaLapangan = stoi(hargaFile);
            break;
        }
    }

    fileValidasi.close();

    if(!lapanganDitemukan)
    {
        cout << "\nLapangan tidak tersedia!\n";
        return;
    }

    cout << "\n===== JAM MULAI TERSEDIA =====\n";

    int jamList[10];
    int jumlahJam = 0;

    for(int jamMulai = 18; jamMulai < 21; jamMulai++)
    {
        if(cekRentangTersedia(lapangan, jamMulai, jamMulai + 1))
        {
            jumlahJam++;
            jamList[jumlahJam] = jamMulai;

            cout << jumlahJam << ". " << formatJam(jamMulai) << endl;
        }
    }

    if(jumlahJam == 0)
    {
        cout << "\nTidak ada jam tersedia!\n";
        return;
    }

    int pilihJam;

    cout << "\nPilih nomor jam : ";
    cin >> pilihJam;

    if(pilihJam < 1 || pilihJam > jumlahJam)
    {
        cout << "\nPilihan jam tidak valid!\n";
        return;
    }

    int jamMulai = jamList[pilihJam];

    int durasi;

    cout << "Mau sewa berapa jam : ";
    cin >> durasi;

    if(durasi < 1)
    {
        cout << "\nDurasi tidak valid!\n";
        return;
    }

    int jamSelesai = jamMulai + durasi;

    if(jamSelesai > 21)
    {
        cout << "\nDurasi melewati batas jam 21:00!\n";
        return;
    }

    if(!cekRentangTersedia(lapangan, jamMulai, jamSelesai))
    {
        cout << "\nJam yang dipilih bertabrakan dengan booking lain!\n";
        return;
    }

    int id = 1;

    ifstream cek("booking.txt");
    string temp;

    getline(cek, temp);

    while(getline(cek, temp))
    {
        id++;
    }

    cek.close();

    string status = "pending";

    ofstream file("booking.txt", ios::app);

    file << id
         << "|"
         << userLogin
         << "|"
         << lapangan
         << "|"
         << formatJam(jamMulai)
         << "|"
         << durasi
         << "|"
         << formatJam(jamSelesai)
         << "|"
         << status
         << endl;

    file.close();

    int totalHarga = hargaLapangan * durasi;

    cout << "\nBooking berhasil dibuat!\n";
    cout << string(27, '=') << endl;
    cout << "ID Booking : " << id << endl;
    cout << "User       : " << userLogin << endl;
    cout << "Lapangan   : " << lapangan << endl;
    cout << "Waktu      : " << formatJam(jamMulai) << " - " << formatJam(jamSelesai) << endl;
    cout << "Durasi     : " << durasi << " Jam" << endl;
    cout << "Harga/Jam  : " << formatRupiahUser(hargaLapangan) << endl;
    cout << "Total      : " << formatRupiahUser(totalHarga) << endl;
    cout << "Status     : " << status << endl;
    cout << string(27, '=') << endl;
}

void riwayatBooking()
{
    buatHeaderBooking();

    ifstream file("booking.txt");

    string header;
    getline(file, header);

    string id;
    string user;
    string lap;
    string jamMulai;
    string durasi;
    string jamSelesai;
    string status;

    cout << "\n===== RIWAYAT BOOKING =====\n";

    bool adaBooking = false;

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jamMulai, '|');
        getline(file, durasi, '|');
        getline(file, jamSelesai, '|');
        getline(file, status);

        if(user == userLogin)
        {
            adaBooking = true;

            int hargaLapangan = 0;

            ifstream lapanganFile("lapangan.txt");

            string idLap;
            string namaLap;
            string hargaLap;
            string statusLap;

            while(getline(lapanganFile, idLap, '|'))
            {
                getline(lapanganFile, namaLap, '|');
                getline(lapanganFile, hargaLap, '|');
                getline(lapanganFile, statusLap);

                if(idLap == lap)
                {
                    hargaLapangan = stoi(hargaLap);
                    break;
                }
            }

            lapanganFile.close();

            int totalHarga = hargaLapangan * stoi(durasi);

            cout << "ID         : " << id << endl;
            cout << "Lapangan   : " << lap << endl;
            cout << "Jam        : "
                 << jamMulai
                 << " - "
                 << jamSelesai
                 << endl;

            cout << "Durasi     : "
                 << durasi
                 << " jam"
                 << endl;

            cout << "Harga/Jam  : "
                 << formatRupiahUser(hargaLapangan)
                 << endl;

            cout << "Total      : "
                 << formatRupiahUser(totalHarga)
                 << endl;

            cout << "Status     : "
                 << status
                 << endl;

            cout << "----------------------\n";
        }
    }

    file.close();

    if(!adaBooking)
    {
        cout << "\nBelum ada riwayat booking!\n";
    }
}

void ubahJadwal()
{
    riwayatBooking();

    string pilihId;

    cout << "\nMasukkan ID Booking : ";
    cin >> pilihId;

    ifstream file("booking.txt");

    string header;
    getline(file, header);

    string id;
    string user;
    string lap;
    string jamMulai;
    string durasi;
    string jamSelesai;
    string status;

    bool ditemukan = false;
    string lapanganUser;

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jamMulai, '|');
        getline(file, durasi, '|');
        getline(file, jamSelesai, '|');
        getline(file, status);

        if(id == pilihId && user == userLogin)
        {
            ditemukan = true;
            lapanganUser = lap;
            break;
        }
    }

    file.close();

    if(!ditemukan)
    {
        cout << "\nID booking tidak ditemukan!\n";
        return;
    }

    cout << string(27, '=') << endl;

    int jamList[10];
    int jumlahJam = 0;

    for(int jam = 18; jam < 21; jam++)
    {
        if(cekRentangTersediaKecualiId(lapanganUser, jam, jam + 1, pilihId))
        {
            jumlahJam++;
            jamList[jumlahJam] = jam;

            cout << jumlahJam << ". " << formatJam(jam) << endl;
        }
    }

    if(jumlahJam == 0)
    {
        cout << "\nTidak ada jam tersedia!\n";
        return;
    }

    int pilihJam;

    cout << "\nPilih nomor jam : ";
    cin >> pilihJam;

    if(pilihJam < 1 || pilihJam > jumlahJam)
    {
        cout << "\nPilihan jam tidak valid!\n";
        return;
    }

    int jamMulaiBaru = jamList[pilihJam];

    int durasiBaru;

    cout << "Durasi baru : ";
    cin >> durasiBaru;

    if(durasiBaru < 1)
    {
        cout << "\nDurasi tidak valid!\n";
        return;
    }

    int jamSelesaiBaru = jamMulaiBaru + durasiBaru;

    if(jamSelesaiBaru > 21)
    {
        cout << "\nDurasi melewati batas jam 21:00!\n";
        return;
    }

    if(!cekRentangTersediaKecualiId(lapanganUser, jamMulaiBaru, jamSelesaiBaru, pilihId))
    {
        cout << "\nJadwal bertabrakan!\n";
        return;
    }

    ifstream baca("booking.txt");
    ofstream temp("temp.txt");

    getline(baca, header);

    temp << "ID|USER|LAPANGAN|JAM_MULAI|DURASI|JAM_SELESAI|STATUS\n";

    bool berhasil = false;

    while(getline(baca, id, '|'))
    {
        getline(baca, user, '|');
        getline(baca, lap, '|');
        getline(baca, jamMulai, '|');
        getline(baca, durasi, '|');
        getline(baca, jamSelesai, '|');
        getline(baca, status);

        if(id == pilihId && user == userLogin)
        {
            jamMulai = formatJam(jamMulaiBaru);
            durasi = to_string(durasiBaru);
            jamSelesai = formatJam(jamSelesaiBaru);
            status = "pending";

            berhasil = true;
        }

        temp << id
             << "|"
             << user
             << "|"
             << lap
             << "|"
             << jamMulai
             << "|"
             << durasi
             << "|"
             << jamSelesai
             << "|"
             << status
             << endl;
    }

    baca.close();
    temp.close();

    remove("booking.txt");
    rename("temp.txt", "booking.txt");

    if(berhasil)
    {
        cout << "\nJadwal berhasil diubah!\n";
    }
}

void batalkanBooking()
{
    riwayatBooking();

    string pilihId;
    bool idDitemukan = false;

    cout << "\nMasukkan ID Booking : ";
    cin >> pilihId;

    ifstream file("booking.txt");
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

        if(id == pilihId && user == userLogin)
        {
            status = "cancel";
            idDitemukan = true;
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

    remove("booking.txt");
    rename("temp.txt", "booking.txt");

    if(idDitemukan)
        cout << "\nBooking berhasil dibatalkan!\n";
    else
        cout << "\nID booking tidak ditemukan atau bukan milik user ini!\n";
}

void menuUser()
{
    int pilih;

    do
    {
        cout << "\n===== MENU USER =====\n";

        cout << "1. Booking Lapangan\n";
        cout << "2. Ubah Jadwal\n";
        cout << "3. Batalkan Booking\n";
        cout << "4. Riwayat Booking\n";
        cout << "5. Exit\n";

        cout << "Pilih : ";
        cin >> pilih;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');

            cout << "\nInput Hanya Bisa Angka!\n";
            continue;
        }

        switch(pilih)
        {
            case 1:
                bookingLapangan();
                break;

            case 2:
                ubahJadwal();
                break;

            case 3:
                batalkanBooking();
                break;

            case 4:
                riwayatBooking();
                break;

            case 5:
                cout << "\nLogout...\n";
                return;

            default:
                cout << "\nPilihan tidak valid!\n";
        }

    } while(true);
}