/*!
 *  \file   tablecellrecordtype.h
 *  \author Leopold Nemcek
 */
#ifndef TABLECELLDBRECORDTYPE_H
#define TABLECELLDBRECORDTYPE_H

#include "../../../tabindividual/listview/tablecell.h"
#include "kernel/record/recordRAM/dbrecord/dbrecordtype.h"
#include "textformatter.h"

#include <QApplication>

class TableCellDbRecordType : public TableCell
{
public:
    TableCellDbRecordType() = default;
    virtual ~TableCellDbRecordType() = default;
    TableCellDbRecordType(Identifier ref, DbRecordType type);
};

#endif // TABLECELLDBRECORDTYPE_H
