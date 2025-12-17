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
    connect(ui->action_horiz_mirror, &QAction::triggered, this, &Redactor::HorizontalMirror);
    connect(ui->action_vert_mirror, &QAction::triggered, this, &Redactor::VerticalMirror);
    connect(ui->action_sobel, &QAction::triggered, this, &Redactor::InvertColors);
    connect(ui->actionter, &QAction::triggered, this, &Redactor::ChangeImageColors);
    connect(ui->action_save, &QAction::triggered, this, &Redactor::Save);
    connect(ui->action_save_as, &QAction::triggered, this, &Redactor::SaveFileAs);
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &Redactor::DeleteTempFile);

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
    this->setMinimumSize(active_pixmap_.size() / 10);
    FitImage();
    file_name_in_ = fileName;
    ui->action_convert->setEnabled(true);
    ui->action_horiz_mirror->setEnabled(true);
    ui->action_vert_mirror->setEnabled(true);
    ui->action_sobel->setEnabled(true);
    ui->action_save_as->setEnabled(true);
    ui->action_save->setEnabled(true);
    LoadImage();
}

void Redactor::slotCustomMenuRequested(QPoint pos) {
    ui->menu->popup(mapToGlobal(pos));
}

void Redactor::getImage () {
    SaveFile(file_name_out_.toStdString());
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
    getImage();
}

void Redactor::LoadImage() {
    const ImageFormatInterface* in_interface = GetFormatInterface(file_name_in_.toStdString());
    if(!in_interface) {
        cerr << "Unknown format of the input file"sv << endl;
        return;
    }
    image_ = in_interface->LoadImage(file_name_in_.toStdString());
    if (!image_) {
        qWarning() << "Loading failed";
    }
}

void Redactor::FitImage() {
    is = false;
    Q_ASSERT(!active_pixmap_.isNull());
    QPixmap temp = ResizeImgToFit(this->active_pixmap_, this->width(), this->height());
    ui->lbl_pixmap->setPixmap(temp);
    ui->lbl_pixmap->resize(temp.width(), temp.height());
    int lbl_x = (this->width() - active_pixmap_.width()) / 2;
    int lbl_y = (this->height() - active_pixmap_.height()) / 2;
    ui->lbl_pixmap->move(lbl_x, lbl_y);
    is = true;
}

void Redactor::resizeEvent(QResizeEvent*)
{
    if(is) {
        FitImage();
    }
}

QString Redactor::SetTempImage() {
    tmp_file_ = file_name_in_.toStdString();
    int pos = tmp_file_.find_last_of('/');
    tmp_file_.insert(pos + 1, "tmp_");
    SaveFile(tmp_file_);
    return QString::fromStdString(tmp_file_);
}

void Redactor::VerticalMirror () {
    VMirrInplace(image_);
    active_pixmap_ = QPixmap(SetTempImage());
    FitImage();
}

void Redactor::HorizontalMirror () {
    HMirrInplace(image_);
    active_pixmap_ = QPixmap(SetTempImage());
    FitImage();
}

void Redactor::InvertColors () {
    image_ = Sobel(image_);
    active_pixmap_ = QPixmap(SetTempImage());
    FitImage();
}

void Redactor::ChangeImageColors () {
    ChangeColors(image_, 55, 55, 55);
    active_pixmap_ = QPixmap(SetTempImage());
    FitImage();
}

void Redactor::SaveFile(std::string file) {
    const ImageFormatInterface* out_interface = GetFormatInterface(file);
    if (!out_interface) {
        cerr << "Unknown format of the output file"sv << endl;
        return;
    }
    if (!out_interface->SaveImage(file, image_)) {
        cerr << "Saving failed"sv << endl;
        return;
    }
}

void Redactor::Save () {
    //QFile::copy(QString::fromStdString(tmp_file_), file_name_in_);
    QFile old_file (file_name_in_);
    old_file.remove();
    QFile file (QString::fromStdString(tmp_file_));
    file.rename(file_name_in_);
}

void Redactor::DeleteTempFile () {
    QFile tmp (QString::fromStdString(tmp_file_));
    tmp.remove();
    tmp_file_ = "";
}

void Redactor::SaveFileAs () {
    std::string name_for_format = file_name_in_.toStdString();
    std::string format = name_for_format.substr(name_for_format.find('.'));
    QString fileName = QFileDialog::getSaveFileName(
        this,
        QString("Сохранить файл"),
        QDir::currentPath(),
        "Images (*.ppm *.bpm *.jpeg)"
        );
    if (fileName.contains('.')) {
        SaveFile(fileName.toStdString());
    } else {
        SaveFile(fileName.toStdString() + format);
    }

}

