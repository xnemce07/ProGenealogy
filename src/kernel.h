/*!
 *  \file   kernel.h
 *  \author Eva Navratilova, Leopold Nemcek
 *  \note Majority of this file was created by Eva Navratilova, parts added by Leopold Nemcek are labeled as such
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include <QStringList>

#include "kernel/record.h"
#include "kernel/importer.h"
#include "kernel/importer/importergedcom.h"
#include "kernel/importer/importernewproject.h"
#include "kernel/exporter.h"
#include "kernel/exporter/exportergedcom.h"
#include "kernel/record/recordram.h"
#include "kernel/record/recordRAM/identifier.h"
#include "kernel/database/databasedemos.h"
#include "kernel/database/databaseneo4j.h"


/**
 * @brief
 *
 */
class Kernel : public QObject
{
    Q_OBJECT
private:
    Record *_record = nullptr; /**< Saves all records about people and their relationships */
    Importer *_importer = nullptr; /**< Imports a file */
    Exporter *_exporter = nullptr; /**< Exports or saves a file */
    DatabaseDemos* _databaseDemos = new DatabaseDemos(); /**< Imports data from the demos database */
    DatabaseNeo4j* _databaseNeo4j = new DatabaseNeo4j();

    /**
     * @brief Returns a Lower Case extension of given filename
     *
     * @param filename  Name of the file to extract extension from
     * @return          Extension of given filename in Lower Case
     */
    QString _getFileExtension(QString filename);

    void _cleanupRecord();
    void _cleanupImporter();
    void _cleanupExporter();

public:
    /**
     * @brief Constructor of Kernel
     *
     */
    Kernel();

    /*static Kernel& getInstance(){
        static Kernel instance;
        return instance;
    }*/

    /**
     * @brief Kernel is not copyable - Copy constructors are deleted
     *
     */
    Kernel(Kernel const&) = delete;
    void operator=(Kernel const&) = delete;

    /**
     * @brief Destructor of Kernel
     *
     */
    virtual ~Kernel();

    /**
     * @brief
     *
     * @return Record
     */
    Record* record();

    /**
     * @brief Imports a file, using its extension to determine the right way of the import.
     *
     * @param filename  Name of the file to import with its extension (if string empty, new project created!)
     * @param saveable  Is the file saveable (eg. this file was previously generated from this program)
     * @return          True if the import was successful (false can indicate missing file, invalid extension, etc.)
     */
    bool importFile(QString filename, bool& saveable);
    bool exportFile(QString filename);

    bool newProject();


    /// DATABASES - Section added by Leopold Nemcek

    /// Demos


    /**
     * @brief demosSynchronize Import all records marked by current user. Also updates their imported state in database.
     * @return bool - True = success; False = error
     */
    bool demosSynchronize();

    /**
     * @brief demosIsConnected Try to send a simple request to API of DEMoS. Can be used to check if correct credentials are used or if server is running
     * @return bool True = success, False = error
     */
    bool demosIsConnected();

    /**
     * @brief demosLastError Get error from last attempt to connect to DEMoS API
     * @return
     */
    QString demosLastError();

    /**
     * @brief demosGetUsername Get currently set username
     * @return
     */
    QString demosGetUsername();

    /**
     * @brief demosGetUsername Get currently set password
     * @return
     */
    QString demosGetPassword();

    void demosSetUsername(QString username);
    void demosSetPassword(QString password);

    /**
     * @brief demosSetRecordImported
     * @param imported
     * @param primaryKey
     * @param type
     */
    void demosSetRecordImported(bool imported, unsigned int primaryKey, DbRecordType type);

    /**
     * @brief neo4jExport Export family tree from record to Neo4j database
     * @return
     */
    bool neo4jExport();

    /**
     * @brief neo4jLastError Get error from last connection to Neo4j
     * @return
     */
    QString neo4jLastError();

    QString neo4jGetUsername();
    QString neo4jGetPassword();
    QString neo4jGetHostname();
    int neo4jGetPort();
    QString neo4jGetDbName();

    void neo4jSetUsername(QString username);
    void neo4jSetPassword(QString password);
    void neo4jSetHostname(QString hostname);
    void neo4jSetPort(int port);
    void neo4jSetDbName(QString dbName);

signals:
    void objectsToImport(int count);
    void objectImported();
    void importDone();

};




#endif // KERNEL_H
