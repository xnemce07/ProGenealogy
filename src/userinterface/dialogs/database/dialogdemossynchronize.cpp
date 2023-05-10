/*!
 *  \file   dialogdemossynchronize.cpp
 *  \author Leopold Nemcek
 */
#include "dialogdemossynchronize.h"

DialogDemosSynchronize::DialogDemosSynchronize(Kernel &k, QWidget *parent):
    Dialog(parent), _kernel(k)
{
    this->fill();
}

void DialogDemosSynchronize::fill()
{
    QSettings settings;

    _laMail.setText(tr("e-mail"));
    _laPassword.setText(tr("Password"));
    _pbSync.setText(tr("Synchronize"));

    // Connect
    connect(&_pbSync, &QPushButton::clicked, this, &DialogDemosSynchronize::syncPressed);

    QString mail = _kernel.demosGetUsername();
    if(mail.isEmpty()){
        mail = settings.value("demos/login/username").toString();
    }

    _leMail.setText(mail);
    _lePassword.setEchoMode(QLineEdit::EchoMode::Password);
    _lePassword.setText(_kernel.demosGetPassword());


    this->setWindowTitle(tr("Synchronize with DEMoS"));

    _layout.addWidget(&_laMail,0,0);
    _layout.addWidget(&_leMail, 0 , 1);
    _layout.addWidget(&_laPassword, 1,0);
    _layout.addWidget(&_lePassword, 1, 1);
    _layout.addWidget(&_pbSync, 2, 1);
    _layout.addWidget(&_laStatus,3,1);

    _layout.setColumnMinimumWidth(1, 175);
}

void DialogDemosSynchronize::showError(QString err)
{
    _laStatus.setStyleSheet("font-weight: bold; color: red");
    _laStatus.setText(err);
}

void DialogDemosSynchronize::showSuccess(QString mssg)
{
    _laStatus.setStyleSheet("font-weight: bold; color: green");
    _laStatus.setText(mssg);
}

void DialogDemosSynchronize::syncPressed()
{
    // Save username
    QSettings settings;
    settings.setValue("demos/login/username",_leMail.text());

    _kernel.demosSetUsername(_leMail.text());
    _kernel.demosSetPassword(_lePassword.text());

    // Set wait cursor
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();

    DialogProgress dialog(_kernel);
    dialog.open();

    _pbSync.setDisabled(true);
    bool isImportOk = _kernel.demosSynchronize();
    _pbSync.setDisabled(false);

    // Unset wait cursor
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();



    if(isImportOk){
        emit this->synchronized();
        showSuccess(tr("Success"));
    } else {
        showError(_kernel.demosLastError());
    }
}

void DialogDemosSynchronize::rememberPwdChecked(bool checked)
{
    Q_UNUSED(checked)
    QSettings settings;
}
