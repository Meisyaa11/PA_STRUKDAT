#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdexcept>
#include <fstream> // create & open csv
#include <sstream>
#include <ctime>    // time and date
#include <cstdlib>  // random number, stoi
#include <stdlib.h> // stoi
#include <algorithm>
#include <bits/stdc++.h>
#include <string> // stoi, substr, length
using namespace std;

void SimpanKeFile();
int menu();
void confirmticket();

// konser
struct konser
{
    int id_konser;
    string nama_konser;
    string nama_artis;
    string lokasi;
    int day;
    int month;
    int year;
    string sudahDikonfirmasi; // untuk menandai tiket sudah di konfirmasi oleh admin apa belum
};

struct node
{
    konser konser;     // manggil struct
    node *next = NULL; // pointer ke data selanjutnya (linked list)
};

node *head = NULL;

struct nodeTicket
{
    konser data;
    string username;
    int jumlahTiket; // untuk menyimpan jumlah tiket yang dibeli
    nodeTicket *next = NULL;
};

nodeTicket *headTicket = NULL;
nodeTicket *headAcc = NULL;

int jumlah = 0;

// akun
struct akun
{
    string username, password;
    akun *next;
};

struct nodeAkun
{
    string username, password;
    nodeAkun *next = NULL;
};
string adminUsername = "a";
string adminPassword = "a";
nodeAkun *headAkun = NULL;
akun akun_p;
nodeAkun *Stored_akun_p = NULL;

string storedUsername, storedPassword;
string Username, Password; // menyimpan username dan password saat register
string siLogin;

// hitung panjang linked list
int size(node *head)
{
    int count = 0;
    node *temp = head;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

// ---------- ERROR HANDLING ---------- //
// cek jika user input string di int variable
bool check_num(string str)
{
    bool numeric = false;
    for (int i = 0; i < str.length(); i++)
    {
        if (isdigit(str[i]))
        {
            numeric = true;
            continue;
        }
        numeric = false;
        cerr << "\n[ Masukan tidak sesuai! ]\n\n";
        break;
    }
    return numeric;
}

// cek id sudah ada atau tidak
bool isIdExist(int id)
{
    node *current = head;
    while (current != nullptr)
    {
        if (current->konser.id_konser == id)
        {
            return false;
        }
        current = current->next;
    }
    return true;
}

// cek apakah inputan tanggal benar
bool isDateValid(string date)
{
    if (date.length() != 8 || date[2] != '-' || date[5] != '-') // format dd-mm-yy
    {
        return false; // format tanggal invalid
    }

    if (!check_num(date.substr(0, 2)) || !check_num(date.substr(3, 2)) || !check_num(date.substr(6, 2)))
    {
        return false; // bukan input numeric
    }

    // mengambil substring dari string utama
    int day = stoi(date.substr(0, 2));   // mengambil dua karakter pertama dari string date (indeks 0 dan 1)
    int month = stoi(date.substr(3, 2)); // indeks 3 dan 4
    int year = stoi(date.substr(6, 2));  // indeks 6 dan 7

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 23 || year > 30)
    {
        return false; // tanggal, bulan, atau tahun invalid
    }
    return true;
}

// konversi string ke uppercase
string toUpper(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        str[i] = toupper(str[i]);
    }
    return str;
}

// untuk mengambil nama bulan
string getMonth(int month)
{
    string months[] = {"JAN", "FEB", "MAR", "APR", "MEI", "JUN", "JUL", "AGU", "SEP", "OKT", "NOV", "DES"};
    if (month >= 1 && month <= 12)
    {
        return months[month - 1];
    }
    return "-";
}

// ---------- ADMIN ---------- //
// login admin

bool adminLogin()
{

    string inputUsername;
    string inputPassword;

    system("cls");
    cout << "=========================================" << endl;
    cout << "               Admin Login               " << endl;
    cout << "=========================================" << endl;
    cout << "Username  : ";
    getline(cin, inputUsername);
    cout << "Password  : ";
    getline(cin, inputPassword);

    if (inputUsername == adminUsername && inputPassword == adminPassword)
    {
        cout << "\n[ Login berhasil! Selamat datang, admin! ]\n\n";
        system("pause");
        return true;
    }
    else
    {
        cout << "\n[ Gagal login. Username atau password salah! ]\n\n";
        system("pause");
        return false;
    }
}

// tambah konser baru
node *newKonser()
{
    node *konserBaru = new node;

    system("cls");
    cout << "=========================================" << endl;
    cout << "           Tambah Data Konser            " << endl;
    cout << "=========================================" << endl;

    while (true)
    {
        string idStr;

        cout << "ID Konser          : ";
        getline(cin, idStr);

        if (check_num(idStr))
        {
            int id = stoi(idStr);

            if (isIdExist(id))
            {
                konserBaru->konser.id_konser = id;
                break;
            }
            else
            {
                cout << "\n[ ID Konser sudah ada! ]\n\n";
                system("pause");
                return NULL;
            }
        }
        else
        {
            system("pause");
            return NULL;
        }
    }

    cout << "Nama Konser        : ";
    getline(cin, konserBaru->konser.nama_konser);

    if (konserBaru->konser.nama_konser.empty())
    {
        cout << "\n[ Nama Konser tidak boleh kosong! ]\n\n";
        system("pause");
        return NULL;
    }

    cout << "Nama Artis         : ";
    getline(cin, konserBaru->konser.nama_artis);

    if (konserBaru->konser.nama_artis.empty())
    {
        cout << "\n[ Nama Artis tidak boleh kosong! ]\n\n";
        system("pause");
        return NULL;
    }

    cout << "Lokasi             : ";
    getline(cin, konserBaru->konser.lokasi);

    if (konserBaru->konser.lokasi.empty())
    {
        cout << "\n[ Lokasi Konser tidak boleh kosong! ]\n\n";
        system("pause");
        return NULL;
    }

    while (true)
    {
        string tanggal;

        cout << "Tanggal [DD-MM-YY] : ";
        getline(cin, tanggal);

        if (isDateValid(tanggal))
        {
            konserBaru->konser.day = stoi(tanggal.substr(0, 2));
            konserBaru->konser.month = stoi(tanggal.substr(3, 2));
            konserBaru->konser.year = stoi(tanggal.substr(6, 2));
            break;
        }
        else
        {
            cout << "\n[ Format tanggal tidak valid! ]\n\n";
            system("pause");
            return NULL;
        }
    }
    cout << "=========================================" << endl;
    cout << "\n\n[ Data berhasil ditambah! ]\n\n";
    system("pause");
    return konserBaru;
}

