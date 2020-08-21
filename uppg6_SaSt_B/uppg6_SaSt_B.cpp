//
//  main.cpp
//  uppg6_SaSt_B
//
//  Created by Samuel Ström on 2020-08-14.
//  Copyright © 2020 Samuel Ström. All rights reserved.
//

/*
================================================================================
Datorer och programmering, DoP, 10 hsp, termin/år: 2020

Inlämningsuppgift nr 4

Namn: Samuel Ström

Personummer: 19930413-2237

Fyll i (alt. stryk det som INTE är relevant) av nedanstående:

Den fil som jag lämnar in går att kompilera och
programmet fungerar för alla de testdata som finns för uppgiften: Ja/Nej
     Om Nej, beskriv vilka testdata som ger problem:  __________________

Jag har använt kompilator/editor (namn/version): Xcode, Version 11.5

Jag har använt följande dator (t.ex. PC, Mac, Annat):        Mac
      med operativsystemet (t.ex. WindowsXP, Windows7,...) : MacOS

Jag har arbetat ungefär 15 timmar med denna uppgift.

================================================================================
*/
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

/* Rapport
 

 Det var väldigt problematiskt och dumt att jag inte genom att hålla över arrayen i Xcode kan se vad som finns inuti arrayen. Det är är man lite bortskämd med sedan tidigare.
 Det gjrode en väldigt förvirrad men fick testa vart i loopen man var genom att skriva ut selektorer eller liknande utskrifter för att förstå hur programmet jobbade.
 
 Måste säga att det tog ett bra tag innan jag riktigt förstod hur jag skulle arbeta med pekare.
 Felsökningen blev väldigt svår med många olika utfall även fast jag matade in samma transaktion via tangentbordet. Det gjorde en väldigt förvirrad.
 Men det släppte till slut. Största problemet var nog att förstå hur man allokerade och vart man skulle deallokera. Jag fick mycket felmeddelanden och ibland...
 fungerade programmet, ibland inte.
  
 Jag hade VÄLDIGT stora bekymmer och man kände sig att man inte hade någon aning om hur man skulle göra. Jag sökte en hel del på nätet (Stackoverflow) för vägledning...
 samt att jag försökte kika i forumet och se vad andra hade problem med. Jag fick försöka börja om flera gånger för att få en "clean slate" för att inte fastna i gamla tankesätt.
 
 Jag adderade någon extra funktion för funktionalitet och några extra selektor för att kunna hämta värden bl.a.

 
 **************
 Frågor att besvara i redovisningen

 Varför behövs tilldelningsoperatorn i klassen Transaktion och exakt när i din kod används den?
 SVAR:
 
 Som jag förstår behöver man den för att göra en djup kopiering. Annars om man gör en t1 = t2 så blir det en grund kopiering och alla förändringar som görs i t2 ändrar i t1 också.
 
 Jag använde mig av en assign-funktion som tilldelningsoperator. Jag använder den i laggTill-funktionen som finns i transaktionslistan på rad 341.
 Så när jag skapar en ny dynamisk array behöver jag tilldela den nya värden och jag kopierar/tilldelar den ifrån den tidigare arrayen trans[].
 Med assign gör jag en djup kopiering av de gamla till den nya arrayen. Samtidigt är det viktigt att det gamla som pekaren pekade på lämnas tillbaka
 med delete. Annars tar minnet slut snabbt.
 
 Vad betyder "if (this != &t)" i tilldelningsoperatorn?
 SVAR:
 Jag använde faktiskt inte detta innan jag började skriva detta men la till den i slutet (oklart om det egentligen gör någon skillnad på just min kod). Som jag förstår betyder det att när man lägger till ett nytt objekt i assign-funktionen. Om this-objektet, som jag assignar det gamla värdet med parametern t, är identiska så läggs inte this objektet till.
 Alltså: "this" inte är lika med &t då genomförs tilldelningen av this-objektets alla attribut men om this-objektet och &t är lika är de redan identiska och ingen tilldelning görs.
 I mitt fall hittar jag inga fall när this-objektet och &t är identiska. Kanske beroende på hur jag gjort min kod.
 I min assign-funktion returnerar jag inte ett Transaktions-objekt utan använder mig av void och en referensparameter för att göra den djupa kopieringen.
 
 
 ***********
 
 
 ================================================================================
 Här följer programkörningar för alla testdata:
 ==============================================

 Samma som i deluppgift A.
 
  =================================
  Testdata 1
  =================================
 Startar med att läsa från en fil.

 Välj i menyn nedan:
 0. Avsluta. Alla transaktioner sparas på fil.
 1. Läs in en transaktion från tangentbordet.
 2. Skriv ut information om alla transaktioner.
 3. Beräkna totala kostnaden.
 4. Hur mycket är en viss person skyldig?
 5. Hur mycket ligger en viss person ute med?
 6. Lista alla personer mm och FIXA!!!
 2
 Antal trans = 10
 050615 transp Eva 6000 5 Bosse Kristin Paul Torsten Stina
 050721 mat Eva 300 2 Bosse Kristin
 050721 mat Paul 400 2 Torsten Stina
 050721 transp Bosse 5000 3 Eva Kristin Paul
 050721 transp Stina 5000 1 Torsten
 050722 boende Kristin 200 1 Eva
 050722 mat Eva 300 2 Kristin Bosse
 050723 mat Torsten 300 2 Paul Stina
 050724 mat Paul 200 1 Stina
 050725 transp Eva 600 3 Bosse Kristin Paul

  =================================
  Testdata 2
  =================================
 Startar med att läsa från en fil.

 Välj i menyn nedan:
 0. Avsluta. Alla transaktioner sparas på fil.
 1. Läs in en transaktion från tangentbordet.
 2. Skriv ut information om alla transaktioner.
 3. Beräkna totala kostnaden.
 4. Hur mycket är en viss person skyldig?
 5. Hur mycket ligger en viss person ute med?
 6. Lista alla personer mm och FIXA!!!
 6
 Eva ligger ute med: 5850 och är skyldig: 1350. Eva skall ha 4500 från potten.
 Paul ligger ute med: 366.667 och är skyldig: 2500. Paul skall lägga 2133.33 till potten.
 Bosse ligger ute med: 3750 och är skyldig: 1350. Bosse skall ha 2400 från potten.
 Stina ligger ute med: 2500 och är skyldig: 1333.33. Stina skall ha 1166.67 från potten.
 Kristin ligger ute med: 100 och är skyldig: 2600. Kristin skall lägga 2500 till potten.
 Torsten ligger ute med: 200 och är skyldig: 3633.33. Torsten skall lägga 3433.33 till potten.
 
 */
