#include <bits/stdc++.h>
using namespace std;

struct Mokinys {
    string vardas;
    string kodas;
    string klase;
    vector<string> dalykai;
};

struct DalykasInfo {
    string kodas;
    int kiekis;
};

bool RikiuotiDalykus(const DalykasInfo& a, const DalykasInfo& b) {
    return a.kiekis > b.kiekis || (a.kiekis == b.kiekis && a.kodas < b.kodas);
}

int SkaitmenuSuma(const string& kodas) {
    int suma = 0;
    for (int i = 6; i < 10; i++) {
        suma += kodas[i] - '0';
    }
    return suma;
}

bool TeisingasPasirinkimas(const vector<string>& dalykai) {
    bool yraMAT = false, yraBL = false;

    for (const string& d : dalykai) {
        if (d == "MAT") yraMAT = true;
        if (d == "BL") yraBL = true;
    }

    return yraMAT && yraBL && dalykai.size() >= 3 && dalykai.size() <= 7;
}

int main() {
    ifstream fin("Duomenys.txt");
    ofstream fout("Rezultatai.txt");

    int n;
    fin >> n >> ws;

    vector<Mokinys> mokiniai(n);
    map<string, int> indeksas;

    int k12a = 0, k12b = 0, k12c = 0;

    // Nuskaitomi mokiniai
    for (int i = 0; i < n; i++) {
        getline(fin, mokiniai[i].vardas, ';');
        getline(fin, mokiniai[i].kodas, ';');
        fin >> mokiniai[i].klase >> ws;

        indeksas[mokiniai[i].kodas] = i;

        if (mokiniai[i].klase == "12a") k12a++;
        else if (mokiniai[i].klase == "12b") k12b++;
        else if (mokiniai[i].klase == "12c") k12c++;
    }

    map<string, int> dalykuKiekiai;
    string eilute;

    // Nuskaitomi pasirinkimai
    while (getline(fin, eilute)) {
        if (eilute.empty()) continue;

        stringstream ss(eilute);
        string mokinioKodas, dalykas;

        getline(ss, mokinioKodas, ';');

        if (!indeksas.count(mokinioKodas)) continue;

        int idx = indeksas[mokinioKodas];

        while (getline(ss, dalykas, ';')) {
            while (!dalykas.empty() && (dalykas.back() == '\r' || dalykas.back() == '\n')) {
                dalykas.pop_back();
            }
            mokiniai[idx].dalykai.push_back(dalykas);
            dalykuKiekiai[dalykas]++;
        }
    }

    // 4. Klasių kiekiai
    fout << k12a << " " << k12b << " " << k12c << "\n";

    // 5. Neteisingi ID
    int bloguID = 0;
    for (const auto& m : mokiniai) {
        if (SkaitmenuSuma(m.kodas) != 15) bloguID++;
    }

    fout << bloguID << "\n";

    for (const auto& m : mokiniai) {
        int suma = SkaitmenuSuma(m.kodas);
        if (suma != 15) {
            fout << m.vardas << " " << m.kodas << " " << suma << "\n";
        }
    }

    // 6. Neteisingi pasirinkimai
    for (const auto& m : mokiniai) {
        if (!m.dalykai.empty() && !TeisingasPasirinkimas(m.dalykai)) {
            fout << m.vardas << "\n";
        }
    }

    // 7. Dalykų sąrašas
    vector<DalykasInfo> sarasas;
    for (auto x : dalykuKiekiai) {
        sarasas.push_back({x.first, x.second});
    }

    sort(sarasas.begin(), sarasas.end(), RikiuotiDalykus);

    for (const auto& x : sarasas) {
        fout << x.kodas << " " << x.kiekis << "\n";
    }

    return 0;
}
