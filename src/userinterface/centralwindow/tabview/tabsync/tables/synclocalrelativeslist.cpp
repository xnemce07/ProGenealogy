/*!
 *  \file   synclocalrelativeslist.cpp
 *  \author Leopold Nemcek
 */
#include "synclocalrelativeslist.h"



SyncLocalRelativesList::SyncLocalRelativesList(Kernel &k) :
    _kernel(k)
{
    _mainIndividual = Identifier();
    _selected = Identifier();
    _dbRecord = Identifier();
    QFontMetrics font(_defaultFont);
    this->setTextElideMode(Qt::ElideRight);
    this->fill();
}

void SyncLocalRelativesList::defaultSettings()
{
    QSettings settings;
    settings.setValue("window/syncTab/localRelated/idCol", 45);
    settings.setValue("window/syncTab/localRelated/nameCol", 105);
    settings.setValue("window/syncTab/localRelated/surnameCol", 105);
    settings.setValue("window/syncTab/localRelated/birthCol", 60);
    settings.setValue("window/syncTab/localRelated/relCol", 85);
}

void SyncLocalRelativesList::saveSettings() const
{
    QSettings settings;
    settings.setValue("window/syncTab/localRelated/idCol", this->columnWidth(0));
    settings.setValue("window/syncTab/localRelated/nameCol", this->columnWidth(1));
    settings.setValue("window/syncTab/localRelated/surnameCol", this->columnWidth(2));
    settings.setValue("window/syncTab/localRelated/birthCol", this->columnWidth(3));
    settings.setValue("window/syncTab/localRelated/relCol", this->columnWidth(4));
}

Identifier SyncLocalRelativesList::getSelectedIndividual() const
{
    return this->_selected;
}

Identifier SyncLocalRelativesList::getPartner() const
{
    return this->_partner;
}

DbRecordRole SyncLocalRelativesList::getSelectedRole() const
{
    return this->_selectedRole;
}

void SyncLocalRelativesList::fill()
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);

    this->setColumnCount(5);
    this->setHorizontalHeaderLabels(QStringList()
                                    <<tr("ID")
                                    <<tr("Name")
                                    <<tr("Surname")
                                    <<tr("Birth")
                                    <<tr("Relation"));

    this->horizontalHeader()->setMinimumSectionSize(0);
    this->verticalHeader()->hide();
    this->applySettings();
    this->setFixedWidth(417);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setWordWrap(true);

    connect(this, &SyncLocalRelativesList::cellClicked, this, &SyncLocalRelativesList::individualClicked);

}

