#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::settings)
{
    ui->setupUi(this);
}

settings::~settings()
{
    delete ui;
}

int settings::GetBrightness() {
    return brightness_;
}

void settings::on_blight_slider_valueChanged(int action)
{
    brightness_ = action;
}


void settings::on_buttonBox_accepted()
{
    emit GetSettings();
}

