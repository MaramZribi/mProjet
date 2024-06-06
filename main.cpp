#include <iostream>
#include <string>
#include<vector>
#include<fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include<chrono>
#include<map>
#include <cassert>
#include "trader.h"
#include "bourse.h"
#include "portefeuille.h"
#include"date.h"
#include"test.h"
#include"prixjournaliers.h"
#include"simulation.h"
#include"titre.h"
#include"traderaleatoire.h"
#include"TraderaMoyenTerme.h"
using namespace std;

bool Date::operator==(const Date& d)const{
   return (jour==d.jour &&mois==d.mois && annee==d.annee);
   }
bool Date::operator < (const Date& d)const{
   return (d.annee>annee || (d.annee==annee && d.mois>mois) || (d.annee==annee && d.mois==mois && d.jour>jour));
}
void Date:: incrementerDate()
{
    int limiteJour = 31;
    if (mois == 2) {
        if (annee % 4 == 0 && annee % 100 != 0 || annee % 400 == 0) {
            limiteJour = 29;
        } else {
            limiteJour = 28;
        }
    } else if (mois == 4 || mois == 6 || mois == 9 || mois == 11) {
        limiteJour = 30;
    }
    jour++;
    if (jour > limiteJour) {
        jour = 1;
        mois++;
    }
    if (mois > 12) {
        jour=1;
        mois = 1;
        annee++;
    }
}
Date::Date(int j,int m,int a):jour(j), mois(m), annee(a){}



class PersistancePrixJournaliers {
public:
    static vector<PrixJournalier> lirePrixJournaliersDUnFichier(string chemin) {
        vector<PrixJournalier> historique;
        ifstream f(chemin);
        int nbLignes = 0;
        string entete;
        if (f.is_open()) {
            getline(f, entete);
            while (!f.eof()) {
                PrixJournalier pj;
                f >> pj;
                historique.push_back(pj);
                nbLignes++;
            }
        }
        f.close();
        return historique;
    }
};
vector<PrixJournalier>BourseVector:: getPrixjournaliers(){
        return prixj;
    }
 vector<string>BourseVector::getActionsDisponiblesParPrix(double prix) const
        {
        vector<PrixJournalier> prj =getPrixJournaliersParPrix(prix);
        vector<string> actionsParPrix;
        for(auto i : prj)
        {
                actionsParPrix.push_back(i.getAction());
        }
        return actionsParPrix;

}

    vector<string> BourseVector::getActionsDisponiblesParAujourdhui() const
    {
        vector<PrixJournalier> prjr=getPrixJournaliersDisponiblesParAujourdhui();
        vector<string> actionsAujourdhui;
        for(auto i : prjr){
                actionsAujourdhui.push_back(i.getAction());
                  }

        return actionsAujourdhui;
    }
    vector<string>BourseVector:: getActionsDisponiblesParDate(Date date) const
    {
        vector<PrixJournalier> prj =getPrixJournaliersParDate(date);
        vector<string> actions;

        for (auto i : prj)
        {
                actions.push_back(i.getAction());
            }
        return actions;

}
vector<PrixJournalier> BourseVector::getPrixJournaliersParDate(Date date) const
{
    vector<PrixJournalier> prix_journaliers;
    if (!prixj.empty() && date < dateAujourdhui)
    {
        int left = 0;
        int right = prixj.size() - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            if (prixj[mid].getDate() == date)
            {
                prix_journaliers.push_back(prixj[mid]);
                // Recherche des autres occurrences éventuelles de la même date
                int prev = mid - 1;
                while (prev >= left && prixj[prev].getDate() == date)
                {
                    prix_journaliers.push_back(prixj[prev]);
                    prev--;
                }
                int next = mid + 1;
                while (next <= right && prixj[next].getDate() == date)
                {
                    prix_journaliers.push_back(prixj[next]);
                    next++;
                }
                break;
            }
            else if (prixj[mid].getDate() < date)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
    }
    return prix_journaliers;
}

    vector<PrixJournalier>BourseVector:: getPrixJournaliersParAction(string action)const{
       vector<PrixJournalier> prix_journaliers;
        int i=0;
        if(!prixj.empty()){
        while (prixj[i].getDate() < dateAujourdhui && prixj.size()>i)
        {

            if (prixj[i].getAction()==action)
            {
                prix_journaliers.push_back(prixj[i]);
            }
             i++;
        }
        }
        return prix_journaliers;
    }
  PrixJournalier BourseVector:: getPrixJournalierParActionAujourdhui(string action)const{
        int i=0;
        PrixJournalier pj;
        if(!prixj.empty()){
        while (prixj[i].getDate() == dateAujourdhui && prixj.size()>i)
        {

            if (prixj[i].getAction()==action)
            {
                pj.setAction(prixj[i].getAction());
                pj.setPrix(prixj[i].getPrix());
                pj.setDate(prixj[i].getDate());
            }
             i++;
        }
    }
    return pj;
    }
   vector<PrixJournalier> BourseVector:: getPrixJournaliersParPrix(double prix)const{
        vector<PrixJournalier> prix_journaliers;
        int i=0;
        if(!prixj.empty()){
           while (prixj[i].getDate() < dateAujourdhui && prixj.size()>i)
          {
              if (prixj[i].getPrix() <=prix)
              {
                prix_journaliers.push_back(prixj[i]);
              }
              i++;
          }
        }
        return prix_journaliers;
   }
    vector<PrixJournalier> BourseVector:: getPrixJournaliersParPrixAujourdhui(double prix)const{
        vector<PrixJournalier> prix_journaliers;
        int i=0;
        if(!prixj.empty()){
           while (prixj[i].getDate()== dateAujourdhui && prixj.size()>i)
          {
              if (prixj[i].getPrix() <=prix)
              {
                prix_journaliers.push_back(prixj[i]);
              }
              i++;
          }
        }
        return prix_journaliers;
   }
