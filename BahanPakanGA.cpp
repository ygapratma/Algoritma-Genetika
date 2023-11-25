// Versi Paling Baru, 14 November 2023 10:20

#include <iostream>
#include <bits/stdc++.h>
// #define cross_prob 0.1
#define mut_rate 0.05
#define generation 100
#define population 75

using namespace std;

string data_makanan[15];
int kkal_given[15];
int harga[15];
int berat_ayam;
// string population[100];
string pagi[population];
string sore[population];
double f_value[population];
int kkal_target = 1620 - berat_ayam; // 1620 itu berat rata2 kalo liat di Google :)

vector<int> bstoia(const string &bin_str) // Binary String to Integer Array Using Vector
{
    vector<int> result;
    for (char digit : bin_str)
        result.push_back(digit - '0');
    return (result);
}

string iatobs(const vector<int> &int_arr) // Integer Array to Binary String Using Vector
{
    string result;
    for (int value : int_arr)
    {
        result += to_string(value);
    }
    return result;
}

void f_val()
{
    for (int i = 0; i < population; i++)
    {
        int p1 = stoi(pagi[i], 0, 2);
        int p2 = stoi(sore[i], 0, 2);

        double sum_kkal = kkal_given[p1] + kkal_given[p2];
        double sum_harga = harga[p1] + harga[p2];

        f_value[i] = fabs(kkal_target - sum_kkal) + (sum_harga / 1000);
    }
}

void pop_init()
{
    srand(time(0));
    for (int i = 0; i < population; i++)
    {
        int r = (rand() % 15) + 1;
        pagi[i] = bitset<4>(r).to_string();
    }
    for (int i = 0; i < population; i++)
    {
        int r = (rand() % 15) + 1;
        sore[i] = bitset<4>(r).to_string();
    }
}

void crossover(int p1, int p2)
{
    srand(time(0));
    vector<int> parent1 = bstoia(pagi[p1] + sore[p1]);
    vector<int> parent2 = bstoia(pagi[p2] + sore[p2]);
    // float r_f = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    int c_point = rand() % 8;

    for (int i = 0; i < c_point; ++i)
    {
        swap(parent1[i], parent2[i]);
    }

    string child1 = iatobs(parent1);
    string child2 = iatobs(parent2);

    pagi[p1] = child1.substr(0, 4);
    sore[p1] = child1.substr(4, 4);
    pagi[p2] = child2.substr(0, 4);
    sore[p2] = child2.substr(4, 4);
}

void mutate(int individual)
{
    vector<int> toMutate = bstoia(pagi[individual] + sore[individual]);
    for (int i = 0; i < toMutate.size(); ++i)
    {
        srand(time(0));
        double rand_val = static_cast<double>(rand()) / RAND_MAX;

        if (rand_val < mut_rate)
        {
            toMutate[i] = 1 - toMutate[i];
        }
    }

    string results = iatobs(toMutate);
    pagi[individual] = results.substr(0, 4);
    sore[individual] = results.substr(4, 4);
}

int r_wheel()
{
    srand(time(0));
    double total_f = 0.0;
    for (int i = 0; i < population; i++)
    {
        total_f += f_value[i];
    }

    double randVal = static_cast<double>(rand()) / RAND_MAX * total_f;

    double cum_f = 0.0;
    for (int i = 0; i < population; ++i)
    {
        cum_f += f_value[i];
        if (randVal <= cum_f)
        {
            return i;
        }
    }
}

void algogen()
{
    for (int i = 0; i < generation + 1; i++)
    {
        // fitness untuk generasi ke i
        f_val();

        // bangun populasi generasi ke i+1
        int p1 = r_wheel(), p2 = r_wheel();
        crossover(p1, p2);
        for (int j = 0; j < population; j++)
        {
            if (j != p1 || j != p2)
                mutate(j);
        }
    }
}

void readDataFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    string flavorText;
    getline(inputFile, flavorText);
    for (int i = 0; i < 15 && inputFile; ++i) {
        inputFile >> quoted(data_makanan[i]) >> kkal_given[i] >> harga[i];
    }

    inputFile.close();
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    string filename = argv[1];
    readDataFromFile(filename);

    cout << "Selamat Datang!" << endl;
    cout << "Anda Sekarang Menggunakan Data Dari File: " << filename << endl;
    cout << "Input Berat Ayam: ";
    cin >> berat_ayam;

    cout << "\nBerikut Ini Adalah Rekomendasi Dari Kami" << endl;

    pop_init();
    algogen();

    double firstBest = 10000.0;
    double secondBest = 10000.0;
    double thirdBest = 10000.0;

    vector<int> best_indices = {-1, -1, -1};
    for (int i = 0; i < population; i++)
    {
        if (best_indices[0] == -1 || f_value[i] < f_value[best_indices[0]])
        {
            best_indices[2] = best_indices[1];
            best_indices[1] = best_indices[0];
            best_indices[0] = i;
        }
        else if (best_indices[1] == -1 || f_value[i] < f_value[best_indices[1]])
        {
            best_indices[2] = best_indices[1];
            best_indices[1] = i;
        }
        else if (best_indices[2] == -1 || f_value[i] < f_value[best_indices[2]])
        {
            best_indices[2] = i;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        int index_pagi = stoi(pagi[best_indices[i]], nullptr, 2);
        int index_sore = stoi(sore[best_indices[i]], nullptr, 2);

        cout << "\nKombinasi-" << i + 1 << ":" << endl;
        cout << "Bahan Pakan Pagi : " << data_makanan[index_pagi] << endl;
        cout << "   - Kalori      : " << kkal_given[index_pagi] << endl;
        cout << "   - Harga       : " << harga[index_pagi] << endl;

        cout << "Bahan Pakan Sore : " << data_makanan[index_sore] << endl;
        cout << "   - Kalori      : " << kkal_given[index_sore] << endl;
        cout << "   - Harga       : " << harga[index_sore] << endl;

        cout << "Fitness          : " << f_value[best_indices[i]] << endl;

        double total_kalori = kkal_given[index_pagi] + kkal_given[index_sore];
        double total_harga = harga[index_pagi] + harga[index_sore];

        cout << "Total Kalori     : " << total_kalori << "kkal/20g" << endl;
        cout << "Total Harga      : " << "Rp" << total_harga << "/20g" << endl;
        cout << " " << endl;
    }

    return 0; 
}
