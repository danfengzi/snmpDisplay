#include "textboxitem.h"


/***********************************************************************
函数名称 : TextBoxItem
函数功能 : 构造函数
输入参数 : QString multi字符串, QWidget *父窗口
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
TextBoxItem::TextBoxItem(QString multiString, QWidget *parent) : QLabel(parent)
{
    multiStr = multiString;
    multiParsing();

    chPositioning();
}


/***********************************************************************
函数名称 : ~TextBoxItem
函数功能 : 析构函数
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
TextBoxItem::~TextBoxItem()
{
    qDebug() << "delete TextBoxItem";
    qDebug() << "CharacterItem : " << characterItems.size();
    for (int i = 0; i < characterItems.size(); ++i)
    {
        qDebug() << "start delete characterItems[" << i << "]";
        delete characterItems[i];
    }
    qDebug() << "MovingTextItem : " << movingTextItems.size();
    for (int i = 0; i < movingTextItems.size(); ++i)
    {
        qDebug() << "start delete movingTextItems[" << i << "]";
        delete movingTextItems[i];
    }
    qDebug() << "MixedItem : " << mixedItems.size();
    for (int i = 0; i < mixedItems.size(); ++i)
    {
        qDebug() << "start delete mixedItems[" << i << "]";
        delete mixedItems[i];
    }
}


/***********************************************************************
函数名称 : paintEvent
函数功能 : 绘画事件
输入参数 : QPaintEvent *绘画事件
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void TextBoxItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QBrush brush(backgroundColor);

    painter.setBrush(brush);

    painter.drawRect(0, 0, width, height);

    for (int i = 0; i < characterItems.size(); ++i)
    {
        characterItems[i]->setGeometry(characterItems[i]->positionX,
                                       characterItems[i]->positionY - characterItems[i]->chPixelSize,
                                       characterItems[i]->chPixelSize, characterItems[i]->chPixelSize);
    }

    for (int i = 0; i < movingTextItems.size(); ++i)
    {
        movingTextItems[i]->setGeometry(movingTextItems[i]->positionX,
                                        movingTextItems[i]->positionY - movingTextItems[i]->itemHeight,
                                        movingTextItems[i]->itemWidth, movingTextItems[i]->itemHeight);
    }
}


/***********************************************************************
函数名称 : chPositioning
函数功能 : 对文本中每一个字符进行定位
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void TextBoxItem::chPositioning()
{
    totalHeight = 0;
    if (0 == mixedItems.size())
    {
        return;
    }
    int lineNumTotal;
    if ("character" == mixedItems.last()->type)
    {
        lineNumTotal = characterItems.last()->lineNum;
    }
    else
    {
        lineNumTotal = movingTextItems.last()->lineNum;
    }

    for (int i = 0; i < lineNumTotal; ++i)
    {
        tempLineLenth = 0;
        tempLineHeight = 0;
        tempLineSpace = 0;
        for (int j = 0; j < mixedItems.size(); ++j)
        {
            if ("character" == mixedItems[j]->type)
            {
                if (i == characterItems[mixedItems[j]->index]->lineNum - 1)
                {
                    if (tempLineHeight < characterItems[mixedItems[j]->index]->chPixelSize)
                    {
                        tempLineHeight = characterItems[mixedItems[j]->index]->chPixelSize;
                    }
                    if (tempLineSpace < characterItems[mixedItems[j]->index]->lineSpace)
                    {
                        tempLineSpace = characterItems[mixedItems[j]->index]->lineSpace;
                    }
                    tempLineLenth += characterItems[mixedItems[j]->index]->chPixelSize
                            + characterItems[mixedItems[j]->index]->chSpace;
                }
            }
            else
            {
                if (i == movingTextItems[mixedItems[j]->index]->lineNum - 1)
                {
                    if (tempLineHeight < movingTextItems[mixedItems[j]->index]->itemHeight)
                    {
                        tempLineHeight = movingTextItems[mixedItems[j]->index]->itemHeight;
                    }
                    if (tempLineSpace < movingTextItems[mixedItems[j]->index]->lineSpace)
                    {
                        tempLineSpace = movingTextItems[mixedItems[j]->index]->lineSpace;
                    }
                    tempLineLenth += movingTextItems[mixedItems[j]->index]->itemWidth;
                }
            }
        }
        if ("character" == mixedItems.last()->type)
        {
            tempLineLenth -= characterItems.last()->chSpace;
        }
        lineHeight.append(tempLineHeight);
        lineLength.append(tempLineLenth);
        lineSpace.append(tempLineSpace);
        totalHeight += tempLineHeight + tempLineSpace;
    }
    totalHeight -= lineSpace.last();

    if (2 == justificationPage)
    {
        tempStartY = 0;
    }
    else if (3 == justificationPage)
    {
        tempStartY = (this->height - totalHeight) / 2;
    }
    if (4 == justificationPage)
    {
        tempStartY = this->height - totalHeight;
    }

    for (int i = 0; i < lineNumTotal; ++i)
    {
        tempStartY += lineHeight[i];
        startY.append(tempStartY);
        tempStartY += lineSpace[i];
    }

    for (int i = 0; i < lineNumTotal; ++i)
    {
        if (2 == justificationLine)
        {
            startX = 0;
        }
        else if (3 == justificationLine)
        {
            startX = (this->width - lineLength[i]) / 2;
        }
        else if (4 == justificationLine)
        {
            startX = this->width - lineLength[i];
        }

        chOffset = 0;
        for (int j = 0; j < mixedItems.size(); j++)
        {
            if ("character" == mixedItems[j]->type)
            {
                if (i == characterItems[mixedItems[j]->index]->lineNum - 1)
                {
                    characterItems[mixedItems[j]->index]->positionX = startX + chOffset;
                    chOffset += characterItems[mixedItems[j]->index]->chPixelSize
                            + characterItems[mixedItems[j]->index]->chSpace;
                    if (2 == justificationPage)
                    {
                        characterItems[mixedItems[j]->index]->positionY = startY[i] - (lineHeight[i]
                                                     - characterItems[mixedItems[j]->index]->chPixelSize);
                    }
                    else if (3 == justificationPage)
                    {
                        characterItems[mixedItems[j]->index]->positionY = startY[i] - (lineHeight[i]
                                                     - characterItems[mixedItems[j]->index]->chPixelSize) / 2;
                    }
                    else if (4 == justificationPage)
                    {
                        characterItems[mixedItems[j]->index]->positionY = startY[i];
                    }
                }
            }
            else
            {
                if (i == movingTextItems[mixedItems[j]->index]->lineNum - 1)
                {
                    movingTextItems[mixedItems[j]->index]->positionX = startX + chOffset;
                    chOffset += movingTextItems[mixedItems[j]->index]->itemWidth;
                    if (2 == justificationPage)
                    {
                        movingTextItems[mixedItems[j]->index]->positionY = startY[i] - (lineHeight[i]
                                                     - movingTextItems[mixedItems[j]->index]->itemHeight);
                    }
                    else if (3 == justificationPage)
                    {
                        movingTextItems[mixedItems[j]->index]->positionY = startY[i] - (lineHeight[i]
                                                     - movingTextItems[mixedItems[j]->index]->itemHeight) / 2;
                    }
                    else if (4 == justificationPage)
                    {
                        movingTextItems[mixedItems[j]->index]->positionY = startY[i];
                    }
                }
            }
        }
    }
}


/***********************************************************************
函数名称 : multiParsing
函数功能 : 解析multi字符串
输入参数 : 无
返回值   : 无
作者    : cj   2017.9.18
*************************************************************************/
void TextBoxItem::multiParsing()
{
    if (multiStr.contains("[tr"))
    {
        //取出文本框的横纵坐标已经长度和宽度
        int begin = multiStr.indexOf("[tr");
        int end = multiStr.indexOf("]");
        QString subString = multiStr.mid(begin, end-begin+1);
        multiStr.remove(subString);
        subString.remove("[tr");
        subString.remove("]");
        QList<QString> strList = subString.split(',');
        positionX = strList.at(0).toInt();
        positionY = strList.at(1).toInt();
        width = strList.at(2).toInt();
        height = strList.at(3).toInt();
    }
    else
    {
        positionX = 0;
        positionY = 0;
        width = 600;
        height = 600;
    }
    this->setGeometry(positionX, positionY, width, height);

    //取出文本的行对齐方式
    if (multiStr.contains("jl"))
    {
        int begin = multiStr.indexOf("[jl");
        int end = multiStr.indexOf("]");
        QString subString = multiStr.mid(begin, end-begin+1);
        multiStr.remove(subString);
        subString.remove("[jl");
        subString.remove("]");
        justificationLine = subString.toInt();
    }
    else
    {
        justificationLine = 3;
    }

    //取出文本的页面对齐方式
    if (multiStr.contains("jp"))
    {
        int begin = multiStr.indexOf("[jp");
        int end = multiStr.indexOf("]");
        QString subString = multiStr.mid(begin, end-begin+1);
        multiStr.remove(subString);
        subString.remove("[jp");
        subString.remove("]");
        justificationPage = subString.toInt();
    }
    else
    {
        justificationPage = 3;
    }

    QList<QString> tempList1 = multiStr.split(']');
    QList<QString> strList;

    for (int i = 0; i < tempList1.size() - 1; ++i)
    {
        tempList1[i].append("]");
        QList<QString> tempList2 = tempList1[i].split('[');
        if ("" == tempList2.at(0))
        {
            tempList2.removeFirst();
        }
        for (int j = 0; j < tempList2.size(); ++j)
        {
            if (tempList2.at(j).endsWith("]"))
            {
                tempList2[j].insert(0, '[');
            }
            strList.append(tempList2.at(j));
        }
    }
    if (!("" == tempList1.last()))
    {
        strList.append(tempList1.last());
    }

    int totalCount = 0;   //记录item总数量
    int chCount = 0;    //记录characterItem数量
    int movCount = 0;  //记录movingTextItem数量
    for (int i = 0; i < strList.size(); ++i)
    {
        //找出被修饰的字符串和移动文本标签
        if ((!strList.at(i).startsWith('[')) || strList.at(i).startsWith("[mv"))
        {
            QList<QString> multiList;
            QList<QString> tempMultiList;
            for (int j = i - 1; j >=0; --j)
            {
                if ((strList.at(j).startsWith('[')) && (!strList.at(j).startsWith("[mv")))
                {
                    multiList.insert(0, strList.at(j));
                    tempMultiList.insert(0, strList.at(j));
                }
            }

            bool hasForeColor = false;  //是否已经拥有前景色
            bool hasFont = false;    //是否已经拥有字体
            bool hasSpace = false;   //是否已经拥有字符间距
            bool meetSpaceEnd = false;  //是否已经遇到过字符间距结尾
            bool hasFlash = false;   //是否已经拥有闪烁时间
            bool meetFlashEnd = false;  //是否已经遇到过闪烁时间结尾
            for (int j = tempMultiList.size() - 1; j >= 0; --j)
            {
                if (tempMultiList.at(j).contains("[cb"))
                {
                    if (!hasForeColor)
                    {
                        hasForeColor = true;
                    }
                    else
                    {
                        multiList.removeAt(j);
                    }
                }

                if (tempMultiList.at(j).contains("[fo"))
                {
                    if (!hasFont)
                    {
                        hasFont = true;
                    }
                    else
                    {
                        multiList.removeAt(j);
                    }
                }

                if (tempMultiList.at(j).contains("[sc"))
                {
                    if ((!hasSpace) && (!meetSpaceEnd))
                    {
                        hasSpace = true;
                    }
                    else
                    {
                        multiList.removeAt(j);
                    }
                }

                if (tempMultiList.at(j).contains("[/sc"))
                {
                    meetSpaceEnd = true;
                    multiList.removeAt(j);
                }

                //如果是静态或闪光文本则判断闪光标签
                if (!strList.at(i).startsWith('['))
                {
                    if (tempMultiList.at(j).contains("[fl"))
                    {
                        if ((!hasFlash) && (!meetFlashEnd))
                        {
                            hasFlash = true;
                        }
                        else
                        {
                            multiList.removeAt(j);
                        }
                    }

                    if (tempMultiList.at(j).contains("[/fl"))
                    {
                        meetFlashEnd = true;
                        multiList.removeAt(j);
                    }
                }
            }

            //生成带有修饰标签的静态或闪光文本字符串
            if (!strList.at(i).startsWith('['))
            {
                QString chTempMultiStr;
                for (int j = 0; j < multiList.size(); ++j)
                {
                    chTempMultiStr.append(multiList.at(j));
                }

                for (int j = 0; j < strList.at(i).size(); ++j)
                {
                    QString chMultiStr = chTempMultiStr;
                    chMultiStr.append(strList.at(i).at(j));
                    CharacterItem *tempChItem = new CharacterItem(chMultiStr, this);
                    characterItems.append(tempChItem);
                    ItemPosition *tempItemPos = new ItemPosition("character", chCount);
                    chCount++;
                    mixedItems.insert(totalCount, tempItemPos);
                    totalCount++;
                }
            }
            //生成带有修饰标签的移动文本字符串
            else
            {
                QString movMultiStr;
                for (int j = 0; j < multiList.size(); ++j)
                {
                    if ((!multiList.at(j).contains("[fl")) && (!multiList.at(j).contains("[/fl")))
                    {
                        movMultiStr.append(multiList.at(j));
                    }
                }
                movMultiStr.append(strList.at(i));

                MovingTextItem *tempMovItem = new MovingTextItem(movMultiStr, this);
                movingTextItems.append(tempMovItem);
                ItemPosition *tempItemPos = new ItemPosition("movingText", movCount);
                movCount++;
                mixedItems.insert(totalCount, tempItemPos);
                totalCount++;
            }
        }
    }
}
