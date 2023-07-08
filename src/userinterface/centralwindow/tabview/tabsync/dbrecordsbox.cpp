/*!
 *  \file   dbrecordsbox.cpp
 *  \author Leopold Nemcek
 */
#include "dbrecordsbox.h"

DbRecordsBox::DbRecordsBox(QString title, Kernel &k, Identifier &p, QWidget *parent): Box(title, BOX_NOT_CHECKABLE, k, p, parent)
{
    this->fill();

    connect(&_pbSetRecordImported, &QPushButton::clicked,this,&DbRecordsBox::setRecordImportedClicked);
    connect(&_pbSetIndividualImported, &QPushButton::clicked,this,&DbRecordsBox::setIndividualImportedClicked);
    connect(&_pbImportIndividual, &QPushButton::clicked,this,&DbRecordsBox::importIndividualClicked);

    connect(_recordsView, &SyncDbRecordsList::recordChosen, _individualsView, &SyncDbIndividualsList::recordChosen);

    connect(_recordsView, &SyncDbRecordsList::recordChosen,this, &DbRecordsBox::selectedDbRecord);
    connect(this, &DbRecordsBox::selectedDbRecord, this, &DbRecordsBox::changedDbRecord);

    connect(_individualsView, &SyncDbIndividualsList::dbIndividualSelected, this, &DbRecordsBox::selectedDbIndividual);
    connect(this, &DbRecordsBox::selectedDbIndividual, this, &DbRecordsBox::changedDbIndividual);

    connect(&_cbFilterImported, &QCheckBox::clicked, this, &DbRecordsBox::filterImportedRecrodsChecked);
}

DbRecordsBox::~DbRecordsBox()
{
    _recordsView->deleteLater();
    _individualsView->deleteLater();
}

void DbRecordsBox::update()
{
    _recordsView->update();
    _individualsView->update();
}

void DbRecordsBox::updateSync(){
    _recordsView->update();
    _individualsView->update();
}

void DbRecordsBox::fill(){

    QSettings settings;


    _recordsView = new SyncDbRecordsList(_kernel);
    _individualsView = new SyncDbIndividualsList(_kernel);

    // Init widgets
    _laRecords.setText(tr("Records"));
    _laRecords.setStyleSheet("font-weight: bold");
    _laIndis.setText(tr("Individuals in record"));
    _laIndis.setStyleSheet("font-weight: bold");

    _laFilterImported.setText(tr("Hide imported records"));
    bool filterImported = settings.value("window/syncTab/filterImportedRecords",false).toBool();
    Qt::CheckState filterImportedState = (filterImported)? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
    _cbFilterImported.setTristate(false);
    _cbFilterImported.setCheckState(filterImportedState);
    _recordsView->setImportedFilter(filterImported);

    _pbImportIndividual.setText(tr("Import individual"));
    _pbImportIndividual.setToolTip(tr("Import individual"));
    _pbSetRecordImported.setText(tr("Set record as imported"));
    _pbSetRecordImported.setToolTip(tr("Change record's imported state"));
    _pbSetIndividualImported.setText(tr("Set individual as imported"));
    _pbSetIndividualImported.setToolTip(tr("Change individual's imported state"));

    _pbSetRecordImported.setEnabled(false);
    _pbSetIndividualImported.setEnabled(false);

    // Add widgets to layout
    _layout.addWidget(&_laRecords,0,0,1,1);
    _layout.addWidget(_recordsView, 1, 0, 1, 3, Qt::AlignHCenter);
    _layout.addWidget(&_laFilterImported,2,0, Qt::AlignHCenter);
    _layout.addWidget(&_cbFilterImported,2,1, Qt::AlignHCenter);
    _layout.addWidget(&_laIndis,3,0,1,1);
    _layout.addWidget(_individualsView, 4, 0, 1, 3, Qt::AlignHCenter);
    _layout.addWidget(&_pbSetRecordImported,5,0,Qt::AlignBottom);
    _layout.addWidget(&_pbSetIndividualImported,5,1,Qt::AlignBottom);
    _layout.addWidget(&_pbImportIndividual,5,2,Qt::AlignBottom);

    // Adjust widgets

    _pbImportIndividual.setFixedHeight(30);
    _pbSetIndividualImported.setFixedHeight(30);
    _pbSetRecordImported.setFixedHeight(30);

    _layout.setContentsMargins(this->mediumSpacing(),this->mediumSpacing(),this->mediumSpacing(),this->mediumSpacing());
    _layout.setVerticalSpacing(this->mediumSpacing());
    _layout.setHorizontalSpacing(this->tightSpacing());
}

