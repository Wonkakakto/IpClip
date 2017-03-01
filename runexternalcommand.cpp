#include "runexternalcommand.h"

RunExternalCommand::RunExternalCommand()
{

    this -> command = "echo"; // empty command

}

RunExternalCommand::RunExternalCommand(QString str)
{

    this -> command = str;

}

void RunExternalCommand::execute()
{

    system(this -> command.toLocal8Bit());
    emit finished();

}
