#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
using namespace std;
class Date{
     int jour;
     int mois;
     int annee;
    public:
     string incrimenterDate(string);
     string afficherDate ();
     void setJour(int j);
     void setMois(int m);
     void setAnnee(int a);
     Date (string d);
     int getJour();
};
int Date::getJour()
{
    return jour;
}
string Date:: incrimenterDate(string)
{

}
void Date::setMois(int m)
{
 mois++;
}
void Date::setJour(int j)
{
 jour++;
}
void Date::setAnnee(int a)
{
 annee++;
}
Date::Date(string d){
    istringstream ss(d);
    string j, m, a;
    getline(ss, j, '/');
    getline(ss, m, '/');
    getline(ss, a);
    int jr = std::stoi(j);
    int ms = std::stoi(m);
    int an = std::stoi(a);
    jour=jr;
    mois=ms;
    annee=an;
}



int main()
{
    Date d("1/02/2022");
    cout<< d.getJour()<<endl;
     cout<<"mregel"<<endl;
    return 0;
}
