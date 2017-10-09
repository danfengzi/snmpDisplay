#include "characteritem.h"
#include <QStringList>
#include <QDebug>


/***********************************************************************
函数名称 : CharacterItem
函数功能 : 构造函数
输入参数 : QString multi字符串
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
CharacterItem::CharacterItem(QString multiString, QWidget *parent) : QLabel(parent)
{
    positionX = 0;
    positionY = 0;
    isVisiable = true;
    flashLast = 0;
    flashClose = 0;
    lineNum = 1;
    lineSpace = 0;
    chHeight = 40;
    chWidth = 40;
    chSpace = 0;
    foregroundColor = Qt::red;
    backgroundColor = Qt::black;
    openBeforeClose = true;
    chFont.setPixelSize(chWidth);
    multiStr = multiString;
    multiParsing();

    this->resize(chWidth, chHeight);

    timerInit();
    this->show();
}


/***********************************************************************
函数名称 : ~CharacterItem
函数功能 : 析构函数
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
CharacterItem::~CharacterItem()
{

}


/***********************************************************************
函数名称 : paintEvent
函数功能 : 绘画事件
输入参数 : QPaintEvent *触发事件
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void CharacterItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QBrush brush(backgroundColor);
    painter.setBrush(brush);

    painter.drawRect(0, 0, this->width(), this->height());

    if (isVisiable)
    {
        painter.setPen(foregroundColor);
        painter.setFont(chFont);
        painter.drawText(0, this->height(), curCh);
    }
}


/***********************************************************************
函数名称 : chOpenTimer_timeOut
函数功能 : 字符显示定时器时间到
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void CharacterItem::chOpenTimer_timeOut()
{
    isVisiable = true;
    this->update();
}


/***********************************************************************
函数名称 : chCloseTimer_timeOut
函数功能 : 字符关闭定时器时间到
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void CharacterItem::chCloseTimer_timeOut()
{
    isVisiable = false;
    this->update();
}


/***********************************************************************
函数名称 : chOpenDelayTimer_timeOut
函数功能 : 字符打开延时定时器时间到
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void CharacterItem::chOpenDelayTimer_timeOut()
{
    chOpenDelayTimer.stop();
    chOpenTimer.start((flashLast + flashClose) * 100);
    connect(&chOpenTimer, SIGNAL(timeout()),
            this, SLOT(chOpenTimer_timeOut()));
    isVisiable = true;
    this->update();
}


/***********************************************************************
函数名称 : chCloseDelayTimer_timeOut
函数功能 : 字符关闭延时定时器时间到
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void CharacterItem::chCloseDelayTimer_timeOut()
{
    chCloseDelayTimer.stop();
    chCloseTimer.start((flashLast + flashClose) * 100);
    connect(&chCloseTimer, SIGNAL(timeout()),
            this, SLOT(chCloseTimer_timeOut()));
    isVisiable = false;
    this->update();
}


/***********************************************************************
函数名称 : multiParsing
函数功能 : 解析multi字符串
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void CharacterItem::multiParsing()
{
    curCh = multiStr.at(multiStr.size() - 1);
    multiStr.remove(multiStr.size() - 1, 1);

    QList<QString> multiList = multiStr.split(']');
    multiList.removeLast();
    for (int i = 0; i < multiList.size(); ++i)
    {
        if (multiList.at(i).contains("[nl"))
        {
            lineNum++;
        }

        else if (multiList.at(i).contains("[cb"))
        {
            backgroundColor = Qt::black;
        }

        else if (multiList.at(i).contains("[cf"))
        {
            foregroundColor = Qt::red;
        }

        else if (multiList.at(i).contains("[fo"))
        {

        }

        else if (multiList.at(i).contains("[sc"))
        {
            multiList[i].remove("[sc");
            chSpace = multiList[i].toInt();
        }

        else if (multiList.at(i).contains("[fl"))
        {
            multiList[i].remove("[fl");
            if ("t" == multiList.at(i).at(0))
            {
                openBeforeClose = true;
                multiList[i].remove(0, 1);
                QList<QString> timeList = multiList.at(i).split('o');
                flashLast = timeList.at(0).toInt();
                flashClose = timeList.at(1).toInt();
            }
            else if ("o" == multiList.at(i).at(0))
            {
                openBeforeClose = false;
                multiList[i].remove(0, 1);
                QList<QString> timeList = multiList.at(i).split('t');
                flashLast = timeList.at(1).toInt();
                flashClose = timeList.at(0).toInt();
            }
            else
            {
                qDebug() << "multi语言闪烁时间出错";
            }
        }
    }
}


/***********************************************************************
函数名称 : timerInit
函数功能 : 定时器初始化
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void CharacterItem::timerInit()
{
    if (openBeforeClose)
    {
        if ((flashLast > 0) && (flashClose > 0))
        {
            chOpenTimer.start((flashLast + flashLast) * 100);
            connect(&chOpenTimer, SIGNAL(timeout()),
                    this, SLOT(chOpenTimer_timeOut()));
            chCloseDelayTimer.start(flashLast * 100);
            connect(&chCloseDelayTimer, SIGNAL(timeout()),
                    this, SLOT(chCloseDelayTimer_timeOut()));
            isVisiable = true;
        }
        else if (0 == flashClose)
        {
            isVisiable = true;
        }
        else
        {
            isVisiable = false;
        }
    }
    else
    {
        if ((flashLast > 0) && (flashClose > 0))
        {
            chCloseTimer.start((flashLast + flashLast) * 100);
            connect(&chCloseTimer, SIGNAL(timeout()),
                    this, SLOT(chCloseTimer_timeOut()));
            chOpenDelayTimer.start(flashClose * 100);
            connect(&chOpenDelayTimer, SIGNAL(timeout()),
                    this, SLOT(chOpenDelayTimer_timeOut()));
            isVisiable = false;
        }
        else if (0 == flashLast)
        {
            isVisiable = false;
        }
        else
        {
            isVisiable = true;
        }
    }
}
