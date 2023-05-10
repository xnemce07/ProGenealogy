/*!
 *  \file   dbrecordindividual.cpp
 *  \author Leopold Nemcek
 */
#include "dbrecordindividual.h"

DbRecordIndividual::DbRecordIndividual(DbRecordRole role, Identifier identifier, unsigned int primaryKey): _role(role), _identifier(identifier), _primaryKey(primaryKey)
{

}

DbRecordRole DbRecordIndividual::getRole() const{
    return _role;
}

void DbRecordIndividual::setRole(DbRecordRole role){
    _role = role;
}

Identifier DbRecordIndividual::getIdentifier() const {
    return _identifier;
}

unsigned int DbRecordIndividual::getPrimaryKey() const
{
    return this->_primaryKey;
}

void DbRecordIndividual::setImported(bool imported){
    _imported = imported;
}

bool DbRecordIndividual::getImported() const{
    return _imported;
}
