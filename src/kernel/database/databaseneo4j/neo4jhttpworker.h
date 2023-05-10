/*!
 *  \file   neo4jhttpworker.h
 *  \author Leopold Nemcek
 */
#ifndef NEO4JHTTPWORKER_H
#define NEO4JHTTPWORKER_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

/**Class providing way to communicate with a Neo4j database.
 */
class Neo4jHttpWorker : public QObject
{
    Q_OBJECT
private:

    QString _username;
    QString _password;
    QString _hostname;
    int _port = 0;
    QString _dbName;

    QNetworkAccessManager* _manager;

    QByteArray _data;
    QString _error;


    QJsonArray _statements;


public:
    Neo4jHttpWorker();
    ~Neo4jHttpWorker() = default;

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


    /**
     * @brief addStatement Add a statement to memory. Statements from memory can be executed with method send()
     * @param statement QString
     */
    void addStatement(QString statement);
    /**
     * @brief addStatement Add a statement to memory. Statements from memory can be executed with method send()
     * @param statement QString
     * @param props props that will be added to statement
     */
    void addStatement(QString statement, QJsonObject props);

    /**
     * @brief clear Clear statements from memory
     */
    void clear();

    /**
     * @brief send Send all statements from memory
     * @return
     */
    bool  send();

    void clearStatements();

    /**
     * @brief getData Get bytes returned from database on last send()
     * @return
     */
    QByteArray getData();

    QString getLastError();

private slots:
    void readyRead();
};

#endif // NEO4JHTTPWORKER_H
