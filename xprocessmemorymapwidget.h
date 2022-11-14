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

#include <QFuture>
#include <QMenu>
#include <QStandardItemModel>
#include <QWidget>
#include <QtConcurrent>

#include "dialogdumpprocess.h"
#include "xformats.h"
#include "xinfodb.h"
#include "xprocess.h"
#include "xshortcutswidget.h"

namespace Ui {
class XProcessMemoryMapWidget;
}

class XProcessMemoryMapWidget : public XShortcutsWidget {
    Q_OBJECT

    enum HEADER_COLUMN {
        HEADER_COLUMN_ADDRESS = 0,
        HEADER_COLUMN_SIZE,
        HEADER_COLUMN_FLAGS,
#ifdef Q_OS_WIN
        HEADER_COLUMN_ALLOCATIONBASE,
        HEADER_COLUMN_ALLOCATIONFLAGS,
        HEADER_COLUMN_STATE,
        HEADER_COLUMN_TYPE,
#endif
#ifdef Q_OS_LINUX
        HEADER_COLUMN_OFFSET,
        HEADER_COLUMN_DEVICE,
        HEADER_COLUMN_FILE,
#endif
        HEADER_COLUMN_MODULE,
        HEADER_COLUMN_REGION,
        HEADER_COLUMN_FILENAME,
        __HEADER_COLUMN_size
    };

    enum SC {
        SC_DUMPTOFILE = 0,
        SC_SHOWIN_FOLDER,
        __SC_SIZE
    };

    enum USERROLE {
        USERROLE_SIZE = 0,
        USERROLE_ADDRESS,
        USERROLE_FILENAME
    };

public:
    explicit XProcessMemoryMapWidget(QWidget *pParent = nullptr);
    ~XProcessMemoryMapWidget();

    void setData(qint64 nProcessId, bool bReload = true);
    void setXInfoDB(XInfoDB *pXInfoDB, bool bReload = true);
    void reload();

protected:
    virtual void registerShortcuts(bool bState);

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonReload_clicked();
    void on_tableViewMemoryMap_customContextMenuRequested(const QPoint &pos);

protected slots:
    void _dumpToFileSlot();
    void _showInFolderSlot();

private:
    Ui::XProcessMemoryMapWidget *ui;
    qint64 g_nProcessId;
    XInfoDB *g_pXInfoDB;
    QStandardItemModel *g_pModel;
    QStandardItemModel *g_pOldModel;
    QShortcut *g_shortCuts[__SC_SIZE];
};

#endif  // XPROCESSMEMORYMAPWIDGET_H
