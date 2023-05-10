/*!
 *  \file   tablecellbool.cpp
 *  \author Leopold Nemcek
 */
#include "tablecellbool.h"




TableCellBool::TableCellBool(Identifier ref, bool b)
{
    this->_ref = ref;
    this->setText(TextFormatter::boolToStr(b));
}
