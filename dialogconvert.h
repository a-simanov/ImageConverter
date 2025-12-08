#ifndef DIALOGCONVERT_H
#define DIALOGCONVERT_H

#include <QDialog>
#include <QFileDialog>>

namespace Ui {
class DialogConvert;
}

class DialogConvert : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConvert(QWidget *parent = nullptr);
    ~DialogConvert();

    QString GetFileName();

private slots:
    void on_pb_convert_clicked();
signals:
    void Convert();
private:
    Ui::DialogConvert *ui;
    QString out_file_name_;
};

#endif // DIALOGCONVERT_H
