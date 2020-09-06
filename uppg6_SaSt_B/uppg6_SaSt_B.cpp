//
//  main.cpp
//  uppg6_SaSt_B
//
//  Created by Samuel Ström on 2020-08-14.
//  Copyright © 2020 Samuel Ström. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
using namespace std;


// TRANSAKTION
class Transaktion
{
private:
    string datum;
    string typ;
    string namn;
    double belopp;
    int ant_kompisar;
    string *kompisar;
    
public:
    Transaktion();
    Transaktion(string d, string t, string n, double b, int antKompisar, string *kompisLista);
    ~Transaktion();
    void assign( const Transaktion& t);
    string haemta_namn();
    double haemta_belopp();
    int haemta_ant_kompisar();
    bool finnsKompis( string namnet );
    bool laesEnTrans( istream &is );
    void skrivEnTrans( ostream &os );
};

// TRANSAKTIONSLISTA
class TransaktionsLista
{
private:
    Transaktion *trans;
    int antalTrans;
    
public:
    TransaktionsLista();
    ~TransaktionsLista();
    void laesIn( istream & is );
    void skrivUt( ostream & os );
    void laggTill( Transaktion & t );
    double totalKostnad();
    double liggerUteMed( string namnet );
    double aerSkyldig( string namnet );
    class PersonLista FixaPersoner();
    int antalTransaktioner();
};

// PERSON
class Person
{
private:
    string namn;
    double betalat_andras;
    double skyldig;
    
public:
    Person();
    Person( string n, double b, double s );
    double haemta_betalat();
    double haemta_skyldig();
    void skrivUt();
    string haemta_namn();
    void assign( const Person& t);
};

class PersonLista
{
private:
    int antal_pers;
    Person *pers;
    
public:
    PersonLista();
    ~PersonLista();
    void laggTillEn( Person pny );
    void skrivUtOchFixa();
    double summaSkyldig();
    double summaBetalat();
    bool finnsPerson(const string& namn);
    double personSkyldig(string namn);
    double personBetalat(string namn);
};


int main() {
    int val = 0;
    string filNamn = "", namn = "";
    
    cout << "Startar med att läsa från en fil." << endl;
    ifstream fin("resa.txt");
    
    TransaktionsLista tList;
    tList.laesIn(fin);
    PersonLista personList = tList.FixaPersoner();

    cout << "\nVälj i menyn nedan: " << endl;
    cout << "0. Avsluta. Alla transaktioner sparas på fil." << endl;
    cout << "1. Läs in en transaktion från tangentbordet." << endl;
    cout << "2. Skriv ut information om alla transaktioner." << endl;
    cout << "3. Beräkna totala kostnaden." << endl;
    cout << "4. Hur mycket är en viss person skyldig?" << endl;
    cout << "5. Hur mycket ligger en viss person ute med?" << endl;
    cout << "6. Lista alla personer mm och FIXA!!!" << endl;

    cin >> val;
    
    while (val >= 0 && val <= 6) {

        if (val == 0) {
            cout << "Välj ett filnamn att spara filen på: ";
            cin >> filNamn;
            ofstream fout(filNamn);

            tList.skrivUt(fout);
            break;
        }
        else if (val == 1){
            string datum = "", typ = "", pNamn = "", kompisNamn;
            double belopp = 0;
            int antKompisar = 0;
            string *kompisLista;
            
            cout << "Mata in datum, typ, namn, belopp, antal kompisar och kompisars namn: ";
            cin >> datum >> typ >> pNamn >> belopp >> antKompisar;
            
            kompisLista = new string[antKompisar];
            for (int i=0; i<antKompisar; i++) {
                cin >> kompisNamn;
                kompisLista[i] = kompisNamn;
            }
            
            Transaktion t(datum, typ, pNamn, belopp, antKompisar, kompisLista);
            delete [] kompisLista;
            tList.laggTill(t);
            
            // Så att listan korrigeras efter en nyinlagd transaktion uppdateras rätt varje gång.
            personList = tList.FixaPersoner();
        }
        else if (val == 2){
            cout << "Antal trans = " << tList.antalTransaktioner() << endl;
            tList.skrivUt(cout);
        }
        else if (val == 3){
            cout << "\nTotalkostnaden är: " << tList.totalKostnad() << endl;
        }
        else if (val == 4){
            cout << "Mata in namnet på personen: ";
            cin >> namn;
            cout << namn << " är skyldig: " << personList.personSkyldig(namn) << endl;
        }
        else if (val == 5){
            cout << "Mata in namnet på personen: ";
            cin >> namn;
            cout << namn << " ligger ute med : " << personList.personBetalat(namn) << endl;
        }
        else if (val == 6){
            personList.skrivUtOchFixa();
        }
        
        cin >> val;
    }

    return 0;
}

