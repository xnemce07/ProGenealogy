/*!
 *  \file   databaserow.cpp
 *  \author Leopold Nemcek
 */
#include "databaserow.h"

DatabaseRow::DatabaseRow(QVariantMap row) :
    _row(row)
{

}

QVariant DatabaseRow::value(QString name) const
{
    if(!_row.contains(name)){
        return QVariant();
    }
    return _row.constFind(name).value();
}
