/*!
 *  \file   tablecellrecordtype.cpp
 *  \author Leopold Nemcek
 */
#include "tablecelldbrecordtype.h"



TableCellDbRecordType::TableCellDbRecordType(Identifier ref, DbRecordType type)
{
    this->_ref = ref;
    this->setText(TextFormatter::recordTypeToStr(type));
}
