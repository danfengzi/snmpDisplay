#ifndef PAGEITEM_H
#define PAGEITEM_H

#include <QLabel>
#include "textboxitem.h"

class PageItem : public QLabel
{
    Q_OBJECT
public:
    PageItem(QString multiString, QWidget *parent = 0);
    PageItem(QString multiString, QPixmap picture, QWidget *parent = 0);
    ~PageItem();

    int pageLast;            //页面显示持续时间
    int pageClose;           //页面关闭持续时间
    int pageCloseId;         //用于计算页面关闭的定时器的id
    int pageDelayId;         //用于打开页面关闭定时器的定时器的id
    int pageChangeId;        //换页定时器的id

    QColor backgroundColor;     //存放背景颜色

    bool isVisible;   //是否可见

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    void multiParsing();   //multi字符串解析

    QString multiStr;



    QVector<TextBoxItem *> textBoxItems;
};

#endif // PAGEITEM_H
