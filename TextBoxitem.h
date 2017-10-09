#ifndef TEXTBOXITEM_H
#define TEXTBOXITEM_H

#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QDebug>
#include <QPixmap>
#include <QRGB>
#include <QImage>
#include <QVector>
#include "characteritem.h"
#include "movingtextitem.h"


class TextBoxItem : public QLabel
{
    Q_OBJECT

public:
    TextBoxItem(QString multiString, QWidget *parent = 0);
    ~TextBoxItem();

    int positionX;
    int positionY;
    int width;
    int height;

protected:
    void paintEvent(QPaintEvent *event);

private:
    void chPositioning();            //用于定位每个字在窗口中的位置
    void multiParsing();    //解析multi字符串

    int chOffset;
    int startX;
    int tempStartY;
    int tempLineLenth;
    int tempLineHeight;
    int totalHeight;
    int tempLineSpace;

    QString multiStr;

    int justificationLine;   //行对齐参数
    int justificationPage;   //页面对齐参数

    QColor backgroundColor;  //背景颜色

    struct ItemPosition
    {
        ItemPosition(QString type, int index)
        {
            this->type = type;
            this->index = index;
        }
        QString type;
        int index;
    };

    QVector<int> lineSpace;
    QVector<int> lineHeight;
    QVector<int> lineLength;
    QVector<int> startY;
    QVector<CharacterItem *> characterItems;
    QVector<MovingTextItem *> movingTextItems;
    QMap<int, ItemPosition *> mixedItems;
};

#endif // TEXTBOXITEM_H