vector<PrixJournalier> BourseVector::getPrixJournaliersDisponiblesParAujourdhui()const{
        vector<PrixJournalier> prix_journaliers;
        int i=0;
        if(!prixj.empty()){
         while ( prixj.size()>i)
        {
            if (prixj[i].getDate() == dateAujourdhui)
            {
                prix_journaliers.push_back(prixj[i]);
            }
             i++;
        }
    }
          return prix_journaliers;
    }



vector<PrixJournalier>  BourseMap::getPrixJournaliersParPrix(double prix)const
{
    vector<PrixJournalier> prixJournaliers;
    for ( auto pair : bourseMap)
    {
        if (pair.second.getPrix() <= prix)
        {
            prixJournaliers.push_back(pair.second);
        }
    }
    return prixJournaliers;
}
vector<PrixJournalier>  BourseMap::getPrixJournaliersDisponiblesParAujourdhui() const
{

    vector<PrixJournalier> prixJournaliers;
    auto it = bourseMap.begin();
    while (it != bourseMap.end())
    {
        if (it->first == dateAujourdhui)
        {
            prixJournaliers.push_back(it->second);

        }
        ++it;
    }
    return prixJournaliers;
}
vector<PrixJournalier> BourseMap::getPrixJournaliersParDate(Date date)const
{

    vector<PrixJournalier> prixJournaliers;
    auto it = bourseMap.begin();
    if (date ==dateAujourdhui){
        vector<PrixJournalier> prixJournaliers=getPrixJournaliersDisponiblesParAujourdhui();
        return prixJournaliers;
    }
  else if ( date < dateAujourdhui){
    while (it != bourseMap.end())
    {
        if (it->first == date)
        {
            prixJournaliers.push_back(it->second);
        }
        ++it;
    }

    return prixJournaliers;
}
}
vector<PrixJournalier> BourseMap::getPrixJournaliersParAction(string action)const
{
    vector<PrixJournalier> prixJournaliers;
    auto it = bourseMap.begin();
    while (it != bourseMap.end())
    {
        if (it->second.getAction() == action)
        {
            prixJournaliers.push_back(it->second);
        }
        ++it;
    }
    return prixJournaliers;
}
vector<PrixJournalier>  BourseMap::getPrixJournaliersParPrixAujourdhui(double prix)const
{

   vector<PrixJournalier> prixJournaliers;
    auto it = bourseMap.begin();
    while (it != bourseMap.end())
    {
        if (it->second.getPrix() <= prix && it->first==dateAujourdhui)
        {
            prixJournaliers.push_back(it->second);
        }
        ++it;
    }
    return prixJournaliers;
}
vector<string> BourseMap:: getActionsDisponiblesParPrix(double prix)const
        {
        vector<PrixJournalier>prj=getPrixJournaliersParPrix(prix);
        vector<string> actionsParPrix;
        for(auto i : prj)
        {
                actionsParPrix.push_back(i.getAction());

        }
        return actionsParPrix;

}
vector<string> BourseMap:: getActionsDisponiblesParAujourdhui()const
    {
       vector<PrixJournalier> prjr=getPrixJournaliersDisponiblesParAujourdhui();
         vector<string> actionsAujourdhui;
        for(auto i : prjr){
                actionsAujourdhui.push_back(i.getAction());
                  }
        return actionsAujourdhui;
    }
