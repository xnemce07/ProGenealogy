/*!
 *  \file   tabsync.cpp
 *  \author Leopold Nemcek
 */
#include "tabsync.h"

TabSync::TabSync(Kernel &k, Identifier &p, QWidget *parent) : QWidget(parent), _kernel(k), _proband(p)
{
    this->setLayout(&_layout);
    this->fill();

    connect(_dbRecordsBox, &DbRecordsBox::selectedDbRecord, _localRecordsBox, &LocalRecordsBox::selectedDbRecord);
    connect(_dbRecordsBox, &DbRecordsBox::selectedDbIndividual, _localRecordsBox, &LocalRecordsBox::selectedDbIndividual);

    connect(_dbRecordsBox, &DbRecordsBox::selectedDbIndividual, _mergeBox, &MergeBox::dbIndividualChanged);

    connect(_localRecordsBox, &LocalRecordsBox::selectedLocalIndividual, _mergeBox, &MergeBox::localIndividualChanged);

    connect(_mergeBox, &MergeBox::individualMerged, this, &TabSync::dataChanged);

    connect(_dbRecordsBox, &DbRecordsBox::dataChanged, this, &TabSync::update);
    connect(_dbRecordsBox, &DbRecordsBox::individualImported, this, &TabSync::dataChanged);

    connect(_localRecordsBox, &LocalRecordsBox::individualImported, this, &TabSync::dataChanged);
}

TabSync::~TabSync()
{
    _dbRecordsBox->deleteLater();
    _mergeBox->deleteLater();
    _localRecordsBox->deleteLater();
}

void TabSync::update()
{
    _localRecordsBox->update();
    _mergeBox->update();
    _dbRecordsBox->update();
}

void TabSync::updateSync()
{
    _dbRecordsBox->updateSync();
}

void TabSync::fill(){


    //Init boxes
    _dbRecordsBox = new DbRecordsBox(tr("Database records"), _kernel, _proband, this);
    _mergeBox = new MergeBox(tr("Merge records"), _kernel, _proband, this);
    _localRecordsBox = new LocalRecordsBox(tr("Local records"), _kernel, _proband, this);

    // Add widgets
    _layout.addWidget(_dbRecordsBox, 0, 0, 1, 1);
    _layout.addWidget(_mergeBox,0,1,1,1);
    _layout.addWidget(_localRecordsBox,0,2,1,1);

    // Adjust sizes
    _layout.setColumnStretch(0,3);
    _layout.setColumnStretch(1,5);
    _layout.setColumnStretch(2,3);

    _layout.setHorizontalSpacing(this->tightSpacing());
    _layout.setContentsMargins(this->mediumSpacing(),this->mediumSpacing(),this->mediumSpacing(),0);
}

void TabSync::defaultSettings()
{
    DbRecordsBox::defaultSettings();
    LocalRecordsBox::defaultSettings();
}

void TabSync::saveSettings() const
{
    _dbRecordsBox->saveSettings();
    _localRecordsBox->saveSettings();
}
