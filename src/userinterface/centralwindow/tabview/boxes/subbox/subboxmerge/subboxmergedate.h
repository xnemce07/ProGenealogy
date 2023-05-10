/*!
 *  \file   subboxmergedate.h
 *  \author Leopold Nemcek
 */
#ifndef SUBBOXMERGEDATE_H
#define SUBBOXMERGEDATE_H

#include <QPushButton>
#include <QLineEdit>
#include "../subbox.h"
#include "../subboxdate.h"
#include "../subboxmerge.h"
#include "kernel.h"

class SubboxMergeDate : public SubboxMerge
{
private:
    Kernel &_kernel;
    Identifier _ref;

    QString _title;

    Date _dbDate1;
    Date _dbDate2;
    DateType _dbDateType;

    Date _lDate1;
    Date _lDate2;
    DateType _lDateType;

    Date _finalDate1;
    Date _finalDate2;
    DateType _finalDateType;

    QLabel _laTitle;

    QPushButton _pbDbVal;
    QPushButton _pbLocVal;

    SubboxDate *_subDate;


    void (*_writeF)(Kernel&, Identifier, Date, Date, DateType);

public:
    SubboxMergeDate(Kernel &k, Identifier ref, QString title, void (*writeF)(Kernel&, Identifier, Date, Date, DateType), QWidget* parent = nullptr);
    void setDbDate(Date date1, Date date2, DateType type);
    void setLocalDate(Date date1, Date date2, DateType type);
    void finish();


    int maxLabelWidth() const override;
    void setLabelsWidth(int width) override;

    Identifier mergeInd() const;
    void setMergeInd(const Identifier &mergeInd);

private:
    void fill() override;
    void dateChanged();

    void updateButtons();

private slots:
    void dbButtonPressed(bool checked);
    void localButtonPressed(bool checked);
    void date1Changed(Date date);
    void date2Changed(Date date);
    void dateTypeChanged(DateType type);
};

#endif // SUBBOXMERGEDATE_H
