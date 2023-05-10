#ifndef SUBBOXMERGERELIGIONLIST_H
#define SUBBOXMERGERELIGIONLIST_H

#include "../../../boxreligion.h"

class SubboxMergeReligionList : public BoxReligion
{
    Q_OBJECT
public:
    SubboxMergeReligionList(Kernel &k, Identifier &p);
signals:
    void reliChosen(unsigned int index);
protected slots:
    void fill() override;
    void clickEdit(unsigned int index) override;


};

#endif // SUBBOXMERGERELIGIONLIST_H
