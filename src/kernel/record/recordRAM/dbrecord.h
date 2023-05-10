/*!
 *  \file   dbrecord.h
 *  \author Leopold Nemcek
 */

#ifndef DBRECORD_H
#define DBRECORD_H

#include <QVector>

#include "identifier.h"
#include "date.h"
#include "eventdetail.h"
#include "dbrecord/dbrecordtype.h"
#include "dbrecord/dbrecordindividual.h"
#include "dbrecord/dbrecordrole.h"
#include "event/commoneventstructure.h"
#include "sourcerecord.h"

/**
 * @brief The DbRecord class Structure representing record downloaded from database
 */
class DbRecord
{
    friend class RecordRAM;
private:
    QVector<DbRecordIndividual*> _individuals;
    DbRecordType _recordType;
    bool _imported = false;

    unsigned int _primaryKey;

    CommonEventStructure* _marriage; /**< Structure saving details about marriage from record (can be marriage of bride and groom, or parents of born person) */
    CommonEventStructure* _divorce; /**< Structure saving details about divorce from record */

    Identifier _sourceRef; /**< Identifier of sourceRecord saving source of record */

    Identifier _ref;
protected:
    DbRecord(Identifier ref, Identifier sourceRef, DbRecordType recordType, unsigned int pk);

public:

    Identifier getSourceRef() const;

    /**
     * @brief addIndividual Aadds existing database individual to database record
     * @param individual Individual's identifier
     * @param role Individual's role in this record
     * @param primaryKey Individual's primary key from database
     */
    void addIndividual(Identifier individual, enum DbRecordRole role, unsigned int primaryKey);

    /**
     * @brief removeIndividual Removes database individual from this record
     * @param individual Individual's identifier
     */
    void removeIndividual(Identifier individual);
    void setIndividualRole(Identifier individual, DbRecordRole role);

    /**
     * @brief setIndividualImported Mark individual as imported
     * @param individual Individual's identifier
     * @param imported True = set as imported, False, set as not imported
     */
    void setIndividualImported(Identifier individual, bool imported);

    /**
     * @brief setImported Set database record as imported
     * @param imported  True = set as imported, False, set as not imported
     */
    void setImported(bool imported);


    /**
     * @brief getIndividuals Return list of identifiers from database individuals in record
     * @return Vector of identifiers
     */
    QVector<Identifier> getIndividuals() const;


    bool containsIndividual(Identifier individual) const;
    
    DbRecordRole getIndividualRole(Identifier individual) const;
    bool getIndividualImported(Identifier individual) const;
    unsigned int getIndividualPK(Identifier ref) const;
    Identifier getIndividualByRole(DbRecordRole role) const;
    Identifier getRef() const;
    DbRecordType getType() const;
    bool getImported() const;
    unsigned int getPK() const;

    /// Marriage and divorce events

    // Getters

    Date getMarriageDate1() const;
    Date getMarriageDate2() const;
    DateType getMarriageDateType() const;
    Date getDivorceDate1() const;
    Date getDivorceDate2() const;
    DateType getDivorceDateType() const;

    QString getMarriagePlace() const;

    //Setters

    void setMarriageDate1(Date date);
    void setMarriageDate2(Date date);
    void setMarriageDateType(DateType type);
    void setDivorceDate1(Date date);
    void setDivorceDate2(Date date);
    void setDivorceDateType(DateType type);

    void setMarriagePlace(QString place);

private:
    DbRecordIndividual *getIndividual(Identifier identifier);
    DbRecordIndividual *constGetIndividual(Identifier identifier) const;

};

#endif // DBRECORD_H