// tambah data konser (last)
void addkonser(node **head)
{
    node *konserBaru = newKonser();

    if (konserBaru != NULL)
    {
        if (*head == NULL)
        {
            *head = konserBaru;
        }
        else
        {
            node *temp = *head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = konserBaru;
        }
    }
    jumlah++;
}

// lihat data konser
void displaykonser(node *head)
{
    system("CLS");
    cout << "=========================================" << endl;
    cout << "              Lihat Konser               " << endl;
    cout << "=========================================" << endl;

    if (head == NULL)
    {
        cout << "\nData masih kosong.\n\n";
        cout << "-----------------------------------------" << endl;
        system("pause");
        return;
    }
    int i = 0;
    node *temp = head;
    while (temp != NULL)
    {
        cout << "[" << i + 1 << "]" << endl;
        cout << "ID Konser     : " << temp->konser.id_konser << endl;
        cout << "Nama Konser   : " << temp->konser.nama_konser << endl;
        cout << "Nama Artis    : " << temp->konser.nama_artis << endl;
        cout << "Lokasi        : " << temp->konser.lokasi << endl;
        cout << "Tanggal       : " << temp->konser.day << " " << getMonth(temp->konser.month) << " 20" << temp->konser.year << endl;
        cout << "-----------------------------------------" << endl;
        temp = temp->next;
        i++;
    }
    system("pause");
}

// ubah data konser
void editkonser(node *head)
{
    if (head != NULL)
    {
        cout << "\nEdit konser pada ID: ";
        string idEditStr;
        getline(cin, idEditStr);

        if (!check_num(idEditStr))
        {
            system("pause");
            return;
        }

        int idEdit = stoi(idEditStr);
        node *temp = head;
        bool found = false;

        while (temp != NULL)
        {
            if (temp->konser.id_konser == idEdit)
            {
                found = true;
                system("cls");
                cout << "=========================================" << endl;
                cout << "            Ubah Data Konser             " << endl;
                cout << "=========================================" << endl;
                cout << "1. Nama Konser" << endl;
                cout << "2. Nama Artis" << endl;
                cout << "3. Lokasi" << endl;
                cout << "4. Tanggal" << endl;
                cout << "=========================================" << endl;
                cout << "Ubah pada [1-4]    : ";

                string pilihStr;

                getline(cin, pilihStr);

                if (check_num(pilihStr))
                {
                    int pilih = stoi(pilihStr);
                    bool validOption = true;
                    string nama_konser, nama_artis, lokasi, newTanggal;

                    switch (pilih)
                    {
                    case 1:
                        cout << "\nNama Konser        : ";
                        getline(cin, nama_konser);
                        if (nama_konser.empty())
                        {
                            cout << "\n[ Nama Konser tidak boleh kosong! ]\n\n";
                            system("pause");
                            return;
                        }
                        else
                        {
                            temp->konser.nama_konser = nama_konser;
                        }
                        break;

                    case 2:
                        cout << "\nNama artis        : ";
                        getline(cin, nama_artis);
                        if (nama_artis.empty())
                        {
                            cout << "\n[ Nama artis tidak boleh kosong! ]\n\n";
                            system("pause");
                            return;
                        }
                        else
                        {
                            temp->konser.nama_artis = nama_artis;
                        }
                        break;
                    case 3:
                        cout << "\nLokasi             : ";
                        getline(cin, lokasi);
                        if (lokasi.empty())
                        {
                            cout << "\n[ Lokasi tidak boleh kosong! ]\n\n";
                            system("pause");
                            return;
                        }
                        else
                        {
                            temp->konser.lokasi = lokasi;
                        }
                        break;
                    case 4:
                        cout << "\nTanggal [DD-MM-YY] : ";
                        getline(cin, newTanggal);

                        if (isDateValid(newTanggal))
                        {
                            temp->konser.day = stoi(newTanggal.substr(0, 2));
                            temp->konser.month = stoi(newTanggal.substr(3, 2));
                            temp->konser.year = stoi(newTanggal.substr(6, 2));
                        }
                        else
                        {
                            cout << "\n[ Format tanggal tidak valid! ]\n\n";
                            system("pause");
                            return;
                        }
                        break;
                    default:
                        cout << "\n[ Pilihan tidak valid! ]\n\n";
                        validOption = false;
                        system("pause");
                        break;
                    }
                    if (validOption)
                    {
                        nodeTicket *temp2 = headTicket;
                        while (temp2 != NULL)
                        {
                            if (temp2->data.id_konser == idEdit)
                            {
                                // temp->konser
                                temp2->data = temp->konser;
                            }
                            temp2 = temp2->next;
                        }

                        nodeTicket *temp3 = headAcc;
                        while (temp3 != NULL)
                        {
                            if (temp3->data.id_konser == idEdit)
                            {
                                temp3->data.day = temp->konser.day;
                                temp3->data.id_konser = temp->konser.id_konser;
                                temp3->data.lokasi = temp->konser.lokasi;
                                temp3->data.month = temp->konser.month;
                                temp3->data.nama_artis = temp->konser.nama_artis;
                                temp3->data.nama_konser = temp->konser.nama_konser;
                                temp3->data.year = temp->konser.year;
                            }
                            temp3 = temp3->next;
                        }

                        cout << "\n\n[ Data berhasil diubah! ]\n\n";
                        system("pause");
                    }
                    return;
                }
                else
                {
                    system("pause");
                }
            }
            temp = temp->next;
        }
        if (!found)
        {
            cout << "\n[ ID Konser tidak ditemukan! ]\n\n";
            system("pause");
        }
    }
}

