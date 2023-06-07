#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDialog>
logindialog::logindialog(QWidget *parent)
    : QDialog(parent),ui(new Ui::logindialog)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);
    this->resize(ui->label->width(),ui->label->height());
    setWindowTitle("科目一考试");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setFixedSize(width(),height());
}

logindialog::~logindialog()
{
    delete ui;
}
void logindialog::on_pushButton_clicked()
{

    QRegExp rx("^[A-Za-z0-9]+([_\.][A-Za-z0-9]+)*@([A-Za-z0-9\-]+\.)+[A-Za-z0-9]{2,6}$");
    bool res = rx.exactMatch(ui->lineEdit->text());
    if(!res){
        QMessageBox::information(this,"提示","非法邮箱请重新输入！");
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        return;
    }else{
        QString filename;
        QString strAccInput;
        QString strCode;
        QString strLine;
        QStringList strList;
        filename = "account.txt";
        strAccInput = ui->lineEdit->text();
        strCode = ui->lineEdit_2->text();
        QFile file(filename);
        QTextStream stream(&file);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text ))
        {
            while(!stream.atEnd()){
            strLine = stream.readLine();
            strList = strLine.split(",");
            if(strAccInput == strList.at(0))
                if(strCode == strList.at(1)){
                    QMessageBox::information(this,"提示","欢迎进入科一考试");
                    file.close();
                    done(Accepted);
                    return;
                }else{
                    QMessageBox::information(this,"提示","输入密码有误，请重新输入！");
                    ui->lineEdit_2->clear();
                    ui->lineEdit_2->setFocus();
                    return;
                }
            }
            QMessageBox::information(this,"提示","账号输入有误请重新输入！");
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ui->lineEdit->setFocus();
            file.close();
            return;
          }else{
            QMessageBox::information(this,"提示","读取账号数据文件失败");
            return;
        }
    }
}

void logindialog::on_pushButton_2_clicked()
{
    done(Rejected);
}
