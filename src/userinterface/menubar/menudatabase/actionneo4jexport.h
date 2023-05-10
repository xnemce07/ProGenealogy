/*!
 *  \file   actionneo4jexport.h
 *  \author Leopold Nemcek
 */
#ifndef ACTIONNEO4JEXPORT_H
#define ACTIONNEO4JEXPORT_H

#include "../action.h"
#include "../../dialogs/database/dialogneo4jexport.h"
#include "kernel.h"


class ActionNeo4jExport : public Action
{
    Q_OBJECT
private:
    Kernel &_kernel;
    DialogNeo4jExport *_dialog;
public:
    ActionNeo4jExport(Kernel &k, QWidget *parent = nullptr);
    void translate() override;
    void exportDb();
public slots:
    void fileClosed();
    void fileOpen();
protected slots:
    void process() override;
};

#endif // ACTIONNEO4JEXPORT_H