// hapus data konser (berdasarkan index)
void hapuskonser(node **head)
{
    if (*head != NULL)
    {
        cout << "\nHapus konser pada ID: ";
        string idDelStr;
        getline(cin, idDelStr);
        bool found = false;

        if (check_num(idDelStr))
        {
            int idDel = stoi(idDelStr);
            node *temp = *head;
            node *prev = NULL;
            nodeTicket *temp2 = headTicket;
            nodeTicket *temp3 = headAcc;

            while (temp != NULL)
            {
                if (temp->konser.id_konser == idDel)
                {
                    int dihapus = temp->konser.id_konser;
                    found = true;
                    cout << "\n[ ID Konser yang akan dihapus: " << idDel << " ]" << endl;
                    cout << "\nWARNING!!!" << endl;
                    cout << "Apakah Anda yakin ingin menghapus konser ini? [y/n] : ";

                    string confirm;
                    getline(cin, confirm);

                    if (toUpper(confirm) == "Y")
                    {
                        if (prev != NULL)
                        {
                            prev->next = temp->next;
                        }
                        else
                        {
                            *head = temp->next;
                        }
                        delete temp;
                        cout << "\n[ Data dengan ID Konser " << idDel << " berhasil dihapus! ]\n\n";
                        // Mengubah status tiket dari konser yang dihapus menjadi dibatalkan
                        while (temp2 != NULL)
                        {
                            if (temp2->data.id_konser == dihapus)
                            {
                                temp2->data.sudahDikonfirmasi = "2";
                            }
                            temp2 = temp2->next;
                        }
                        while (temp3 != NULL)
                        {
                            if (temp3->data.id_konser == dihapus)
                            {
                                temp3->data.sudahDikonfirmasi = "2";
                            }
                            temp3 = temp3->next;
                        }
                        system("pause");
                        return;
                    }
                    else if (toUpper(confirm) == "N")
                    {
                        cout << "\n[ Penghapusan konser dibatalkan! ]\n\n";
                        system("pause");
                        return;
                    }
                    else
                    {
                        cout << "\n[ Masukan hanya y (yes) atau n (no)! ]\n\n";
                        system("pause");
                        return;
                    }
                }
                prev = temp;
                temp = temp->next;
            }
            if (!found)
            {
                cout << "\n[ ID Konser tidak ditemukan! ]\n\n";
                system("pause");
            }
        }
        else
        {
            system("pause");
        }
    }
}

// lihat data pesanan
void lihatpesanan()
{
    nodeAkun *temp = headAkun;
    nodeTicket *temp2 = headTicket;

    system("cls");
    cout << "=========================================" << endl;
    cout << "              Data Pesanan               " << endl;
    cout << "=========================================" << endl;
    // jika blm ada yang membeli tiket
    if (headTicket == NULL && headAcc == NULL)
    {
        cout << "\nTidak ada pesanan.\n\n";
        cout << "-----------------------------------------" << endl;
        system("pause");
        return;
    }

    while (temp != NULL)
    {
        nodeTicket *temp2 = headTicket;
        while (temp2 != NULL)
        {
            if (temp->username == temp2->username)
            {
                cout << "Pembeli       : " << temp2->username << endl;
                cout << "ID Konser     : " << temp2->data.id_konser << endl;
                cout << "Nama Konser   : " << temp2->data.nama_konser << endl;
                cout << "Tanggal       : " << temp2->data.day << " " << getMonth(temp2->data.month) << " 20" << temp2->data.year << endl;
                cout << "Jumlah Tiket  : " << temp2->jumlahTiket << endl;
                if (temp2->data.sudahDikonfirmasi == "1")
                {
                    cout << "Status        : Sudah dikonfirmasi" << endl;
                }
                else if (temp2->data.sudahDikonfirmasi == "2")
                {
                    cout << "Status        : Dibatalkan" << endl;
                }
                else
                {
                    cout << "Status        : Belum dikonfirmasi" << endl;
                }
                cout << "-----------------------------------------" << endl;
            }
            temp2 = temp2->next;
        }
        nodeTicket *temp3 = headAcc;
        while (temp3 != NULL)
        {
            if (temp->username == temp3->username)
            {
                cout << "Pembeli       : " << temp3->username << endl;
                cout << "ID Konser     : " << temp3->data.id_konser << endl;
                cout << "Nama Konser   : " << temp3->data.nama_konser << endl;
                cout << "Tanggal       : " << temp3->data.day << " " << getMonth(temp3->data.month) << " 20" << temp3->data.year << endl;
                cout << "Jumlah Tiket  : " << temp3->jumlahTiket << endl;
                if (temp3->data.sudahDikonfirmasi == "1")
                {
                    cout << "Status        : Sudah dikonfirmasi" << endl;
                }
                else if (temp3->data.sudahDikonfirmasi == "2")
                {
                    cout << "Status        : Dibatalkan" << endl;
                }
                else
                {
                    cout << "Status        : Belum dikonfirmasi" << endl;
                }
                cout << "-----------------------------------------" << endl;
            }
            temp3 = temp3->next;
        }
        temp = temp->next;
    }
    system("pause");
    return;
}

