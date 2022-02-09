/* Copyright (c) 2021-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef XPROCESSMEMORYMAPWIDGET_H
#define XPROCESSMEMORYMAPWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QFuture>
#include <QtConcurrent>
#include "xprocess.h"
#include "xshortcutswidget.h"

namespace Ui {
class XProcessMemoryMapWidget;
}

class XProcessMemoryMapWidget : public XShortcutsWidget
{
    Q_OBJECT

    enum HEADER_COLUMN
    {
        HEADER_COLUMN_ADDRESS=0,
        HEADER_COLUMN_SIZE,
        HEADER_COLUMN_FLAGS,
    #ifdef Q_OS_WINDOWS
        HEADER_COLUMN_ALLOCATIONBASE,
        HEADER_COLUMN_ALLOCATIONFLAGS,
        HEADER_COLUMN_STATE,
        HEADER_COLUMN_TYPE,
    #endif
    #ifdef Q_OS_LINUX
        HEADER_COLUMN_OFFSET,
        HEADER_COLUMN_DEVICE,
        HEADER_COLUMN_FILE,
        HEADER_COLUMN_FILENAME,
    #endif
        __HEADER_COLUMN_size
    };

public:
    explicit XProcessMemoryMapWidget(QWidget *pParent=nullptr);
    ~XProcessMemoryMapWidget();

    void setData(qint64 nProcessId);
    void reload();

private:
    void deleteOldModel();

protected:
    virtual void registerShortcuts(bool bState);

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonReload_clicked();

private:
    Ui::XProcessMemoryMapWidget *ui;
    qint64 g_nProcessId;
    QStandardItemModel *g_pModel;
    QStandardItemModel *g_pOldModel;
};

#endif // XPROCESSMEMORYMAPWIDGET_H
