/*!
 *  \file   databasedemos.h
 *  \author Leopold Nemcek
 */

#ifndef DBIMPORTERDEMOS_H
#define DBIMPORTERDEMOS_H


#include <QSettings>
#include "..\record.h"
#include "databasedemos/demoshttpworker.h"
#include "../record/recordRAM/primitives.h"

/**
 * @brief Class managing synchronization with the DEMoS database.
 */
class DatabaseDemos : public QObject
{
    Q_OBJECT
private:
    QHash<QString, DbRecordRole> _roleResolveTable;

    QString _error;

    DemosHttpWorker _worker;

    bool _preferNormalized;

    const QString _address = DEMOS_API_ADDRESS;

public:
    DatabaseDemos();

    /**
     * @brief synchronize Update imported state of record and import all marked records
     * @param record Record structure
     * @return True = success, False = error
     */
    bool synchronize(Record &record);

    /**
     * @brief isConnected Check whether the connection to the database is open
     * @return True is connected, False = isn't connected
     */
    bool isReady();

    /**
     * @brief lastError Get error message from last synchronize() call
     * @return message
     */
    QString lastError();

    //Getters

    QString getUsername() const;
    QString getPassword() const;

    void clearPassword();


    //Setters

    void setUsername(QString username);
    void setPassword(QString password);

    /**
     * @brief markAsImported Mark record in database as imported for current user.
     * @param imported bool
     * @param recordId Primary key of record
     * @param type Type of record
     */
    void markAsImported(bool imported, unsigned int recordId, DbRecordType type);

private:


    bool importRecord(DbRecordType recordType, unsigned int recordId, bool imported, Record &record);

    bool importBirthRecord(unsigned int recordId, bool imported, Record &record);
    bool importBurialRecord(unsigned int recordId, bool imported, Record &record);
    bool importMarriageRecord(unsigned int recordId, bool imported, Record &record);

    bool importIndividual(Identifier ref, DatabaseRow row, Record &record);
    QVector<unsigned int> getRecordIndividuals(DbRecordType recordType, unsigned int recordPk);

    bool addBirthRecordData(unsigned int recordId, Identifier recordRef, Record &record);
    bool addBurialRecordData(unsigned int recordId, Identifier recordRef, Record &record);
    bool addMarriageRecordData(unsigned int recordId, Identifier brideRef, Identifier groomRef, Record &record);
    bool addMarriageRecordData(unsigned int recordId, Identifier recordRef, Record &record);

    bool addSource(DatabaseRow row, Identifier recordRef, DbRecordType type, Record &record, QString plac = "", Date dateFrom = Date());

    QPair<QString, Gender> getGivenNames(unsigned int personId);
    QVector<QString> getOccupations(unsigned int personId);
    QString getAddress(unsigned int addressId);
    
    DbRecordRole roleToEnum(QString roleString) const;

    /**
     * @brief dbDateToDate Converts date in string from the Demos database to Date structure
     * @param date String representing a date from the Demos database
     * @return Date structure
     */
    Date dbDateToDate(QString date) const;



    /**
     * @brief isBrideSide Checks whether the DbRecordRole belongs to DbRecord of the bride's side of marriage
     * @param role Role in question
     * @return Boolean with result
     */
    static bool isBrideSide(DbRecordRole role);

    /**
     * @brief isGroomSide Checks whether the DbRecordRole belongs to DbRecord of the groom's side of marriage
     * @param role Role in question
     * @return Boolean with result
     */
    static bool isGroomSide(DbRecordRole role);

    bool setError(QString error);

    void clearError();

    void initDbRecordRoleResolveTable();

signals:
    void objectsToImport(int count);
    void objectImported();
    void importDone();

};



#endif // DBIMPORTERDEMOS_H
