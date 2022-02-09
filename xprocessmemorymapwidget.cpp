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
#include "xprocessmemorymapwidget.h"
#include "ui_xprocessmemorymapwidget.h"

XProcessMemoryMapWidget::XProcessMemoryMapWidget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::XProcessMemoryMapWidget)
{
    ui->setupUi(this);

    // mb TODO autorefresh

    g_nProcessId=0;
    g_pOldModel=nullptr;
    g_pModel=nullptr;
}

XProcessMemoryMapWidget::~XProcessMemoryMapWidget()
{
    delete ui;
}

void XProcessMemoryMapWidget::setData(qint64 nProcessId)
{
    g_nProcessId=nProcessId;

    reload();
}

void XProcessMemoryMapWidget::reload()
{
#ifdef QT_DEBUG
    qDebug("void XProcessMemoryMapWidget::reload()");
#endif

    if(g_nProcessId)
    {
        g_pOldModel=g_pModel;

        quint64 nMemorySize=0;

#ifdef Q_OS_WINDOWS
        if(sizeof(void *)==8)
        {
            nMemorySize=0x7FFFFFFFFFFFFFFF;
        }
        else
        {
            nMemorySize=0x7FFFFFFF;
        }
#endif
#ifdef Q_OS_LINUX
        if(sizeof(void *)==8)
        {
            nMemorySize=0xFFFFFFFFFFFFFFFF;
        }
        else
        {
            nMemorySize=0xFFFFFFFF;
        }
#endif

        XBinary::MODE modeAddress=XBinary::getWidthModeFromSize(nMemorySize);

        QList<XBinary::MEMORY_REGION> listRegions=XProcess::getMemoryRegionsList(g_nProcessId,0,nMemorySize);

        qint32 nNumberOfRecords=listRegions.count();

        g_pModel=new QStandardItemModel(nNumberOfRecords,__HEADER_COLUMN_size);

        g_pModel->setHeaderData(HEADER_COLUMN_ADDRESS,Qt::Horizontal,tr("Address"));
        g_pModel->setHeaderData(HEADER_COLUMN_SIZE,Qt::Horizontal,tr("Size"));
        g_pModel->setHeaderData(HEADER_COLUMN_FLAGS,Qt::Horizontal,tr("Flags"));
    #ifdef Q_OS_WINDOWS
        g_pModel->setHeaderData(HEADER_COLUMN_ALLOCATIONBASE,Qt::Horizontal,tr("Allocation base"));
        g_pModel->setHeaderData(HEADER_COLUMN_ALLOCATIONFLAGS,Qt::Horizontal,tr("Allocation flags"));
        g_pModel->setHeaderData(HEADER_COLUMN_STATE,Qt::Horizontal,tr("State"));
        g_pModel->setHeaderData(HEADER_COLUMN_TYPE,Qt::Horizontal,tr("Type"));
    #endif
    #ifdef Q_OS_LINUX
        g_pModel->setHeaderData(HEADER_COLUMN_OFFSET,Qt::Horizontal,tr("Offset"));
        g_pModel->setHeaderData(HEADER_COLUMN_DEVICE,Qt::Horizontal,tr("Device"));
        g_pModel->setHeaderData(HEADER_COLUMN_FILE,Qt::Horizontal,tr("File"));
        g_pModel->setHeaderData(HEADER_COLUMN_FILENAME,Qt::Horizontal,tr("File name"));
    #endif

        for(int i=0;i<nNumberOfRecords;i++)
        {
            // TODO rename pType -> pItem
            QStandardItem *pTypeAddress=new QStandardItem;
            pTypeAddress->setText(XBinary::valueToHex(modeAddress,listRegions.at(i).nAddress));
            pTypeAddress->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_ADDRESS,pTypeAddress);

            QStandardItem *pTypeSize=new QStandardItem;
//            pTypeSize->setText(XBinary::valueToHex(XBinary::MODE_32,modeAddress,listRegions.at(i).nSize));
            pTypeSize->setText(XBinary::valueToHex(XBinary::MODE_32,listRegions.at(i).nSize));
            pTypeSize->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_SIZE,pTypeSize);

            QStandardItem *pTypeFlags=new QStandardItem;
            pTypeFlags->setText(XBinary::memoryFlagsToString(listRegions.at(i).mf));
            pTypeFlags->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_FLAGS,pTypeFlags);
        #ifdef Q_OS_WINDOWS
            QStandardItem *pItemAllocationBase=new QStandardItem;
            pItemAllocationBase->setText(XBinary::valueToHex(modeAddress,listRegions.at(i).nAllocationBase));
            pItemAllocationBase->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_ALLOCATIONBASE,pItemAllocationBase);

            QStandardItem *pTypeAllocationFlags=new QStandardItem;
            pTypeAllocationFlags->setText(XBinary::memoryFlagsToString(listRegions.at(i).mfAllocation));
            pTypeAllocationFlags->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_ALLOCATIONFLAGS,pTypeAllocationFlags);

            QStandardItem *pTypeState=new QStandardItem;
            pTypeState->setText(XBinary::valueToHex(XBinary::MODE_32,listRegions.at(i).nState));
            pTypeState->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_STATE,pTypeState);

            QStandardItem *pItemType=new QStandardItem;
            pItemType->setText(XBinary::valueToHex(XBinary::MODE_32,listRegions.at(i).nType));
            pItemType->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_TYPE,pItemType);
        #endif
        #ifdef Q_OS_LINUX
            QStandardItem *pTypeOffset=new QStandardItem;
            pTypeOffset->setText(XBinary::valueToHex(modeAddress,listRegions.at(i).nOffset));
            pTypeOffset->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_OFFSET,pTypeOffset);

            QStandardItem *pTypeDevice=new QStandardItem;
            pTypeDevice->setText(listRegions.at(i).sDevice);
            pTypeDevice->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_DEVICE,pTypeDevice);

            QStandardItem *pTypeFile=new QStandardItem;
            pTypeFile->setText(QString::number(listRegions.at(i).nFile));
            pTypeFile->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_FILE,pTypeFile);

            QStandardItem *pTypeFileName=new QStandardItem;
            pTypeFileName->setText(listRegions.at(i).sFileName);
            pTypeFileName->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            g_pModel->setItem(i,HEADER_COLUMN_FILENAME,pTypeFileName);
        #endif
        }

        ui->tableViewMemoryMap->setModel(g_pModel);

        #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            QFuture<void> future=QtConcurrent::run(&XProcessMemoryMapWidget::deleteOldModel,this);
        #else
            QFuture<void> future=QtConcurrent::run(this,&XProcessMemoryMapWidget::deleteOldModel);
        #endif

        // TODO get all modules; compare names;
        // TODO
        // TODO save scrollbar position
    }
}

void XProcessMemoryMapWidget::deleteOldModel()
{
    if(g_pOldModel)
    {
        delete g_pOldModel;

        g_pOldModel=0;
    }
}

void XProcessMemoryMapWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void XProcessMemoryMapWidget::on_pushButtonSave_clicked()
{
    if(g_pModel)
    {
        XShortcutsWidget::saveModel(g_pModel,QString("%1.txt").arg(tr("Memory map")));
    }
}

void XProcessMemoryMapWidget::on_pushButtonReload_clicked()
{
    reload();
}
