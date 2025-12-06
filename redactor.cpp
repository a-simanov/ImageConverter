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
    active_pixmap_ = QPixmap(fileName);
    this->setMinimumSize(50,50);
    FitImage();
    file_name_in_ = fileName;
    ui->action_convert->setEnabled(true);
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
    const ImageFormatInterface* out_interface = GetFormatInterface(file_name_in_.toStdString());
    if (!out_interface) {
        cerr << "Unknown format of the output file"sv << endl;
        return;
    }
    img_lib::Image img = in_interface->LoadImage(fileName.toStdString());
    if (!img) {
        qWarning() << "Loading failed";
    }
    if (!out_interface->SaveImage(file_name_out_.toStdString(), img)) {
        cerr << "Saving failed"sv << endl;
        return;
    }
    dial_conv_.close();
}

[[nodiscard]] QPixmap ResizeImgToFit(const QPixmap &src, int window_width, int window_height) {
    int img_w = src.width();
    int img_h = src.height();

    double w_ratio = double(img_w) / window_width;
    double h_ratio = double(img_h) / window_height;

    if ( w_ratio > h_ratio ) {
        return src.scaledToWidth(window_width);
    } else {
        return src.scaledToHeight(window_height);
    }
}

void Redactor::SetDialogOption() {
    file_name_out_ = dial_conv_.GetFileName();
    getImage(file_name_in_);
}

void Redactor::FitImage() {
    is = false;
    Q_ASSERT(!active_pixmap_.isNull());
    QPixmap temp = ResizeImgToFit(this->active_pixmap_, this->width(), this->height());
    ui->lbl_pixmap->setPixmap(temp);
    ui->lbl_pixmap->resize(temp.width(), temp.height());
    int lbl_x = (this->width() - ui->lbl_pixmap->pixmap().width()) / 2;
    int lbl_y = (this->height() - ui->lbl_pixmap->pixmap().height()) / 2;
    ui->lbl_pixmap->move(lbl_x, lbl_y);
    is = true;
}

void Redactor::resizeEvent(QResizeEvent*)
{
    if(is) {
        FitImage();
    }
}