vector<string> BourseMap::getActionsDisponiblesParDate(Date date)const
    {
         vector<PrixJournalier> prj =getPrixJournaliersParDate(date);
        vector<string> actions;


        for (auto i : prj)
        {
                actions.push_back(i.getAction());
            }
        return actions;

}
PrixJournalier BourseMap:: getPrixJournalierParActionAujourdhui(string action)const{

    PrixJournalier pj;
    auto it = bourseMap.begin();
    while (it != bourseMap.end()&& it->first==dateAujourdhui )
    {
        if (it->second.getAction() == action)
        {
              pj.setAction(action);
                pj.setPrix(it->second.getPrix());
                pj.setDate(it->first);
        }
        ++it;
}
    return pj;
}


void Portefeuille:: ajouterTitre (Titre titre){
        bool found = false;
        for (auto &t : titres) {
            if (t.getAction() == titre.getAction()) {
                t.setQt(t.getQt() + titre.getQt());
                found = true;
                break;
            }
        }
        if (!found) {
            titres.push_back(titre);
        }

}
void Portefeuille:: retirerTitre(Titre* titre) {
        for (auto it = titres.begin(); it != titres.end(); ++it) {
            if (it->getAction() == titre->getAction() && it->getQt() >= titre->getQt()) {
                if (it->getQt() == titre->getQt()) {
                    titres.erase(it);
                } else {
                    it->setQt(it->getQt() - titre->getQt());
                }
                break;
            }
        }
}
void Portefeuille:: deposerMontant(double montant){solde+=montant;}
void Portefeuille:: retirerMontant(double montant){solde-=montant;}


map<string, long> Simulation::executer(Bourse& bourse, Trader& trader, Date dateDebut, Date dateFin, double solde){
        map<string, long> stats;
        vector<Titre> Titres;
        Portefeuille portefeuille(Titres,solde);
        bourse.setDateAujourdhui(dateDebut);
        int nbrTxParJour=0;

         cout<<"Solde initiale"<<solde<<endl;
         cout<<"Taille de titres initiale:"<<portefeuille.getTitres().size()<<endl;
        // Mesurer le temps d'exécution d'une partie du code
        auto start = chrono::high_resolution_clock::now();
        while (bourse.getDateAujourdhui() < dateFin){
             cout<<bourse.getDateAujourdhui()<<endl;

            do{

                Transaction transaction = trader.choisirTransaction(bourse, portefeuille);

                nbrTxParJour++;
                cout<<"nb"<<nbrTxParJour<<endl;
                if (transaction.getType() == RIEN_FAIRE){
                    cout<<"RIEN_FAIRE"<<endl;
                    bourse.getDateAujourdhui().incrementerDate();
                } else if (transaction.getType() == ACHETER){
                    cout<<"ACHETER"<<endl;
                    double prixAction = bourse.getPrixJournalierParActionAujourdhui(transaction.getNomAction()).getPrix();
                    cout<<"action:" <<transaction.getNomAction()<<"|"<<"prixAction:"<<prixAction<<endl;
                    int quantite = transaction.getQuantite();
                    cout<<"QT:"<<quantite<<endl;
                    double montant = prixAction * quantite;
                    cout<<"à montant:"<<montant<<endl;
                    Titre titre(transaction.getNomAction(), quantite);
                    portefeuille.retirerMontant(montant);
                    cout<<"Current Solde:"<< portefeuille.getSolde()<<endl;
                    portefeuille.ajouterTitre(titre);
                    cout<<"Taille du titres :"<<portefeuille.getTitres().size()<<endl;
                    for(auto i: portefeuille.getTitres()){
                        cout<<i.getAction()<<i.getQt()<<endl;

                    }
                } else if (transaction.getType() == VENDRE){
                    cout<<"VENDRE"<<endl;
                    double prixAction = bourse.getPrixJournalierParActionAujourdhui(transaction.getNomAction()).getPrix();
                    cout<<"action:" <<transaction.getNomAction()<<"prix:"<<prixAction<<endl;
                    int quantite = transaction.getQuantite();
                    cout<<"QT:"<<quantite<<endl;
                    Titre titre(transaction.getNomAction(), quantite);
                    cout<<"Taille du titres i :"<<portefeuille.getTitres().size()<<endl;
                    portefeuille.retirerTitre(&titre);
                    cout<<"Taille du titres i+1:"<<portefeuille.getTitres().size()<<endl;
                    portefeuille.deposerMontant(prixAction*quantite);
                     cout<<"Current Solde:"<< portefeuille.getSolde()<<endl;
                }
            } while (nbrTxParJour<100);
            cout<<(nbrTxParJour<2)<<endl;
            nbrTxParJour = 0;
            bourse.getDateAujourdhui().incrementerDate();

            Date d1= bourse.getDateAujourdhui();
            d1.incrementerDate();

            bourse.setDateAujourdhui(d1);

        }


        for (Titre& titre : portefeuille.getTitres()){

            for (int i = bourse.getPrixJournaliersParAction(titre.getAction()).size() - 1; i >= 0; i--){

                if (bourse.getPrixJournaliersParAction(titre.getAction())[i].getDate() < dateFin){
                    double prixAction = bourse.getPrixJournaliersParAction(titre.getAction())[i].getPrix();
                    int quantite = titre.getQt();
                    Titre titreCopie(titre.getAction(), quantite);
                     cout<<"Taille du titres i:"<<portefeuille.getTitres().size()<<endl;
                    portefeuille.retirerTitre(&titreCopie);
                     cout<<"Taille du titres i+1:"<<portefeuille.getTitres().size()<<endl;
                    portefeuille.deposerMontant(prixAction * quantite);

                    cout<<"Current Solde:"<< portefeuille.getSolde()<<endl;
                    break;
                }
            }
        }
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

        // Ajouter la durée à la statistique correspondante
        stats["TEMPS_EXECUTION_SIMULATION_µs"] += duration.count();
    return stats;
}



