
#ifndef TRADERAMOYENTERME_H
#define TRADERAMOYENTERME_H

#include <string>
#include <vector>
#include "bourse.h"
#include "portefeuille.h"
#include"transaction.h"
#include"trader.h"


class TraderaMoyenTerme: public Trader{
  public:
      TraderaMoyenTerme():Trader(){};

  Transaction choisirTransaction(const Bourse& bourse, const Portefeuille &portefeuille){


      vector<PrixJournalier> prixJournaliersDisponiblesDansLaBourseAujourdhui = bourse.getPrixJournaliersDisponiblesParAujourdhui();
      vector<Titre> titresDeTrader = portefeuille.getTitres();
      vector<PrixJournalier> titresDeTraderDisponiblesDansLaBourseAujourdhui;
      vector<PrixJournalier> prixJournaliersDisponiblesaAcheter = bourse.getPrixJournaliersParPrixAujourdhui(portefeuille.getSolde());
      vector<string>actionsTrader;
      vector<string> actionsDisponibles = bourse.getActionsDisponiblesParAujourdhui();

      for (auto i : prixJournaliersDisponiblesDansLaBourseAujourdhui) { //bourse disponible;
        for (auto titre : titresDeTrader) {
            actionsTrader.push_back(titre.getAction());
            if (i.getAction() == titre.getAction()) {

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
        double prixAction;
        for (const string& action : actionsDisponibles) {
        // Obtenez les prix journaliers pour l'action en cours
        vector<PrixJournalier> prixJournaliers = bourse.getPrixJournaliersParAction(action);

        // V�rifiez si les prix journaliers sont suffisamment nombreux pour prendre une d�cision
        if (prixJournaliers.size() >= 2) {
            // Obtenez le dernier prix journalier
            PrixJournalier dernierPrix = prixJournaliers.back();

            // Obtenez le prix journalier pr�c�dent
            PrixJournalier prixPrecedent = prixJournaliers[prixJournaliers.size() - 2];

            // V�rifiez si le prix a augment� entre le prix pr�c�dent et le dernier prix
            if (dernierPrix.getPrix() > prixPrecedent.getPrix()) {



                    // Achetez l'action si elle n'est pas d�j� d�tenue
                    Transaction transaction(ACHETER,action,100); // Par exemple, achat de 100 actions
                    return transaction;                 }
            }
        }
    }
}
 else {
        if (portefeuille.getSolde() == 0) {
            if (titresDeTraderDisponiblesDansLaBourseAujourdhui.empty()) {
                Transaction transaction(RIEN_FAIRE);
                return transaction;
            }
        }

    else{
     for (const string& action : actionsTrader) {

        PrixJournalier prixActuel = bourse.getPrixJournalierParActionAujourdhui(action);


        if (prixActuel.getPrix() > portefeuille.getPrixAchat(action)) {

            int quantiteAvendre=100;
            Transaction transaction(VENDRE, action, quantiteAvendre);
            return transaction;
        }
    }

    }

      }
       };


};

#endif  // TRADER_H