QVector<QPair<Identifier, DbRecordRole>> SyncLocalRelativesList::getRelatives()
{
    QVector<QPair<Identifier, DbRecordRole>> relatives;
    DbRecordType recType = _kernel.record()->getDbRecordType(_dbRecord);
    if(recType == DbRecordType::RT_BIRTH)
    {
        /// PARENTS
        QPair<Identifier, DbRecordRole> main(_mainIndividual,DbRecordRole::BIR_MAIN);
        relatives.append(main);
        
        QPair<Identifier, DbRecordRole> m(_kernel.record()->getIndividualMother(_mainIndividual), DbRecordRole::BIR_M);
        relatives.append(m);
        
        QPair<Identifier, DbRecordRole> f(_kernel.record()->getIndividualFather(_mainIndividual), DbRecordRole::BIR_F);
        relatives.append(f);
        
        QPair<Identifier, DbRecordRole> mm(_kernel.record()->getIndividualMother(m.first), DbRecordRole::BIR_M_M);
        relatives.append(mm);
        
        QPair<Identifier, DbRecordRole> mf(_kernel.record()->getIndividualFather(m.first), DbRecordRole::BIR_M_F);
        relatives.append(mf);
        
        QPair<Identifier, DbRecordRole> fm(_kernel.record()->getIndividualMother(f.first), DbRecordRole::BIR_F_M);
        relatives.append(fm);
        
        QPair<Identifier, DbRecordRole> ff(_kernel.record()->getIndividualFather(f.first), DbRecordRole::BIR_F_F);
        relatives.append(ff);
        
        QPair<Identifier, DbRecordRole> mmm(_kernel.record()->getIndividualMother(mm.first), DbRecordRole::BIR_M_M_M);
        relatives.append(mmm);
        
        QPair<Identifier, DbRecordRole> mmf(_kernel.record()->getIndividualFather(mm.first), DbRecordRole::BIR_M_M_F);
        relatives.append(mmf);
        
        QPair<Identifier, DbRecordRole> fmm(_kernel.record()->getIndividualMother(fm.first), DbRecordRole::BIR_F_M_M);
        relatives.append(fmm);
        
        QPair<Identifier, DbRecordRole> fmf(_kernel.record()->getIndividualFather(fm.first), DbRecordRole::BIR_F_M_F);
        relatives.append(fmf);

        /// PARTNERS
        QVector<Identifier> partners = _kernel.record()->getIndividualPartners(main.first);
        for(auto part = partners.begin(); part != partners.end(); ++part){
            QPair<Identifier, DbRecordRole> p(*part, DbRecordRole::PARTNER);
            relatives.append(p);
        }
        QPair<Identifier, DbRecordRole> p(Identifier(), DbRecordRole::PARTNER);
        relatives.append(p);

        /// CHILDREN
        QVector<Identifier> children = _kernel.record()->getChildrenOfIndividual(main.first);
        for(auto child = children.begin(); child != children.end(); ++child){
            QPair<Identifier, DbRecordRole> c(*child, DbRecordRole::CHILD);
            relatives.append(c);
        }
        QPair<Identifier, DbRecordRole> c(Identifier(), DbRecordRole::CHILD);
        relatives.append(c);


        /// OTHERS
        QPair<Identifier, DbRecordRole> midwife(_kernel.record()->getIndividualBirthObstetrix(_mainIndividual), DbRecordRole::BIR_MIDWIFE);
        relatives.append(midwife);
        
        QPair<Identifier, DbRecordRole> granted(_kernel.record()->getIndividualChristeningPriest(_mainIndividual), DbRecordRole::BIR_GRANTED);
        relatives.append(granted);
        
        QPair<Identifier, DbRecordRole> gf1(_kernel.record()->getIndividualChristeningGodfather(_mainIndividual), DbRecordRole::BIR_GF_1);
        relatives.append(gf1);
        
        QPair<Identifier, DbRecordRole> gf2(_kernel.record()->getIndividualChristeningGodmother(_mainIndividual), DbRecordRole::BIR_GF_2);
        relatives.append(gf2);



    } else if(recType == DbRecordType::RT_BURIAL) {
        /// PARENTS
        QPair<Identifier, DbRecordRole> main(_mainIndividual,DbRecordRole::BIR_MAIN);
        relatives.append(main);
        
        QPair<Identifier, DbRecordRole> m(_kernel.record()->getIndividualMother(_mainIndividual), DbRecordRole::BIR_M);
        relatives.append(m);
        
        QPair<Identifier, DbRecordRole> f(_kernel.record()->getIndividualFather(_mainIndividual), DbRecordRole::BIR_F);
        relatives.append(f);
        
        QPair<Identifier, DbRecordRole> mm(_kernel.record()->getIndividualMother(m.first), DbRecordRole::BIR_M_M);
        relatives.append(mm);
        
        QPair<Identifier, DbRecordRole> mf(_kernel.record()->getIndividualFather(m.first), DbRecordRole::BIR_M_F);
        relatives.append(mf);

        /// PARTNERS
        QVector<Identifier> partners = _kernel.record()->getIndividualPartners(main.first);
        for(auto part = partners.begin(); part != partners.end(); ++part){
            QPair<Identifier, DbRecordRole> p(*part, DbRecordRole::PARTNER);
            relatives.append(p);
        }
        QPair<Identifier, DbRecordRole> p(Identifier(), DbRecordRole::PARTNER);
        relatives.append(p);


        /// CHILDREN
        QVector<Identifier> children = _kernel.record()->getChildrenOfIndividual(main.first);
        for(auto child = children.begin(); child != children.end(); ++child){
            QPair<Identifier, DbRecordRole> c(*child, DbRecordRole::CHILD);
            relatives.append(c);
        }
        QPair<Identifier, DbRecordRole> c(Identifier(), DbRecordRole::CHILD);
        relatives.append(c);



    } else if(recType == DbRecordType::RT_MARRIAGE || recType == DbRecordType::RT_MARRIAGE_BRIDE || recType == DbRecordType::RT_MARRIAGE_GROOM) {
        
        QPair<Identifier, DbRecordRole> main(_mainIndividual,DbRecordRole::BIR_MAIN);
        relatives.append(main);

        /// PARENTS
        
        QPair<Identifier, DbRecordRole> m(_kernel.record()->getIndividualMother(_mainIndividual), DbRecordRole::BIR_M);
        relatives.append(m);
        
        QPair<Identifier, DbRecordRole> f(_kernel.record()->getIndividualFather(_mainIndividual), DbRecordRole::BIR_F);
        relatives.append(f);
        
        QPair<Identifier, DbRecordRole> mm(_kernel.record()->getIndividualMother(m.first), DbRecordRole::BIR_M_M);
        relatives.append(mm);
        
        QPair<Identifier, DbRecordRole> mf(_kernel.record()->getIndividualFather(m.first), DbRecordRole::BIR_M_F);
        relatives.append(mf);

        /// PARTNERS
        QVector<Identifier> partners = _kernel.record()->getIndividualPartners(main.first);
        for(auto part = partners.begin(); part != partners.end(); ++part){
            QPair<Identifier, DbRecordRole> p(*part, DbRecordRole::PARTNER);
            relatives.append(p);
        }
        QPair<Identifier, DbRecordRole> p(Identifier(), DbRecordRole::PARTNER);
        relatives.append(p);

        /// CHILDREN
        QVector<Identifier> children = _kernel.record()->getChildrenOfIndividual(main.first);
        for(auto child = children.begin(); child != children.end(); ++child){
            QPair<Identifier, DbRecordRole> c(*child, DbRecordRole::CHILD);
            relatives.append(c);
        }
        QPair<Identifier, DbRecordRole> c(Identifier(), DbRecordRole::CHILD);
        relatives.append(c);

        /// MARRIAGE
        if(_partner.isValid()/* && _kernel.record()->getFamilyOfCouple(_mainIndividual, _partner).isValid()*/ ){
            Identifier fam = _kernel.record()->getFamilyOfCouple(_mainIndividual, _partner);
            QPair<Identifier, DbRecordRole> p(_kernel.record()->getFamilyMarriagePriest(fam), DbRecordRole::MAR_PRIEST);
            relatives.append(p);
            
            QPair<Identifier, DbRecordRole> w1(_kernel.record()->getFamilyMarriageWitness1(fam), DbRecordRole::MAR_SV_1);
            relatives.append(w1);
            
            QPair<Identifier, DbRecordRole> w2(_kernel.record()->getFamilyMarriageWitness2(fam), DbRecordRole::MAR_SV_2);
            relatives.append(w2);
        }
    }




    return relatives;
}

