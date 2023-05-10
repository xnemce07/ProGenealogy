/*!
 *  \file   tablecellbool.h
 *  \author Leopold Nemcek
 */
#ifndef TABLECELLBOOL_H
#define TABLECELLBOOL_H

#include "../../../tabindividual/listview/tablecell.h"
#include "textformatter.h"

class TableCellBool : public TableCell
{
public:
    TableCellBool() = default;
    virtual ~TableCellBool() = default;
    TableCellBool(Identifier ref, bool b);
};

#endif // TABLECELLBOOL_H
