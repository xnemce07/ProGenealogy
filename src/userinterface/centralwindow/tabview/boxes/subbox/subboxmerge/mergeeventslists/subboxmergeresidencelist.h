#ifndef SUBBOXMERGERESIDENCELIST_H
#define SUBBOXMERGERESIDENCELIST_H

#include "../../../boxresidence.h"

class SubboxMergeResidenceList : public BoxResidence
{
    Q_OBJECT
public:
    SubboxMergeResidenceList(Kernel &k, Identifier &p);
signals:
    void residenceChosen(unsigned int index);
protected slots:
    void fill() override;
    void clickEdit(unsigned int index) override;
};

#endif // SUBBOXMERGERESIDENCELIST_H
