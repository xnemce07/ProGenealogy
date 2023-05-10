/*!
 *  \file   syncdbindividualslist.cpp
 *  \author Leopold Nemcek
 */
#include "syncdbindividualslist.h"

SyncDbIndividualsList::SyncDbIndividualsList(Kernel &k):
    _kernel(k)
{
    Identifier invalid = Identifier();
    _dbRecord = invalid;
    QFontMetrics font(_defaultFont);
    this->setTextElideMode(Qt::ElideRight);
    this->fill();

    connect(this, &SyncDbIndividualsList::cellClicked, this, &SyncDbIndividualsList::recordClicked);
}

Identifier SyncDbIndividualsList::getSelectedRef() const
{
    return this->_selected;
}

void SyncDbIndividualsList::defaultSettings()
{
    QSettings settings;
    settings.setValue("window/syncTab/dbIndividualsList/idCol", 25);
    settings.setValue("window/syncTab/dbIndividualsList/nameCol", 95);
    settings.setValue("window/syncTab/dbIndividualsList/surnameCol", 95);
    settings.setValue("window/syncTab/dbIndividualsList/birthCol", 65);
    settings.setValue("window/syncTab/dbIndividualsList/roleCol", 85);
    settings.setValue("window/syncTab/dbIndividualsList/impCol", 85);
}

void SyncDbIndividualsList::saveSettings() const
{
    QSettings settings;
    settings.setValue("window/syncTab/dbIndividualsList/idCol", this->columnWidth(0));
    settings.setValue("window/syncTab/dbIndividualsList/nameCol", this->columnWidth(1));
    settings.setValue("window/syncTab/dbIndividualsList/surnameCol", this->columnWidth(2));
    settings.setValue("window/syncTab/dbIndividualsList/birthCol", this->columnWidth(3));
    settings.setValue("window/syncTab/dbIndividualsList/roleCol", this->columnWidth(4));
    settings.setValue("window/syncTab/dbIndividualsList/impCol", this->columnWidth(5));
}

void SyncDbIndividualsList::fill()
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    this->setColumnCount(6);
    this->setHorizontalHeaderLabels(QStringList()
                                    <<tr("ID")
                                    <<tr("Name")
                                    <<tr("Surname")
                                    <<tr("Birth")
                                    <<tr("Role")
                                    <<tr("Imported"));

    this->horizontalHeader()->setMinimumSectionSize(0);
    this->verticalHeader()->hide();
    this->applySettings();
    this->setFixedWidth(467);
    this->setWordWrap(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void SyncDbIndividualsList::applySettings()
{
    QSettings settings;
    this->setColumnWidth(0, settings.value("window/syncTab/dbIndividualsList/idCol").toInt());
    this->setColumnWidth(1, settings.value("window/syncTab/dbIndividualsList/nameCol").toInt());
    this->setColumnWidth(2, settings.value("window/syncTab/dbIndividualsList/surnameCol").toInt());
    this->setColumnWidth(3, settings.value("window/syncTab/dbIndividualsList/birthCol").toInt());
    this->setColumnWidth(4, settings.value("window/syncTab/dbIndividualsList/roleCol").toInt());
    this->setColumnWidth(5, settings.value("window/syncTab/dbIndividualsList/impCol").toInt());
}

void SyncDbIndividualsList::recordClicked(int row, int col)
{
    Identifier ref = _items[row][col]->getIdentifier();
    this->_selected = ref;
    emit(this->dbIndividualSelected(ref, _dbRecord));
}

void SyncDbIndividualsList::update()
{
    // Clear the list first
    this->remove();

    // Prepare list items
    if(_kernel.record()->isDbRecord(_dbRecord)){
        QVector<Identifier> indis = _kernel.record()->getDbRecordIndividuals(_dbRecord);
        for(auto current = indis.begin(); current != indis.end(); ++current){

            QString name = _kernel.record()->getIndividualNameGiven(*current);
            QString surname = _kernel.record()->getIndividualNameSurnameMarriedBirth(*current);
            Date birth = _kernel.record()->getIndividualBirthDate1(*current);
            bool imported = _kernel.record()->getDbRecordIndividualImported(_dbRecord, *current);
            DbRecordRole role = _kernel.record()->getDbRecordIndividualRole(_dbRecord, *current);

            QVector<TableCell *> item;
            item.append(new TableCellRef(*current));
            item.append(new TableCellText(*current, name));
            item.append(new TableCellText(*current, surname));
            item.append(new TableCellDate(*current, birth));
            item.append(new TableCellDbRecordRole(*current, role));
            item.append(new TableCellBool(*current, imported));

            this->_items.append(item);
        }

        this->setRowCount(this->_items.length());
        // Add items to list
        for(int i = 0; i < this->_items.length(); i++)
        {
            QVector<TableCell *> item = this->_items[i];
            this->setItem(i, 0, item[0]);
            this->setItem(i, 1, item[1]);
            this->setItem(i, 2, item[2]);
            this->setItem(i, 3, item[3]);
            this->setItem(i, 4, item[4]);
            this->setItem(i, 5, item[5]);
            this->setRowHeight(i, 40);
        }

        // Select previously selected individual
        TableCell *item = nullptr;
        for(auto it=this->_items.begin(); it!=this->_items.end(); it++)
        {
            if(it->first()->getIdentifier() == this->_selected)
            {
                item = it->first();
                break;
            }
        }

        if(item != nullptr){
            this->selectRow(this->row(item));
        } else {
            this->_selected = Identifier();
        }
    }
}

void SyncDbIndividualsList::remove()
{
    for(auto v1 = _items.begin(); v1 != _items.end(); v1++)
    {
        for(auto v2 = v1->begin(); v2 != v1->end(); v2++)
        {
            delete (*v2);
        }
    }

    _items.clear();
    this->setRowCount(0);
}

void SyncDbIndividualsList::recordChosen(Identifier ref)
{
    this->remove();
    _dbRecord = ref;

    if(_kernel.record()->isDbRecord(_dbRecord)){
        this->update();
    } else {
        _dbRecord = Identifier();
    }
}

