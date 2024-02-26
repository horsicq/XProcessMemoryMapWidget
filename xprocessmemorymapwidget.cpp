/* Copyright (c) 2021-2024 hors<horsicq@gmail.com>
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
#include "xprocessmemorymapwidget.h"

#include "ui_xprocessmemorymapwidget.h"

XProcessMemoryMapWidget::XProcessMemoryMapWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XProcessMemoryMapWidget)
{
    ui->setupUi(this);

    // mb TODO autorefresh

    g_nProcessId = 0;
    g_pXInfoDB = nullptr;
    g_pOldModel = nullptr;
    g_pModel = nullptr;

    memset(g_shortCuts, 0, sizeof g_shortCuts);
}

XProcessMemoryMapWidget::~XProcessMemoryMapWidget()
{
    delete ui;
}

void XProcessMemoryMapWidget::setData(qint64 nProcessId, bool bReload)
{
    g_nProcessId = nProcessId;

    if (bReload) {
        reload();
    }
}

void XProcessMemoryMapWidget::setXInfoDB(XInfoDB *pXInfoDB, bool bReload)
{
    g_pXInfoDB = pXInfoDB;

    if (bReload) {
        reload();
    }
}

void XProcessMemoryMapWidget::reload()
{
#ifdef QT_DEBUG
    qDebug("void XProcessMemoryMapWidget::reload()");
#endif

    quint64 nMemorySize = 0;

    // mb TODO a function
#ifdef Q_OS_WIN
    if (sizeof(void *) == 8) {
        nMemorySize = 0x7FFFFFFFFFFFFFFF;
    } else {
        nMemorySize = 0x7FFFFFFF;
    }
#else
    if (sizeof(void *) == 8) {
        nMemorySize = 0xFFFFFFFFFFFFFFFF;
    } else {
        nMemorySize = 0xFFFFFFFF;
    }
#endif

    QList<XProcess::MEMORY_REGION> listMemoryRegions;
    QList<XProcess::MODULE> listModules;

    QList<XProcess::MODULE> *pListModules = nullptr;
    QList<XProcess::MEMORY_REGION> *pListMemoryRegions = nullptr;

    if (g_nProcessId) {
        // TODO getCurrent...
        listMemoryRegions = XProcess::getMemoryRegionsList_Id(g_nProcessId, 0, nMemorySize);
        listModules = XProcess::getModulesList(g_nProcessId);

        pListModules = &listModules;
        pListMemoryRegions = &listMemoryRegions;
    } else if (g_pXInfoDB) {
        pListMemoryRegions = g_pXInfoDB->getCurrentMemoryRegionsList();
        pListModules = g_pXInfoDB->getCurrentModulesList();
    }

    if (g_nProcessId || g_pXInfoDB) {
        g_pOldModel = g_pModel;

        XBinary::MODE modeAddress = XBinary::getWidthModeFromSize(nMemorySize);

        qint32 nNumberOfRecords = pListMemoryRegions->count();

        g_pModel = new QStandardItemModel(nNumberOfRecords, __HEADER_COLUMN_size);

        g_pModel->setHeaderData(HEADER_COLUMN_ADDRESS, Qt::Horizontal, tr("Address"));
        g_pModel->setHeaderData(HEADER_COLUMN_SIZE, Qt::Horizontal, tr("Size"));
        g_pModel->setHeaderData(HEADER_COLUMN_FLAGS, Qt::Horizontal, tr("Flags"));
#ifdef Q_OS_WIN
        g_pModel->setHeaderData(HEADER_COLUMN_ALLOCATIONBASE, Qt::Horizontal, tr("Allocation base"));
        g_pModel->setHeaderData(HEADER_COLUMN_ALLOCATIONFLAGS, Qt::Horizontal, tr("Allocation flags"));
        g_pModel->setHeaderData(HEADER_COLUMN_STATE, Qt::Horizontal, tr("State"));
        g_pModel->setHeaderData(HEADER_COLUMN_TYPE, Qt::Horizontal, tr("Type"));
#endif
#ifdef Q_OS_LINUX
        g_pModel->setHeaderData(HEADER_COLUMN_OFFSET, Qt::Horizontal, tr("Offset"));
        g_pModel->setHeaderData(HEADER_COLUMN_DEVICE, Qt::Horizontal, tr("Device"));
        g_pModel->setHeaderData(HEADER_COLUMN_FILE, Qt::Horizontal, tr("File"));
#endif
        g_pModel->setHeaderData(HEADER_COLUMN_MODULE, Qt::Horizontal, tr("Module"));
        g_pModel->setHeaderData(HEADER_COLUMN_REGION, Qt::Horizontal, tr("Region"));
        g_pModel->setHeaderData(HEADER_COLUMN_FILENAME, Qt::Horizontal, tr("File name"));

        quint64 nCurrentBase = -1;
        XBinary::_MEMORY_MAP memoryMap = {};

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            QStandardItem *pItemAddress = new QStandardItem;
            pItemAddress->setText(XBinary::valueToHex(modeAddress, pListMemoryRegions->at(i).nAddress));
            pItemAddress->setData(pListMemoryRegions->at(i).nAddress, Qt::UserRole + USERROLE_ADDRESS);
            pItemAddress->setData(pListMemoryRegions->at(i).nSize, Qt::UserRole + USERROLE_SIZE);
            pItemAddress->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            g_pModel->setItem(i, HEADER_COLUMN_ADDRESS, pItemAddress);

            QStandardItem *pItemSize = new QStandardItem;
            //            pTypeSize->setText(XBinary::valueToHex(XBinary::MODE_32,modeAddress,listRegions.at(i).nSize));
            pItemSize->setText(XBinary::valueToHex(XBinary::MODE_32, pListMemoryRegions->at(i).nSize));
            pItemSize->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            g_pModel->setItem(i, HEADER_COLUMN_SIZE, pItemSize);

            QStandardItem *pItemFlags = new QStandardItem;
            pItemFlags->setText(XProcess::memoryFlagsToString(pListMemoryRegions->at(i).mf));
            pItemFlags->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
            g_pModel->setItem(i, HEADER_COLUMN_FLAGS, pItemFlags);
#ifdef Q_OS_WIN
            QStandardItem *pItemAllocationBase = new QStandardItem;
            pItemAllocationBase->setText(XBinary::valueToHex(modeAddress, pListMemoryRegions->at(i).nAllocationBase));
            pItemAllocationBase->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            g_pModel->setItem(i, HEADER_COLUMN_ALLOCATIONBASE, pItemAllocationBase);

            QStandardItem *pItemAllocationFlags = new QStandardItem;
            pItemAllocationFlags->setText(XProcess::memoryFlagsToString(pListMemoryRegions->at(i).mfAllocation));
            pItemAllocationFlags->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
            g_pModel->setItem(i, HEADER_COLUMN_ALLOCATIONFLAGS, pItemAllocationFlags);

            QStandardItem *pItemState = new QStandardItem;
            pItemState->setText(XBinary::valueToHex(XBinary::MODE_32, pListMemoryRegions->at(i).nState));
            pItemState->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            g_pModel->setItem(i, HEADER_COLUMN_STATE, pItemState);

            QStandardItem *pItemType = new QStandardItem;
            pItemType->setText(XBinary::valueToHex(XBinary::MODE_32, pListMemoryRegions->at(i).nType));
            pItemType->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            g_pModel->setItem(i, HEADER_COLUMN_TYPE, pItemType);
#endif
#ifdef Q_OS_LINUX
            QStandardItem *pItemOffset = new QStandardItem;
            pItemOffset->setText(XBinary::valueToHex(modeAddress, pListMemoryRegions->at(i).nOffset));
            pItemOffset->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            g_pModel->setItem(i, HEADER_COLUMN_OFFSET, pItemOffset);

            QStandardItem *pItemDevice = new QStandardItem;
            pItemDevice->setText(pListMemoryRegions->at(i).sDevice);
            pItemDevice->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            g_pModel->setItem(i, HEADER_COLUMN_DEVICE, pItemDevice);

            QStandardItem *pItemFile = new QStandardItem;
            pItemFile->setText(QString::number(pListMemoryRegions->at(i).nFile));
            pItemFile->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            g_pModel->setItem(i, HEADER_COLUMN_FILE, pItemFile);
#endif

            XADDR nCurrentAddress = pListMemoryRegions->at(i).nAddress;

            XProcess::MODULE module = XProcess::getModuleByAddress(pListModules, nCurrentAddress);

            if ((module.nSize) && (module.sFileName != "")) {
                if (nCurrentBase != module.nAddress) {
                    nCurrentBase = module.nAddress;
                    memoryMap = XFormats::getMemoryMap(module.sFileName, XBinary::MAPMODE_UNKNOWN, false, module.nAddress);
                }

                XADDR nModuleAddress = pListMemoryRegions->at(i).nAddress;

                XBinary::_MEMORY_RECORD memoryRecord = XBinary::getMemoryRecordByAddress(&memoryMap, nModuleAddress);

                QStandardItem *pItemModule = new QStandardItem;
                pItemModule->setText(module.sName);
                pItemModule->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                g_pModel->setItem(i, HEADER_COLUMN_MODULE, pItemModule);

                QStandardItem *pItemRegion = new QStandardItem;
                pItemRegion->setText(memoryRecord.sName);
                pItemRegion->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                g_pModel->setItem(i, HEADER_COLUMN_REGION, pItemRegion);

                QStandardItem *pItemFileName = new QStandardItem;
                pItemFileName->setText(module.sFileName);
                pItemFileName->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                g_pModel->setItem(i, HEADER_COLUMN_FILENAME, pItemFileName);

                g_pModel->item(i, HEADER_COLUMN_ADDRESS)->setData(module.sFileName, Qt::UserRole + USERROLE_FILENAME);
            } else {
#ifdef Q_OS_LINUX
                QStandardItem *pItemModule = new QStandardItem;
                pItemModule->setText(pListMemoryRegions->at(i).sFileName);
                pItemModule->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                g_pModel->setItem(i, HEADER_COLUMN_MODULE, pItemModule);
#endif
            }
        }

        ui->tableViewMemoryMap->setModel(g_pModel);

        deleteOldStandardModel(&g_pOldModel);

        // TODO get all modules; compare names;
        // TODO
        // TODO save scrollbar position
    }
}

void XProcessMemoryMapWidget::registerShortcuts(bool bState)
{
    if (bState) {
        if (!g_shortCuts[SC_DUMPTOFILE])
            g_shortCuts[SC_DUMPTOFILE] = new QShortcut(getShortcuts()->getShortcut(X_ID_MEMORYMAP_DUMPTOFILE), this, SLOT(_dumpToFileSlot()));
        if (!g_shortCuts[SC_SHOWIN_FOLDER])
            g_shortCuts[SC_SHOWIN_FOLDER] = new QShortcut(getShortcuts()->getShortcut(X_ID_MEMORYMAP_SHOWIN_FOLDER), this, SLOT(_showInFolderSlot()));
    } else {
        for (qint32 i = 0; i < __SC_SIZE; i++) {
            if (g_shortCuts[i]) {
                delete g_shortCuts[i];
                g_shortCuts[i] = nullptr;
            }
        }
    }
}

void XProcessMemoryMapWidget::on_pushButtonSave_clicked()
{
    if (g_pModel) {
        XShortcutsWidget::saveTableModel(g_pModel, QString("%1.txt").arg(tr("Memory map")));
    }
}

void XProcessMemoryMapWidget::on_pushButtonReload_clicked()
{
    reload();
}

void XProcessMemoryMapWidget::on_tableViewMemoryMap_customContextMenuRequested(const QPoint &pos)
{
    QMenu menuContext(this);

    QMenu menuShowIn(tr("Show in"), this);  // TODO Show only if not empty

    QAction actionDumpToFile(tr("Dump to file"), this);
    actionDumpToFile.setShortcut(getShortcuts()->getShortcut(X_ID_MEMORYMAP_DUMPTOFILE));
    connect(&actionDumpToFile, SIGNAL(triggered()), this, SLOT(_dumpToFileSlot()));

    QAction actionShowInFolder(tr("Folder"), this);
    actionShowInFolder.setShortcut(getShortcuts()->getShortcut(X_ID_MODULES_SHOWIN_FOLDER));
    connect(&actionShowInFolder, SIGNAL(triggered()), this, SLOT(_showInFolderSlot()));

    menuContext.addAction(&actionDumpToFile);

    menuShowIn.addAction(&actionShowInFolder);
    menuContext.addMenu(&menuShowIn);

    menuContext.exec(ui->tableViewMemoryMap->viewport()->mapToGlobal(pos));
}

void XProcessMemoryMapWidget::_dumpToFileSlot()
{
    QString sSaveFileName = QString("%1.bin").arg(tr("Dump"));
    QString sFileName = QFileDialog::getSaveFileName(this, tr("Save dump"), sSaveFileName, QString("%1 (*.bin)").arg(tr("Raw data")));

    if (!sFileName.isEmpty()) {
        qint32 nRow = ui->tableViewMemoryMap->currentIndex().row();

        if ((nRow != -1) && (g_pModel)) {
            QModelIndex index = ui->tableViewMemoryMap->selectionModel()->selectedIndexes().at(0);

            quint64 nAddress = ui->tableViewMemoryMap->model()->data(index, Qt::UserRole + USERROLE_ADDRESS).toString().toULongLong(0, 16);
            quint64 nSize = ui->tableViewMemoryMap->model()->data(index, Qt::UserRole + USERROLE_SIZE).toString().toULongLong(0, 16);

            XProcess pd(g_nProcessId, nAddress, nSize);

            if (pd.open(QIODevice::ReadOnly)) {
                DialogDumpProcess dd(this);
                dd.setData(&pd, 0, nSize, sFileName, DumpProcess::DT_DUMP_DEVICE_OFFSET);

                dd.showDialogDelay();

                pd.close();
            }
        }
    }
}

void XProcessMemoryMapWidget::_showInFolderSlot()
{
    qint32 nRow = ui->tableViewMemoryMap->currentIndex().row();

    if ((nRow != -1) && (g_pModel)) {
        QModelIndex index = ui->tableViewMemoryMap->selectionModel()->selectedIndexes().at(0);

        QString sFilePath = ui->tableViewMemoryMap->model()->data(index, Qt::UserRole + USERROLE_FILENAME).toString();

        XOptions::showInFolder(sFilePath);
    }
}
