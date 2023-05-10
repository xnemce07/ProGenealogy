/*!
 *  \file   dialogneo4jexport.h
 *  \author Leopold Nemcek
 */
#ifndef DIALOGNEO4JEXPORT_H
#define DIALOGNEO4JEXPORT_H

#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>

#include "kernel.h"
#include "userinterface/dialogs/dialog.h"
#include "../messageboxes/messagebox.h"


class DialogNeo4jExport: public Dialog
{
    Q_OBJECT
private:
    Kernel &_kernel;

    QLabel _laUsername;
    QLabel _laPassword;
    QLabel _laAddress;
    QLabel _laDbName;
    QLabel _laPort;
    QLabel _laResult;

    QLineEdit _leUsername;
    QLineEdit _lePassword;
    QLineEdit _leAddress;
    QLineEdit _leDbName;
    QLineEdit _lePort;

    QPushButton _pbExport;
public:
    DialogNeo4jExport(Kernel &k,QWidget *parent = nullptr);

private:
    void fill() override;

private slots:
    void exportPressed();
};

#endif // DIALOGNEO4JEXPORT_H
