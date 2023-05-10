/*!
 *  \file   subboxmergereligion.h
 *  \author Leopold Nemcek
 */
#ifndef SUBBOXMERGERELIGION_H
#define SUBBOXMERGERELIGION_H

#include <QLabel>

#include "../subboxmerge.h"
#include "kernel.h"
#include "../../boxreligion.h"
#include "mergeeventslists/subboxmergereligionlist.h"

class SubboxMergeReligion : public SubboxMerge
{
    Q_OBJECT
private:
    Kernel &_kernel;

    Identifier _dbIndi;
    Identifier _localIndi;
    Identifier _mergeIndi;

    QLabel _laTitle;

    SubboxMergeReligionList* _dbList;
    SubboxMergeReligionList* _lList;
    BoxReligion* _finalList;


public:
    SubboxMergeReligion(Kernel &k, Identifier dbRef, Identifier localRef, QWidget* parent = nullptr);

    int maxLabelWidth() const override;
    void setLabelsWidth(int width) override;
private:
    void fill() override;
private slots:
    void dbReligionAdd(unsigned int index);
    void localReligionAdd(unsigned int index);
};

#endif // SUBBOXMERGERELIGION_H
