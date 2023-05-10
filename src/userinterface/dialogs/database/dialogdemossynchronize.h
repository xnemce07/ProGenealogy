/*!
 *  \file   dialogdemossynchronize.h
 *  \author Leopold Nemcek
 */
#ifndef DIALOGDEMOSSYNCHRONIZE_H
#define DIALOGDEMOSSYNCHRONIZE_H

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>
#include <QSettings>

#include "../../../kernel.h"
#include "userinterface/dialogs/dialog.h"
#include "../../dialogs/import/dialogprogress.h"

class DialogDemosSynchronize: public Dialog
{
    Q_OBJECT
private:
    Kernel &_kernel;

    QLabel _laMail;
    QLabel _laPassword;
    QLabel _laStatus;

    QLineEdit _leMail;
    QLineEdit _lePassword;

    QPushButton _pbSync;

public:
    DialogDemosSynchronize(Kernel &k,QWidget *parent = nullptr);

private:
    void fill() override;

    void showError(QString err);
    void showSuccess(QString mssg);

public slots:
    void syncPressed();
    void rememberPwdChecked(bool checked);
signals:
    void synchronized();
};

#endif // DIALOGDEMOSSYNCHRONIZE_H
