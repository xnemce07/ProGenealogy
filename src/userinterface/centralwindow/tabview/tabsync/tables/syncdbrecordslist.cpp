/*!
 *  \file   syncdbrecordslist.cpp
 *  \author Leopold Nemcek
 */
#include "syncdbrecordslist.h"

SyncDbRecordsList::SyncDbRecordsList(Kernel &k):
    _kernel(k)
{
    QFontMetrics font(_defaultFont);
    this->setTextElideMode(Qt::ElideRight);
    this->fill();

    connect(this, &SyncDbRecordsList::cellClicked, this, &SyncDbRecordsList::individualClicked);
}

Identifier SyncDbRecordsList::getSelectedRef() const
{
    return this->_selected;
}

void SyncDbRecordsList::setImportedFilter(bool state)
{
    _filterImported = state;
    update();
}

void SyncDbRecordsList::defaultSettings()
{
    QSettings settings;
    settings.setValue("window/syncTab/dbRecordsList/idCol", 25);
    settings.setValue("window/syncTab/dbRecordsList/nameCol", 175);
    settings.setValue("window/syncTab/dbRecordsList/typeCol", 130);
    settings.setValue("window/syncTab/dbRecordsList/impCol", 85);
}

void SyncDbRecordsList::saveSettings() const
{
    QSettings settings;
    settings.setValue("window/syncTab/dbRecordsList/idCol", this->columnWidth(0));
    settings.setValue("window/syncTab/dbRecordsList/nameCol", this->columnWidth(1));
    settings.setValue("window/syncTab/dbRecordsList/typeCol", this->columnWidth(2));
    settings.setValue("window/syncTab/dbRecordsList/impCol", this->columnWidth(3));
}

void SyncDbRecordsList::fill()
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    this->setColumnCount(4);
    this->setHorizontalHeaderLabels(QStringList()
                                    <<tr("ID")
                                    <<tr("Name")
                                    <<tr("Type")
                                    <<tr("Imported"));

    this->horizontalHeader()->setMinimumSectionSize(0);
    this->verticalHeader()->hide();
    this->applySettings();
    this->setFixedWidth(432);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void SyncDbRecordsList::applySettings()
{
    QSettings settings;
    this->setColumnWidth(0, settings.value("window/syncTab/dbRecordsList/idCol").toInt());
    this->setColumnWidth(1, settings.value("window/syncTab/dbRecordsList/nameCol").toInt());
    this->setColumnWidth(2, settings.value("window/syncTab/dbRecordsList/typeCol").toInt());
    this->setColumnWidth(3, settings.value("window/syncTab/dbRecordsList/impCol").toInt());
}

void SyncDbRecordsList::individualClicked(int row, int col)
{
    Identifier ref = _items[row][col]->getIdentifier();
    this->_selected = ref;
    emit this->recordChosen(ref);
}

void SyncDbRecordsList::update()
{
    // Clear list first
    this->remove();

    Identifier first,last;

    first = this->_kernel.record()->getIdentifierDbRecordFirst();
    last = this->_kernel.record()->getIdentifierDbRecordLast();

    // Prepare list items
    for(Identifier current = first; current != last; _kernel.record()->getIdentifierDbRecordNext(current))
    {
        bool imported = this->_kernel.record()->getDbRecordImported(current);
        if(!_filterImported || !imported){

            QString name = this->_kernel.record()->getStringDbRecord(current);
            DbRecordType type = this->_kernel.record()->getDbRecordType(current);

            QVector<TableCell *> item;
            item.append(new TableCellRef(current));
            item.append(new TableCellText(current, name));
            item.append(new TableCellDbRecordType(current, type));
            item.append(new TableCellBool(current, imported));

            this->_items.append(item);
        }

        if(current == last)
            break;
    }

    this->setRowCount(this->_items.length());

    // Insert items into the table
    for(int i = 0; i < this->_items.length(); i++)
    {
        QVector<TableCell *> item = this->_items[i];
        this->setItem(i, 0, item[0]);
        this->setItem(i, 1, item[1]);
        this->setItem(i, 2, item[2]);
        this->setItem(i, 3, item[3]);
    }


    // Select previously selected record
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
        emit(this->recordChosen(Identifier()));
    }



}

void SyncDbRecordsList::remove()
{
    for(auto v1 = _items.begin(); v1 != _items.end(); v1++)
    {
        for(auto v2 = v1->begin(); v2 != v1->end(); v2++)
        {
            delete (*v2);
        }
    }

    _items.clear();
}
