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
    chOpenId = -1;
    chCloseId = -1;
    chOpenDelayId = -1;
    chCloseDelayId = -1;
    chHeight = 40;
    chWidth = 40;
    chSpace = 0;
    foregroundColor = Qt::red;
    backgroundColor = Qt::black;
    openBeforeClose = true;
    chFont.setPixelSize(chWidth);
    multiStr = multiString;
    multiParsing();
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
