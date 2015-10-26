#ifndef EDITCOMMAND_H
#define EDITCOMMAND_H

#include <QDialog>

namespace Ui {
class editCommand;
}

class editCommand : public QDialog
{
    Q_OBJECT

public:
    explicit editCommand(QWidget *parent = 0);
    ~editCommand();

private:
    Ui::editCommand *ui;
};

#endif // EDITCOMMAND_H
