#ifndef TIMETEST_H
#define TIMETEST_H
#include <QDialog>
#include <QTimer>
#include <QTextEdit>
#include <QLabel>
#include <QRadioButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QButtonGroup>
class timetest : public QDialog
{
    Q_OBJECT
public:
    timetest(QWidget* parent = 0);
    void initTimer();
    void initLayout();
    bool initTextEdit();
    void initButtons();
    bool hasNoSelect();
private:
    QTimer *m_timer;
    QButtonGroup *m_btnGroups[9];
    int m_timego;
    QTextEdit *m_textEdit;
    QLabel *m_titleLabels[10];
    QRadioButton *m_radioBtns[32];
    QCheckBox *m_chechBtns[4];
    QRadioButton *m_radioA;
    QRadioButton *m_radioB;
    QGridLayout *m_layout;
    QStringList m_answerList;
private slots:
    void freshTime();
    void getScore();

};

#endif // TIMETEST_H
