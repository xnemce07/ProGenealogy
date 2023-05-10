/*!
 *  \file   subboxmergeoccupation.h
 *  \author Leopold Nemcek
 */
#ifndef SUBBOXMERGEOCCUPATION_H
#define SUBBOXMERGEOCCUPATION_H

#include <QLabel>

#include "../subboxmerge.h"
#include "kernel.h"
#include "../../boxoccupation.h"
#include "mergeeventslists/subboxmergeoccupationlist.h"

class SubboxMergeOccupation : public SubboxMerge
{
    Q_OBJECT
private:
    Kernel &_kernel;

    Identifier _dbIndi;
    Identifier _localIndi;
    Identifier _mergeIndi;

    QLabel _laTitle;

    SubboxMergeOccupationList* _dbList;
    SubboxMergeOccupationList* _lList;
    BoxOccupation* _finalList;
public:
    SubboxMergeOccupation(Kernel &k, Identifier dbRef, Identifier localRef, QWidget* parent = nullptr);

    int maxLabelWidth() const override;
    void setLabelsWidth(int width) override;
private:
    void fill() override;
private slots:
    void dbOccupationAdd(unsigned int index);
    void localOccupationAdd(unsigned int index);
};

#endif // SUBBOXMERGEOCCUPATION_H
