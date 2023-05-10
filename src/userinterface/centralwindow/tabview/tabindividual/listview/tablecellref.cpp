/*!
 *  \file   tablecellref.cpp
 *  \author Eva Navratilova
 */

#include "tablecellref.h"

TableCellRef::TableCellRef(Identifier ref)
{
    this->_ref = ref;
    if(_ref.isValid()){
        this->setText(QString::number(ref.getNumber()));
    } else {
        this->setText("N/A");
    }

    /*this->_item.setText(QString::number(ref.getNumber()));
    this->_name.setText(name);
    this->_surname.setText(surname);
    this->_birth.setText(birth.toString(Qt::ISODate));*/
}

