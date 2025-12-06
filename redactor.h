#ifndef REDACTOR_H
#define REDACTOR_H

#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>
#include <bmp_image.h>

#include "dialogconvert.h"
#include "converter.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class Redactor;
}
QT_END_NAMESPACE

class Redactor : public QMainWindow
{
    Q_OBJECT

public:
    Redactor(QWidget *parent = nullptr);
    ~Redactor();
    void getImage (QString fileName);
    void FitImage();
private slots:
    void slotCustomMenuRequested(QPoint pos);
private:
    Ui::Redactor *ui;    
    DialogConvert dial_conv_;
    QString file_name_in_{};
    QString file_name_out_{};
    QPixmap active_pixmap_;
    bool is = false;

    void SetImage();
    void ImageSobel();
    void OpenConvertDialog();
    void SetDialogOption();
    void resizeEvent(QResizeEvent *event) override;
};
#endif // REDACTOR_H
