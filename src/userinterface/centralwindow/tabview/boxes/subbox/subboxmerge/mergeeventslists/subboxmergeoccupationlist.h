#ifndef SUBBOXMERGEOCCUPATIONLIST_H
#define SUBBOXMERGEOCCUPATIONLIST_H

#include "../../../boxoccupation.h"

class SubboxMergeOccupationList : public BoxOccupation
{
    Q_OBJECT
public:
    SubboxMergeOccupationList(Kernel &k, Identifier &p);
signals:
    void occupChosen(unsigned int index);
protected slots:
    void fill() override;
    void clickEdit(unsigned int index) override;
};

#endif // SUBBOXMERGEOCCUPATIONLIST_H
