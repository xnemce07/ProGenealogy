/*!
 *  \file   tablecellrecordrole.cpp
 *  \author Leopold Nemcek
 */
#include "tablecelldbrecordrole.h"

TableCellDbRecordRole::TableCellDbRecordRole(Identifier ref, DbRecordRole role) :
    _role(role)
{
    this->_ref = ref;
    this->setText(TextFormatter::recordRoleToStr(role));
}

DbRecordRole TableCellDbRecordRole::getRole() const
{
    return this->_role;
}
