#ifndef DIALOGCONVERT_H
#define DIALOGCONVERT_H

#include <QDialog>

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
};

#endif // DIALOGCONVERT_H
