#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_dialog_OK_Btn_clicked()
{
    emit sendData(ui->cardInfo->toPlainText());
}

void Dialog::on_dialog_Close_Btn_clicked()
{
    this->close();
}
