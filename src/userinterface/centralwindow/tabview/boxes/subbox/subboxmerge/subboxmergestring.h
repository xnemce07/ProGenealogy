/*!
 *  \file   subboxmergestring.h
 *  \author Leopold Nemcek
 */
#ifndef SUBBOXMERGESTRING_H
#define SUBBOXMERGESTRING_H

#include <QPushButton>
#include <QLineEdit>
#include "../subboxmerge.h"
#include "kernel.h"

class SubboxMergeString : public SubboxMerge
{
    Q_OBJECT
private:

    Kernel &_kernel;
    Identifier _mergeInd;

    QString _title;

    QString _dbVal;
    QString _localVal;    

    QLabel _laTitle;

    QPushButton _pbDbVal;
    QPushButton _pbLocVal;

    QLineEdit _leResult;


    void (*_writeF)(Kernel&, Identifier, QString);


public:
    SubboxMergeString(Kernel &k, Identifier mergeIndi, QString title ,QString dbValue, QString localValue,void(*writeF)(Kernel&, Identifier, QString), QWidget* parent = nullptr);


    int maxLabelWidth() const override;
    void setLabelsWidth(int width) override;
private:
    void fill() override;

    void updateButtons();
private slots:
    void dbButtonPressed(bool checked);
    void localButtonPressed(bool checked);
    void lineEditChanged(QString text);
};

#endif // SUBBOXMERGESTRING_H
