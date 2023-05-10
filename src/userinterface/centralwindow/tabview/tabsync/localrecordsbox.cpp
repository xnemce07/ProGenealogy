/*!
 *  \file   localrecordsbox.cpp
 *  \author Leopold Nemcek
 */
#include "localrecordsbox.h"

LocalRecordsBox::LocalRecordsBox(QString title, Kernel &k, Identifier &p, QWidget *parent): Box(title, BOX_NOT_CHECKABLE, k, p, parent)
{
    this->fill();
    
    connect(this, &LocalRecordsBox::selectedDbRecord, _matchList, &SyncLocalIndividualsList::refRecordSet);
    connect(this, &LocalRecordsBox::selectedDbRecord, _relList, &SyncLocalRelativesList::changedDbRecord);
    connect(this, &LocalRecordsBox::selectedDbRecord, this, &LocalRecordsBox::changedDbRecord);

    connect(this, &LocalRecordsBox::localMainIndiviudalChosen, this, &LocalRecordsBox::changedLocalMainIndi);

    connect(this, &LocalRecordsBox::localMainIndiviudalChosen, _relList, &SyncLocalRelativesList::changedMainIndividual);
    
    connect(_matchList, &SyncLocalIndividualsList::individualChosen, _partnerSubbox, &SubboxPartnerRef::mainChanged);
    connect(_matchList, &SyncLocalIndividualsList::individualChosen, this, &LocalRecordsBox::localMainIndiviudalChosen);

    connect(_partnerSubbox, &SubboxPartnerRef::partnerSelected, _relList, &SyncLocalRelativesList::changedPartner);

    connect(_relList, &SyncLocalRelativesList::selectedLocalIndividual, this, &LocalRecordsBox::selectedLocalIndividual);
    connect(_relList, &SyncLocalRelativesList::selectedLocalIndividual, this, &LocalRecordsBox::changedLocalIndiForMerge);
    
    connect(&_cbShowAll, &QCheckBox::stateChanged, _matchList, &SyncLocalIndividualsList::changeMatchesOnly);
    connect(this, &LocalRecordsBox::selectedDbIndividual, this, &LocalRecordsBox::changedDbIndividual);
    connect(&_pbAddPersonToRole, &QPushButton::clicked, this, &LocalRecordsBox::addPersonToRoleClicked);
}

LocalRecordsBox::~LocalRecordsBox()
{
    _partnerSubbox->deleteLater();
}


void LocalRecordsBox::fill(){

    // Init widgets
    _matchList = new SyncLocalIndividualsList(_kernel);
    _partnerSubbox = new SubboxPartnerRef(_kernel);
    _relList = new SyncLocalRelativesList(_kernel);

    _laIndividuals.setText(tr("Local individuals"));
    _laIndividuals.setStyleSheet("font-weight: bold;");
    _laPartner.setText(tr("Select partner"));
    _laPartner.setStyleSheet("font-weight: bold;");
    _laRelatives.setText(tr("Individual's relatives"));
    _laRelatives.setStyleSheet("font-weight: bold;");

    _laShowAll.setText(tr("Show only potential matches"));
    _laShowAll.setToolTip(tr("Show only individuals, that match selected individual from database"));
    _cbShowAll.setChecked(false);
    _cbShowAll.setTristate(false);
    _cbShowAll.setToolTip(tr("Show only individuals, that match selected individual from database"));

    _partnerSubbox->setDisabled(true);

    _pbAddPersonToRole.setText(tr("Add person to role"));
    _pbAddPersonToRole.setToolTip(tr("Import selected individual from database and import it as currently selected relative"));
    _pbAddPersonToRole.setEnabled(false);

    // Add widgets to layout
    _layout.addWidget(&_laIndividuals,0,0);
    _layout.addWidget(_matchList,1,0,1,2, Qt::AlignHCenter);
    _layout.addWidget(&_laShowAll, 2, 0, 1, 1, Qt::AlignHCenter);
    _layout.addWidget(&_cbShowAll, 2, 1, 1, 1, Qt::AlignHCenter);
    _layout.addWidget(&_laPartner,3,0);
    _layout.addWidget(_partnerSubbox, 4,0,1,2);
    _layout.addWidget(&_laRelatives,5,0);
    _layout.addWidget(_relList, 6,0,1,2, Qt::AlignHCenter);
    _layout.addWidget(&_pbAddPersonToRole, 7,0, Qt::AlignBottom);

    // Adjust widgets
    _layout.setHorizontalSpacing(this->tightSpacing());
    _layout.setVerticalSpacing(this->mediumSpacing());
    _layout.setContentsMargins(this->mediumSpacing(),this->mediumSpacing(),this->mediumSpacing(),this->mediumSpacing());
}

