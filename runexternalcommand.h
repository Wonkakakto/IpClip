#ifndef RUNEXTERNALCOMMAND_H
#define RUNEXTERNALCOMMAND_H

#include <QObject>

/*class RunExternalCommand : public QObject
{
    Q_OBJECT
public:
    explicit RunExternalCommand(QObject *parent = 0);

signals:

public slots:
};*/

class RunExternalCommand : public QObject {
    Q_OBJECT

public:
    RunExternalCommand();
    RunExternalCommand(QString);

public slots:
    void execute();

signals:
    void finished();

private:
    QString command;
};

#endif // RUNEXTERNALCOMMAND_H
