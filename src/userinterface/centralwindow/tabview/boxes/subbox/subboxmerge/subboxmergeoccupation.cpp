/*!
 *  \file   subboxmergeoccupation.cpp
 *  \author Leopold Nemcek
 */
#include "subboxmergeoccupation.h"



SubboxMergeOccupation::SubboxMergeOccupation(Kernel &k, Identifier dbRef, Identifier localRef, QWidget *parent) :
    SubboxMerge (parent), _kernel(k), _dbIndi(dbRef), _localIndi(localRef)
{
    _mergeIndi = _kernel.record()->getMergeIndividual();
    this->fill();

    connect(_dbList, &SubboxMergeOccupationList::occupChosen, this, &SubboxMergeOccupation::dbOccupationAdd);
    connect(_lList, &SubboxMergeOccupationList::occupChosen, this, &SubboxMergeOccupation::localOccupationAdd);
}

void SubboxMergeOccupation::fill()
{
    _dbList = new SubboxMergeOccupationList(_kernel,_dbIndi);
    _lList = new SubboxMergeOccupationList(_kernel,_localIndi);
    _finalList = new BoxOccupation(_kernel, _mergeIndi);

    _dbList->setTitle("Occupation");
    _finalList->setTitle("");

    _layout.addWidget(_dbList,0,0);
    _layout.addWidget(_lList,0,1);
    _layout.addWidget(_finalList,0,2);
    _dbList->update();
    _lList->update();
    _finalList->update();

    this->setFixedHeight(200);
}

void SubboxMergeOccupation::dbOccupationAdd(unsigned int index)
{
    if(index < _kernel.record()->getIndividualOccuCount(_dbIndi)){
        Date date1 = _kernel.record()->getIndividualOccuDate1(_dbIndi, index);
        Date date2 = _kernel.record()->getIndividualOccuDate2(_dbIndi, index);
        DateType dateType = _kernel.record()->getIndividualOccuDateType(_dbIndi, index);
        QString desc = _kernel.record()->getIndividualOccuDescription(_dbIndi, index);

        unsigned int mergeIndex = _kernel.record()->setIndividualOccu(_mergeIndi);
        _kernel.record()->setIndividualOccuDate1(_mergeIndi, mergeIndex, date1);
        _kernel.record()->setIndividualOccuDate2(_mergeIndi, mergeIndex, date2);
        _kernel.record()->setIndividualOccuDateType(_mergeIndi, mergeIndex, dateType);
        _kernel.record()->setIndividualOccuDescription(_mergeIndi, mergeIndex, desc);
        _finalList->update();
    }
}

void SubboxMergeOccupation::localOccupationAdd(unsigned int index)
{
    if(index < _kernel.record()->getIndividualOccuCount(_localIndi)){
        Date date1 = _kernel.record()->getIndividualOccuDate1(_localIndi, index);
        Date date2 = _kernel.record()->getIndividualOccuDate2(_localIndi, index);
        DateType dateType = _kernel.record()->getIndividualOccuDateType(_localIndi, index);
        QString desc = _kernel.record()->getIndividualOccuDescription(_localIndi, index);

        unsigned int mergeIndex = _kernel.record()->setIndividualOccu(_mergeIndi);
        _kernel.record()->setIndividualOccuDate1(_mergeIndi, mergeIndex, date1);
        _kernel.record()->setIndividualOccuDate2(_mergeIndi, mergeIndex, date2);
        _kernel.record()->setIndividualOccuDateType(_mergeIndi, mergeIndex, dateType);
        _kernel.record()->setIndividualOccuDescription(_mergeIndi, mergeIndex, desc);
        _finalList->update();
    }
}

int SubboxMergeOccupation::maxLabelWidth() const
{
    QFont font;
    QFontMetrics fontMetrics(font);
    int width = fontMetrics.horizontalAdvance(_laTitle.text());
    return width;
}

void SubboxMergeOccupation::setLabelsWidth(int width)
{
    _laTitle.setFixedWidth(width);
}
