#ifndef CHARACTERITEM_H
#define CHARACTERITEM_H

#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QColor>
#include <QLabel>


class CharacterItem : public QLabel
{
    Q_OBJECT

public:
    CharacterItem(QString multiString, QWidget *parent = 0);
    ~CharacterItem();

    int flashLast;   //闪烁持续时间
    int flashClose;  //闪烁关闭时间
    int chOpenId;    //文字显示定时器Id
    int chCloseId;   //文字关闭定时器Id
    int chOpenDelayId;  //文字打开延时定时器Id
    int chCloseDelayId; //文字关闭延时定时器Id
    int positionX;   //文字横坐标
    int positionY;   //文字纵坐标
    int chHeight;    //文字高度
    int chWidth;     //文字宽度
    int chSpace;     //文字间间隔
    int lineNum;     //文字所在页面行号
    int lineSpace;   //文字行间隔
    QString curCh;   //文字文本
    bool isVisiable; //文字是否可见
    bool openBeforeClose; //文字是否先显示

    QFont chFont;  //字体

    QColor foregroundColor;  //前景颜色
    QColor backgroundColor;  //背景颜色

    QTimer chOpenTimer;
    QTimer chCloseTimer;
    QTimer chOpenDelayTimer;
    QTimer chCloseDelayTimer;

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void chOpenTimer_timeOut();
    void chCloseTimer_timeOut();
    void chOpenDelayTimer_timeOut();
    void chCloseDelayTimer_timeOut();

private:
    void multiParsing();

    void timerInit();

    QString multiStr;
};

#endif // CHARACTERITEM_H