void SyncLocalRelativesList::applySettings()
{
    QSettings settings;
    this->setColumnWidth(0, settings.value("window/syncTab/localRelated/idCol").toInt());
    this->setColumnWidth(1, settings.value("window/syncTab/localRelated/nameCol").toInt());
    this->setColumnWidth(2, settings.value("window/syncTab/localRelated/surnameCol").toInt());
    this->setColumnWidth(3, settings.value("window/syncTab/localRelated/birthCol").toInt());
    this->setColumnWidth(4, settings.value("window/syncTab/localRelated/relCol").toInt());

}

void SyncLocalRelativesList::selectRow(int row)
{
    // Uncolor other rows
    for(int r = 0; r < this->rowCount(); r++){
        for(int i = 0; i < 5; i++){
            this->item(r, i)->setData(Qt::BackgroundRole, QVariant());
        }
    }

    // Color selected row
    if(row < this->rowCount()){
        for(int i = 0; i < 5; i++){
            this->item(row,i)->setData(Qt::BackgroundRole, QBrush(QColor("#DFF2BF")));
        }
    }
}

void SyncLocalRelativesList::individualClicked(int row, int col)
{
    selectRow(row);
    this->_selected = _items[row][col]->getIdentifier();

    this->_selectedRole = dynamic_cast<TableCellDbRecordRole*>(_items[row][4])->getRole();

    DbRecordType type = _kernel.record()->getDbRecordType(_dbRecord);
    if(type == DbRecordType::RT_MARRIAGE || type == DbRecordType::RT_MARRIAGE_BRIDE || type == DbRecordType::RT_MARRIAGE_GROOM){
        emit(this->selectedLocalIndividual(_selected, _partner));
    } else {
        emit(this->selectedLocalIndividual(_selected, Identifier()));
    }

}

void SyncLocalRelativesList::update()
{
    this->remove();

    DbRecordType recType = _kernel.record()->getDbRecordType(_dbRecord);
    if(recType != DbRecordType::RT_UNDEFINED && _kernel.record()->isIndividual(_mainIndividual))
    {
        QVector<QPair<Identifier, DbRecordRole>> relatives = getRelatives();
        for(auto p = relatives.begin();p != relatives.end(); ++p)
        {
            QString name;
            QString surname;
            Date birth;

            if(p->first.isValid()){
                name = this->_kernel.record()->getIndividualNameGiven(p->first);
                surname = this->_kernel.record()->getIndividualNameSurnameMarriedBirth(p->first);
                birth = this->_kernel.record()->getIndividualBirthDate1(p->first);
            }

            QVector<TableCell *> item;
            item.append(new TableCellRef(p->first));
            item.append(new TableCellText(p->first, name));
            item.append(new TableCellText(p->first, surname));
            item.append(new TableCellDate(p->first, birth));
            item.append(new TableCellDbRecordRole(p->first, p->second));

            this->_items.append(item);
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
            this->setItem(i, 4, item[4]);
            this->setRowHeight(i, 55);
        }

        // Select previously selected individual
        if(this->_selected.isValid()){
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
                this->clearSelection();
            }
        }
    }
}

void SyncLocalRelativesList::remove()
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

void SyncLocalRelativesList::changedDbRecord(Identifier rec)
{
    this->_dbRecord = rec;
    this->update();
}

void SyncLocalRelativesList::changedMainIndividual(Identifier ref)
{
    this->_mainIndividual = ref;
    this->update();
}

void SyncLocalRelativesList::changedPartner(Identifier ref)
{
    this->_partner = ref;
    this->update();
    DbRecordType type = _kernel.record()->getDbRecordType(_dbRecord);
    if(type == DbRecordType::RT_MARRIAGE || type == DbRecordType::RT_MARRIAGE_BRIDE || type == DbRecordType::RT_MARRIAGE_GROOM){
        emit(this->selectedLocalIndividual(_selected, _partner));
    }
}
