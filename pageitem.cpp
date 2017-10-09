#include "pageitem.h"


/***********************************************************************
函数名称 : PageItem
函数功能 : 构造函数
输入参数 : QString multi字符串, QWidget *父窗口
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
PageItem::PageItem(QString multiString, QWidget *parent) : QLabel(parent)
{
    pageLast = 30;
    pageClose = 10;
    isVisible = true;
    multiStr = multiString;
    multiParsing();
}


/***********************************************************************
函数名称 : PageItem
函数功能 : 构造函数
输入参数 : QString multi字符串, QPixmap 图片pixmap, QWidget *父窗口
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
PageItem::PageItem(QString multiString, QPixmap picture, QWidget *parent) : QLabel(parent)
{
    pageLast = 30;
    pageClose = 10;
    isVisible = true;
    multiStr = multiString;
    multiParsing();
}


/***********************************************************************
函数名称 : ~PageItem
函数功能 : 析构函数
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
PageItem::~PageItem()
{
    qDebug() << "delete PageItem";
    qDebug() << "TextBoxItem : " << textBoxItems.size();
    for (int i = 0; i < textBoxItems.size(); ++i)
    {
        qDebug() << "start delete textBoxItems[" << i << "]";
        delete textBoxItems[i];
    }
}


/***********************************************************************
函数名称 : paintEvent
函数功能 : 页面显示
输入参数 : QPaintEvent *绘画事件
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void PageItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (isVisible)
    {
        this->show();
    }
    else
    {
        this->hide();
    }
}


/***********************************************************************
函数名称 : multiParsing
函数功能 : 解析multi字符串
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void PageItem::multiParsing()
{
    //取出页面背景色
    if (multiStr.contains("pb"))
    {
        int begin = multiStr.indexOf("[pb");
        int end = multiStr.indexOf("]", begin);
        QString subString = multiStr.mid(begin, end-begin+1);
        multiStr.remove(subString);
    }
    else
    {
        backgroundColor = Qt::black;
    }
    //取出页面持续和关闭时间
    if (multiStr.contains("[pt"))
    {
        int begin = multiStr.indexOf("[pt");
        int end = multiStr.indexOf("]", begin);
        QString subString = multiStr.mid(begin, end-begin+1);
        multiStr.remove(subString);
        subString.remove("[pt");
        subString.remove("]");
        QList<QString> strList = subString.split('o');
        if (0 == strList.size())
        {
            pageLast = 30;
            pageClose = 10;
        }
        else if (1 == strList.size())
        {
            if ("o" == subString.at(0))
            {
                pageLast = 30;
                pageClose = strList.at(0).toInt();
            }
            else if ("o" == subString.at(subString.size()-1))
            {
                pageLast = strList.at(0).toInt();
                pageClose = 10;
            }
            else
            {
                qDebug() << "multi字符串页面时间描述错误";
                return;
            }
        }
        else if (2 == strList.size())
        {
            pageLast = strList.at(0).toInt();
            pageClose = strList.at(1).toInt();
        }
        else
        {
            qDebug() << "multi字符串页面时间描述错误";
            return;
        }
    }

    //取出multi字符串中描述图片的字符串，并新建相应的pictureItem
    while (multiStr.contains("[g"))
    {
        int picBegin = multiStr.indexOf("[g");
        int picEnd = multiStr.indexOf("]", picBegin);
        QString picMultiStr = multiStr.mid(picBegin, picEnd-picBegin+1);
        multiStr.remove(picBegin, picEnd-picBegin+1);
    }

    //取出multi字符串中描述文本框的字符串，根据该字符串新建相应的staticTextItem
    while (multiStr.contains("[tr"))
    {
        int staticBegin = multiStr.indexOf("[tr");
        int staticEnd = multiStr.indexOf("[tr", staticBegin+1);
        QString textMultiStr;
        if (-1 == staticEnd)
        {
            textMultiStr = multiStr.mid(staticBegin, multiStr.size()-staticBegin);
            multiStr.remove(staticBegin, multiStr.size()-staticBegin);
        }
        else
        {
            textMultiStr = multiStr.mid(staticBegin, staticEnd-staticBegin+1);
            multiStr.remove(staticBegin, staticEnd-staticBegin+1);
        }
        TextBoxItem *tempTextBox = new TextBoxItem(textMultiStr, this);
        textBoxItems.append(tempTextBox);
    }

    //最后如果还剩下字符串，则是默认位置的字符串，新建staticTextItem
    if (!("" == multiStr))
    {
        TextBoxItem *tempTextBox = new TextBoxItem(multiStr, this);
        textBoxItems.append(tempTextBox);
    }
}
