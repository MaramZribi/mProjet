

#ifndef TRADERALEATOIRE_H
#define TRADERALEATOIRE_H

#include <string>
#include <vector>
#include "bourse.h"
#include "portefeuille.h"
#include"transaction.h"
#include"trader.h"


class TraderAleatoire: public Trader{
  public:
      TraderAleatoire():Trader(){};
      Transaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille){
      vector<PrixJournalier> prixJournaliersDisponiblesDansLaBourseAujourdhui = bourse.getPrixJournaliersDisponiblesParAujourdhui();
      vector<Titre> titresDeTrader = portefeuille.getTitres();
      vector<PrixJournalier> titresDeTraderDisponiblesDansLaBourseAujourdhui;
      vector<PrixJournalier> prixJournaliersDisponiblesaAcheter = bourse.getPrixJournaliersParPrixAujourdhui(portefeuille.getSolde());

    for (auto i : prixJournaliersDisponiblesDansLaBourseAujourdhui) {
        for (auto j : titresDeTrader) {
            if (i.getAction() == j.getAction()) {
                titresDeTraderDisponiblesDansLaBourseAujourdhui.push_back(i);
            }
        }
    }

    if (titresDeTrader.empty()) {
        if (prixJournaliersDisponiblesaAcheter.empty()) {
            Transaction transaction(RIEN_FAIRE);
            return transaction;
        }
        else {
            int index = rand() % prixJournaliersDisponiblesaAcheter.size();
            PrixJournalier pj = prixJournaliersDisponiblesaAcheter[index];
            double prixAction = pj.getPrix();
            string action = pj.getAction();
            int quantite = floor(portefeuille.getSolde() / prixAction);
            Transaction transaction(ACHETER, action, quantite);
            return transaction;
        }
    }
    else {
        if (portefeuille.getSolde() == 0) {
            if (titresDeTraderDisponiblesDansLaBourseAujourdhui.empty()) {
                Transaction transaction(RIEN_FAIRE);
                return transaction;
            }
            else {
                int index = rand() % titresDeTraderDisponiblesDansLaBourseAujourdhui.size();
                PrixJournalier prixJournalierAvendre = titresDeTraderDisponiblesDansLaBourseAujourdhui[index];
                int quantiteAvendre = 0;
                for (Titre i : titresDeTrader) {
                    if (i.getAction() == prixJournalierAvendre.getAction()) {
                        quantiteAvendre = 1.0 + static_cast<int>(rand()) / RAND_MAX * (i.getQt());
                        break;
                    }
                }
                string action = prixJournalierAvendre.getAction();
                double prixAction = prixJournalierAvendre.getPrix();
                Transaction transaction(VENDRE, action, quantiteAvendre);
                return transaction;
            }
        }
        else {
            TypeTransaction type = static_cast<TypeTransaction>(rand() % 3);
            if (type == ACHETER) {
                if (prixJournaliersDisponiblesaAcheter.empty()) {
                    do {
                        type = static_cast<TypeTransaction>(rand() % 3);
                    } while (type == ACHETER);

                    if (type == VENDRE) {
                        if (titresDeTraderDisponiblesDansLaBourseAujourdhui.empty()) {
                            Transaction transaction(RIEN_FAIRE);
                            return transaction;
                        }
                        else {
                            int index = rand() % titresDeTraderDisponiblesDansLaBourseAujourdhui.size();
                            PrixJournalier prixJournalierAvendre = titresDeTraderDisponiblesDansLaBourseAujourdhui[index];
                            int quantiteAvendre=0;
                            for(Titre i: titresDeTrader){
                                if (i.getAction()== prixJournalierAvendre.getAction()){
                                  int quantiteAvendre = 1.0 + static_cast<int>(rand()) / RAND_MAX * (i.getQt());
                                  break;}
                            }
                            string action =prixJournalierAvendre.getAction();
                            double prixAction = prixJournalierAvendre.getPrix();
                            Transaction transaction(VENDRE,action,quantiteAvendre);
                            return transaction;
                            }
                    }
                    else{
                        Transaction transaction(RIEN_FAIRE);
                        return transaction;}
                        }
                    else{
                      int index = rand() % prixJournaliersDisponiblesaAcheter.size();
                      PrixJournalier pj= prixJournaliersDisponiblesaAcheter[index];
                      double prixAction= pj.getPrix();
                      string action =pj.getAction();
                      int quantite= floor(portefeuille.getSolde()/prixAction);
                      Transaction transaction(ACHETER,action,quantite);
                      return transaction;
                      }
                }
                   else if (type==VENDRE){
                              int quantiteAvendre;
                              if(titresDeTraderDisponiblesDansLaBourseAujourdhui.empty()){
                                   do{
                                       type = static_cast<TypeTransaction>(rand()%3);}while(type==VENDRE);
                                   if (type==ACHETER){
                                       if(prixJournaliersDisponiblesaAcheter.empty()){
                                           Transaction transaction(RIEN_FAIRE);
                                           return RIEN_FAIRE;
                                       }
                                       else{
                                       int index = rand() % prixJournaliersDisponiblesaAcheter.size();
                                       PrixJournalier pj= prixJournaliersDisponiblesaAcheter[index];
                                       double prixAction= pj.getPrix();
                                       string action =pj.getAction();
                                       int quantite= floor(portefeuille.getSolde()/prixAction);
                                       Transaction transaction(ACHETER,action,quantite);
                                       return transaction;
                                       }
                                   }
                                    else{
                                         Transaction transaction(RIEN_FAIRE);
                                         return transaction;}

                              }
                               else{
                                  int index = rand() % titresDeTraderDisponiblesDansLaBourseAujourdhui.size();
                                  PrixJournalier prixJournalierAvendre=titresDeTraderDisponiblesDansLaBourseAujourdhui[index];
                                  for(Titre i: titresDeTrader){
                                         if (i.getAction()== prixJournalierAvendre.getAction()){
                                               quantiteAvendre = 1.0 + static_cast<int>(rand()) / RAND_MAX * (i.getQt());
                                              break;}
                                              }
                                  string action =prixJournalierAvendre.getAction();
                                  double prixAction = prixJournalierAvendre.getPrix();
                                  Transaction transaction(VENDRE,action,quantiteAvendre);
                                  return transaction;
                 }

              }
                else{
                    Transaction transaction(RIEN_FAIRE);
                    return transaction;
                }
            }
    }
};


};

#endif  // TRADER_H

