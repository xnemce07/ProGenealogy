/*!
 *  \file   subboxmergeresidence.cpp
 *  \author Leopold Nemcek
 */
#include "subboxmergeresidence.h"

SubboxMergeResidence::SubboxMergeResidence(Kernel &k, Identifier dbRef, Identifier localRef, QWidget *parent) :
    SubboxMerge (parent), _kernel(k), _dbIndi(dbRef), _localIndi(localRef)
{
    _mergeIndi = _kernel.record()->getMergeIndividual();
    this->fill();

    connect(_dbList, &SubboxMergeResidenceList::residenceChosen, this, &SubboxMergeResidence::dbResidenceAdd);
    connect(_lList, &SubboxMergeResidenceList::residenceChosen, this, &SubboxMergeResidence::localResidenceAdd);
}

void SubboxMergeResidence::fill()
{

    _dbList = new SubboxMergeResidenceList(_kernel,_dbIndi);
    _lList = new SubboxMergeResidenceList(_kernel,_localIndi);
    _finalList = new BoxResidence(_kernel, _mergeIndi);

    _dbList->setTitle("Residence");
    _finalList->setTitle("");

    _layout.addWidget(_dbList,0,0);
    _layout.addWidget(_lList,0,1);
    _layout.addWidget(_finalList,0,2);
    _dbList->update();
    _lList->update();
    _finalList->update();
    this->setFixedHeight(200);
}

void SubboxMergeResidence::dbResidenceAdd(unsigned int index)
{
    if(index < _kernel.record()->getIndividualResiCount(_dbIndi)){
        Date date1 = _kernel.record()->getIndividualResiDate1(_dbIndi, index);
        Date date2 = _kernel.record()->getIndividualResiDate2(_dbIndi, index);
        DateType dateType = _kernel.record()->getIndividualResiDateType(_dbIndi, index);
        QString addr = _kernel.record()->getIndividualResiAddrAddr(_dbIndi, index);
        QString city = _kernel.record()->getIndividualResiAddrCity(_dbIndi, index);

        unsigned int mergeIndex = _kernel.record()->setIndividualResi(_mergeIndi);
        _kernel.record()->setIndividualResiDate1(_mergeIndi, mergeIndex, date1);
        _kernel.record()->setIndividualResiDate2(_mergeIndi, mergeIndex, date2);
        _kernel.record()->setIndividualResiDateType(_mergeIndi, mergeIndex, dateType);
        _kernel.record()->setIndividualResiAddrAddr(_mergeIndi, mergeIndex, addr);
        _kernel.record()->setIndividualResiAddrCity(_mergeIndi, mergeIndex, city);

        _finalList->update();
    }
}

void SubboxMergeResidence::localResidenceAdd(unsigned int index)
{
    if(index < _kernel.record()->getIndividualResiCount(_localIndi)){
        Date date1 = _kernel.record()->getIndividualResiDate1(_localIndi, index);
        Date date2 = _kernel.record()->getIndividualResiDate2(_localIndi, index);
        DateType dateType = _kernel.record()->getIndividualResiDateType(_localIndi, index);
        QString addr = _kernel.record()->getIndividualResiAddrAddr(_localIndi, index);
        QString city = _kernel.record()->getIndividualResiAddrCity(_localIndi, index);

        unsigned int mergeIndex = _kernel.record()->setIndividualResi(_mergeIndi);
        _kernel.record()->setIndividualResiDate1(_mergeIndi, mergeIndex, date1);
        _kernel.record()->setIndividualResiDate2(_mergeIndi, mergeIndex, date2);
        _kernel.record()->setIndividualResiDateType(_mergeIndi, mergeIndex, dateType);
        _kernel.record()->setIndividualResiAddrAddr(_mergeIndi, mergeIndex, addr);
        _kernel.record()->setIndividualResiAddrCity(_mergeIndi, mergeIndex, city);

        _finalList->update();
    }
}


int SubboxMergeResidence::maxLabelWidth() const
{
    QFont font;
    QFontMetrics fontMetrics(font);
    int width = fontMetrics.horizontalAdvance(_laTitle.text());
    return width;
}

void SubboxMergeResidence::setLabelsWidth(int width)
{
    _laTitle.setFixedWidth(width);
}