// konfirmasi pesanan denga queue
void confirmticket()
{
    system("cls");
    cout << "Anda akan mengonfirmasi pesanan pembeli sesuai urutan...\n\n";
    Sleep(1000);
    //  Jika tidak ada data di antrian maka kembali
    if (headTicket == NULL)
    {
        cout << "\n[ Tidak ada pesanan yang bisa dikonfirmasi! ]\n\n";
        system("pause");
        return;
    }
    int batal = 0;

    nodeTicket *newAcc = new nodeTicket;
    newAcc->data = headTicket->data;
    newAcc->username = headTicket->username;
    if (newAcc->data.sudahDikonfirmasi == "2")
    {
        batal = 1;
    }
    else
    {
        newAcc->data.sudahDikonfirmasi = "1";
    }
    newAcc->jumlahTiket = headTicket->jumlahTiket; // set jumlah tiket yang dibeli
    newAcc->next = NULL;                           // pointer 'next' dari node baru menjadi nullptr
    nodeTicket *temp = headAcc;

    if (headAcc == NULL)
    {
        // jika linked list masih kosong, node baru menjadi node pertama
        headAcc = newAcc;
    }
    else
    {
        // menemukan node terakhir saat ini dalam linked list
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        // menambahkan node baru setelah node terakhir
        temp->next = newAcc;
    }
    // Menghapus data yg di acc dari antrian
    nodeTicket *hapus = headTicket;
    headTicket = (headTicket)->next;
    delete hapus;

    // untuk menampilkan data yg diconfirm
    if (batal == 0)
    {

        cout << "[ Berhasil dikonfirmasi! ]" << endl;
        cout << "-----------------------------------------" << endl;
        cout << "Pembeli       : " << newAcc->username << endl;
        cout << "ID Konser     : " << newAcc->data.id_konser << endl;
        cout << "Nama Konser   : " << newAcc->data.nama_konser << endl;
        cout << "Nama Artis    : " << newAcc->data.nama_artis << endl;
        cout << "Lokasi        : " << newAcc->data.lokasi << endl;
        cout << "Tanggal       : " << newAcc->data.day << " " << getMonth(newAcc->data.month) << " 20" << newAcc->data.year << endl;
        cout << "Jumlah Tiket  : " << newAcc->jumlahTiket << endl;
        cout << "-----------------------------------------" << endl;
    }
    else
    {
        cout << "[ Konser dibatalkan! ]" << endl;
        cout << "-----------------------------------------" << endl;
        cout << "Pembeli       : " << newAcc->username << endl;
        cout << "ID Konser     : " << newAcc->data.id_konser << endl;
        cout << "Nama Konser   : " << newAcc->data.nama_konser << endl;
        cout << "Nama Artis    : " << newAcc->data.nama_artis << endl;
        cout << "Lokasi        : " << newAcc->data.lokasi << endl;
        cout << "Tanggal       : " << newAcc->data.day << " " << getMonth(newAcc->data.month) << " 20" << newAcc->data.year << endl;
        cout << "Jumlah Tiket  : " << newAcc->jumlahTiket << endl;
        cout << "-----------------------------------------" << endl;
    }
    system("pause");
}

// menu admin
void menuAdmin(node **head)
{
    while (true)
    {
        string pilihStr;

        system("CLS");
        cout << "=========================================" << endl;
        cout << "               Menu Admin                " << endl;
        cout << "=========================================" << endl;
        cout << " 1. Tambah Data Konser                   " << endl;
        cout << " 2. Lihat Data Konser                    " << endl;
        cout << " 3. Ubah Data Konser                     " << endl;
        cout << " 4. Hapus Data Konser                    " << endl;
        cout << " 5. Lihat Data Pesanan                   " << endl;
        cout << " 6. Konfirmasi Pesanan                   " << endl;
        cout << " 7. Keluar                               " << endl;
        cout << "=========================================" << endl;
        cout << "Pilih menu [1-7] : ";
        getline(cin, pilihStr);

        if (check_num(pilihStr))
        {
            int pilih = stoi(pilihStr);

            switch (pilih)
            {
            case 1:
                addkonser(head);
                break;
            case 2:
                displaykonser(*head);
                break;
            case 3:
                displaykonser(*head);
                editkonser(*head);
                break;
            case 4:
                displaykonser(*head);
                hapuskonser(head);
                break;
            case 5:
                lihatpesanan();
                break;
            case 6:
                confirmticket();
                break;
            case 7:
                cout << "\n[ Keluar dari menu admin... ]\n\n";
                system("pause");
                return;
            default:
                cout << "\n[ Mohon masukan angka! ]\n\n";
                system("pause");
            }
        }
        else
        {
            system("pause");
        }
    }
}

// simpan data konser ke data.csv
void SimpanKeFile(node *head)
{
    node *temp = head;
    fstream file;
    file.open("data.csv", ios::out | ios::trunc); // menyimpan data ke file dengan cara menulis ulang atau overwrite atau menimpa
    while (temp != NULL)
    {

        file << temp->konser.id_konser << ",";
        file << temp->konser.nama_konser << ",";
        file << temp->konser.nama_artis << ",";
        file << temp->konser.lokasi << ",";
        file << temp->konser.day << ",";
        file << temp->konser.month << ",";
        file << temp->konser.year << ",";

        temp = temp->next;
        file << "\n";
    }
    file.close();
}

// ---------- USER ---------- //
// login user
string userLogin()
{
    string username, password;
    bool isLoggedIn = false;

    system("cls");
    cout << "=========================================" << endl;
    cout << "               User Login               " << endl;
    cout << "=========================================" << endl;
    cout << "Username  : ";
    getline(cin, username);
    cout << "Password  : ";
    getline(cin, password);

    // cek username dan password yang ada di dalam file
    ifstream file("akun.csv");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, storedUsername, ',');
            getline(ss, storedPassword, ',');

            if (username == storedUsername && password == storedPassword)
            {
                siLogin = username;
                isLoggedIn = true;
                break;
            }
        }
        file.close();
    }

    if (isLoggedIn)
    {
        cout << "\n[ Login berhasil! Selamat datang, " << username << "! ]\n\n";
        system("pause");
        return siLogin;
    }
    else
    {
        cout << "\n[ Gagal login. Username atau password salah! ]\n\n";
        system("pause");
        menu();
    }
}

