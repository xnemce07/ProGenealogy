/*!
 *  \file   actiondemossync.cpp
 *  \author Leopold Nemcek
 */
#include "actiondemossync.h"

const char* ActionDemosSync::_connectText = QT_TR_NOOP("Sync with DEMoS...");

ActionDemosSync::ActionDemosSync(Kernel &k, QWidget *parent)
    : Action(parent), _kernel(k)
{
    this->setDisabled(true);
}

void ActionDemosSync::translate()
{
    this->setText(tr(_connectText));
}

//void ActionDemosSync::connectToDb()
//{
//    if(_kernel.demosIsConnected()){
//        emit dbConnected();
//    }else{
//        emit dbDisconnected();
//    }
//}

void ActionDemosSync::process()
{
    _dialog = new DialogDemosSynchronize(_kernel);

    connect(_dialog, &DialogDemosSynchronize::synchronized, this, &ActionDemosSync::synchronizedDemos);

    _dialog->exec();
}

void ActionDemosSync::fileOpen()
{
    this->setDisabled(false);
}

void ActionDemosSync::fileClosed()
{
    this->setDisabled(true);
}
