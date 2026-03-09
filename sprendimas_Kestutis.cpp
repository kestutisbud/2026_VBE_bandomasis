#include <bits/stdc++.h>
using namespace std;

struct mokinys
{
    string vardas; //vardas pavardë
    string kodas; //mokinio kodas
    string klase;
};

struct pasirinkimai
{
    string kodas;           //mokinio kodas
    vector <string> dalykai; //pasirinkti dalykai
};

struct dalykai
{
    string pav;
    int kiekis;
};
//-------Rikiavimo tvarka
bool tvarka (dalykai a, dalykai b)
{
    return a.kiekis > b.kiekis || (a.kiekis == b.kiekis && a.pav < b.pav);
}
//------FUNKCIJA gražinanti skaitmenų sumą
int SkaitmenuSuma(string kodas)
{
    int suma = 0;
    for (int i = 6; i < 10; i++) {
        suma += kodas[i] - '0';
    }
    return suma;
}
//------FUNKCIJA patikrinanti ar mokinio kodo skaitmenų suma lygi 15
bool ArSkaitmenuSuma(mokinys M)
{
    return (SkaitmenuSuma(M.kodas) == 15);
}
//------Skaièiavimas kiek neteisingų mokinių kodų
int NeteisinguKiekis(mokinys M[], int n)
{
    int kiek = 0;
    for (int i=0; i<n; i++)
    {
        if (!ArSkaitmenuSuma(M[i])) //nelygu 15
            kiek++;
    }
    return kiek;
}
//-------Funkcija randanti ar tesingai pasirinko mokinys
bool TeisingasPasirinkimas(pasirinkimai P)
{
    bool mat = 0, bl = 0;
    for (int i=0; i<P.dalykai.size();i++)
    {
        if (P.dalykai[i]=="MAT")  mat = 1;
        if (P.dalykai[i]=="BL") bl = 1;
    }
    return (mat && bl && P.dalykai.size()>=3 && P.dalykai.size()<=7);
}

int main()
{
    mokinys M[500]; //mokiniø duomenø masyvas
    pasirinkimai P[500]; //mokinio pasirinkimai
    int n, a;          //mokiniu kiekis ir uþpildytø anketø kiekis
    int k12a = 0, k12b = 0, k12c = 0; //klasių kiekis
    map <string, int> D; //dalykai
    vector <dalykai> Da;


    ifstream fin("Duomenys.txt");
    ofstream fout("Rezultatai.txt");
    ///-------------------------SKAITYMAS--------------------------

    fin >> n >> ws;
    //---------Skaitymas mokiniu duomenu--------
    for (int i=0; i<n; i++)
    {
        getline(fin, M[i].vardas,';');
        getline(fin, M[i].kodas,';');
        fin >> M[i].klase >> ws;
        if (M[i].klase == "12a") k12a++; //klasių skaičiavimas
        else if (M[i].klase == "12b") k12b++;
        else if (M[i].klase == "12c") k12c++;
    }
    //-------Skaitymas pasirinkimu--------------
    string eilute;
    string kodas, dalykai;
    a = 0; //neturim uþpildytø anketø
    while (getline(fin, eilute))
    {
        if (eilute.empty()) continue;

        stringstream ss(eilute);
        getline(ss, kodas,';');
        P[a].kodas = kodas;
        while(getline(ss, dalykai,';'))
        {
            while (!dalykai.empty() && (dalykai.back() == '\n' || dalykai.back() == '\r')) //pašalinama eilutės pabaiga Windows/Linux
                dalykai.pop_back();
            P[a].dalykai.push_back(dalykai);
            D[dalykai]++; //skaičiuojame kiek pasirinko dalykų
        }
        a++; //anketø skaièius
    }
    ///---------------SKAIÈIAVIMAI 12a, 12b, 12c klasiø
    fout << k12a <<" "<< k12b <<" "<< k12c << endl;
    fout << NeteisinguKiekis(M, n) << endl;
    //----------Spausdinimas neteisingu kodu--------------------
    for (int i=0; i<n; i++)
    {
       if (!ArSkaitmenuSuma(M[i])) //nelygu 15
            fout << M[i].vardas << " " <<M[i].kodas <<" "<< SkaitmenuSuma(M[i].kodas)<< endl;
    }
    //----------Spausdinimas neteisingu pasirinkimu------------
    for (int i=0; i<n; i++) //eina per sarasa
    {
        //---------Eina per pasirinkimus-------
        for (int j=0; j<a; j++)
        {
           if ( M[i].kodas == P[j].kodas && !TeisingasPasirinkimas(P[j])) //jei sutampa kodai ir neteisingas pasirinkimas
                fout << M[i].vardas << endl;
        }
    }
    //-------Dalyku pasirinkimu kiekis
    for (auto i: D)
    {
        Da.push_back({i.first, i.second}); //susidedam iš map į paprastą struktūrą
    }
    sort(Da.begin(), Da.end(), tvarka);

    for (auto i: Da)
        fout << i.pav <<" "<<i.kiekis << endl;

    fin.close();
    fout.close();
    return 0;
}

