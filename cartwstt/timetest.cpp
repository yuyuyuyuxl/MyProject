#include "timetest.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include <QString>
#include <QDialog>
#include <QPushButton>
timetest::timetest(QWidget* parent):QDialog(parent)
{
    QFont font;
    font.setPixelSize(30);
    setFont(font);
    setPalette(QPalette(QColor(209,215,255)));
    setWindowTitle("考试已用时间：0分0秒");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint );
    initTimer();
    initLayout();
    if(!initTextEdit()){
        QMessageBox::information(this,"提示","初始化题库文件失败");
        QTimer::singleShot(0,qApp,SLOT(quit()));
    }
    initButtons();
    show();
}

void timetest::initTimer()
{
    m_timego = 0;
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->start();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(freshTime()));
}

void timetest::initLayout()
{
    m_layout = new QGridLayout(this);
    m_layout->setSpacing(10);
    m_layout->setMargin(10);
}

bool timetest::initTextEdit()
{

    QString strLine;
    QStringList strList;
    QString fileName=("exam.txt");
    QFile file(fileName);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text )){
       m_textEdit = new QTextEdit(this);
       QString strText;
       m_textEdit->setReadOnly(true);
       QString setText;
       int nLine=0;
      while(!stream.atEnd()){
           if(nLine ==0 ){
               stream.readLine();
               nLine++;
               continue;
           }
           if((nLine >= 6 && nLine <=6 * 9 &&(nLine % 6 == 0))
                   ||(nLine == 6 * 9 + 4)){
                   strLine =stream.readLine();
                   strList = strLine.split(" ");
                   m_answerList.append(strList.at(1));
                   strText += "\n";
                   nLine++;
                   continue;
           }
           strText += stream.readLine();
           strText +="\n";
           nLine++;
       }
      m_textEdit->setText(strText);
      m_layout->addWidget(m_textEdit,0,0,1,10);
      file.close();

      return true;

    }else{
        return false;
    }
}

void timetest::initButtons()
{
    QStringList strlist = {"A","B","C","D"};
    for(int i=0;i<10;i++){
        m_titleLabels[i] = new QLabel(this);
        m_titleLabels[i]->setText("第"+ QString::number(i+1)+"题");
        m_layout->addWidget(m_titleLabels[i],1,i);

        if(i==9){
            m_radioA = new QRadioButton(this);
            m_radioB = new QRadioButton(this);
            m_radioA->setText("正确");
            m_radioB->setText("错误");
            m_layout->addWidget(m_radioA,2,9);
            m_layout->addWidget(m_radioB,3,9);

            m_btnGroups[8] = new QButtonGroup(this);
            m_btnGroups[8]->addButton(m_radioA);
            m_btnGroups[8]->addButton(m_radioB);
            break;
        }

        if(i<8 ) m_btnGroups[i] = new QButtonGroup(this);
        for(int j = 0;j<4;j++)
        {
            if(i==8){
                m_chechBtns[j] = new QCheckBox(this);
                m_chechBtns[j]->setText(strlist.at(j));
                m_layout->addWidget(m_chechBtns[j],2+j,8);


            }else{
                m_radioBtns[4 * i + j] = new QRadioButton(this);
                m_radioBtns[4 * i + j]->setText(strlist.at(j));
                m_layout->addWidget(m_radioBtns[4 * i + j],2+j,i);
                m_btnGroups[i]->addButton(m_radioBtns[4 * i + j]);
            }
        }
    }
    QPushButton *submitBtn = new QPushButton(this);
    submitBtn->setText("提交");
    submitBtn->setFixedSize(100,35);
    connect(submitBtn,SIGNAL(clicked(bool)),this,SLOT(getScore()));
    m_layout->addWidget(submitBtn,6,9);

}

bool timetest::hasNoSelect()
{
    int radioSelects = 0;
    for(int i=0;i<8;i++)
    {
        if(m_btnGroups[i]->checkedButton())
            radioSelects++;
    }
    if(radioSelects != 8)
        return true;
    int checkSelects = 0;
    for(int i=0;i<4;i++)
    {
        if(m_chechBtns[i]->isChecked())
            checkSelects++;

    }

    if(checkSelects == 0 || checkSelects == 1)
        return true;
    if(!m_radioA->isChecked() && !m_radioB->isChecked())
        return true;
    return false;
}

void timetest::freshTime()
{
    m_timego++;
    QString min = QString::number(m_timego / 60);
    QString sec = QString::number(m_timego % 60);
    setWindowTitle("考试时间"+ min + "分" + sec + "秒");
}

void timetest::getScore()
{
    if(hasNoSelect()){
        QMessageBox::information(this,"提示","你有未完成的题目请完成","是");
        return;
    }
    int scores = 0;
    for(int i =0;i<10;i++)
    {
        if(i<8)
            if(m_btnGroups[i]->checkedButton()->text()== m_answerList.at(i))
                scores += 10;
        if(i == 8){
            QString answer = m_answerList.at(i);
            bool hasA = false;
            bool hasB = false;
            bool hasC = false;
            bool hasD = false;

            if(answer.contains("A")) hasA = true;
            if(answer.contains("B")) hasA = true;
            if(answer.contains("C")) hasA = true;
            if(answer.contains("D")) hasA = true;

            bool checkA = m_chechBtns[0]->checkState();
            bool checkB = m_chechBtns[1]->checkState();
            bool checkC = m_chechBtns[2]->checkState();
            bool checkD = m_chechBtns[3]->checkState();

            if(hasA != checkA) continue;
            if(hasB != checkB) continue;
            if(hasC != checkC) continue;
            if(hasD != checkD) continue;
            scores +=10;
        }
        if(i==9){
            if(m_btnGroups[8]->checkedButton()->text()==m_answerList.at(i))
                scores +=10;
        }
    }
    QString str ="你的分数是："+QString::number(scores)+"分，是否重新考试？";
    int res = QMessageBox::information(this,"提示",str,
                                       QMessageBox::Yes | QMessageBox::No);
    if(res == QMessageBox::Yes)
        return;
    else
        close();
}
