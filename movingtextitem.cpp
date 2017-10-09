#include <QDebug>
#include "movingtextitem.h"


/***********************************************************************
函数名称 : MovingTextItem
函数功能 : 构造函数
输入参数 : QString multi字符串, QWidget *父窗口
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
MovingTextItem::MovingTextItem(QString multiString, QWidget *parent) : QLabel(parent)
{
    foregroundColor = Qt::red;
    backgroundColor = Qt::black;
    chSpace = 0;
    lineSpace = 0;
    font.setPixelSize(40);
    itemHeight = font.pixelSize();
    endLength = 80;
    lineNum = 1;


    multiStr = multiString;
    multiParsing();

    this->resize(itemWidth, font.pixelSize());

    positionInit();
}


/***********************************************************************
函数名称 : ~MovingTextItem
函数功能 : 析构函数
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
MovingTextItem::~MovingTextItem()
{

}


/***********************************************************************
函数名称 : paintEvent
函数功能 : 绘画事件
输入参数 : QPaintEvent *触发事件
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void MovingTextItem::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QBrush brush(Qt::black);
    painter.setBrush(brush);
    painter.setFont(font);
    painter.drawRect(0, 0, this->width(), this->height());

    painter.setPen(Qt::red);

    if ("c" == movingType)
    {
        for (int i = 0; i < movingText.size(); ++i)
        {
            painter.drawText(startX + i * (font.pixelSize() + chSpace),
                             itemHeight, movingText.at(i));
            painter.drawText(startX + movingText.size() * (font.pixelSize() + chSpace)
                             + endLength + i * (font.pixelSize() + chSpace),
                             itemHeight, movingText.at(i));
        }
    }
    else
    {
        for (int i = 0; i < movingText.size(); ++i)
        {
            painter.drawText(startX + i * (font.pixelSize() + chSpace),
                             itemHeight, movingText.at(i));
        }
    }
}


/***********************************************************************
函数名称 : timerEvent
函数功能 : 定时器事件
输入参数 : QTimerEvent *触发事件
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void MovingTextItem::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == delayTimerId)
    {
        this->killTimer(delayTimerId);
        refreshTimer.start(refreshTime * 10);
        if ("l" == movingDirection)
        {
            startX = 0;
        }
        else
        {
            startX = itemWidth - movingText.size() * (font.pixelSize() + chSpace);
        }
        this->update();
    }
}



/***********************************************************************
函数名称 : multiParsing
函数功能 : multi字符串解析
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void MovingTextItem::multiParsing()
{
    QList<QString> multiList = multiStr.split(']');
    for (int i = 0; i < multiList.size(); ++i)
    {
        QString tempStr = multiList.at(i);
        if (tempStr.contains("[nl"))
        {
            lineNum++;
        }

        else if (tempStr.contains("[cb"))
        {
            tempStr.remove("[cb");
            backgroundColor = Qt::black;
        }

        else if (tempStr.contains("[cf"))
        {
            tempStr.remove("[cf");
            backgroundColor = Qt::red;
        }

        else if (tempStr.contains("[fo"))
        {
            tempStr.remove("fo");
            font.setPixelSize(40);
        }

        else if (tempStr.contains("[sc"))
        {
            tempStr.remove("[sc");
            chSpace = tempStr.toInt();
        }

        else if (tempStr.contains("[mv"))
        {
            tempStr.remove("[mv");
            QList<QString> paraList = tempStr.split(',');
            movingType = paraList.at(0).at(0);
            paraList[0].remove(0, 1);
            if ("l" == movingType)
            {
                int index;
                if (paraList.at(0).contains("r"))
                {
                    index = paraList.at(0).indexOf("r");
                }
                else if (paraList.at(0).indexOf("l"))
                {
                    index = paraList.at(0).indexOf("l");
                }
                else
                {
                    qDebug() << "multi字符串移动文本标签出错" << endl;
                    return;
                }

                if (0 != index)
                {
                    refreshDelay = paraList.at(0).mid(0, index).toInt();
                    paraList[0].remove(0, index);
                }
            }
            movingDirection = paraList.at(0).at(0);
            paraList[0].remove(0, 1);
            itemWidth = paraList.at(0).toInt();
            movingSpeed = paraList.at(1).toInt();
            refreshTime = paraList.at(2).toInt();
            movingText = paraList.at(3);
        }
    }
}


/***********************************************************************
函数名称 : positionInit
函数功能 : 起始位置初始化
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void MovingTextItem::positionInit()
{
    if ("l" == movingDirection)
    {
        startX = 0;
    }
    else
    {
        if ("c" == movingType)
        {
            startX = itemWidth - 2 * movingText.size() * (font.pixelSize() + chSpace)
                    - endLength;
        }
        else
        {
            startX = itemWidth - movingText.size() * (font.pixelSize() + chSpace);
        }
    }
    refreshTimer.start(refreshTime * 10);
    connect(&refreshTimer, SIGNAL(timeout()),
            this, SLOT(positionRefresh()));
}


/***********************************************************************
函数名称 : positionRefresh
函数功能 : 位置刷新
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void MovingTextItem::positionRefresh()
{
    if ("l" == movingDirection)
    {
        if ("c" == movingType)
        {
            if (itemWidth - 1 >= startX + 2 * (movingText.size() * (font.pixelSize() + chSpace)
                                           + endLength))
            {
                startX = itemWidth - movingText.size() * (font.pixelSize() + chSpace)
                        - endLength - 1;
            }
            else
            {
                startX -= movingSpeed;
            }
        }
        else
        {
            if (itemWidth - 1 >= startX + (movingText.size() * (font.pixelSize() + chSpace)))
            {
                if (refreshDelay > 0)
                {
                    refreshTimer.stop();
                    delayTimerId = this->startTimer(refreshDelay * 100);
                }
                else
                {
                    startX = 0;
                }
            }
            else
            {
                startX -= movingSpeed;
            }
        }
    }
    else
    {
        if ("c" == movingType)
        {
            if (startX >= 0)
            {
                startX = -movingText.size() * (font.pixelSize() + chSpace) - endLength;
            }
            else
            {
                startX += movingSpeed;
            }
        }
        else
        {
            if (startX >= 0)
            {
                if (refreshDelay > 0)
                {
                    refreshTimer.stop();
                    delayTimerId = this->startTimer(refreshDelay * 100);
                }
                else
                {
                    startX = itemWidth - movingText.size() * (font.pixelSize() + chSpace);
                }
            }
            else
            {
                startX += movingSpeed;
            }
        }
    }

    this->update();
}
