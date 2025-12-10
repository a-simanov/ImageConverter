#ifndef REDACTOR_H
#define REDACTOR_H

#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>
#include <bmp_image.h>

#include "dialogconvert.h"
#include "converter.h"
#include "h_mirror.h"
#include "v_mirror.h"
#include "sobel.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QFile>

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
    void getImage ();
    void FitImage();
    void DeleteTempFile ();
private slots:
    void slotCustomMenuRequested(QPoint pos);
private:
    Ui::Redactor *ui;    
    DialogConvert dial_conv_;
    QString file_name_in_{};
    QString file_name_out_{};
    QPixmap active_pixmap_;
    bool is = false;
    img_lib::Image image_;
    QString original_file_;
    std::string tmp_file_;

    void SetImage();
    void ImageSobel();
    void OpenConvertDialog();
    void SetDialogOption();
    void resizeEvent(QResizeEvent *event) override;
    QString SetTempImage();
    void HorizontalMirror();
    void VerticalMirror ();
    void InvertColors ();
    void LoadImage();
    void Save();
    void SaveFileAs ();
    void SaveFile(std::string);
};
#endif // REDACTOR_H
