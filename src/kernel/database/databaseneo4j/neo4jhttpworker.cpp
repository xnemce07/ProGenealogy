/*!
 *  \file   neo4jhttpworker.cpp
 *  \author Leopold Nemcek
 */
#include "neo4jhttpworker.h"

Neo4jHttpWorker::Neo4jHttpWorker()
{
    _manager = new QNetworkAccessManager();
}

int Neo4jHttpWorker::getPort() const
{
    return _port;
}

void Neo4jHttpWorker::setUsername(QString username)
{
    this->_username = username;
}

void Neo4jHttpWorker::setPassword(QString password)
{
    this->_password = password;
}

void Neo4jHttpWorker::setHostname(QString hostname)
{
    this->_hostname = hostname;
}

void Neo4jHttpWorker::setPort(int port)
{
    this->_port = port;
}

void Neo4jHttpWorker::setDbName(QString name)
{
    this->_dbName = name;
}


QString Neo4jHttpWorker::getUsername() const
{
    return this->_username;
}

QString Neo4jHttpWorker::getPassword() const
{
    return this->_password;
}

QString Neo4jHttpWorker::getHostname() const
{
    return this->_hostname;
}

QString Neo4jHttpWorker::getDbName() const
{
    return _dbName;
}


void Neo4jHttpWorker::addStatement(QString statement)
{
    QJsonObject statementObj;
    statementObj.insert("statement",statement);
    _statements.push_back(statementObj);
}

void Neo4jHttpWorker::addStatement(QString statement, QJsonObject props)
{
    QJsonObject statementObj;
    QJsonObject parametersObj;

    statementObj.insert("statement",statement);

//    for(auto i : props.keys()){
//        propsObj.insert(i,props.value(i));
//    }

    parametersObj.insert("props",props);
    statementObj.insert("parameters",parametersObj);
    _statements.push_back(statementObj);
}

bool Neo4jHttpWorker::send()
{

    QString location = "http://" + _hostname + ":" + QString::number(_port) + "/db/" + _dbName + "/tx/commit";
    QByteArray auth = (_username + ":" + _password).toUtf8().toBase64();
    QNetworkRequest request = QNetworkRequest(QUrl(location));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Accept", "application/json;charset=UTF-8");
    request.setRawHeader("Authorization", "Basic " + auth);

    QJsonObject jsonObj;
    jsonObj.insert("statements", _statements);
    QJsonDocument doc(jsonObj);
    QByteArray data = doc.toJson();

    QTimer timer;
    timer.setSingleShot(true);

    QEventLoop loop;

    QNetworkReply* reply = _manager->post(request, data);

    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    timer.start(5000);
    loop.exec();

    if(timer.isActive())
    {
        timer.stop();
        disconnect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        _data = reply->readAll();

        QJsonParseError parseErr;
        QJsonObject resObj = QJsonDocument::fromJson(_data, &parseErr).object();
        QJsonArray errorArray = resObj.value("errors").toArray();
        QJsonObject errorObj = errorArray[0].toObject();
        QString errorMessage = errorObj.value("message").toString();

        clearStatements();
        if(!errorMessage.isEmpty()){
            _error = errorMessage;

            return false;
        }
        return true;
    } else {
        disconnect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        _error = "Request timeout";
        return false;
    }
}

void Neo4jHttpWorker::clearStatements()
{
    _statements = QJsonArray();
}

QByteArray Neo4jHttpWorker::getData()
{
    return _data;
}

QString Neo4jHttpWorker::getLastError()
{
    return _error;
}

void Neo4jHttpWorker::readyRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply *>(sender());

    QByteArray rep = reply->readAll();
    qInfo() << rep;
}

