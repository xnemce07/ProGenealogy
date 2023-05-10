/*!
 *  \file   demoshttpworker.h
 *  \author Leopold Nemcek
 */
#ifndef DEMOSHTTPWORKER_H
#define DEMOSHTTPWORKER_H



#include "databaserow.h"
#include <QString>
#include <QVariant>
#include <QMap>
#include <QVector>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>
#include <QEventLoop>
#include <QtCore>
#include <QRegularExpression>


/**
 * @brief Class providing communication with the DEMoS database trough API of the system.
 */
class DemosHttpWorker : public QObject
{
    Q_OBJECT
private:

    QString _username;
    QString _password;
    QString _address;



    QNetworkAccessManager* _manager;

    QJsonArray _resultRows;
    QString _error;
    bool _lastOpOk;

    const unsigned int _timeout = 5000;

public:
    DemosHttpWorker();

    bool isOpen();
    QString lastError();
    bool lastOperationOk();

    //Getters
    QString getUsername() const;
    QString getPassword() const;
    QString getAddress() const;


    //Setters
    void setUsername(QString username);
    void setPassword(QString password);
    void setAddress(QString address);


    // Get data from database

    QVector<DatabaseRow> getMarkedRecords();
    QVector<DatabaseRow> getBirthRecordIndividuals(unsigned int recordId);
    QVector<DatabaseRow> getMarriageRecordIndividuals(unsigned int recordId);
    QVector<DatabaseRow> getBurialRecordIndividuals(unsigned int recordId);
    QVector<DatabaseRow> getIndividualGivenNames(unsigned int personId);
    QVector<DatabaseRow> getIndividualOccupations(unsigned int personId);
    QVector<DatabaseRow> getBirthRecord(unsigned int recordId);
    QVector<DatabaseRow> getBirthMarriage(unsigned int birthRecordId);
    QVector<DatabaseRow> getMarriageRecord(unsigned int recordId);
    QVector<DatabaseRow> getAddress(unsigned int addressId);
    QVector<DatabaseRow> getBurialRecord(unsigned int recordId);
    QVector<DatabaseRow> getSourceInfo(unsigned int registerID);

    bool setBirthRecordImported(bool imported, unsigned int recordId);
    bool setBurialRecordImported(bool imported, unsigned int recordId);
    bool setMarriageRecordImported(bool imported, unsigned int recordId);

private:
    bool sendRequest(QString methodName, QString param1Name = "", QString param1 = "", QString param2Name = "", QString param2 = "");

    void setError(QString error);
    void clearError();

    QVector<DatabaseRow> dataToRows();
};

#endif // DEMOSHTTPWORKER_H
