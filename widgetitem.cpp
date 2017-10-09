#include "widgetitem.h"
#include <QDataStream>
#include <QBuffer>
#include <QImageReader>


/***********************************************************************
函数名称 : WidgetItem
函数功能 : 构造函数
输入参数 : QString multi字符串, QWidget *父窗口
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
WidgetItem::WidgetItem(QString multiString, QWidget *parent) : QWidget(parent)
{
    this->resize(800, 600);
    multiStr = multiString;
    startPlay();
/*
    hasReadHead = false;
    dataSize = 0;
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any, 60001);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));*/
}


/***********************************************************************
函数名称 : ~WidgetItem
函数功能 : 析构函数
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
WidgetItem::~WidgetItem()
{
    qDebug() << "delete WidgetItem";
    qDebug() << "PageItem : " << pageItems.size();
    for (int i = 0; i < pageItems.size(); ++i)
    {
        qDebug() << "start delete pageItems[" << i << "]";
        delete pageItems[i];
    }
}


/***********************************************************************
函数名称 : timerEvent
函数功能 : 定时器事件
输入参数 : QTimerEvent *定时器事件
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void WidgetItem::timerEvent(QTimerEvent *event)
{
    for (int i = 0; i < pageItems.size(); ++i)
    {
        if (event->timerId() == pageItems[i]->pageChangeId)
        {
            if (i == pageItems.size() - 1)
            {
                this->killTimer(pageItems[i]->pageCloseId);
                this->killTimer(pageItems[i]->pageChangeId);
                pageItems[i]->pageCloseId = -1;
                pageItems[i]->pageChangeId = -1;
                pageItems[0]->pageCloseId = this->startTimer((pageItems[0]->pageLast) * 100);
                pageItems[0]->pageChangeId = this->startTimer((pageItems[0]->pageLast
                                                             + pageItems[0]->pageClose) * 100);

                pageItems[i]->isVisible = false;
                pageItems[i]->update();
                pageItems[0]->isVisible = true;
                pageItems[0]->show();
                return;
            }
            else
            {
                this->killTimer(pageItems[i]->pageCloseId);
                this->killTimer(pageItems[i]->pageChangeId);
                pageItems[i]->pageCloseId = -1;
                pageItems[i]->pageChangeId = -1;
                pageItems[i+1]->pageCloseId = this->startTimer((pageItems[i+1]->pageLast) * 100);
                pageItems[i+1]->pageChangeId = this->startTimer((pageItems[i+1]->pageLast
                                                             + pageItems[i+1]->pageClose) * 100);
                pageItems[i]->isVisible = false;
                pageItems[i]->update();
                pageItems[i+1]->isVisible = true;
                pageItems[i+1]->show();
                return;
            }
        }
        else if (event->timerId() == pageItems[i]->pageCloseId)
        {
            pageItems[i]->isVisible = false;
            pageItems[i]->update();

            QPalette pal(this->palette());
            pal.setColor(QPalette::Background, pageItems[i]->backgroundColor);
            this->setAutoFillBackground(true);
            this->setPalette(pal);
            return;
        }
    }
    QWidget::timerEvent(event);
}


/***********************************************************************
函数名称 : multiParsing
函数功能 : 解析multi字符串
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void WidgetItem::multiParsing()
{
    QList<QString> pageMultiList = multiStr.split("[np]");
    for (int i = 0; i < pageMultiList.size(); ++i)
    {
        PageItem *tempPage = new PageItem(pageMultiList.at(i), this);
        tempPage->hide();
        pageItems.append(tempPage);
    }
}


/***********************************************************************
函数名称 : startPlay
函数功能 : 开始播放
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void WidgetItem::startPlay()
{
    multiParsing();

    for (int i = 0; i < pageItems.size(); ++i)
    {
        pageItems[i]->setGeometry(0, 0, 600, 600);
    }

    if (pageItems.size() > 1)
    {

        pageItems[0]->pageCloseId = this->startTimer((pageItems[0]->pageLast) * 100);
        pageItems[0]->pageChangeId = this->startTimer((pageItems[0]->pageLast
                                                     + pageItems[0]->pageClose) * 100);
    }
    pageItems[0]->show();
}


/***********************************************************************
函数名称 : newConnection
函数功能 : 有新的连接进来
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void WidgetItem::newConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    qDebug() << "有新的连接";
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
}


/***********************************************************************
函数名称 : receiveData
函数功能 : 接收到数据
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void WidgetItem::receiveData()
{
    qDebug() << "receive data";
/*
    if (!hasReadHead)
    {
        imageData.clear();
        QByteArray temp = tcpSocket->readAll();
        int picLength = temp.left(4).toLong();
        temp.remove(0, 4);
        imageData.append(temp);
        hasReadHead = true;
        qDebug() << picLength << imageData.size();
        if (picLength <= imageData.size())
        {
            QImage image = QImage::fromData(imageData, "png");
            QLabel *label = new QLabel(this);
            label->setGeometry(0, 0, 800, 600);
            QPixmap pic;
            pic.convertFromImage(image);
            label->setPixmap(pic);
            label->show();
            hasReadHead = false;
        }
    }*/

    if (0 == dataSize)
    {
        QDataStream in(tcpSocket);
        if (tcpSocket->bytesAvailable() < sizeof(quint32))
        {
            return;
        }
        in >> dataSize;
    }
    if (dataSize > tcpSocket->bytesAvailable())
    {
        qDebug() << "dataSize > tcpSocket->bytesAvailable()";
        return;
    }
    qDebug() << dataSize << tcpSocket->bytesAvailable();
    QByteArray array = tcpSocket->read(dataSize);
    QBuffer buffer(&array);

    buffer.open(QIODevice::ReadOnly);
    QImageReader reader(&buffer, "PNG");
    QImage image = reader.read();
    if (!image.isNull())
    {
        qDebug() << "image exist";
        QLabel *label = new QLabel(this);
        this->show();
        label->setGeometry(0, 0, 800, 600);
        QPixmap pic;
        pic.convertFromImage(image);
        label->setPixmap(pic);
        label->show();
    }
    else
    {
        qDebug() << "error";
    }

}
