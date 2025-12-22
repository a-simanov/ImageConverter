#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    ~settings();

    int GetBrightness();

private slots:
    void on_blight_slider_valueChanged(int action);
    void on_buttonBox_accepted();

signals:
    void GetSettings();
private:
    Ui::settings *ui;
    int brightness_;
};

#endif // SETTINGS_H