void DbRecordsBox::addRecord()
{
    Identifier indi = _individualsView->getSelectedRef();
    Identifier rec = _recordsView->getSelectedRef();
    Identifier newIndiRef = _kernel.record()->addDbRecordIndividualToMemory(indi);
    DbRecordRole role = _kernel.record()->getDbRecordIndividualRole(rec, indi);
    DbRecordType type = _kernel.record()->getDbRecordType(rec);
    if(type == DbRecordType::RT_BIRTH && role == DbRecordRole::BIR_MAIN){
        Identifier sourceRef = _kernel.record()->addDbSourceToMemory(_kernel.record()->getDbRecordSourceRef(rec));
        _kernel.record()->setIndividualBirthSourceCitationRef(newIndiRef, sourceRef);
    } else if(type == DbRecordType::RT_BURIAL && role == DbRecordRole::BUR_MAIN){
        Identifier sourceRef = _kernel.record()->addDbSourceToMemory(_kernel.record()->getDbRecordSourceRef(rec));
        _kernel.record()->setIndividualDeathSourceCitationRef(newIndiRef, sourceRef);
    }
    _kernel.record()->setDbRecordIndividualImported(rec, indi, true);
    emit(this->individualImported(newIndiRef));
}

void DbRecordsBox::defaultSettings()
{
    SyncDbRecordsList::defaultSettings();
    SyncDbIndividualsList::defaultSettings();
}

void DbRecordsBox::saveSettings() const
{
    _recordsView->saveSettings();
    _individualsView->saveSettings();
}

void DbRecordsBox::importIndividualClicked()
{
    this->addRecord();
}

void DbRecordsBox::setIndividualImportedClicked()
{
    Identifier indi = _individualsView->getSelectedRef();
    Identifier rec = _recordsView->getSelectedRef();
    bool current = _kernel.record()->getDbRecordIndividualImported(rec, indi);
    _kernel.record()->setDbRecordIndividualImported(rec, indi, !current);

    emit(this->dataChanged());
}

void DbRecordsBox::setRecordImportedClicked()
{
    Identifier rec = _recordsView->getSelectedRef();
    bool current = _kernel.record()->getDbRecordImported(rec);
    DbRecordType type = _kernel.record()->getDbRecordType(rec);
    unsigned int pk = _kernel.record()->getDbRecordPrimaryKey(rec);
    _kernel.record()->setDbRecordImported(rec, !current);
    emit(this->dataChanged());
    _kernel.demosSetRecordImported(!current, pk, type);
}

void DbRecordsBox::changedDbRecord(Identifier ref)
{
    Q_UNUSED(ref)
    _pbImportIndividual.setEnabled(false);
    _pbSetIndividualImported.setEnabled(false);
    if(ref.isValid()){
        _pbSetRecordImported.setEnabled(true);
    } else {
        _pbSetRecordImported.setEnabled(false);
    }
}

void DbRecordsBox::changedDbIndividual(Identifier ref)
{
    Q_UNUSED(ref)
    _pbImportIndividual.setEnabled(true);
    _pbSetIndividualImported.setEnabled(true);
}

void DbRecordsBox::filterImportedRecrodsChecked(bool checked)
{
    QSettings settings;
    settings.setValue("window/syncTab/filterImportedRecords", checked);
    _recordsView->setImportedFilter(checked);
}
