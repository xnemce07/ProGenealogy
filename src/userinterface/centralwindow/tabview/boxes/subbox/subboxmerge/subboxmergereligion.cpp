/*!
 *  \file   subboxmergereligion.cpp
 *  \author Leopold Nemcek
 */
#include "subboxmergereligion.h"



SubboxMergeReligion::SubboxMergeReligion(Kernel &k, Identifier dbRef, Identifier localRef, QWidget *parent):
    SubboxMerge (parent), _kernel(k), _dbIndi(dbRef), _localIndi(localRef)
{
    _mergeIndi = _kernel.record()->getMergeIndividual();
    this->fill();

    connect(_dbList, &SubboxMergeReligionList::reliChosen, this, &SubboxMergeReligion::dbReligionAdd);
    connect(_lList, &SubboxMergeReligionList::reliChosen, this, &SubboxMergeReligion::localReligionAdd);
}

void SubboxMergeReligion::fill()
{

    _dbList = new SubboxMergeReligionList(_kernel,_dbIndi);
    _lList = new SubboxMergeReligionList(_kernel,_localIndi);
    _finalList = new BoxReligion(_kernel, _mergeIndi);

    _dbList->setTitle("Religion");
    _finalList->setTitle("");

    _layout.addWidget(_dbList,0,0);
    _layout.addWidget(_lList,0,1);
    _layout.addWidget(_finalList,0,2);
    _dbList->update();
    _lList->update();
    _finalList->update();

    this->setFixedHeight(200);

}

void SubboxMergeReligion::dbReligionAdd(unsigned int index)
{
    if(index < _kernel.record()->getIndividualReliCount(_dbIndi)){
        Date date1 = _kernel.record()->getIndividualReliDate1(_dbIndi, index);
        Date date2 = _kernel.record()->getIndividualReliDate2(_dbIndi, index);
        DateType dateType = _kernel.record()->getIndividualReliDateType(_dbIndi, index);
        QString desc = _kernel.record()->getIndividualReliDescription(_dbIndi, index);

        unsigned int mergeIndex = _kernel.record()->setIndividualReli(_mergeIndi);
        _kernel.record()->setIndividualReliDate1(_mergeIndi, mergeIndex, date1);
        _kernel.record()->setIndividualReliDate2(_mergeIndi, mergeIndex, date2);
        _kernel.record()->setIndividualReliDateType(_mergeIndi, mergeIndex, dateType);
        _kernel.record()->setIndividualReliDescription(_mergeIndi, mergeIndex, desc);
        _finalList->update();
    }
}

void SubboxMergeReligion::localReligionAdd(unsigned int index)
{
    if(index < _kernel.record()->getIndividualReliCount(_localIndi)){
        Date date1 = _kernel.record()->getIndividualReliDate1(_localIndi, index);
        Date date2 = _kernel.record()->getIndividualReliDate2(_localIndi, index);
        DateType dateType = _kernel.record()->getIndividualReliDateType(_localIndi, index);
        QString desc = _kernel.record()->getIndividualReliDescription(_localIndi, index);

        unsigned int mergeIndex = _kernel.record()->setIndividualReli(_mergeIndi);
        _kernel.record()->setIndividualReliDate1(_mergeIndi, mergeIndex, date1);
        _kernel.record()->setIndividualReliDate2(_mergeIndi, mergeIndex, date2);
        _kernel.record()->setIndividualReliDateType(_mergeIndi, mergeIndex, dateType);
        _kernel.record()->setIndividualReliDescription(_mergeIndi, mergeIndex, desc);
        _finalList->update();
    }
}


int SubboxMergeReligion::maxLabelWidth() const
{
    QFont font;
    QFontMetrics fontMetrics(font);
    int width = fontMetrics.horizontalAdvance(_laTitle.text());
    return width;
}

void SubboxMergeReligion::setLabelsWidth(int width)
{
    _laTitle.setFixedWidth(width);
}