void LocalRecordsBox::addPersonToRole()
{
    DbRecordRole localRole = _relList->getSelectedRole();

   Identifier importedIndi;
   Identifier partner = _relList->getPartner();
   Identifier mainIndi = _matchList->getSelected();


   switch(localRole)
   {
   // ANCESTORS
   case DbRecordRole::BIR_F:
   case DbRecordRole::MAR_B_F:
   case DbRecordRole::MAR_G_F:
   case DbRecordRole::BUR_F:
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       _kernel.record()->setIndividualFather(mainIndi, importedIndi);
       break;
   case DbRecordRole::BIR_M:
   case DbRecordRole::MAR_B_M:
   case DbRecordRole::MAR_G_M:
   case DbRecordRole::BUR_M:
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       _kernel.record()->setIndividualMother(mainIndi, importedIndi);
       break;
   case DbRecordRole::BIR_M_M:
   case DbRecordRole::MAR_B_M_M:
   case DbRecordRole::MAR_G_M_M:
   case DbRecordRole::BUR_M_M:
   {
       Identifier parent = _kernel.record()->getIndividualMother(mainIndi);
       if(parent.isValid())
       {
           importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
            _kernel.record()->setIndividualMother(parent, importedIndi);
       }
   }
       break;
   case DbRecordRole::BIR_M_F:
   case DbRecordRole::MAR_B_M_F:
   case DbRecordRole::MAR_G_M_F:
   case DbRecordRole::BUR_M_F:
   {
       Identifier parent = _kernel.record()->getIndividualMother(mainIndi);
       if(parent.isValid())
       {
            importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
            _kernel.record()->setIndividualFather(parent, importedIndi);
       }
   }
       break;
   case DbRecordRole::BIR_F_M:
   {
       Identifier parent = _kernel.record()->getIndividualFather(mainIndi);
       if(parent.isValid())
       {
            importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
            _kernel.record()->setIndividualMother(parent, importedIndi);
       }
   }
       break;
   case DbRecordRole::BIR_F_F:
   {
       Identifier parent = _kernel.record()->getIndividualFather(mainIndi);
       if(parent.isValid())
       {
            importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
            _kernel.record()->setIndividualFather(parent, importedIndi);
       }
   }
       break;
   case DbRecordRole::BIR_M_M_M:
   {
       Identifier parent1 = _kernel.record()->getIndividualMother(mainIndi);
       if(parent1.isValid())
       {
           Identifier parent2 = _kernel.record()->getIndividualMother(parent1);
           if(parent2.isValid())
           {
               importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
               _kernel.record()->setIndividualMother(parent1, importedIndi);
           }
       }
   }
       break;
   case DbRecordRole::BIR_M_M_F:
   {
       Identifier parent1 = _kernel.record()->getIndividualMother(mainIndi);
       if(parent1.isValid())
       {
           Identifier parent2 = _kernel.record()->getIndividualMother(parent1);
           if(parent2.isValid())
           {
               importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
               _kernel.record()->setIndividualFather(parent1, importedIndi);
           }
       }
   }
       break;
   case DbRecordRole::BIR_F_M_M:
   {
       Identifier parent1 = _kernel.record()->getIndividualFather(mainIndi);
       if(parent1.isValid())
       {
           Identifier parent2 = _kernel.record()->getIndividualMother(parent1);
           if(parent2.isValid())
           {
               importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
               _kernel.record()->setIndividualMother(parent1, importedIndi);
           }
       }
   }
       break;
   case DbRecordRole::BIR_F_M_F:
   {
       Identifier parent1 = _kernel.record()->getIndividualFather(mainIndi);
       if(parent1.isValid())
       {
           Identifier parent2 = _kernel.record()->getIndividualMother(parent1);
           if(parent2.isValid())
           {
               importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
               _kernel.record()->setIndividualFather(parent1, importedIndi);
           }
       }
   }
       break;

       // OTHERS - BIRTH

   case DbRecordRole::BIR_GF_1:
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       _kernel.record()->setIndividualChristeningGodfather(mainIndi, importedIndi);
       break;
   case DbRecordRole::BIR_GF_2:
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       _kernel.record()->setIndividualChristeningGodmother(mainIndi, importedIndi);
       break;
   case DbRecordRole::BIR_GRANTED:
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       _kernel.record()->setIndividualChristeningPriest(mainIndi, importedIndi);
       break;

       // OTHERS - MARRIAGE

   case DbRecordRole::MAR_SV_1:
   {
       if(!partner.isValid())
       {
           break;
       }
       Identifier fam = _kernel.record()->getFamilyOfCouple(mainIndi, partner);
       if(!fam.isValid())
       {
           fam = _kernel.record()->newFamily();
           _kernel.record()->setFamilyHusband(fam, mainIndi);
           _kernel.record()->setFamilyWife(fam, partner);
       }
       _kernel.record()->setFamilyMarried(fam,true);
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       _kernel.record()->setFamilyMarriageWitness1(fam, importedIndi);
   }
       break;
   case DbRecordRole::MAR_SV_2:
   {
       if(!partner.isValid())
       {
           break;
       }
       Identifier fam = _kernel.record()->getFamilyOfCouple(mainIndi, partner);
       if(!fam.isValid())
       {
           fam = _kernel.record()->newFamily();
           _kernel.record()->setFamilyHusband(fam, mainIndi);
           _kernel.record()->setFamilyWife(fam, partner);
       }
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       _kernel.record()->setFamilyMarriageWitness2(fam, importedIndi);
   }
       break;
   case DbRecordRole::MAR_PRIEST:
   {
       if(!partner.isValid())
       {
           break;
       }
       Identifier fam = _kernel.record()->getFamilyOfCouple(mainIndi, partner);
       if(!fam.isValid())
       {
           fam = _kernel.record()->newFamily();
           _kernel.record()->setFamilyHusband(fam, mainIndi);
           _kernel.record()->setFamilyWife(fam, partner);
       }
       _kernel.record()->setFamilyMarried(fam,true);
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       _kernel.record()->setFamilyMarriagePriest(fam, importedIndi);
   }
       break;

       // OTHERS - BURIAL
   case DbRecordRole::CHILD:
   {
       if(!partner.isValid())
       {
           break;
       }
       Identifier fam = _kernel.record()->getFamilyOfCouple(mainIndi, partner);
       if(!fam.isValid())
       {
           fam = _kernel.record()->newFamily();
           _kernel.record()->setFamilyHusband(fam, mainIndi);
           _kernel.record()->setFamilyWife(fam, partner);
       }
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       _kernel.record()->setFamilyChild(fam, importedIndi);
   }
       break;
    case DbRecordRole::PARTNER:
   {
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       Identifier fam = _kernel.record()->newFamily();
       _kernel.record()->setFamilyHusband(fam, mainIndi);
       _kernel.record()->setFamilyWife(fam, importedIndi);
       break;
   }
    case DbRecordRole::BIR_MIDWIFE:
    {
       importedIndi = _kernel.record()->addDbRecordIndividualToMemory(_selectedDbIndividual);
       _kernel.record()->setIndividualBirthObstetrix(mainIndi, importedIndi);
       break;
    }
   default:
       break;

   }

   if(importedIndi.isValid()){
       _kernel.record()->setDbRecordIndividualImported(_selectedDbRecord, _selectedDbIndividual, true);
       emit(this->individualImported(importedIndi));
   }

}

