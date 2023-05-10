#ifndef SUBBOXMERGEEDUCATIONLIST_H
#define SUBBOXMERGEEDUCATIONLIST_H

#include "../../../boxeducation.h"

class SubboxMergeEducationList : public BoxEducation
{
    Q_OBJECT
public:
    SubboxMergeEducationList(Kernel &k, Identifier &p);
signals:
    void eduChosen(unsigned int index);
protected slots:
    void fill() override;
    void clickEdit(unsigned int index) override;
};

#endif // SUBBOXMERGEEDUCATIONLIST_H