// registrasi user
void userRegistration()
{
    system("cls");
    cout << "=========================================" << endl;
    cout << "              Registrasi User            " << endl;
    cout << "=========================================" << endl;
    cout << "Username  : ";
    getline(cin, akun_p.username);
    cout << "Password  : ";
    getline(cin, akun_p.password);

    if (akun_p.username.empty() || akun_p.password.empty())
    {
        cout << "\n[ Username dan password tidak boleh kosong! ]\n\n";
        system("pause");
        return; // Return from the function
    }

    if (akun_p.username == adminUsername)
    {
        cout << "\n[ Username sudah ada! ]\n\n";
        system("pause");
        menu();
    }

    fstream file("akun.csv", ios::in);
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, storedUsername, ',');
            getline(ss, storedPassword, ',');

            Username = storedUsername;
            // Mengecek apakah sudah ada username yang sama
            if (akun_p.username == Username)
            {
                cout << "\n[ Username sudah ada! ]\n\n";
                system("pause");
                file.close();
                menu();
            }
        }
        // jika tidak ada username yg sama maka melakukan append pada file
        fstream file("akun.csv", ios::app);
        file << akun_p.username << ',';
        file << akun_p.password << ',';
        file << "\n";

        file.close();
        cout << "\n[ Registrasi berhasil! ]\n\n";
        system("pause");
    }
    else
    {
        cout << "\n[ Gagal membuka file untuk penyimpanan data! ]\n\n";
        system("pause");
    }
}

// beli tiket
// (menambahkan tiket yang telah dibeli ke dalam linked list `headTicket`)
void beliTiket(nodeTicket **headTicket, konser konserData, int jumlahTiket, string siLogin)
{
    nodeTicket *newTicket = new nodeTicket;
    newTicket->data = konserData;
    newTicket->username = siLogin;
    newTicket->data.sudahDikonfirmasi = "0"; // set status pembelian tiket ke 'true'
    newTicket->jumlahTiket = jumlahTiket;    // set jumlah tiket yang dibeli
    newTicket->next = nullptr;               // pointer 'next' dari node baru menjadi nullptr

    // Jika linked list masih kosong, node baru menjadi node pertama
    if (*headTicket == nullptr)
    {
        // jika linked list masih kosong, node baru menjadi node pertama
        *headTicket = newTicket;
    }
    else
    {
        // Jika linked list tidak kosong, mencari node terakhir dalam linked list
        nodeTicket *temp = *headTicket;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        // menambahkan node baru setelah node terakhir
        temp->next = newTicket;
    }
}

void displaykonseruser(node *head)
{
    system("CLS");
    cout << "=========================================" << endl;
    cout << "              Lihat Konser               " << endl;
    cout << "=========================================" << endl;

    if (head == NULL)
    {
        cout << "\nData masih kosong.\n\n";
        cout << "-----------------------------------------" << endl;
        system("pause");
        return;
    }
    int i = 0;
    node *temp = head;
    // Menampilkan informasi konser dalam linked list
    while (temp != NULL)
    {
        cout << "[" << i + 1 << "]" << endl;
        cout << "Nama Konser   : " << temp->konser.nama_konser << endl;
        cout << "Nama Artis    : " << temp->konser.nama_artis << endl;
        cout << "Lokasi        : " << temp->konser.lokasi << endl;
        cout << "Tanggal       : " << temp->konser.day << " " << getMonth(temp->konser.month) << " 20" << temp->konser.year << endl;
        cout << "-----------------------------------------" << endl;
        temp = temp->next;
        i++;
    }
    system("pause");
}

void beliTiket(string siLogin)
{
    while (true)
    {
        system("CLS");
        cout << "=========================================" << endl;
        cout << "          Menu Pembelian Tiket          " << endl;
        cout << "=========================================" << endl;
        cout << "Pilihan Konser : " << endl;
        displaykonseruser(head); // menampilkan daftar konser yang tersedia

        string pilihStr;
        cout << "\nPilih nomor konser : ";
        getline(cin, pilihStr);

        // Memeriksa apakah input nomor konser valid
        if (check_num(pilihStr))
        {
            int pilih = stoi(pilihStr);

            // Memeriksa apakah nomor konser ada dalam rentang yang valid
            if (pilih >= 1 && pilih <= size(head))
            {
                node *selectedConcert = head;
                // mengambil konser yang dipilih berdasarkan nomor yang diinput user
                for (int i = 1; i < pilih; i++)
                {
                    selectedConcert = selectedConcert->next;
                }

                int jumlahtiket;
                cout << "Masukkan jumlah tiket yang ingin dibeli : ";

                string jumlahtiketStr;
                getline(cin, jumlahtiketStr);

                // Memeriksa apakah input jumlah tiket valid
                if (check_num(jumlahtiketStr))
                {
                    jumlahtiket = stoi(jumlahtiketStr);

                    // Menampilkan informasi pembelian tiket
                    system("CLS");
                    cout << "\n=========================================" << endl;
                    cout << "            Detail Pembelian            " << endl;
                    cout << "=========================================" << endl;
                    cout << "Nama Konser   : " << selectedConcert->konser.nama_konser << endl;
                    cout << "Jumlah Tiket  : " << jumlahtiket << endl;
                    cout << "=========================================" << endl;

                    cout << "Apakah Anda yakin ingin melanjutkan pembelian ini? [y/n] : ";

                    string confirm;
                    getline(cin, confirm);

                    // Inputan Konfirmasi
                    if (toUpper(confirm) == "Y")
                    {
                        beliTiket(&headTicket, selectedConcert->konser, jumlahtiket, siLogin); // menambahkan konser yang dibeli ke dalam linked list headTicket
                        cout << "\n[ Pembelian " << jumlahtiket << " tiket untuk konser " << pilih << " berhasil dilakukan! ]\n\n";
                        system("pause");
                        break; // Keluar dari loop setelah pembelian berhasil
                    }
                    else if (toUpper(confirm) == "N")
                    {
                        cout << "\n[ Pembelian tiket dibatalkan! ]\n\n";
                        system("pause");
                        break;
                    }
                    else
                    {
                        cout << "\n[ Masukan hanya y (yes) atau n (no)! Pembelian tiket dibatalkan. ]\n\n";
                        system("pause");
                        break;
                    }
                }
                else
                {
                    system("pause");
                    break;
                }
            }
            else
            {
                cout << "\n[ Nomor konser tidak ada! ]\n\n";
                system("pause");
                break;
            }
        }
        else
        {
            system("pause");
            break;
        }
    }
}

