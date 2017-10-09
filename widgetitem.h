#ifndef WIDGETITEM_H
#define WIDGETITEM_H

#include <QWidget>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include "pageitem.h"

class WidgetItem : public QWidget
{
    Q_OBJECT
public:
    WidgetItem(QString multiString, QWidget *parent = 0);
    ~WidgetItem();

signals:

public slots:

protected:
    void timerEvent(QTimerEvent *event);

private:
    void multiParsing();   //multi字符串解析
    void startPlay();    //开始播放

    QString multiStr;

    QVector<PageItem *> pageItems;

    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    QByteArray imageData;
    bool hasReadHead;
    int dataSize;

private slots:
    void newConnection();
    void receiveData();

};

#endif // WIDGETITEM_H
