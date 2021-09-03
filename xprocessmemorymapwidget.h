#ifndef XPROCESSMEMORYMAPWIDGET_H
#define XPROCESSMEMORYMAPWIDGET_H

#include <QWidget>

namespace Ui {
class XProcessMemoryMapWidget;
}

class XProcessMemoryMapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XProcessMemoryMapWidget(QWidget *parent = nullptr);
    ~XProcessMemoryMapWidget();

private:
    Ui::XProcessMemoryMapWidget *ui;
};

#endif // XPROCESSMEMORYMAPWIDGET_H
