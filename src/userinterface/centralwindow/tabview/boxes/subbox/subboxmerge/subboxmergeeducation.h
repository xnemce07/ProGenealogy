/*!
 *  \file   subboxmergeeducation.h
 *  \author Leopold Nemcek
 */
#ifndef SUBBOXMERGEEDUCATION_H
#define SUBBOXMERGEEDUCATION_H

#include <QLabel>

#include "../subboxmerge.h"
#include "kernel.h"
#include "../../boxeducation.h"
#include "mergeeventslists/subboxmergeeducationlist.h"

class SubboxMergeEducation : public SubboxMerge
{
    Q_OBJECT
private:
    Kernel &_kernel;

    Identifier _dbIndi;
    Identifier _localIndi;
    Identifier _mergeIndi;

    QLabel _laTitle;

    SubboxMergeEducationList* _dbList;
    SubboxMergeEducationList* _lList;
    BoxEducation* _finalList;

public:
    SubboxMergeEducation(Kernel &k, Identifier dbRef, Identifier localRef, QWidget* parent = nullptr);

    int maxLabelWidth() const override;
    void setLabelsWidth(int width) override;
private:
    void fill() override;
private slots:
    void dbEduAdd(unsigned int index);
    void localEduAdd(unsigned int index);
};

#endif // SUBBOXMERGEEDUCATION_H
