/*!
 *  \file   dbrecord.cpp
 *  \author Leopold Nemcek
 */
#include "dbrecord.h"


void DbRecord::addIndividual(Identifier individual, DbRecordRole role, unsigned int primaryKey){
    DbRecordIndividual* indi = new DbRecordIndividual(role,individual, primaryKey);
    _individuals.append(indi);
}

DbRecordIndividual* DbRecord::getIndividual(Identifier identifier){
    for (DbRecordIndividual* indi : _individuals)
    {
        if(indi->getIdentifier() == identifier){
            return indi;
        }
    }

    return nullptr;
}

DbRecordIndividual* DbRecord::constGetIndividual(Identifier identifier) const{
    for (DbRecordIndividual* indi : _individuals)
    {
        if(indi->getIdentifier() == identifier){
            return indi;
        }
    }

    return nullptr;
}

DbRecord::DbRecord(Identifier ref, Identifier sourceRef, DbRecordType recordType, unsigned int pk)
    : _recordType(recordType), _primaryKey(pk), _sourceRef(sourceRef), _ref(ref)
{
    _marriage = new CommonEventStructure(CommonEventType::MARR);
    _divorce = new CommonEventStructure(CommonEventType::DIV);
}

Identifier DbRecord::getSourceRef() const
{
    return _sourceRef;
}

void DbRecord::removeIndividual(Identifier individual)
{
    DbRecordIndividual *indi = getIndividual(individual);

    _individuals.removeOne(indi);
}

void DbRecord::setIndividualRole(Identifier individual, DbRecordRole role)
{
    DbRecordIndividual *indi = getIndividual(individual);
    indi->setRole(role);
}

void DbRecord::setIndividualImported(Identifier individual, bool imported)
{
    DbRecordIndividual *indi = getIndividual(individual);
    indi->setImported(imported);
    bool allImported = true;
    for(auto indi = _individuals.begin(); indi != _individuals.end(); ++indi)
    {
        if(!(*indi)->getImported()){
            allImported = false;
            break;
        }
    }
    if(allImported){
        this->_imported = true;
    }
}

DbRecordRole DbRecord::getIndividualRole(Identifier individual) const
{
    DbRecordIndividual *indi = constGetIndividual(individual);
    return indi?indi->getRole():DbRecordRole::UNDEFINED;
}

bool DbRecord::getIndividualImported(Identifier individual) const
{
    DbRecordIndividual *indi = constGetIndividual(individual);
    return indi?indi->getImported():false;
}

QVector<Identifier> DbRecord::getIndividuals() const
{
    QVector<Identifier> identifiers;
    for (DbRecordIndividual* indi : _individuals){
        identifiers.push_back(indi->getIdentifier());
    }
    return identifiers;
}

Identifier DbRecord::getRef() const
{
    return this->_ref;
}

DbRecordType DbRecord::getType() const
{
    return this->_recordType;
}

bool DbRecord::containsIndividual(Identifier individual) const
{
    return getIndividuals().contains(individual);
}

unsigned int DbRecord::getIndividualPK(Identifier ref) const
{
    return constGetIndividual(ref)->getPrimaryKey();
}

Identifier DbRecord::getIndividualByRole(DbRecordRole role) const
{
    for(DbRecordIndividual* indi : _individuals){
        if(indi->getRole() == role){
            return indi->getIdentifier();
        }
    }

    return Identifier();
}

bool DbRecord::getImported() const
{
    return this->_imported;
}

unsigned int DbRecord::getPK() const
{
    return _primaryKey;
}

Date DbRecord::getMarriageDate1() const
{
    //    return _marriageDate1;
    return _marriage->detail().getDate1();
}

Date DbRecord::getMarriageDate2() const
{
    return _marriage->detail().getDate2();
}

DateType DbRecord::getMarriageDateType() const
{
    return _marriage->detail().getDateType();
}

Date DbRecord::getDivorceDate1() const
{
    return _divorce->detail().getDate1();
}

Date DbRecord::getDivorceDate2() const
{
    return _divorce->detail().getDate2();
}

DateType DbRecord::getDivorceDateType() const
{
    return _divorce->detail().getDateType();
}

QString DbRecord::getMarriagePlace() const
{
    return _marriage->detail().constPlace().getName();
}

void DbRecord::setMarriageDate1(Date date)
{
    _marriage->detail().setDate1(date);
}

void DbRecord::setMarriageDate2(Date date)
{
    _marriage->detail().setDate2(date);
}

void DbRecord::setMarriageDateType(DateType type)
{
    _marriage->detail().setDateType(type);
}

void DbRecord::setDivorceDate1(Date date)
{
    _divorce->detail().setDate1(date);
}

void DbRecord::setDivorceDate2(Date date)
{
    _divorce->detail().setDate2(date);
}

void DbRecord::setDivorceDateType(DateType type)
{
    _divorce->detail().setDateType(type);
}

void DbRecord::setMarriagePlace(QString place)
{
    _marriage->detail().place().setName(place);
}

void DbRecord::setImported(bool imported)
{
    this->_imported = imported;

//    for(DbRecordIndividual* indi : _individuals){
//        indi->setImported(imported);
//    }
}
