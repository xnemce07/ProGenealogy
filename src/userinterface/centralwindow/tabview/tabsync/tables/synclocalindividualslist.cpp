/*!
 *  \file   syncmatchlist.cpp
 *  \author Leopold Nemcek
 */
#include "synclocalindividualslist.h"


SyncLocalIndividualsList::SyncLocalIndividualsList(Kernel &k):
    _kernel(k)
{
    QFontMetrics font(_defaultFont);
    this->setTextElideMode(Qt::ElideRight);
    this->fill();
    
    
    connect(this, &SyncLocalIndividualsList::cellClicked, this, &SyncLocalIndividualsList::individualClicked);

}

Identifier SyncLocalIndividualsList::getSelected() const
{
    return this->_selected;
}

void SyncLocalIndividualsList::defaultSettings()
{
    QSettings settings;
    settings.setValue("window/syncTab/localMatch/idCol", 25);
    settings.setValue("window/syncTab/localMatch/nameCol", 155);
    settings.setValue("window/syncTab/localMatch/surnameCol", 105);
    settings.setValue("window/syncTab/localMatch/birthCol", 55);
}

void SyncLocalIndividualsList::saveSettings() const
{
    QSettings settings;
    settings.setValue("window/syncTab/localMatch/idCol", this->columnWidth(0));
    settings.setValue("window/syncTab/localMatch/nameCol", this->columnWidth(1));
    settings.setValue("window/syncTab/localMatch/surnameCol", this->columnWidth(2));
    settings.setValue("window/syncTab/localMatch/birthCol", this->columnWidth(3));
}

QVector<Identifier> SyncLocalIndividualsList::getRefIndividualMatches()
{
    QVector<Identifier> matches;

    if(_kernel.record()->isDbRecord(_dbRec)){
        QVector<Identifier> indis;

        DbRecordType type = _kernel.record()->getDbRecordType(_dbRec);
        if(type == DbRecordType::RT_BIRTH){
            indis.append(_kernel.record()->getDbRecordIndividualByRole(_dbRec, DbRecordRole::BIR_MAIN));
        } else if(type == DbRecordType::RT_BURIAL){
            indis.append(_kernel.record()->getDbRecordIndividualByRole(_dbRec, DbRecordRole::BUR_MAIN));
        } else if(type == DbRecordType::RT_MARRIAGE){
            indis.append(_kernel.record()->getDbRecordIndividualByRole(_dbRec, DbRecordRole::MAR_BRIDE));
            indis.append(_kernel.record()->getDbRecordIndividualByRole(_dbRec, DbRecordRole::MAR_GROOM));
        }

        foreach(Identifier indi, indis){
            if(_kernel.record()->isDbIndividual(indi)){
                QString sNameGiven = _kernel.record()->getIndividualNameGiven(indi);
                QString sSurname = _kernel.record()->getIndividualNameSurnameBirthMarried(indi);
                Date sBirth = _kernel.record()->getIndividualBirthDate1(indi);
                Date sDeath = _kernel.record()->getIndividualDeathDate1(indi);

                Identifier first, last;
                first = _kernel.record()->getIdentifierIndividualFirst();
                last = _kernel.record()->getIdentifierIndividualLast();
                for(Identifier current = first; current != last; _kernel.record()->getIdentifierIndividualNext(current))
                {
                    QString cNameGiven = _kernel.record()->getIndividualNameGiven(current);
                    QString cSurnameBirth = _kernel.record()->getIndividualNameSurnameBirth(current);
                    QString cSurnameMarried = _kernel.record()->getIndividualNameSurnameMarried(current);
                    Date cBirth = _kernel.record()->getIndividualBirthDate1(current);
                    Date cDeath = _kernel.record()->getIndividualDeathDate1(current);

                    if(sNameGiven == cNameGiven && (sSurname == cSurnameBirth || sSurname == cSurnameMarried) && !sNameGiven.isEmpty() && !sSurname.isEmpty())
                    {
                        matches.append(current);
                    } else if((sNameGiven == cNameGiven && !sNameGiven.isEmpty())|| (sSurname == cSurnameBirth && !sSurname.isEmpty()) || (sSurname == cSurnameMarried && !sSurname.isEmpty()))
                    {
                        if((sBirth.year() == cBirth.year() && sBirth.isValid()) ||( sDeath.year() == cDeath.year()  && sDeath.isValid())){
                            matches.append(current);
                        }
                    }
                }
            }
        }
    }

    return matches;
}