// searching (boyer-moore)
const int NO_OF_CHARS = 256;
void badCharHeuristic(string str, int size, int badChar[NO_OF_CHARS])
{
    for (int i = 0; i < NO_OF_CHARS; i++)
    {
        badChar[i] = -1;
    }

    for (int i = 0; i < size; i++)
    {
        badChar[(int)str[i]] = i;
    }
}

void BoyerMoore(node *head, string pat)
{
    node *temp = head;
    int count = 0;

    // mengubah input menjadi uppercase
    pat = toUpper(pat);

    while (temp != NULL)
    {
        int month = temp->konser.month;

        string monthStr = getMonth(month);

        // mengubah bulan menjadi uppercase
        monthStr = toUpper(monthStr);

        int m = pat.size();
        int n = monthStr.size();
        int badChar[NO_OF_CHARS];

        badCharHeuristic(pat, m, badChar);
        int s = 0;
        int i = 0;

        while (s <= (n - m))
        {
            int j = m - 1;

            while (j >= 0 && pat[j] == monthStr[s + j])
            {
                j--;
            }

            if (j < 0)
            {
                // Menampilkan informasi konser yang ditemukan
                cout << "[" << i + 1 << "]" << endl;
                cout << "ID Konser     : " << temp->konser.id_konser << endl;
                cout << "Nama Konser   : " << temp->konser.nama_konser << endl;
                cout << "Nama Artis    : " << temp->konser.nama_artis << endl;
                cout << "Lokasi        : " << temp->konser.lokasi << endl;
                cout << "Tanggal       : " << temp->konser.day << " " << getMonth(temp->konser.month) << " 20" << temp->konser.year << endl;
                cout << "-----------------------------------------" << endl;
                count++;
                i++;
                break;
            }
            else
            {
                s += max(1, j - badChar[monthStr[s + j]]);
            }
        }
        temp = temp->next;
    }

    if (count == 0)
    {
        cout << "[ Konser yang anda cari tidak ada! ]\n\n";
    }
}

// cari konser berdasarkan bulan
void cariKonser(node *head)
{
    string keyword;
    system("cls");
    cout << "Cari konser pada bulan [3 huruf awal bulan] : ";
    getline(cin, keyword);
    cout << "\n";
    // cek apakah input lebih dari 3
    if (keyword.length() != 3)
    {
        cout << "\n[ Input tidak valid! Masukkan 3 huruf awalan dari bulan. ]\n\n";
        system("pause");
        return;
    }

    // mengubah input menjadi uppercase
    keyword = toUpper(keyword);

    // menampilkan hasil pencarian menggunakan algoritma Boyer-Moore
    BoyerMoore(head, keyword);
    system("pause");
}

// sorting data (quick sort)
// Untuk menukar konser
void swapKonser(konser &a, konser &b)
{
    konser temp = a;
    a = b;
    b = temp;
}

// Untuk mengembalikan pointer ke node terakhir dalam linked list
node *tail(node *cur)
{
    while (cur != NULL && cur->next != NULL)
    {
        cur = cur->next;
    }
    return cur;
}

// Membagi linked list berasarkan elemen pivot
node *partition(node *head, node *end, node **newHead, node **newEnd)
{
    node *pivot = end;
    node *prev = NULL, *current = head, *tail = pivot;

    while (current != pivot)
    {
        if ((current->konser.year < pivot->konser.year) ||
            ((current->konser.year == pivot->konser.year) && (current->konser.month < pivot->konser.month)) ||
            ((current->konser.year == pivot->konser.year) && (current->konser.month == pivot->konser.month) && (current->konser.day <= pivot->konser.day)))
        {
            if ((*newHead) == NULL)
                (*newHead) = current;

            prev = current;
            current = current->next;
        }
        else
        {
            if (prev)
                prev->next = current->next;
            node *temp = current->next;
            current->next = NULL;
            tail->next = current;
            tail = current;
            current = temp;
        }
    }

    if ((*newHead) == NULL)
        (*newHead) = pivot;

    (*newEnd) = tail;
    return pivot;
}

// Melakukan pengurutan Quick Sort
node *quickSortRecur(node *head, node *end)
{
    if (!head || head == end)
        return head;

    node *newHead = NULL, *newEnd = NULL;
    node *pivot = partition(head, end, &newHead, &newEnd);

    if (newHead != pivot)
    {
        node *tmp = newHead;
        while (tmp->next != pivot)
            tmp = tmp->next;

        tmp->next = NULL;
        newHead = quickSortRecur(newHead, tmp);

        tmp = tail(newHead);
        tmp->next = pivot;
    }

    pivot->next = quickSortRecur(pivot->next, newEnd);
    return newHead;
}

void quickSort(node **headRef)
{
    (*headRef) = quickSortRecur(*headRef, tail(*headRef));
}

// melihat konser yang tersedia dan di sorting secara ascending berdasarkan tanggal konser
void lihatKonser(node *head)
{
    int count = 0;
    int i = 0;
    node *temp = head;

    cout << "=========================================" << endl;
    cout << "              Konser Tersedia           " << endl;
    cout << "=========================================" << endl;
    while (temp != nullptr)
    {
        cout << "[" << i + 1 << "]" << endl;
        cout << "ID Konser     : " << temp->konser.id_konser << endl;
        cout << "Nama Konser   : " << temp->konser.nama_konser << endl;
        cout << "Nama Artis    : " << temp->konser.nama_artis << endl;
        cout << "Lokasi        : " << temp->konser.lokasi << endl;
        cout << "Tanggal       : " << temp->konser.day << " " << getMonth(temp->konser.month) << " 20" << temp->konser.year << endl;
        cout << "-----------------------------------------" << endl;
        count++;
        i++;
        temp = temp->next;
    }
    system("pause");
}

