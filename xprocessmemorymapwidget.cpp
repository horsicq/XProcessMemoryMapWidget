#include "xprocessmemorymapwidget.h"
#include "ui_xprocessmemorymapwidget.h"

XProcessMemoryMapWidget::XProcessMemoryMapWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XProcessMemoryMapWidget)
{
    ui->setupUi(this);
}

XProcessMemoryMapWidget::~XProcessMemoryMapWidget()
{
    delete ui;
}
