/*!
 *  \file   actiondemossync.h
 *  \author Leopold Nemcek
 */
#ifndef ACTIONDEMOSSYNC_H
#define ACTIONDEMOSSYNC_H

#include "../action.h"
#include "../../dialogs/database/dialogdemossynchronize.h"
#include "kernel.h"

class ActionDemosSync : public Action
{
    Q_OBJECT
private:
    static const char* _connectText;
    Kernel &_kernel;
    DialogDemosSynchronize *_dialog;


public:
    ActionDemosSync(Kernel &k, QWidget *parent = nullptr);
    void translate() override;
//    void connectToDb();
protected slots:
    void process() override;
signals:
//    void dbConnected();
//    void dbDisconnected();
    void synchronizedDemos();
public slots:
    void fileOpen();
    void fileClosed();
};

#endif // ACTIONDEMOSSYNC_H