//KONSTRUKTORER FÖR KLASSEN TRANSAKTION

Transaktion::Transaktion(){
    kompisar = 0;
};

Transaktion::Transaktion(string d, string t, string n, double b, int antKompisar, string *kompisLista)
: datum(d), typ(t), namn(n), belopp(b), ant_kompisar(antKompisar)
{
    kompisar = new string[antKompisar];
    for (int i = 0; i<antKompisar; i++) {
        kompisar[i] = kompisLista[i];
    }
};

Transaktion::~Transaktion(){
};

// FUNKTIONER FÖR KLASSEN TRANSAKTION

void Transaktion::assign(const Transaktion & t)
{
    if (this != &t){
        datum = t.datum;
        typ = t.typ;
        namn = t.namn;
        belopp = t.belopp;
        ant_kompisar = t.ant_kompisar;
        
        delete [] kompisar;
        if (ant_kompisar > 0)
        {
            kompisar = new string[ant_kompisar];
            for (int i = 0; i < ant_kompisar; i++)
                kompisar[i] = t.kompisar[i];
        }
        
        else
            kompisar = NULL;
    }
}

string Transaktion::haemta_namn(){
    return namn;
}

double Transaktion::haemta_belopp(){
    return belopp;
}
int Transaktion::haemta_ant_kompisar(){
    return ant_kompisar;
}

// Letar igenom arrayen och returnerar true om namnet finns, annars false
bool Transaktion::finnsKompis( string namnet ){
    for (int i=0; i<ant_kompisar; i++) {
        if (namnet == kompisar[i]) {
            return true;
        }
    }
    return false;
}

bool Transaktion::laesEnTrans( istream &is )
{
    double doubleTemp;
    int temp;      // tempor‰r variabel
    string strTemp;
    
    if (!(is >> strTemp))
        return false;
    datum = strTemp;
    
    if (!(is >> strTemp))
        return false;
    typ = strTemp;
    
    if (!(is >> strTemp))
        return false;
    namn = strTemp;
    
    if (!(is >> doubleTemp))
        return false;
    belopp = doubleTemp;
    
    if (!(is >> temp))
        return false;
    ant_kompisar = temp;
    
    kompisar = new string[ant_kompisar];
    for (int i=0; i<ant_kompisar; i++) {
        if (!(is >> strTemp))
        return false;
        
        kompisar[i] = strTemp;
    }
    return true;
}

void Transaktion::skrivEnTrans( ostream &os ){
    os << datum << " ";
    os << typ << " ";
    os << namn << " ";
    os << belopp << " ";
    os << ant_kompisar << " ";
    
    for (int i=0; i<ant_kompisar; i++) {
        os << kompisar[i] << " ";
    }
    os << endl;
}
// ***TRANSAKTIONSLISTA***

TransaktionsLista::TransaktionsLista(){
    antalTrans = 0;
    trans = NULL;
};
TransaktionsLista::~TransaktionsLista(){};

void TransaktionsLista::laesIn( istream & is ){
    Transaktion t;
    
    // Så länge det går bra att läsa (filen ej slut)
    while ( t.laesEnTrans( is ) )
    {
        laggTill( t );
    }
}
void TransaktionsLista::skrivUt( ostream & os ){
    for (int i=0; i<antalTrans; i++) {
        trans[i].skrivEnTrans(os);
    }
}

void TransaktionsLista::laggTill(Transaktion & nyTrans)
{
    antalTrans++;
    
    // Allokerar ny array
    Transaktion* tempTrans = new Transaktion[antalTrans];
    //Kopierar objekt med assign-funktionen
    for (int i = 0; i < antalTrans - 1; i++){
        tempTrans[i].assign(trans[i]);
    }
    // Deallokerar
    delete[] trans;
    
    //trans pekar på ny
    trans = tempTrans;
    
    //Adderar det nya objektet
    trans[antalTrans-1].assign(nyTrans);
}

