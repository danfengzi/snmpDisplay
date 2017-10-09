
#include <QApplication>
#include <QTextCodec>
#include "widgetitem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    WidgetItem w("[pt30o5][tr0,0,600,300][cf1][flt10o10]ab[/fl]c[mvl10r80,1,1,this is a test]dsv[nl]ghgh[mvcl80,1,2,金晓电子股份有限公司]");
    w.show();
    return a.exec();
}
