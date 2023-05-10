/*!
 *  \file   tablecellrecordrole.h
 *  \author Leopold Nemcek
 */
#ifndef TABLECELLDBRECORDROLE_H
#define TABLECELLDBRECORDROLE_H

#include "../../../tabindividual/listview/tablecell.h"
#include "kernel/record/recordRAM/dbrecord/dbrecordrole.h"
#include "textformatter.h"

class TableCellDbRecordRole : public TableCell
{
private:
    DbRecordRole _role;
public:
    TableCellDbRecordRole() = default;
    virtual ~TableCellDbRecordRole() = default;
    TableCellDbRecordRole(Identifier ref, DbRecordRole role);
    
    DbRecordRole getRole() const;
};

#endif // TABLECELLDBRECORDROLE_H
