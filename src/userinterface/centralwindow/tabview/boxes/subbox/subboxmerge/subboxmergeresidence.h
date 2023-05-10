/*!
 *  \file   subboxmergeresidence.h
 *  \author Leopold Nemcek
 */
#ifndef SUBBOXMERGERESIDENCE_H
#define SUBBOXMERGERESIDENCE_H

#include <QLabel>
#include <QSizePolicy>


#include "../subboxmerge.h"
#include "kernel.h"
#include "../../boxresidence.h"
#include "mergeeventslists/subboxmergeresidencelist.h"

class SubboxMergeResidence : public SubboxMerge
{
    Q_OBJECT

    Kernel &_kernel;

    Identifier _dbIndi;
    Identifier _localIndi;
    Identifier _mergeIndi;

    QLabel _laTitle;

    SubboxMergeResidenceList* _dbList;
    SubboxMergeResidenceList* _lList;
    BoxResidence* _finalList;
public:
    SubboxMergeResidence(Kernel &k, Identifier dbRef, Identifier localRef, QWidget* parent = nullptr);

    int maxLabelWidth() const override;
    void setLabelsWidth(int width) override;
private:
    void fill() override;
private slots:
    void dbResidenceAdd(unsigned int index);
    void localResidenceAdd(unsigned int index);
};

#endif // SUBBOXMERGERESIDENCE_H
