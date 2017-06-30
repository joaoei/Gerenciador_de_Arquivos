#ifndef AVISO_H
#define AVISO_H

#include <QDialog>

namespace Ui {
class Aviso;
}

class Aviso : public QDialog
{
    Q_OBJECT

public:
    explicit Aviso(QWidget *parent = 0);
    ~Aviso();

private:
    Ui::Aviso *ui;
};

#endif // AVISO_H