// cek status tiket
void displayTicketStatus(nodeTicket *head, string siLogin)
{
    system("CLS");
    cout << "=========================================" << endl;
    cout << "             Status Tiket Anda           " << endl;
    cout << "=========================================" << endl;

    int i = 0;
    nodeTicket *temp = head;
    // Menampilkan informasi tiket dari linked list headTicket
    while (temp != NULL)
    {
        if (temp->username == siLogin)
        {
            cout << "Username      : " << temp->username << endl;
            cout << "Nama Konser   : " << temp->data.nama_konser << endl;
            cout << "Nama Artis    : " << temp->data.nama_artis << endl;
            cout << "Lokasi        : " << temp->data.lokasi << endl;
            cout << "Tanggal       : " << temp->data.day << " " << getMonth(temp->data.month) << " 20" << temp->data.year << endl;
            cout << "Jumlah Tiket  : " << temp->jumlahTiket << endl;
            // Menampilkan status konfirmasi
            if (temp->data.sudahDikonfirmasi == "1")
            {
                cout << "Status        : Sudah dikonfirmasi" << endl;
            }
            else if (temp->data.sudahDikonfirmasi == "2")
            {
                cout << "Status        : Dibatalkan" << endl;
            }
            else
            {
                cout << "Status        : Belum dikonfirmasi" << endl;
            }
            cout << "-----------------------------------------" << endl;
            i++;
        }
        temp = temp->next;
    }
    // Menampilkan informasi tiket dari linked list headAcc
    nodeTicket *temp2 = headAcc;
    while (temp2 != NULL)
    {
        if (temp2->username == siLogin)
        {
            cout << "Username      : " << temp2->username << endl;
            cout << "Nama Konser   : " << temp2->data.nama_konser << endl;
            cout << "Nama Artis    : " << temp2->data.nama_artis << endl;
            cout << "Lokasi        : " << temp2->data.lokasi << endl;
            cout << "Tanggal       : " << temp2->data.day << " " << getMonth(temp2->data.month) << " 20" << temp2->data.year << endl;
            cout << "Jumlah Tiket  : " << temp2->jumlahTiket << endl;
            if (temp2->data.sudahDikonfirmasi == "1")
            {
                cout << "Status        : Sudah dikonfirmasi" << endl;
            }
            else if (temp2->data.sudahDikonfirmasi == "2")
            {
                cout << "Status        : Dibatalkan" << endl;
            }
            else
            {
                cout << "Status        : Belum dikonfirmasi" << endl;
            }
            cout << "-----------------------------------------" << endl;
            i++;
        }
        temp2 = temp2->next;
    }

    // Belum membeli tiket
    if (i == 0)
    {
        cout << "\n         Anda Belum Membeli Tiket        \n"
             << endl;
        cout << "-----------------------------------------" << endl;
    }
    i = 0;
    system("pause");
}

// menu user
void userMenu(string siLogin)
{
    while (true)
    {
        string pilihStr;

        system("CLS");
        cout << "Halo, " << siLogin << "!" << endl;
        cout << "\n=========================================" << endl;
        cout << "               Menu Pengguna             " << endl;
        cout << "=========================================" << endl;
        cout << " 1. Menu Pembelian Tiket                " << endl;
        cout << " 2. Cari Konser                         " << endl;
        cout << " 3. Cek Status Tiket                    " << endl;
        cout << " 4. Lihat Konser                        " << endl;
        cout << " 5. Keluar                              " << endl;
        cout << "=========================================" << endl;
        cout << "Pilih menu [1-5] : ";
        getline(cin, pilihStr);

        if (check_num(pilihStr))
        {
            int pilih = stoi(pilihStr);

            switch (pilih)
            {
            case 1:
                if (head == NULL)
                {
                    cout << "\n[ Maaf, tidak ada konser yang tersedia saat ini. ]\n\n";
                    system("pause");
                }
                else
                {
                    beliTiket(siLogin);
                }
                break;
            case 2:
                if (head == NULL)
                {
                    cout << "\n[ Data kosong. Tidak dapat melakukan pencarian. ]\n\n";
                    system("pause");
                }
                else
                {
                    cariKonser(head);
                }
                break;
            case 3:
                displayTicketStatus(headTicket, siLogin);
                break;
            case 4:
                if (head == NULL)
                {
                    cout << "\n[ Data kosong. Belum ada konser. ]\n\n";
                    system("pause");
                }
                else
                {
                    system("cls");
                    quickSort(&head);
                    lihatKonser(head);
                }
                break;
            case 5:
                cout << "\n[ Keluar dari menu pengguna... ]\n\n";
                system("pause");
                return;
            default:
                cout << "\n[ Mohon masukan angka! ]\n\n";
                system("pause");
            }
        }
        else
        {
            system("pause");
        }
    }
}

// simpan data pemesanan konser ke tiket.csv
void simpanTiket()
{
    nodeTicket *temp = headTicket;
    nodeTicket *temp2 = headAcc;
    fstream file;
    file.open("tiket.csv", ios::out | ios::trunc); // menyimpan data ke file dengan cara menulis ulang atau overwrite atau menimpa
    while (temp != NULL)
    {

        file << temp->username << ",";
        file << temp->data.id_konser << ",";
        file << temp->data.nama_konser << ",";
        file << temp->data.nama_artis << ",";
        file << temp->data.lokasi << ",";
        file << temp->data.day << ",";
        file << temp->data.month << ",";
        file << temp->data.year << ",";
        file << temp->jumlahTiket << ",";
        file << temp->data.sudahDikonfirmasi << ",";

        temp = temp->next;
        file << "\n";
    }
    // Menyinmpan tiket yang sudah di confirm
    while (temp2 != NULL)
    {

        file << temp2->username << ",";
        file << temp2->data.id_konser << ",";
        file << temp2->data.nama_konser << ",";
        file << temp2->data.nama_artis << ",";
        file << temp2->data.lokasi << ",";
        file << temp2->data.day << ",";
        file << temp2->data.month << ",";
        file << temp2->data.year << ",";
        file << temp2->jumlahTiket << ",";
        file << temp2->data.sudahDikonfirmasi << ",";

        temp2 = temp2->next;
        file << "\n";
    }
    file.close();
}
// main menu
int menu()
{
    while (true)
    {
        string pilihStr;

        system("CLS");
        cout << "=========================================" << endl;
        cout << "             Manajemen Konser            " << endl;
        cout << "=========================================" << endl;
        cout << " 1. Login Admin                          " << endl;
        cout << " 2. Login User                           " << endl;
        cout << " 3. Registrasi User                      " << endl;
        cout << " 4. Keluar                               " << endl;
        cout << "=========================================" << endl;
        cout << "Pilih menu [1-4] : ";
        getline(cin, pilihStr);

        if (check_num(pilihStr))
        {
            int pilih = stoi(pilihStr);

            switch (pilih)
            {
            case 1:
                if (adminLogin())
                {
                    menuAdmin(&head);
                }
                break;
            case 2:

                userMenu(userLogin());
                break;
            case 3:
                userRegistration();
                break;
            case 4:
                cout << "\n[ Terima kasih! ]\n\n";
                SimpanKeFile(head);
                simpanTiket();
                exit(0);
            default:
                cout << "\n[ Menu tidak ada! ]\n\n";
                system("pause");
            }
        }
        else
        {
            system("pause");
        }
    }
}