void LocalRecordsBox::updateImportButton()
{
   DbRecordRole localRole = _relList->getSelectedRole();
    Identifier mainIndi = _matchList->getSelected();
    Identifier partner = _relList->getPartner();

    bool isRoleAvailable = false;

    switch(localRole)
    {
    case DbRecordRole::BIR_M_M:
    case DbRecordRole::MAR_B_M_M:
    case DbRecordRole::MAR_G_M_M:
    case DbRecordRole::BUR_M_M:
    {
        Identifier parent = _kernel.record()->getIndividualMother(mainIndi);
        isRoleAvailable = parent.isValid();
    }
        break;
    case DbRecordRole::BIR_M_F:
    case DbRecordRole::MAR_B_M_F:
    case DbRecordRole::MAR_G_M_F:
    case DbRecordRole::BUR_M_F:
    {
        Identifier parent = _kernel.record()->getIndividualMother(mainIndi);
        isRoleAvailable = parent.isValid();
    }
        break;
    case DbRecordRole::BIR_F_M:
    {
        Identifier parent = _kernel.record()->getIndividualFather(mainIndi);
        isRoleAvailable = parent.isValid();
    }
        break;
    case DbRecordRole::BIR_F_F:
    {
        Identifier parent = _kernel.record()->getIndividualFather(mainIndi);
        isRoleAvailable = parent.isValid();
    }
        break;
    case DbRecordRole::BIR_M_M_M:
    {
        Identifier parent1 = _kernel.record()->getIndividualMother(mainIndi);
        if(parent1.isValid())
        {
            Identifier parent2 = _kernel.record()->getIndividualMother(parent1);
            isRoleAvailable = parent2.isValid();
        }
    }
        break;
    case DbRecordRole::BIR_M_M_F:
    {
        Identifier parent1 = _kernel.record()->getIndividualMother(mainIndi);
        if(parent1.isValid())
        {
            Identifier parent2 = _kernel.record()->getIndividualMother(parent1);
            isRoleAvailable = parent2.isValid();
        }
    }
        break;
    case DbRecordRole::BIR_F_M_M:
    {
        Identifier parent1 = _kernel.record()->getIndividualFather(mainIndi);
        if(parent1.isValid())
        {
            Identifier parent2 = _kernel.record()->getIndividualMother(parent1);
            isRoleAvailable = parent2.isValid();
        }
    }
        break;
    case DbRecordRole::BIR_F_M_F:
    {
        Identifier parent1 = _kernel.record()->getIndividualFather(mainIndi);
        if(parent1.isValid())
        {
            Identifier parent2 = _kernel.record()->getIndividualMother(parent1);
            isRoleAvailable = parent2.isValid();
        }
    }
        break;
    case DbRecordRole::MAR_SV_1:
    case DbRecordRole::MAR_SV_2:
    case DbRecordRole::MAR_PRIEST:
    case DbRecordRole::CHILD:

    {
        isRoleAvailable = partner.isValid();
    }
        break;
    default:
        isRoleAvailable = true;
        break;
    }


    _pbAddPersonToRole.setEnabled(_isLocalIndividualSlotSelected && _isDbIndividualSelected && isRoleAvailable);
}

