#include "subboxmergeeducationlist.h"


SubboxMergeEducationList::SubboxMergeEducationList(Kernel &k, Identifier &p) :
    BoxEducation (k, p)
{
    this->fill();
}

void SubboxMergeEducationList::fill()
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


void SubboxMergeEducationList::clickEdit(unsigned int index)
{
    emit eduChosen(index);
}
