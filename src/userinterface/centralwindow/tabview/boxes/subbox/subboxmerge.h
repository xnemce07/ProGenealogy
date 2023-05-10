
#ifndef SUBBOXMERGE_H
#define SUBBOXMERGE_H

#include "subbox.h"



class SubboxMerge : public Subbox
{
protected:
    QString _styleSheetNoMatch = "background: #FFBABA; color: #D8000C; font-weight: bold";
    QString _styleSheetMatch =  "background: #DFF2BF; color: #00529B; font-weight: bold";
public:
    SubboxMerge(QWidget *parent = nullptr);
};

#endif // SUBBOXMERGE_H
