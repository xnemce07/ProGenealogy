/*!
 *  \file   kernel.cpp
 *  \author Eva Navratilova, Leopold Nemcek
 *  \note Majority of this file was created by Eva Navratilova, parts added by Leopold Nemcek are labeled as such in kernel.h
 */

#include "kernel.h"

QString Kernel::_getFileExtension(QString filename)
{
    QString extension;
    QStringList namePieces = filename.split('.');
    if(namePieces.length() > 0)
    {
        extension = namePieces.last();
    }
    return extension.toLower();
}

void Kernel::_cleanupRecord()
{
    if(this->_record != nullptr)
    {
        delete this->_record;
        this->_record = nullptr;
    }
}

void Kernel::_cleanupImporter()
{
    if(this->_importer != nullptr)
    {
        delete this->_importer;
        this->_importer = nullptr;
    }
}

void Kernel::_cleanupExporter()
{
    if(this->_exporter != nullptr)
    {
        delete this->_exporter;
        this->_exporter = nullptr;
    }
}

Kernel::Kernel()
{
    this->_record = new RecordRAM();

    connect(_databaseDemos, &DatabaseDemos::objectsToImport, this, &Kernel::objectsToImport);
    connect(_databaseDemos, &DatabaseDemos::objectImported, this, &Kernel::objectImported);
    connect(_databaseDemos, &DatabaseDemos::importDone, this, &Kernel::importDone);
}

Kernel::~Kernel()
{
    this->_cleanupRecord();
    this->_cleanupImporter();
    this->_cleanupExporter();
}


Record* Kernel::record()
{
    return this->_record;
}

bool Kernel::importFile(QString filename, bool &saveable)
{
    ImportType impType;
    bool retVal;
    QString extension = this->_getFileExtension(filename);

    saveable = false;

    if(filename.isEmpty())
    {
        _record->deleteDatabase();
        this->_importer = new ImporterNewProject();
    }
    else if(extension == "ged")
    {
        _record->deleteDatabase();
        this->_importer = new ImporterGedcom();
    }
    // else if(other extension types)
    else
    {
        // Not able to instantiate an importer for this extension
        emit(this->importDone());
        return false;
    }

    connect(_importer, &Importer::objectsToImport, this, &Kernel::objectsToImport);
    connect(_importer, &Importer::objectImported, this, &Kernel::objectImported);

    retVal = this->_importer->importFile(filename, *_record, impType);

    if(impType == ImportType::THIS_APP)
        saveable = true;

    // Cleanup of the importer
    this->_cleanupImporter();

    emit(this->importDone());
    return retVal;
}


bool Kernel::exportFile(QString filename)
{
    QString extension = this->_getFileExtension(filename);
    if(extension == "ged")
        this->_exporter = new ExporterGedcom();
    else
        return false;

    bool retVal = this->_exporter->exportFile(filename, *(this->_record));

    // Cleanup of the exporter
    this->_cleanupExporter();

    return retVal;
}

bool Kernel::newProject()
{
    bool saveable;
    return this->importFile(QString(), saveable);
}

QString Kernel::demosGetUsername()
{
    return _databaseDemos->getUsername();
}

QString Kernel::demosGetPassword()
{
    return _databaseDemos->getPassword();
}

bool Kernel::demosIsConnected()
{
    return _databaseDemos->isReady();
}

QString Kernel::demosLastError()
{
    return _databaseDemos->lastError();
}

void Kernel::demosSetUsername(QString username)
{
    _databaseDemos->setUsername(username);
}

void Kernel::demosSetPassword(QString password)
{
    _databaseDemos->setPassword(password);
}

void Kernel::demosSetRecordImported(bool imported ,unsigned int primaryKey, DbRecordType type)
{
    _databaseDemos->markAsImported(imported, primaryKey, type);
}

bool Kernel::neo4jExport()
{
    return _databaseNeo4j->dbExport(*_record);
}

QString Kernel::neo4jLastError()
{
    return _databaseNeo4j->lastError();
}

QString Kernel::neo4jGetUsername()
{
    return _databaseNeo4j->getUsername();
}

QString Kernel::neo4jGetPassword()
{
    return _databaseNeo4j->getPassword();
}

QString Kernel::neo4jGetHostname()
{
    return _databaseNeo4j->getHostname();
}

int Kernel::neo4jGetPort()
{
    return _databaseNeo4j->getPort();
}

QString Kernel::neo4jGetDbName()
{
    return _databaseNeo4j->getDbName();
}

void Kernel::neo4jSetUsername(QString username)
{
    _databaseNeo4j->setUsername(username);
}

void Kernel::neo4jSetPassword(QString password)
{
    _databaseNeo4j->setPassword(password);
}

void Kernel::neo4jSetHostname(QString hostname)
{
    _databaseNeo4j->setHostname(hostname);
}

void Kernel::neo4jSetPort(int port)
{
    _databaseNeo4j->setPort(port);
}

void Kernel::neo4jSetDbName(QString dbName)
{
    _databaseNeo4j->setDbName(dbName);
}


bool Kernel::demosSynchronize()
{
    return _databaseDemos->synchronize(*_record);
}


