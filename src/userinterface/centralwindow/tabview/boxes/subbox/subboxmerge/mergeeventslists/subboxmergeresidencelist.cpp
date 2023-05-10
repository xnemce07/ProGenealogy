#include "subboxmergeresidencelist.h"


SubboxMergeResidenceList::SubboxMergeResidenceList(Kernel &k, Identifier &p) :
    BoxResidence (k, p)
{
    this->fill();
}


void SubboxMergeResidenceList::fill()
{
    this->setTitle("");

    _layout.removeWidget(&_buNew);
    _buNew.setParent(nullptr);
    _layout.removeWidget(&_buDelete);
    _buDelete.setParent(nullptr);
    _layout.removeWidget(&_buUp);
    _buUp.setParent(nullptr);
    _layout.removeWidget(&_buDown);
    _buDown.setParent(nullptr);

    _layout.removeWidget(&_buEdit);
    _layout.addWidget(&_buEdit, 1, 5);
    _buEdit.setIcon(QIcon(":icons/add"));
    _buEdit.setToolTip(tr("Add"));
}


void SubboxMergeResidenceList::clickEdit(unsigned int index)
{
    emit residenceChosen(index);
}
