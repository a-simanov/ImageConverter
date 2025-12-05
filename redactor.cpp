#include "redactor.h"
#include "./ui_redactor.h"

Redactor::Redactor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Redactor)
{
    ui->setupUi(this);
    ui->menubar->hide();

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QMainWindow::customContextMenuRequested,this, &Redactor::slotCustomMenuRequested);
    connect(ui->action_open_file, &QAction::triggered, this, &Redactor::SetImage);
    connect(ui->action_convert, &QAction::triggered, this, &Redactor::OpenConvertDialog);
    connect(&dial_conv_, &DialogConvert::Convert, this, &Redactor::SetDialogOption);
}

Redactor::~Redactor()
{
    delete ui;
}

void Redactor::OpenConvertDialog() {
    dial_conv_.open();
}

void Redactor::SetImage() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        QString("Открыть файл"),
        QDir::currentPath(),
        "Images (*.ppm *.bpm *.jpeg)"
        );
    ui->lbl_pixmap->setPixmap(fileName);
    ui->lbl_pixmap->setFixedHeight(ui->lbl_pixmap->pixmap().height());
    ui->lbl_pixmap->setFixedWidth(ui->lbl_pixmap->pixmap().width());
}

void Redactor::slotCustomMenuRequested(QPoint pos) {
    ui->menu->popup(mapToGlobal(pos));
}

void Redactor::getImage (QString fileName) {
    const ImageFormatInterface* in_interface = GetFormatInterface(fileName.toStdString());
    if(!in_interface) {
        cerr << "Unknown format of the input file"sv << endl;
        return;
    }
    const ImageFormatInterface* out_interface = GetFormatInterface(file_name_.toStdString());
    if (!out_interface) {
        cerr << "Unknown format of the output file"sv << endl;
        return;
    }
    img_lib::Image img = in_interface->LoadImage(fileName.toStdString());
    if (!img) {
        qWarning() << "Loading failed";
    }
    if (!out_interface->SaveImage(file_name_.toStdString(), img)) {
        cerr << "Saving failed"sv << endl;
        return;
    }
}

void Redactor::SetDialogOption() {
    file_name_ = dial_conv_.GetFileName();
    getImage("Nature.jpeg");
}
