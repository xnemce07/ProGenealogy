/*!
 *  \file   subboxmergegender.h
 *  \author Leopold Nemcek
 */
#ifndef SUBBOXMERGEGENDER_H
#define SUBBOXMERGEGENDER_H

#include <QPushButton>
#include <QComboBox>

#include "../subboxmerge.h"
#include "kernel.h"

class SubboxMergeGender : public SubboxMerge
{
    Q_OBJECT
private:
    Kernel &_kernel;
    Identifier _mergeInd;

    QString _title;

    Gender _dbVal;
    Gender _localVal;

    QLabel _laTitle;

    QPushButton _pbDbVal;
    QPushButton _pbLocVal;

    QComboBox _comboGender;

public:
    SubboxMergeGender(Kernel &k, QString title, Gender dbValue, Gender localValue, QWidget *parent=nullptr);

    int maxLabelWidth() const override;
    void setLabelsWidth(int width) override;
    void updateButtons();
private:
    void fill() override;

    void setGenderIndex(Gender g);
    QString gend2Str(Gender gender);

private slots:
    void dbButtonPressed(bool checked);
    void localButtonPressed(bool checked);
    void genderChanged(QString text);
};

#endif // SUBBOXMERGEGENDER_H
