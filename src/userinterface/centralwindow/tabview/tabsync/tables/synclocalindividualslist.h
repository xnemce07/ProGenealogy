/*!
 *  \file   syncmatchlist.h
 *  \author Leopold Nemcek
 */
#ifndef SYNCLOCALINDIVIDUALSLIST_H
#define SYNCLOCALINDIVIDUALSLIST_H


#include <QHeaderView>
#include <QTableWidget>
#include <QFontMetrics>
#include <QSettings>

#include "kernel.h"
#include "../../tabindividual/listview/tablecelldate.h"
#include "../../tabindividual/listview/tablecellref.h"
#include "../../tabindividual/listview/tablecelltext.h"
#include "tablecells/tablecellbool.h"
#include "tablecells/tablecelldbrecordtype.h"

class SyncLocalIndividualsList : public QTableWidget
{
    Q_OBJECT
private:
    Kernel &_kernel;
    Identifier _dbRec = Identifier();
    Identifier _selected = Identifier();
    
    bool _showMatchesOnly = true;


    QVector<QVector <TableCell *> > _items;

    const QFont _defaultFont;
    const int _cellPadding = 0;

public:
    SyncLocalIndividualsList(Kernel &k);
    ~SyncLocalIndividualsList() = default;

    Identifier getSelected() const;

    static void defaultSettings();
    void saveSettings() const;
private:
    QVector<Identifier> getRefIndividualMatches();
    void fill();

    void applySettings();
    void selectRow(int row);
public slots:
    void update();
    void updateSync();
    void remove();
    void changeMatchesOnly(int state);


    void refRecordSet(Identifier ref);
private slots:
    void individualClicked(int row, int col);

signals:
    void individualChosen(Identifier ref);
};

#endif // SYNCLOCALINDIVIDUALSLIST_H
