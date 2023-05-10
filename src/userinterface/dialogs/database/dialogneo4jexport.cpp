/*!
 *  \file   dialogneo4jexport.cpp
 *  \author Leopold Nemcek
 */
#include "dialogneo4jexport.h"

DialogNeo4jExport::DialogNeo4jExport(Kernel &k, QWidget *parent):
    Dialog(parent), _kernel(k)
{
    this->fill();
    connect(&_pbExport, &QPushButton::clicked,this, &DialogNeo4jExport::exportPressed);
}

void DialogNeo4jExport::fill()
{
    QSettings settings;

    // init labels
    _laUsername.setText(tr("Username"));
    _laPassword.setText(tr("Password"));
    _laAddress.setText(tr("Hostname"));
    _laPort.setText(tr("Port"));
    _laDbName.setText(tr("Database name"));
    _pbExport.setText(tr("Export"));

    // Get connection information from exporter, if empty, get them from QSettings
    QString uname = _kernel.neo4jGetUsername();
    if(uname.isEmpty()){
        uname = settings.value("neo4j/login/username").toString();
    }

    QString hostname = _kernel.neo4jGetHostname();
    if(hostname.isEmpty()){
        hostname = settings.value("neo4j/login/hostname").toString();
    }

    QString dbName = _kernel.neo4jGetDbName();
    if(dbName.isEmpty()){
        dbName = settings.value("neo4j/login/dbName").toString();
    }

    unsigned int port = _kernel.neo4jGetPort();
    if(port == 0){
        port = settings.value("neo4j/login/port", 7474).toUInt();
    }



    _leUsername.setText(uname);
    _leAddress.setText(hostname);
    _lePassword.setEchoMode(QLineEdit::EchoMode::Password);
    _lePassword.setText(_kernel.neo4jGetPassword());
    _lePort.setText(QString::number(port));
    _leDbName.setText(dbName);



    // Add widgets
    _layout.addWidget(&_laUsername,0,0);
    _layout.addWidget(&_leUsername, 0 , 1);
    _layout.addWidget(&_laPassword, 1,0);
    _layout.addWidget(&_lePassword, 1, 1);
    _layout.addWidget(&_laDbName, 2,0);
    _layout.addWidget(&_leDbName, 2 ,1);
    _layout.addWidget(&_laAddress, 3,0);
    _layout.addWidget(&_leAddress, 3 ,1);
    _layout.addWidget(&_laPort, 4, 0);
    _layout.addWidget(&_lePort, 4, 1);
    _layout.addWidget(&_pbExport, 5, 1);
    _layout.addWidget(&_laResult,6,1);

    // Adjust window
    this->setWindowTitle(tr("Export to a Neo4j database"));
    _layout.setColumnMinimumWidth(1, 175);
}

void DialogNeo4jExport::exportPressed()
{
    QSettings settings;

    // Save connection info
    settings.setValue("neo4j/login/username",_leUsername.text());
    settings.setValue("neo4j/login/hostname",_leAddress.text());
    settings.setValue("neo4j/login/dbname",_leDbName.text());
    settings.setValue("neo4j/login/port",_lePort.text().toUInt());

    // Show warning
    MessageBox message(MessageBox::Icon::Warning,
                       tr("Exporting to a Neo4jDatabase"),
                       tr("This is a experimental feature and it might not work as intended if the database is not empty.\n"
                          "If there is any data in the database, it is recommended to perform a backup before proceeding.\n"
                          "Do you wish to continue?"),
                       MessageBox::StandardButton::Yes | MessageBox::StandardButton::No,
                       MessageBox::StandardButton::Yes);
    if(message.exec() != MessageBox::Accepted){
        return;
    }


    QString address = _leAddress.text();

    _kernel.neo4jSetUsername(_leUsername.text());
    _kernel.neo4jSetPassword(_lePassword.text());
    _kernel.neo4jSetHostname(_leAddress.text());
    _kernel.neo4jSetPort(_lePort.text().toUInt());
    _kernel.neo4jSetDbName(_leDbName.text());

    // Set wait cursor
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();

    _pbExport.setDisabled(true);
    bool isExportOk = _kernel.neo4jExport();

    // Unset wait cursor
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();

    _pbExport.setDisabled(false);
    if(isExportOk){
        _laResult.setText(tr("Success"));
        _laResult.setStyleSheet("font-weight: bold; color: green");
    }else{
        _laResult.setText(_kernel.neo4jLastError());
        _laResult.setStyleSheet("font-weight: bold; color: red");
    }

}