bool Test :: testDate(Date d){
    bool DateValide = true;
    if (d.annee < 0 || d.mois < 1 || d.mois > 12 || d.jour < 1) {
        DateValide = false;
    } else {
        int limiteJour = 31;
        if (d.mois == 2) {
            if (d.annee % 4 == 0 && d.annee % 100 != 0 || d.annee % 400 == 0) {
                limiteJour = 29;
            } else {
                limiteJour = 28;
            }
        } else if (d.mois == 4 || d.mois == 6 || d.mois == 9 || d.mois == 11) {
            limiteJour = 30;
        }
        DateValide = (d.jour >= 1 && d.jour <= limiteJour);
    }
    if(DateValide==false){
        return DateValide;
    }
    return DateValide ;
}
bool Test :: testPrixJournaliers(PrixJournalier PJ){
    bool PJValide = true;
    if (testDate(PJ.getDate())){PJValide = false;}
    if (PJ.getPrix()<=0){PJValide = false;}
    return PJValide ;
}



int main()
{
    srand(time(NULL));
    map<string, long> stats;
    stats["MON_COMPTEUR"]++;

    Date d1,d2;
    d1=Date(4,1,2010);
    d2=Date(5,1,2010);
    vector<PrixJournalier> historique = PersistancePrixJournaliers::lirePrixJournaliersDUnFichier("C:\\Users\\Zhome\\Documents\\prices_simple.csv");
    BourseMap v (d2,historique);
    v.getPrixjournaliers();
    /*for(const auto& it : v.getPrixjournaliers()) {
            cout << it.first<< "\t" << it.second << endl;
       }*/
    /*Titre t1("act", 123);
    Titre t2("at", 12);
    Titre t3("ac", 13);
    vector<Titre> titres;
    titres.push_back(t1);
    titres.push_back(t2);
    titres.push_back(t3);*/

    TraderAleatoire trader;
    auto start = chrono::high_resolution_clock::now();
    auto prixAujourdhui = v.getPrixJournaliersParAction("IRM");
    //cout<<prixAujourdhui;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);


    Simulation sim;
    //stats = sim.executer(v, trader, d1, d2,1000);
    stats["TEMPS_GET_ACTIONS_DISPO_AUJ_µs"]+=duration.count();
   for (const auto& it   : stats) {
        cout << it.first << "\t" << it.second << endl;
    }
  int nb ;
 for (auto i: prixAujourdhui){

    cout<<i<<endl;
    nb++;
  }
  cout<<nb;

    return 0;
    }


