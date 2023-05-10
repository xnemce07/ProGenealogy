/*!
 *  \file   demoshttpworker.cpp
 *  \author Leopold Nemcek
 */
#include "demoshttpworker.h"

DemosHttpWorker::DemosHttpWorker()
{
    _manager = new QNetworkAccessManager();
}

bool DemosHttpWorker::isOpen()
{
    sendRequest("get-marked");
    return _lastOpOk;
}

QString DemosHttpWorker::lastError()
{
    return _error;
}

bool DemosHttpWorker::lastOperationOk()
{
    return _lastOpOk;
}

void DemosHttpWorker::setUsername(QString username)
{
    this->_username = username;
}

void DemosHttpWorker::setPassword(QString password)
{
    this->_password = password;
}

void DemosHttpWorker::setAddress(QString address)
{
    this->_address = address;
}

QString DemosHttpWorker::getUsername() const
{
    return this->_username;
}

QString DemosHttpWorker::getPassword() const
{
    return this->_password;
}

QString DemosHttpWorker::getAddress() const
{
    return this->_address;
}


QVector<DatabaseRow> DemosHttpWorker::getMarkedRecords()
{
    sendRequest("get-marked");
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getBirthRecordIndividuals(unsigned int recordId)
{
    sendRequest("get-birth-persons", "birthId", QString::number(recordId));
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getMarriageRecordIndividuals(unsigned int recordId){
    sendRequest("get-marriage-persons", "marriageId", QString::number(recordId));
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getBurialRecordIndividuals(unsigned int recordId){
    sendRequest("get-burial-persons", "burialId", QString::number(recordId));
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getIndividualGivenNames(unsigned int personId){
    sendRequest("get-given-names", "personId", QString::number(personId));
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getIndividualOccupations(unsigned int personId){
    sendRequest("get-occupations", "personId", QString::number(personId));
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getBirthRecord(unsigned int recordId){
    sendRequest("get-birth-record", "birthId", QString::number(recordId));
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getBirthMarriage(unsigned int birthRecordId){
    sendRequest("get-birth-marriage", "birthId", QString::number(birthRecordId));
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getMarriageRecord(unsigned int recordId){
    sendRequest("get-marriage-record", "marriageId", QString::number(recordId));
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getAddress(unsigned int addressId){
    sendRequest("get-address", "addressId", QString::number(addressId));
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getBurialRecord(unsigned int recordId){
    sendRequest("get-burial-record", "burialId", QString::number(recordId));
    return dataToRows();
}

QVector<DatabaseRow> DemosHttpWorker::getSourceInfo(unsigned int registerID){
    sendRequest("get-register", "registerId", QString::number(registerID));
    return dataToRows();
}


bool DemosHttpWorker::setBirthRecordImported(bool imported, unsigned int recordId){
    if(imported){
        return sendRequest("mark-birth-imported", "imported", "1", "birthId", QString::number(recordId));
    } else {
        return sendRequest("mark-birth-imported", "imported", "0", "birthId", QString::number(recordId));
    }

}

bool DemosHttpWorker::setBurialRecordImported(bool imported, unsigned int recordId){
    if(imported){
        return sendRequest("mark-burial-imported", "imported", "1", "burialId", QString::number(recordId));
    } else {
        return sendRequest("mark-burial-imported", "imported", "0", "burialId", QString::number(recordId));
    }

}

bool DemosHttpWorker::setMarriageRecordImported(bool imported, unsigned int recordId){
    if(imported){
        return sendRequest("mark-marriage-imported", "imported", "1", "marriageId", QString::number(recordId));
    } else {
        return sendRequest("mark-marriage-imported", "imported", "0", "marriageId", QString::number(recordId));
    }
}

bool DemosHttpWorker::sendRequest(QString methodName, QString param1Name, QString param1, QString param2Name, QString param2)
{
    clearError();

    QUrl url(_address + methodName +"/");
//    QUrl url(_address);

    QByteArray auth = (_username + ":" + _password).toUtf8().toBase64();

    QUrlQuery query;


    if(!param1Name.isEmpty()){
        query.addQueryItem(param1Name, param1);
    }
    if(!param2Name.isEmpty()){
        query.addQueryItem(param2Name, param2);
    }

    url.setQuery(query);

    QNetworkRequest request = QNetworkRequest(url);
    request.setRawHeader("Accept", "application/json;charset=UTF-8");
    request.setRawHeader("Authorization", "Basic " + auth);

    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    QNetworkReply* reply = _manager->get(request);

    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    timer.start(5000);
    loop.exec();

    if(!timer.isActive())
    {
        disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        disconnect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

        setError("Could not establish connection");
        return false;
    } else {
        timer.stop();

        disconnect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        QByteArray bytes = reply->readAll();
        QNetworkReply::NetworkError replyError = reply->error();
        reply->deleteLater();

        bytes = bytes.replace("\\\"", "\"");
        bytes = bytes.replace("\\\\", "\\");

        if(!bytes.isEmpty() && bytes.at(0) == '"' && bytes.endsWith('"')){
            bytes = bytes.mid(1, bytes.length() - 2);
        }





        QString jsonString = QString::fromUtf8(bytes);

        QRegExp rx("(\\\\u[0-9a-fA-F]{4})");
        int pos = 0;
        while ((pos = rx.indexIn(jsonString, pos)) != -1) {
            jsonString.replace(pos++, 6, QChar(rx.cap(1).right(4).toUShort(0, 16)));
        }

        QJsonParseError parseErr;
        QJsonObject resData = QJsonDocument::fromJson(jsonString.toUtf8(), &parseErr).object();

        if(replyError != QNetworkReply::NoError){
            QString error = resData.value("error").toString();
            if(error.isEmpty()){
                error = reply->errorString();
            }
            setError(error);
            return false;
        }

        _resultRows = resData.value("results").toArray();
    }
    return true;
}

void DemosHttpWorker::setError(QString error)
{
    _error = error;
    _lastOpOk = false;
}

void DemosHttpWorker::clearError()
{
    _error = "";
    _lastOpOk = true;
}

QVector<DatabaseRow> DemosHttpWorker::dataToRows()
{
    QVector<DatabaseRow> rows;

    foreach(QJsonValue val, _resultRows){
        if(val.isObject()){
            QJsonObject object = val.toObject();

            QVariantMap map = object.toVariantMap();
            rows.append(DatabaseRow(map));
        }
    }

    return rows;
}