void SyncLocalIndividualsList::fill()
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);

    this->setColumnCount(4);
    this->setHorizontalHeaderLabels(QStringList()
                                    <<tr("ID")
                                    <<tr("Name")
                                    <<tr("Surname")
                                    <<tr("Birth"));

    this->horizontalHeader()->setMinimumSectionSize(0);
    this->verticalHeader()->hide();
    this->applySettings();
    this->setFixedWidth(357);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void SyncLocalIndividualsList::applySettings()
{
    QSettings settings;
    this->setColumnWidth(0, settings.value("window/syncTab/localMatch/idCol").toInt());
    this->setColumnWidth(1, settings.value("window/syncTab/localMatch/nameCol").toInt());
    this->setColumnWidth(2, settings.value("window/syncTab/localMatch/surnameCol").toInt());
    this->setColumnWidth(3, settings.value("window/syncTab/localMatch/birthCol").toInt());
}

void SyncLocalIndividualsList::selectRow(int row)
{
    // Uncolor other rows
    for(int r = 0; r < this->rowCount(); r++){
        for(int i = 0; i < 4; i++){
            this->item(r, i)->setData(Qt::BackgroundRole, QVariant());
        }
    }

    // Color selected row
    if(row < this->rowCount()){
        for(int i = 0; i < 4; i++){
            this->item(row,i)->setData(Qt::BackgroundRole, QBrush(QColor("#a2caf2")));
        }
    }
}

void SyncLocalIndividualsList::individualClicked(int row, int col)
{
    this->_selected = _items[row][col]->getIdentifier();
    this->selectRow(row);
    emit(this->individualChosen(this->_selected));
}

void SyncLocalIndividualsList::update()
{
    this->remove();
    if(_showMatchesOnly)
    {
        Identifier first, last;
        first = this->_kernel.record()->getIdentifierIndividualFirst();
        last  = this->_kernel.record()->getIdentifierIndividualLast();

        for(Identifier current = first; current != last; _kernel.record()->getIdentifierIndividualNext(current))
        {
            if(! _kernel.record()->isIndividual(current))
            {
                qDebug() << "ID not INDI - B U G !!!! " << current.get();
                continue;
            }

            QString name = this->_kernel.record()->getIndividualNameGiven(current);
            QString surname = this->_kernel.record()->getIndividualNameSurnameBirthMarried(current);
            Date birth = this->_kernel.record()->getIndividualBirthDate1(current);

            QVector<TableCell *> item;
            item.append(new TableCellRef(current));
            item.append(new TableCellText(current, name));
            item.append(new TableCellText(current, surname));
            item.append(new TableCellDate(current, birth));
            //IndividualItemModel item(current, name, surname, birth);

            this->_items.append(item);

            // End the cycle
            if(current == last)
                break;
        }
    } else
    {
        QVector<Identifier> matches = getRefIndividualMatches();
        for(auto i = matches.begin();i != matches.end(); ++i){


            QString name = this->_kernel.record()->getIndividualNameGiven(*i);
            QString surname = this->_kernel.record()->getIndividualNameSurnameBirthMarried(*i);
            Date birth = this->_kernel.record()->getIndividualBirthDate1(*i);

            QVector<TableCell *> item;
            item.append(new TableCellRef(*i));
            item.append(new TableCellText(*i, surname));
            item.append(new TableCellText(*i, name));
            item.append(new TableCellDate(*i, birth));
            //IndividualItemModel item(current, name, surname, birth);

            this->_items.append(item);
        }
    }

    // Set row count
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

    // Select proband
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
        emit(this->individualChosen(this->_selected));
    } else {
        emit(this->individualChosen(Identifier()));
    }
}

void SyncLocalIndividualsList::updateSync()
{
    if(!_showMatchesOnly){
        this->remove();
        this->setRowCount(0);
        this->clearSelection();
        this->clearFocus();
    }
}

void SyncLocalIndividualsList::remove()
{
    for(auto v1 = _items.begin(); v1 != _items.end(); v1++)
    {
        v1->clear();
    }

    _items.clear();
}

void SyncLocalIndividualsList::refRecordSet(Identifier ref)
{
    this->_dbRec = ref;
    if(!_showMatchesOnly){
        this->update();
    }
}

void SyncLocalIndividualsList::changeMatchesOnly(int all)
{
    _showMatchesOnly = (all != Qt::CheckState::Checked);
    this->update();
}
