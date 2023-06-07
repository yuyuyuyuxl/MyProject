#include "logindialog.h"
#include <timetest.h>
#include <QApplication>
#include <QDebug>
#include "logindialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //logindialog s;
   //s.show();
    logindialog w;
   int res = w .exec();
   if(res == QDialog::Accepted){
       timetest *Timetest = new timetest;
   }else{
        return 0;
   }
    return a.exec();
}
