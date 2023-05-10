/*!
 *  \file   localrecordsbox.h
 *  \author Leopold Nemcek
 */
#ifndef NEWRECORDSBOX_H
#define NEWRECORDSBOX_H

#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include "../boxes/box.h"
#include "tables/synclocalindividualslist.h"
#include "tables/synclocalrelativeslist.h"
#include "../boxes/subbox/subboxpartnerref.h"

class LocalRecordsBox : public Box
{
    Q_OBJECT
private:
    SyncLocalIndividualsList *_matchList;
    SyncLocalRelativesList *_relList;


    QLabel _laShowAll;
    QCheckBox _cbShowAll;

    QLabel _laIndividuals;
    QLabel _laPartner;
    QLabel _laRelatives;

    Identifier _selectedDbRecord;
    Identifier _selectedDbIndividual;

    SubboxPartnerRef *_partnerSubbox;

    QPushButton _pbAddPersonToRole;

    bool _isDbIndividualSelected = false;
    bool _isLocalIndividualSlotSelected = false;

public:
    LocalRecordsBox(QString title, Kernel &k, Identifier &p, QWidget *parent = nullptr);
    ~LocalRecordsBox() override;

    void update() override;
    void updateSync();
private:
    void fill() override;
    /**
     * @brief addPersonToRole Saves db individual to record and fills it to currently selected role of currently selected individual/pair
     */
    void addPersonToRole();

    /**
     * @brief updateImportButton Enables/disables 'Add to role' button based on what dbindividual, local individual and role to add to is selected
     */
    void updateImportButton();
signals:
    void selectedDbIndividual(Identifier ref);
    void selectedDbRecord(Identifier ref);

    void selectedLocalIndividual(Identifier ref, Identifier partner);
    void individualImported(Identifier ref);
    void localMainIndiviudalChosen(Identifier ref);

public slots:
    static void defaultSettings();
    void saveSettings() const;

private slots:
    void changedDbIndividual(Identifier ref);
    void changedDbRecord(Identifier ref);

    void changedLocalMainIndi(Identifier ref);

    void changedLocalIndiForMerge(Identifier ref);

    void addPersonToRoleClicked(bool clicked);

    void showMatchesCBChecked(bool checked);

};

#endif // NEWRECORDSBOX_H
