/*!
 *  \file   syncdbindividualslist.h
 *  \author Leopold Nemcek
 */
#ifndef SYNCDBRECORDINDIVIDUALSLIST_H
#define SYNCDBRECORDINDIVIDUALSLIST_H

#include <QHeaderView>
#include <QTableWidget>
#include <QFontMetrics>
#include <QSettings>

#include "kernel.h"
#include "textformatter.h"
#include "../../tabindividual/listview/tablecelldate.h"
#include "../../tabindividual/listview/tablecellref.h"
#include "../../tabindividual/listview/tablecelltext.h"
#include "tablecells/tablecelldbrecordrole.h"
#include "tablecells/tablecellbool.h"
#include "tablecells/tablecelldbrecordtype.h"

class SyncDbIndividualsList : public QTableWidget
{
    Q_OBJECT
private:
    Kernel &_kernel;
    Identifier _dbRecord;
    Identifier _selected;

    QVector<QVector <TableCell *> > _items;

    const QFont _defaultFont;
    const int _cellPadding = 0;
public:
    SyncDbIndividualsList(Kernel &k);
    ~SyncDbIndividualsList() = default;

    Identifier getSelectedRef() const;

    static void defaultSettings();
    void saveSettings() const;
private:
    void fill();

    void applySettings();
public slots:
    void recordClicked(int row, int col);
    virtual void update();
    void remove();
    void recordChosen(Identifier ref);
signals:
    void dbIndividualSelected(Identifier ref, Identifier record);
};

#endif // SYNCDBRECORDINDIVIDUALSLIST_H
