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
    QString fileName = QFileDialog::getSaveFileName(
        this,
        QString("Сохранить файл"),
        QDir::currentPath(),
        "Images (*.ppm *.bpm *.jpeg)"
        );
    if (ui->rb_BMP->isChecked()) {
        out_file_name_ = fileName + ".bmp";
    } else if (ui->rb_PPM->isChecked()) {
        out_file_name_ = fileName + ".ppm";
    } else {
        out_file_name_ = fileName + ".jpeg";
    }
    emit Convert();
}

QString DialogConvert::GetFileName() {
    return out_file_name_;
}
