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
}

Redactor::~Redactor()
{
    delete ui;
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
    getImage(fileName);
}

void Redactor::slotCustomMenuRequested(QPoint pos) {
    ui->menu->popup(mapToGlobal(pos));
}

void Redactor::getImage (QString fileName) {
    img_lib::Image img = img_lib::LoadJPEG(fileName.toStdString());
    if (!img) {
        qWarning() << "Loading failed";
    }
    img_lib::SaveBMP("kot.bmp", img);
}
