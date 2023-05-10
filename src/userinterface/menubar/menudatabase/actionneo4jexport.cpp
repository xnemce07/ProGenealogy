/*!
 *  \file   actionneo4jexport.cpp
 *  \author Leopold Nemcek
 */
#include "actionneo4jexport.h"

ActionNeo4jExport::ActionNeo4jExport(Kernel &k, QWidget *parent)
    : Action(parent), _kernel(k)
{
    this->setDisabled(true);
}

void ActionNeo4jExport::translate()
{
    this->setText(tr("Export to a Neo4j database..."));
}

void ActionNeo4jExport::exportDb()
{
    this->process();
}

void ActionNeo4jExport::fileClosed()
{
    this->setDisabled(true);
}

void ActionNeo4jExport::fileOpen()
{
    this->setDisabled(false);
}

void ActionNeo4jExport::process()
{
    _dialog = new DialogNeo4jExport(_kernel);
    _dialog->exec();
}