double TransaktionsLista::totalKostnad(){
    double totalKostnad = 0;
    for (int i = 0; i < antalTrans; i++) {
        totalKostnad += trans[i].haemta_belopp();
    }
    return totalKostnad;
}
double TransaktionsLista::liggerUteMed( string namnet ){
    
    double liggerUteMed = 0;
    double belopp = 0;
    for (int i = 0; i < antalTrans; i++) {
        if (trans[i].haemta_namn() == namnet) {
            belopp = trans[i].haemta_belopp()/(trans[i].haemta_ant_kompisar()+1);
            liggerUteMed += belopp * trans[i].haemta_ant_kompisar();
        }
    }
    return liggerUteMed;
}
    
double TransaktionsLista::aerSkyldig( string namnet ){
    double arSkyldig = 0;
    for (int i=0; i<antalTrans; i++) {
        if (trans[i].finnsKompis(namnet)) {
            arSkyldig += trans[i].haemta_belopp()/(trans[i].haemta_ant_kompisar()+1);
        }
    }
    return arSkyldig;
}

PersonLista TransaktionsLista::FixaPersoner(){
    PersonLista pList;
    
    for (int i=0; i<antalTrans; i++) {
        if (!pList.finnsPerson(trans[i].haemta_namn())) {
            Person pNy(trans[i].haemta_namn(), liggerUteMed(trans[i].haemta_namn()), aerSkyldig(trans[i].haemta_namn()) );
            pList.laggTillEn(pNy);
        }
    }
    return pList;
}

int TransaktionsLista::antalTransaktioner(){
    return antalTrans;
}

// ***PERSON***

Person::Person(){};
Person::Person( string n, double b, double s )
: namn(n), betalat_andras(b), skyldig(s)
{};


double Person::haemta_betalat(){
    return betalat_andras;
}
double Person::haemta_skyldig(){
    return skyldig;
}
string Person::haemta_namn(){
    return namn;
}
void Person::skrivUt(){
    cout << namn << " ligger ute med: " << haemta_betalat() << " och är skyldig: " << haemta_skyldig() << ". ";
    
    if (haemta_betalat()-haemta_skyldig() > 0) {
        cout << namn << " skall ha " << haemta_betalat() - haemta_skyldig() << " från potten.";
    }
    else
        cout << namn << " skall lägga " << haemta_skyldig() - haemta_betalat() << " till potten.";
}

void Person::assign(const Person & p)
{
    namn = p.namn;
    skyldig = p.skyldig;
    betalat_andras = p.betalat_andras;
}


// ***PERSONLISTA***

PersonLista::PersonLista(){
    antal_pers = 0;
    pers = NULL;
};
PersonLista::~PersonLista(){};

void PersonLista::laggTillEn( Person pny ){
    antal_pers++;
    
    //Allokerar ny array
    Person* tempPerson = new Person[antal_pers];
    //Kopierar objekt med assign-funktionen
    for (int i = 0; i < antal_pers - 1; i++){
        tempPerson[i].assign(pers[i]);
    }
    //deallokerar
    delete[] pers;
    
    //pers pekar på ny
    pers = tempPerson;
    // Adderar det nya objektet
    pers[antal_pers-1].assign(pny);
}
void PersonLista::skrivUtOchFixa(){
    // Så att skuld och utlagda summan stämmer överens
    // Avrundade till int för få "lika tal" att decimalerna långt ute skiljde sig
    if (((int)summaSkyldig()) == ((int)summaBetalat()) ) {
        for (int i=0; i<antal_pers; i++) {
            pers[i].skrivUt();
            cout << endl;
        }
    }
}

double PersonLista::summaSkyldig(){
    double totalSkyldig = 0;
    for (int i = 0; i < antal_pers; i++) {
        totalSkyldig += pers[i].haemta_skyldig();
    }
    return totalSkyldig;
}
double PersonLista::summaBetalat(){
    double totalBetalat = 0;
    for (int i=0; i<antal_pers; i++) {
        totalBetalat += pers[i].haemta_betalat();
    }
    return totalBetalat;
}
bool PersonLista::finnsPerson(const string& namn){
    for (int i=0; i<antal_pers; i++) {
        if (pers[i].haemta_namn() == namn) {
            return true;
        }
    }
    return false;
}

double PersonLista::personSkyldig(string namn){
    double betalat = 0;
    for (int i=0; i<antal_pers; i++) {
        if (pers[i].haemta_namn() == namn)
            betalat = pers[i].haemta_skyldig();
    }
    return betalat;
}
double PersonLista::personBetalat(string namn){
    double betalat = 0;
    for (int i=0; i<antal_pers; i++) {
        if (pers[i].haemta_namn() == namn)
            betalat = pers[i].haemta_betalat();
    }
    return betalat;
}
