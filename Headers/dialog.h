#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_dialog_OK_Btn_clicked();

    void on_dialog_Close_Btn_clicked();

signals:
    void sendData(QString str);  //用来传递数据的信号

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
