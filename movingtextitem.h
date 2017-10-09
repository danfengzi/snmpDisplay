#ifndef MOVINGTEXTITEM_H
#define MOVINGTEXTITEM_H

#include <QLabel>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QPainter>
#include <QTimer>
#include "characteritem.h"

class MovingTextItem : public QLabel
{
    Q_OBJECT

public:
    MovingTextItem(QString multiString, QWidget *parent = 0);
    ~MovingTextItem();

    int itemWidth;
    int itemHeight;
    int positionX;
    int positionY;
    int startX;
    int endLength;
    int lineNum;
    int lineSpace;

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

private slots:
    void positionRefresh(); //位置刷新

private:
    void multiParsing();    //解析multi字符串
    void positionInit();    //位置初始化


    QString multiStr;
    QString movingType;
    QString movingDirection;
    QString movingText;

    int chSpace;
    int refreshDelay;
    int movingSpeed;
    int refreshTime;
    int delayTimerId;

    QColor foregroundColor;
    QColor backgroundColor;

    QTimer refreshTimer;

    QFont font;

};

#endif // MOVINGTEXTITEM_H
