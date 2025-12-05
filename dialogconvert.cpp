#include "dialogconvert.h"
#include "ui_dialogconvert.h"

DialogConvert::DialogConvert(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogConvert)
{
    ui->setupUi(this);
}

DialogConvert::~DialogConvert()
{
    delete ui;
}

void DialogConvert::on_pb_convert_clicked()
{
    emit Convert();
}

QString DialogConvert::GetFileName(){
    if (ui->le_file_format->text() == "") {
        ui->le_file_format->setText("Введите имя файла");
        return {};
    }
    if (ui->rb_BMP->isChecked()) {
        return ui->le_file_format->text() + ".bmp";
    } else if (ui->rb_PPM->isChecked()) {
        return ui->le_file_format->text() + ".ppm";
    }
    return ui->le_file_format->text() + ".jpeg";
}

