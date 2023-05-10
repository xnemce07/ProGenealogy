/*!
 *  \file   mergebox.h
 *  \author Leopold Nemcek
 */
#ifndef MERGEBOX_H
#define MERGEBOX_H

#include <QScrollArea>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>

#include "../boxes/box.h"
#include "../boxes/subbox/subboxmerge.h"
#include "../boxes/subbox/subboxmerge/subboxmergestring.h"
#include "../boxes/subbox/subboxmerge/subboxmergedate.h"
#include "../boxes/subbox/subboxmerge/subboxmergegender.h"
#include "../boxes/subbox/subboxmerge/subboxmergereligion.h"
#include "../boxes/subbox/subboxmerge/subboxmergeoccupation.h"
#include "../boxes/subbox/subboxmerge/subboxmergeeducation.h"
#include "../boxes/subbox/subboxmerge/subboxmergeresidence.h"



class MergeBox : public Box
{
    Q_OBJECT
private:
    Identifier _dbIndi;
    Identifier _dbrecord;

    Identifier _localIndi;
    Identifier _partner;

    Identifier _mergeInd;

    QVector<SubboxMerge *> _items;

    QScrollArea _scrollArea;

    QVBoxLayout _vLayout;

    QWidget* _scrollAreaWidget;

    QSpacerItem* _spacer;

    QLabel _laEmpty;

    QPushButton _pbMerge;
    QPushButton _pbReset;

public:
    MergeBox(QString title, Kernel &k, Identifier &p, QWidget *parent = nullptr);
    void update() override;
private:
    void fill() override;
    void remove();

    void merge();

signals:
    void individualMerged(Identifier ref);
public slots:
    void dbIndividualChanged(Identifier indi, Identifier record);
    void localIndividualChanged(Identifier indi, Identifier partner);

private slots:
    void resetClicked(bool checked);
    void mergeClicked(bool checked);

};

#endif // MERGEBOX_H
