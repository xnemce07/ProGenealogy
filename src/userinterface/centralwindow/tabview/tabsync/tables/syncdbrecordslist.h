/*!
 *  \file   syncdbrecordslist.h
 *  \author Leopold Nemcek
 */
#ifndef SYNCTABDBRECORDSTABLE_H
#define SYNCTABDBRECORDSTABLE_H

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


class SyncDbRecordsList : public QTableWidget
{
    Q_OBJECT
private:
    Kernel &_kernel;

    QVector<QVector <TableCell *> > _items;

    Identifier _selected;
    bool _filterImported;

    const QFont _defaultFont;
    const int _cellPadding = 0;
public:
    SyncDbRecordsList(Kernel &k);
    ~SyncDbRecordsList() = default;

    static void defaultSettings();
    void saveSettings() const;

    Identifier getSelectedRef() const;

    void setImportedFilter(bool state);

private:
    void fill();

    void applySettings();
public slots:
    void individualClicked(int row, int col);
    virtual void update();
    void remove();
signals:
    void recordChosen(Identifier ref);
};


#endif // SYNCTABDBRECORDSTABLE_H
