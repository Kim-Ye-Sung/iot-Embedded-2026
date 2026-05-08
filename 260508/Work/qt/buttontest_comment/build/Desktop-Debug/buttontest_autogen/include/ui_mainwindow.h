/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDial>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *TitleLabel;
    QGroupBox *groupBox;
    QLabel *label_led;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *ColorChangeBtn;
    QGroupBox *groupBox_2;
    QSlider *NoteSlider;
    QDial *VolumeDial;
    QLabel *label_2;
    QLabel *label;
    QGroupBox *groupBox_3;
    QLabel *UltraSonicLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        TitleLabel = new QLabel(centralwidget);
        TitleLabel->setObjectName("TitleLabel");
        TitleLabel->setGeometry(QRect(110, 50, 581, 71));
        QFont font;
        font.setPointSize(32);
        font.setBold(true);
        TitleLabel->setFont(font);
        TitleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(90, 130, 211, 381));
        QFont font1;
        font1.setBold(true);
        groupBox->setFont(font1);
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 3px solid #ff0000;\n"
"    border-radius: 6px;\n"
"    margin-top: 40px;\n"
"\n"
"    font-size: 25px;\n"
"    font-weight: bold;\n"
"    color: #ff0000;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    padding: 0 10px;\n"
"}"));
        groupBox->setAlignment(Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignTop);
        label_led = new QLabel(groupBox);
        label_led->setObjectName("label_led");
        label_led->setGeometry(QRect(50, 60, 111, 91));
        label_led->setStyleSheet(QString::fromUtf8("background-color:gray;\n"
"border-radius:20px;"));
        label_led->setAlignment(Qt::AlignmentFlag::AlignCenter);
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(60, 180, 92, 30));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(60, 240, 92, 30));
        ColorChangeBtn = new QPushButton(groupBox);
        ColorChangeBtn->setObjectName("ColorChangeBtn");
        ColorChangeBtn->setGeometry(QRect(50, 300, 121, 30));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(370, 130, 381, 221));
        groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 3px solid #7C3AED;\n"
"    border-radius: 6px;\n"
"    margin-top: 40px;\n"
"\n"
"    font-size: 25px;\n"
"    font-weight: bold;\n"
"    color: #7C3AED;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    padding: 0 10px;\n"
"}"));
        NoteSlider = new QSlider(groupBox_2);
        NoteSlider->setObjectName("NoteSlider");
        NoteSlider->setGeometry(QRect(200, 140, 160, 61));
        NoteSlider->setMaximum(100);
        NoteSlider->setSingleStep(1);
        NoteSlider->setOrientation(Qt::Orientation::Horizontal);
        VolumeDial = new QDial(groupBox_2);
        VolumeDial->setObjectName("VolumeDial");
        VolumeDial->setGeometry(QRect(220, 40, 121, 101));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 150, 151, 41));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("NanumSquareRound")});
        font2.setPointSize(15);
        label_2->setFont(font2);
        label_2->setStyleSheet(QString::fromUtf8("border: 1px solid black;"));
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label = new QLabel(groupBox_2);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 70, 151, 41));
        label->setFont(font2);
        label->setStyleSheet(QString::fromUtf8("border: 1px solid black;"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(370, 380, 381, 131));
        groupBox_3->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 3px solid #14B8A6;\n"
"    border-radius: 6px;\n"
"    margin-top: 40px;\n"
"\n"
"    font-size: 25px;\n"
"    font-weight: bold;\n"
"    color: #14B8A6;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    padding: 0 10px;\n"
"}"));
        UltraSonicLabel = new QLabel(groupBox_3);
        UltraSonicLabel->setObjectName("UltraSonicLabel");
        UltraSonicLabel->setGeometry(QRect(70, 55, 241, 61));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("UnDinaru")});
        font3.setPointSize(18);
        UltraSonicLabel->setFont(font3);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 27));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        TitleLabel->setText(QCoreApplication::translate("MainWindow", "Embedded Controller", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "LED Controller", nullptr));
        label_led->setText(QCoreApplication::translate("MainWindow", "OFF!", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "LED ON", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "LED OFF", nullptr));
        ColorChangeBtn->setText(QCoreApplication::translate("MainWindow", "Color Change", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Buzzer Controller", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Note Value", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Buzzer Volume", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "UltraSonic Value", nullptr));
        UltraSonicLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
