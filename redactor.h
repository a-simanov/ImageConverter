#ifndef REDACTOR_H
#define REDACTOR_H

#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>
#include <bmp_image.h>

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
private slots:
    void slotCustomMenuRequested(QPoint pos);
private:
    Ui::Redactor *ui;
    void SetImage();
    void ImageSobel();

};
#endif // REDACTOR_H
