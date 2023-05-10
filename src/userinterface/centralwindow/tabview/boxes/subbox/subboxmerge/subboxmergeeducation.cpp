/*!
 *  \file   subboxmergeeducation.cpp
 *  \author Leopold Nemcek */
#include "subboxmergeeducation.h"

SubboxMergeEducation::SubboxMergeEducation(Kernel &k, Identifier dbRef, Identifier localRef, QWidget *parent) :
    SubboxMerge (parent), _kernel(k), _dbIndi(dbRef), _localIndi(localRef)
{
    _mergeIndi = _kernel.record()->getMergeIndividual();
    this->fill();

    connect(_dbList, &SubboxMergeEducationList::eduChosen, this, &SubboxMergeEducation::dbEduAdd);
    connect(_lList, &SubboxMergeEducationList::eduChosen, this, &SubboxMergeEducation::localEduAdd);
}

void SubboxMergeEducation::fill()
{
    _dbList = new SubboxMergeEducationList(_kernel,_dbIndi);
    _lList = new SubboxMergeEducationList(_kernel,_localIndi);
    _finalList = new BoxEducation(_kernel, _mergeIndi);

    _dbList->setTitle("Education");
    _finalList->setTitle("");

    _layout.addWidget(_dbList,0,0);
    _layout.addWidget(_lList,0,1);
    _layout.addWidget(_finalList,0,2);
    _dbList->update();
    _lList->update();
    _finalList->update();

    this->setFixedHeight(200);
}

void SubboxMergeEducation::dbEduAdd(unsigned int index)
{
    if(index < _kernel.record()->getIndividualEducCount(_dbIndi)){
        Date date1 = _kernel.record()->getIndividualEducDate1(_dbIndi, index);
        Date date2 = _kernel.record()->getIndividualEducDate2(_dbIndi, index);
        DateType dateType = _kernel.record()->getIndividualEducDateType(_dbIndi, index);
        QString desc = _kernel.record()->getIndividualEducDescription(_dbIndi, index);

        unsigned int mergeIndex = _kernel.record()->setIndividualEduc(_mergeIndi);
        _kernel.record()->setIndividualEducDate1(_mergeIndi, mergeIndex, date1);
        _kernel.record()->setIndividualEducDate2(_mergeIndi, mergeIndex, date2);
        _kernel.record()->setIndividualEducDateType(_mergeIndi, mergeIndex, dateType);
        _kernel.record()->setIndividualEducDescription(_mergeIndi, mergeIndex, desc);
        _finalList->update();
    }
}

void SubboxMergeEducation::localEduAdd(unsigned int index)
{
    if(index < _kernel.record()->getIndividualEducCount(_localIndi)){
        Date date1 = _kernel.record()->getIndividualEducDate1(_localIndi, index);
        Date date2 = _kernel.record()->getIndividualEducDate2(_localIndi, index);
        DateType dateType = _kernel.record()->getIndividualEducDateType(_localIndi, index);
        QString desc = _kernel.record()->getIndividualEducDescription(_localIndi, index);

        unsigned int mergeIndex = _kernel.record()->setIndividualEduc(_mergeIndi);
        _kernel.record()->setIndividualEducDate1(_mergeIndi, mergeIndex, date1);
        _kernel.record()->setIndividualEducDate2(_mergeIndi, mergeIndex, date2);
        _kernel.record()->setIndividualEducDateType(_mergeIndi, mergeIndex, dateType);
        _kernel.record()->setIndividualEducDescription(_mergeIndi, mergeIndex, desc);
        _finalList->update();
    }
}


int SubboxMergeEducation::maxLabelWidth() const
{
    QFont font;
    QFontMetrics fontMetrics(font);
    int width = fontMetrics.horizontalAdvance(_laTitle.text());
    return width;
}

void SubboxMergeEducation::setLabelsWidth(int width)
{
    _laTitle.setFixedWidth(width);
}
