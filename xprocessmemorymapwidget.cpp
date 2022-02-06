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
    QWidget(pParent),
    ui(new Ui::XProcessMemoryMapWidget)
{
    ui->setupUi(this);

    // mb TODO autorefresh

    g_nProcessId=0;
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
        quint64 nMemorySize=0;

        if(sizeof(void *)==8)
        {
            nMemorySize=0x7FFFFFFFFFFFFFFF;
        }
        else
        {
            nMemorySize=0x7FFFFFFF;
        }

        XBinary::MODE modeAddress=XBinary::getWidthModeFromSize(nMemorySize);

        QList<XBinary::MEMORY_REGION> listRegions=XProcess::getMemoryRegionsList(g_nProcessId,0,nMemorySize);

        qint32 nNumberOfRecords=listRegions.count();

        QStandardItemModel *pModel=new QStandardItemModel(nNumberOfRecords,__HEADER_COLUMN_size);

        pModel->setHeaderData(HEADER_COLUMN_ADDRESS,Qt::Horizontal,tr("Address"));
        pModel->setHeaderData(HEADER_COLUMN_SIZE,Qt::Horizontal,tr("Size"));
        pModel->setHeaderData(HEADER_COLUMN_FLAGS,Qt::Horizontal,tr("Flags"));

        for(int i=0;i<nNumberOfRecords;i++)
        {
            QStandardItem *pTypeAddress=new QStandardItem;
            pTypeAddress->setText(XBinary::valueToHex(modeAddress,listRegions.at(i).nAddress));
            pTypeAddress->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            pModel->setItem(i,HEADER_COLUMN_ADDRESS,pTypeAddress);

            QStandardItem *pTypeSize=new QStandardItem;
//            pTypeSize->setText(XBinary::valueToHex(XBinary::MODE_32,modeAddress,listRegions.at(i).nSize));
            pTypeSize->setText(XBinary::valueToHex(XBinary::MODE_32,listRegions.at(i).nSize));
            pTypeSize->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
            pModel->setItem(i,HEADER_COLUMN_SIZE,pTypeSize);

            QStandardItem *pTypeFlags=new QStandardItem;
            // TODO flags to text
            pTypeFlags->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
            pModel->setItem(i,HEADER_COLUMN_FLAGS,pTypeFlags);
        }

        ui->tableViewMemoryMap->setModel(pModel);

//        #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
//            QFuture<void> future=QtConcurrent::run(&SearchStringsWidget::deleteOldModel,this);
//        #else
//            QFuture<void> future=QtConcurrent::run(this,&SearchStringsWidget::deleteOldModel);
//        #endif

        // TODO get all modules; compare names;
        // TODO
        // TODO save scrollbar position
    }
}