int main()
{
    // membaca file csv
    ifstream file;
    file.open("data.csv");
    string data;
    if (file.is_open())
    {
        while (getline(file, data))
        {
            string id_konser_baru;
            string nama_konser_baru;
            string nama_artis_baru;
            string lokasi_baru;
            string day;
            int day_int;
            string month;
            int month_int;
            string year;
            int year_int;
            stringstream ss(data);

            getline(ss, id_konser_baru, ',');
            getline(ss, nama_konser_baru, ',');
            getline(ss, nama_artis_baru, ',');
            getline(ss, lokasi_baru, ',');
            getline(ss, day, ',');
            getline(ss, month, ',');
            getline(ss, year, ',');

            node *newNode = new node;

            stringstream id_konser_str(id_konser_baru);
            id_konser_str >> newNode->konser.id_konser;

            newNode->konser.nama_konser = nama_konser_baru;
            newNode->konser.nama_artis = nama_artis_baru;
            newNode->konser.lokasi = lokasi_baru;

            stringstream day_str(day);
            day_str >> day_int;
            newNode->konser.day = day_int;

            stringstream month_str(month);
            month_str >> month_int;
            newNode->konser.month = month_int;

            stringstream year_str(year);
            year_str >> year_int;
            newNode->konser.year = year_int;

            node *temp = head;

            if (head == NULL)
            {
                newNode->next = head;
                head = newNode;
            }
            else
            {
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }

            jumlah++;
        }
        file.close();
    }
    else
    {
        cout << "\n[ File tidak ditemukan! ]\n\n";
    }

    string storedUsername, storedPassword;
    fstream file2("tiket.csv");
    string line;

    if (file2.is_open())
    {
        while (getline(file2, line))
        {
            string id_konser;
            string nama_konser;
            string nama_artis;
            string lokasi;
            string sudahDikonfirmasi;
            string jumlahTiket;
            int jumlahTiket_int;
            string day;
            int day_int;
            string month;
            int month_int;
            string year;
            int year_int;
            stringstream ss(line);

            getline(ss, storedUsername, ',');
            getline(ss, id_konser, ',');
            getline(ss, nama_konser, ',');
            getline(ss, nama_artis, ',');
            getline(ss, lokasi, ',');
            getline(ss, day, ',');
            getline(ss, month, ',');
            getline(ss, year, ',');
            getline(ss, jumlahTiket, ',');
            getline(ss, sudahDikonfirmasi, ',');

            nodeTicket *newData = new nodeTicket;

            newData->username = storedUsername;

            stringstream id_konser_str(id_konser);
            id_konser_str >> newData->data.id_konser;

            newData->data.nama_konser = nama_konser;
            newData->data.nama_artis = nama_artis;
            newData->data.lokasi = lokasi;

            stringstream day_str(day);
            day_str >> day_int;
            newData->data.day = day_int;

            stringstream month_str(month);
            month_str >> month_int;
            newData->data.month = month_int;

            stringstream year_str(year);
            year_str >> year_int;
            newData->data.year = year_int;

            stringstream jumlahTiket_string(jumlahTiket);
            jumlahTiket_string >> jumlahTiket_int;
            newData->jumlahTiket = jumlahTiket_int;

            newData->data.sudahDikonfirmasi = sudahDikonfirmasi;
            if (newData->data.sudahDikonfirmasi == "0")
            {

                if (headTicket == NULL)
                {
                    headTicket = newData;
                }
                else
                {
                    nodeTicket *temp = headTicket;
                    while (temp->next != NULL)
                    {
                        temp = temp->next;
                    }
                    temp->next = newData;
                }
            }
            else
            {
                if (headAcc == NULL)
                {
                    headAcc = newData;
                }
                else
                {
                    nodeTicket *temp2 = headAcc;
                    while (temp2->next != NULL)
                    {
                        temp2 = temp2->next;
                    }
                    temp2->next = newData;
                }
            }
        }

        file2.close();
    }
    else
    {
        cout << "\n[ Gagal membuka file untuk penyimpanan data! ]\n\n";
    }

    ifstream file3;
    file3.open("akun.csv");

    if (file3.is_open())
    {
        while (getline(file3, line))
        {

            stringstream ss(line);
            getline(ss, storedUsername, ',');
            getline(ss, storedPassword, ',');

            nodeAkun *newAkun = new nodeAkun;
            newAkun->username = storedUsername;
            newAkun->password = storedPassword;

            if (headAkun == NULL)
            {
                newAkun->next = headAkun;
                headAkun = newAkun;
            }
            else
            {
                nodeAkun *temp = headAkun;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }

                temp->next = newAkun;
            }
        }

        file3.close();
    }
    else
    {
        cout << "\nGagal membuka file untuk penyimpanan data.\n";
    }

    system("cls");
    menu();

    return 0;
}