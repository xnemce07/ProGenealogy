/*!
 *  \file   databaseneo4j.h
 *  \author Leopold Nemcek
 */

#ifndef DATABASENEO4J_H
#define DATABASENEO4J_H

#include "../record.h"

#include "databaseneo4j/neo4jhttpworker.h"

/**
 * @brief Class managing export to a neo4j database
 */
class DatabaseNeo4j
{
private:
    Neo4jHttpWorker* _httpWorker;

public:
    DatabaseNeo4j();


    /**
     * @brief dbExport Export all records to databse
     * @param record Record structure
     * @return True = success, False = error
     */
    bool dbExport(Record &record);

    QString lastError();

    //Getters

    int getPort() const ;
    QString getUsername() const;
    QString getPassword() const;
    QString getHostname() const;
    QString getDbName() const;

    //Setters

    void setUsername(QString username);
    void setPassword(QString password);
    void setHostname(QString hostname);
    void setPort(int port);
    void setDbName(QString value);

private:
    // Export helper functions
    void exportPerson(Record &record, Identifier ref);

    void exportFamily(Record &record, Identifier ref);

    void exportPersonRelatives(Record &record, Identifier ref);

    QChar gender2char(Gender gender);
};

#endif // DATABASENEO4J_H
