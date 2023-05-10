/*!
 *  \file   dbrecordsbox.h
 *  \author Leopold Nemcek
 */
#ifndef RECORDCHOICEBOX_H
#define RECORDCHOICEBOX_H

#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QCheckBox>

#include "../boxes/box.h"
#include "../tabsync/tables/syncdbrecordslist.h"
#include "../tabsync/tables/syncdbindividualslist.h"

class DbRecordsBox : public Box
{
    Q_OBJECT
private:
    SyncDbRecordsList  *_recordsView;
    SyncDbIndividualsList *_individualsView;

    QPushButton _pbImportIndividual;
    QPushButton _pbSetIndividualImported;
    QPushButton _pbSetRecordImported;

    QCheckBox _cbFilterImported;

    QLabel _laFilterImported;
    QLabel _laRecords;
    QLabel _laIndis;

public:
    DbRecordsBox(QString title, Kernel &k, Identifier &p, QWidget *parent = nullptr);
    ~DbRecordsBox() override;
    void update() override;
    void updateSync();

private:
    void fill() override;
    void addRecord();

signals:
    void selectedDbIndividual(Identifier ref, Identifier rec);
    void selectedDbRecord(Identifier ref);
    void individualImported(Identifier ref);
    void dataChanged();
public slots:
    static void defaultSettings();
    void saveSettings() const;
private slots:
    void importIndividualClicked();
    void setIndividualImportedClicked();
    void setRecordImportedClicked();

    void changedDbRecord(Identifier ref);
    void changedDbIndividual(Identifier ref);

    void filterImportedRecrodsChecked(bool checked);
};

#endif // RECORDCHOICEBOX_H
