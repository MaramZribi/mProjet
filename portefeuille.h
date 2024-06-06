#ifndef PORTEFEUILLE_H
#define PORTEFEUILLE_H

#include <string>
#include <vector>
#include "bourse.h"
#include"titre.h"
#include"prixjournaliers.h"


class Portefeuille{
   private:

       vector<Titre> titres;
       map<string,double>PrixjTrader;
       double solde;
   public:

       Portefeuille(vector<Titre> t, double s):titres(t), solde(s){};
       double getSolde()const{return solde;};
       void setSolde(double s){solde=s;};
       vector<Titre> getTitres()const{return titres;};
       map<string,double>getPrixjTrader()const{return PrixjTrader;};
       double getPrixAchat(string action)const {
        auto it = PrixjTrader.begin();
        while (it != PrixjTrader.end()){
        if (it->first == action){
           return (it->second);}
        ++it;}};
       void ajouterTitre (Titre titre);
       void retirerTitre(Titre* titre);
       void deposerMontant(double montant);
       void retirerMontant(double montant);

};
#endif
