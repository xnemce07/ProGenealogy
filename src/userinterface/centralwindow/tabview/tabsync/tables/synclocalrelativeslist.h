/*!
 *  \file   synclocalrelativeslist.h
 *  \author Leopold Nemcek
 */
#ifndef SYNCLOCALRELATIVESLIST_H
#define SYNCLOCALRELATIVESLIST_H

#include <QHeaderView>
#include <QTableWidget>
#include <QFontMetrics>
#include <QSettings>


//#include "synctabtable.h"
#include "kernel.h"
#include "textformatter.h"
#include "../../tabindividual/listview/tablecelldate.h"
#include "../../tabindividual/listview/tablecellref.h"
#include "../../tabindividual/listview/tablecelltext.h"
#include "tablecells/tablecellbool.h"
#include "tablecells/tablecelldbrecordtype.h"
#include "tablecells/tablecelldbrecordrole.h"

class SyncLocalRelativesList : public QTableWidget
{
    Q_OBJECT
private:
    Kernel &_kernel;

    QVector<QVector <TableCell *> > _items;

    Identifier _selected;
    DbRecordRole _selectedRole;

    Identifier _mainIndividual;
    Identifier _dbRecord;
    Identifier _partner;

    const QFont _defaultFont;
    const int _cellPadding = 0;
public:
    SyncLocalRelativesList(Kernel &k);
    ~SyncLocalRelativesList() = default;

    static void defaultSettings();
    void saveSettings() const;

    Identifier getSelectedIndividual() const;
    Identifier getPartner() const;
    DbRecordRole getSelectedRole() const;
private:
    void fill();

    /**
     * @brief getRelatives Find all relatives of selected individual
     * @return Vector of individual's identifiers with  their roles
     */
    QVector<QPair<Identifier, DbRecordRole>> getRelatives();

    void applySettings();

    void selectRow(int row);
public slots:
    void individualClicked(int row, int col);
    void update();
    void remove();
    void changedDbRecord(Identifier record);
    void changedMainIndividual(Identifier ref);
    void changedPartner(Identifier ref);
signals:
    void selectedLocalIndividual(Identifier record, Identifier partner);

};

#endif // SYNCLOCALRELATIVESLIST_H