void LocalRecordsBox::defaultSettings()
{
    SyncLocalIndividualsList::defaultSettings();
    SyncLocalRelativesList::defaultSettings();
}

void LocalRecordsBox::saveSettings() const
{
    _matchList->saveSettings();
    _relList->saveSettings();
}


void LocalRecordsBox::changedDbIndividual(Identifier ref)
{
    this->_selectedDbIndividual = ref;
    this->_isDbIndividualSelected = true;
    this->updateImportButton();

}

void LocalRecordsBox::changedDbRecord(Identifier ref)
{
    this->_selectedDbRecord = ref;
    _isDbIndividualSelected = false;
    this->updateImportButton();

}

void LocalRecordsBox::changedLocalMainIndi(Identifier ref)
{
    _partnerSubbox->setDisabled(!ref.isValid());

    _isLocalIndividualSlotSelected = false;
    this->updateImportButton();
}

void LocalRecordsBox::changedLocalIndiForMerge(Identifier ref)
{
    _isLocalIndividualSlotSelected = !ref.isValid();
    this->updateImportButton();
}

void LocalRecordsBox::addPersonToRoleClicked(bool clicked)
{
    Q_UNUSED(clicked)
    this->addPersonToRole();
}

void LocalRecordsBox::showMatchesCBChecked(bool checked)
{
    Q_UNUSED(checked)
    emit(this->localMainIndiviudalChosen(Identifier()));
}


void LocalRecordsBox::update(){
    _matchList->update();
    _relList->update();
    _partnerSubbox->update();
}
