/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab1;
    QGroupBox *Sp_GroupBox_2;
    QLabel *Sp_Label_Nom_2;
    QLabel *Sp_Label_DA_2;
    QLineEdit *Sp_Line_Nom_2;
    QLabel *Sp_Label_ID_2;
    QLabel *Sp_Label_CIN_2;
    QLabel *Sp_Label_Email_2;
    QLineEdit *Sp_Line_Email_2;
    QPushButton *Sp_Button_Ajouter;
    QComboBox *comboBox;
    QDateEdit *dateEdit;
    QLineEdit *Sp_Line_ID_2;
    QPushButton *Sp_Button_auto;
    QPushButton *Sp_Button_Fermer_2;
    QLabel *label;
    QPushButton *Sp_Button_Modifier_3;
    QWidget *tab_2;
    QPushButton *Sp_Button_Tri_Nom;
    QPushButton *Sp_Button_Tri_Type;
    QTableView *Sp_TableView_Res;
    QLineEdit *Sp_Line_Recherche;
    QLabel *Sp_Label_TriPar;
    QPushButton *Sp_Button_Fermer;
    QPushButton *Sp_Button_ExportPDF;
    QLabel *label_2;
    QPushButton *Sp_Button_Modifier_4;
    QWidget *tab_3;
    QPushButton *pushButton_7;
    QLabel *Sp_Label_Stats;
    QPushButton *Sp_Button_Fermer_4;
    QLabel *label_4;
    QTableView *Sp_TableView_Res_3;
    QLabel *Bg_imzge;
    QFrame *frame;
    QLabel *label_5;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QFrame *frame_2;
    QLabel *label_6;
    QLabel *Sp_Label_Titre;
    QLabel *label_8;
    QPushButton *pushButton;
    QPushButton *Sp_Button_Modifier_2;
    QPushButton *pushButton_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1376, 800);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    background-color: #F5F5F5;  \n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(220, 150, 1131, 591));
        tabWidget->setStyleSheet(QString::fromUtf8("/* \360\237\224\271 Tab Widget Background */\n"
"QTabWidget::pane {\n"
"\n"
"   border: 1px solid rgba(35, 10, 80, 150); /* Darker purple, semi-transparent */\n"
"    background: rgba(248, 249, 250, 150); /* Light neutral with transparency */\n"
"    border-radius: 8px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"/* \360\237\224\271 Tab Bar (The Strip Holding Tabs) */\n"
"QTabBar::tab {\n"
"    background: white;\n"
"    border: 1px solid #230A50; /* Darker purple, thin border */\n"
"    padding: 10px 20px;\n"
"    border-radius: 6px 6px 0px 0px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    color: #555;\n"
"    transition: all 0.3s ease-in-out;\n"
"    min-width: 150px;\n"
"}\n"
"\n"
"/* \360\237\224\271 Hover Effect */\n"
"QTabBar::tab:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, \n"
"        stop:0 #5A1E8B, /* Dark purple */\n"
"        stop:1 #230A50); /* Even darker purple */\n"
"    color: white;\n"
"    border-color: #230A50; /* Darker purple */\n"
"}\n"
"\n"
"/"
                        "* \360\237\224\271 Selected Tab */\n"
"QTabBar::tab:selected {\n"
"    background: rgba(35,10,152,200); /* Blue */\n"
"    color: white;\n"
"    border-color: #230A50; /* Darker purple */\n"
"}\n"
"\n"
"/* \360\237\224\271 Unselected Tabs */\n"
"QTabBar::tab:!selected {\n"
"    margin-top: 2px;\n"
"}\n"
"\n"
"/* \360\237\224\271 Close Button on Tabs (if enabled) */\n"
"QTabBar::close-button {\n"
"    image: url(:/icons/close.png);\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QTabBar::close-button:hover {\n"
"    image: url(:/icons/close_hover.png);\n"
"}\n"
"\n"
""));
        tab1 = new QWidget();
        tab1->setObjectName("tab1");
        tab1->setStyleSheet(QString::fromUtf8(""));
        Sp_GroupBox_2 = new QGroupBox(tab1);
        Sp_GroupBox_2->setObjectName("Sp_GroupBox_2");
        Sp_GroupBox_2->setGeometry(QRect(90, 70, 401, 391));
        Sp_GroupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    background-color: rgb(171, 186, 255);\n"
"    border: 2px solid red; /* Red border */\n"
"    border-radius: 8px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    color: white;\n"
"    padding: 10px;\n"
"    margin-top: 15px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    padding: 5px 15px;\n"
"    background-color: red; /* Red title background */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"}\n"
""));
        Sp_Label_Nom_2 = new QLabel(Sp_GroupBox_2);
        Sp_Label_Nom_2->setObjectName("Sp_Label_Nom_2");
        Sp_Label_Nom_2->setGeometry(QRect(30, 100, 99, 31));
        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        Sp_Label_Nom_2->setFont(font);
        Sp_Label_DA_2 = new QLabel(Sp_GroupBox_2);
        Sp_Label_DA_2->setObjectName("Sp_Label_DA_2");
        Sp_Label_DA_2->setGeometry(QRect(30, 250, 101, 31));
        Sp_Label_DA_2->setFont(font);
        Sp_Line_Nom_2 = new QLineEdit(Sp_GroupBox_2);
        Sp_Line_Nom_2->setObjectName("Sp_Line_Nom_2");
        Sp_Line_Nom_2->setGeometry(QRect(130, 60, 209, 31));
        Sp_Line_Nom_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"		border: 1px solid #230a98;\n"
"		border-radius: 5px;\n"
"}\n"
"QLineEdit:hover {\n"
"		border: 1px solid #7359f0;\n"
"}"));
        Sp_Label_ID_2 = new QLabel(Sp_GroupBox_2);
        Sp_Label_ID_2->setObjectName("Sp_Label_ID_2");
        Sp_Label_ID_2->setGeometry(QRect(30, 50, 99, 31));
        Sp_Label_ID_2->setFont(font);
        Sp_Label_CIN_2 = new QLabel(Sp_GroupBox_2);
        Sp_Label_CIN_2->setObjectName("Sp_Label_CIN_2");
        Sp_Label_CIN_2->setGeometry(QRect(30, 150, 99, 31));
        Sp_Label_CIN_2->setFont(font);
        Sp_Label_Email_2 = new QLabel(Sp_GroupBox_2);
        Sp_Label_Email_2->setObjectName("Sp_Label_Email_2");
        Sp_Label_Email_2->setGeometry(QRect(30, 200, 99, 31));
        Sp_Label_Email_2->setFont(font);
        Sp_Line_Email_2 = new QLineEdit(Sp_GroupBox_2);
        Sp_Line_Email_2->setObjectName("Sp_Line_Email_2");
        Sp_Line_Email_2->setGeometry(QRect(130, 200, 209, 31));
        Sp_Line_Email_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"		border: 1px solid #230a98;\n"
"		border-radius: 5px;\n"
"}\n"
"QLineEdit:hover {\n"
"		border: 1px solid #7359f0;\n"
"}"));
        Sp_Button_Ajouter = new QPushButton(Sp_GroupBox_2);
        Sp_Button_Ajouter->setObjectName("Sp_Button_Ajouter");
        Sp_Button_Ajouter->setGeometry(QRect(120, 350, 151, 31));
        QFont font1;
        font1.setBold(true);
        Sp_Button_Ajouter->setFont(font1);
        Sp_Button_Ajouter->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        comboBox = new QComboBox(Sp_GroupBox_2);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(130, 150, 211, 41));
        dateEdit = new QDateEdit(Sp_GroupBox_2);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(130, 255, 201, 31));
        Sp_Line_ID_2 = new QLineEdit(Sp_GroupBox_2);
        Sp_Line_ID_2->setObjectName("Sp_Line_ID_2");
        Sp_Line_ID_2->setGeometry(QRect(130, 100, 209, 31));
        Sp_Line_ID_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"		border: 1px solid #230a98;\n"
"		border-radius: 5px;\n"
"}\n"
"QLineEdit:hover {\n"
"		border: 1px solid #7359f0;\n"
"}"));
        Sp_Line_ID_2->setEchoMode(QLineEdit::EchoMode::Normal);
        Sp_Button_auto = new QPushButton(Sp_GroupBox_2);
        Sp_Button_auto->setObjectName("Sp_Button_auto");
        Sp_Button_auto->setGeometry(QRect(340, 100, 51, 31));
        Sp_Button_auto->setFont(font1);
        Sp_Button_auto->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        Sp_Button_Fermer_2 = new QPushButton(tab1);
        Sp_Button_Fermer_2->setObjectName("Sp_Button_Fermer_2");
        Sp_Button_Fermer_2->setGeometry(QRect(510, 510, 81, 23));
        Sp_Button_Fermer_2->setFont(font1);
        Sp_Button_Fermer_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        label = new QLabel(tab1);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 0, 1131, 541));
        label->setStyleSheet(QString::fromUtf8("background-image: url(:/imgs/backgroundservice.png);"));
        Sp_Button_Modifier_3 = new QPushButton(tab1);
        Sp_Button_Modifier_3->setObjectName("Sp_Button_Modifier_3");
        Sp_Button_Modifier_3->setGeometry(QRect(690, 190, 151, 31));
        Sp_Button_Modifier_3->setFont(font1);
        Sp_Button_Modifier_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        tabWidget->addTab(tab1, QString());
        label->raise();
        Sp_GroupBox_2->raise();
        Sp_Button_Fermer_2->raise();
        Sp_Button_Modifier_3->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        Sp_Button_Tri_Nom = new QPushButton(tab_2);
        Sp_Button_Tri_Nom->setObjectName("Sp_Button_Tri_Nom");
        Sp_Button_Tri_Nom->setGeometry(QRect(90, 440, 81, 31));
        Sp_Button_Tri_Nom->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        Sp_Button_Tri_Type = new QPushButton(tab_2);
        Sp_Button_Tri_Type->setObjectName("Sp_Button_Tri_Type");
        Sp_Button_Tri_Type->setGeometry(QRect(180, 440, 81, 31));
        Sp_Button_Tri_Type->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        Sp_TableView_Res = new QTableView(tab_2);
        Sp_TableView_Res->setObjectName("Sp_TableView_Res");
        Sp_TableView_Res->setGeometry(QRect(10, 80, 1091, 331));
        Sp_TableView_Res->setStyleSheet(QString::fromUtf8("background-color:rgba(35,10,152, 50);"));
        Sp_Line_Recherche = new QLineEdit(tab_2);
        Sp_Line_Recherche->setObjectName("Sp_Line_Recherche");
        Sp_Line_Recherche->setGeometry(QRect(300, 20, 511, 41));
        Sp_Line_Recherche->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"		border: 1px solid #230a98;\n"
"		border-radius: 5px;\n"
"}\n"
"QLineEdit:hover {\n"
"		border: 1px solid #7359f0;\n"
"}"));
        Sp_Label_TriPar = new QLabel(tab_2);
        Sp_Label_TriPar->setObjectName("Sp_Label_TriPar");
        Sp_Label_TriPar->setGeometry(QRect(10, 440, 71, 31));
        Sp_Label_TriPar->setFont(font1);
        Sp_Button_Fermer = new QPushButton(tab_2);
        Sp_Button_Fermer->setObjectName("Sp_Button_Fermer");
        Sp_Button_Fermer->setGeometry(QRect(510, 510, 81, 23));
        Sp_Button_Fermer->setFont(font1);
        Sp_Button_Fermer->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        Sp_Button_ExportPDF = new QPushButton(tab_2);
        Sp_Button_ExportPDF->setObjectName("Sp_Button_ExportPDF");
        Sp_Button_ExportPDF->setGeometry(QRect(1000, 30, 101, 31));
        Sp_Button_ExportPDF->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        label_2 = new QLabel(tab_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(-10, -10, 1151, 551));
        label_2->setStyleSheet(QString::fromUtf8("background-image: url(:/imgs/backgroundservice.png);"));
        Sp_Button_Modifier_4 = new QPushButton(tab_2);
        Sp_Button_Modifier_4->setObjectName("Sp_Button_Modifier_4");
        Sp_Button_Modifier_4->setGeometry(QRect(280, 440, 151, 31));
        Sp_Button_Modifier_4->setFont(font1);
        Sp_Button_Modifier_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        tabWidget->addTab(tab_2, QString());
        label_2->raise();
        Sp_Button_Tri_Nom->raise();
        Sp_Button_Tri_Type->raise();
        Sp_TableView_Res->raise();
        Sp_Line_Recherche->raise();
        Sp_Label_TriPar->raise();
        Sp_Button_Fermer->raise();
        Sp_Button_ExportPDF->raise();
        Sp_Button_Modifier_4->raise();
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        pushButton_7 = new QPushButton(tab_3);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(940, 480, 131, 21));
        pushButton_7->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        Sp_Label_Stats = new QLabel(tab_3);
        Sp_Label_Stats->setObjectName("Sp_Label_Stats");
        Sp_Label_Stats->setGeometry(QRect(280, 50, 570, 431));
        Sp_Button_Fermer_4 = new QPushButton(tab_3);
        Sp_Button_Fermer_4->setObjectName("Sp_Button_Fermer_4");
        Sp_Button_Fermer_4->setGeometry(QRect(510, 510, 81, 23));
        Sp_Button_Fermer_4->setFont(font1);
        Sp_Button_Fermer_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        label_4 = new QLabel(tab_3);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(-20, -10, 1131, 541));
        label_4->setStyleSheet(QString::fromUtf8("background-image: url(:/imgs/backgroundservice.png);"));
        Sp_TableView_Res_3 = new QTableView(tab_3);
        Sp_TableView_Res_3->setObjectName("Sp_TableView_Res_3");
        Sp_TableView_Res_3->setGeometry(QRect(20, 10, 1091, 435));
        Sp_TableView_Res_3->setStyleSheet(QString::fromUtf8("background-color:rgba(35,10,152, 50);"));
        tabWidget->addTab(tab_3, QString());
        label_4->raise();
        Sp_Label_Stats->raise();
        Sp_Button_Fermer_4->raise();
        Sp_TableView_Res_3->raise();
        pushButton_7->raise();
        Bg_imzge = new QLabel(centralwidget);
        Bg_imzge->setObjectName("Bg_imzge");
        Bg_imzge->setEnabled(true);
        Bg_imzge->setGeometry(QRect(0, 0, 1381, 771));
        Bg_imzge->setMinimumSize(QSize(1201, 0));
        Bg_imzge->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: red; /* Text color */\n"
"    background-color: rgb(174, 171, 255) /* Background color */\n"
"}\n"
""));
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(10, 10, 191, 741));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(198, 198, 255);\n"
"border-radius : 20px"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        label_5 = new QLabel(frame);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 30, 141, 141));
        label_5->setStyleSheet(QString::fromUtf8("\n"
"QLabel {\n"
"    background-color: transparent; /* Makes background transparent */\n"
"    border: 2px solid #3498db; /* Adds a blue border */\n"
"    border-radius: 10px; /* Rounds the corners */\n"
"    padding: 5px; /* Adds space inside the label */image : url(:/imgs/logo.png)\n"
"}\n"
""));
        label_9 = new QLabel(frame);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(10, 200, 111, 41));
        label_9->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: white;\n"
"    background-color: rgb(168, 151, 255); /* Dark blue */\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"    border-radius: 10px;\n"
"    text-align: center;\n"
"    text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5); /* Adds a shadow effect */\n"
"}\n"
"\n"
""));
        label_10 = new QLabel(frame);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(10, 460, 161, 41));
        label_10->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: white;\n"
"    background-color: #2c3e50; /* Dark blue */\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"    border-radius: 10px;\n"
"    text-align: center;\n"
"    text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5); /* Adds a shadow effect */\n"
"}\n"
"\n"
""));
        label_11 = new QLabel(frame);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(10, 340, 161, 41));
        label_11->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: white;\n"
"    background-color: #2c3e50; /* Dark blue */\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"    border-radius: 10px;\n"
"    text-align: center;\n"
"    text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5); /* Adds a shadow effect */\n"
"}\n"
"\n"
""));
        label_12 = new QLabel(frame);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(10, 400, 161, 41));
        label_12->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: white;\n"
"    background-color: #2c3e50; /* Dark blue */\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"    border-radius: 10px;\n"
"    text-align: center;\n"
"    text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5); /* Adds a shadow effect */\n"
"}\n"
"\n"
""));
        label_13 = new QLabel(frame);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(10, 520, 161, 51));
        label_13->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: white;\n"
"    background-color: #2c3e50; /* Dark blue */\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"    border-radius: 10px;\n"
"    text-align: center;\n"
"    text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5); /* Adds a shadow effect */\n"
"}\n"
"\n"
""));
        label_14 = new QLabel(frame);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(10, 590, 161, 51));
        label_14->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: white;\n"
"    background-color: #2c3e50; /* Dark blue */\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"    border-radius: 10px;\n"
"    text-align: center;\n"
"    text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5); /* Adds a shadow effect */\n"
"}\n"
"\n"
""));
        label_15 = new QLabel(frame);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(10, 270, 161, 51));
        label_15->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: white;\n"
"    background-color: #2c3e50; /* Dark blue */\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"    border-radius: 10px;\n"
"    text-align: center;\n"
"    text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5); /* Adds a shadow effect */\n"
"}\n"
"\n"
""));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(220, 10, 1151, 131));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(198, 198, 255);\n"
"border-radius: 20px;"));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        label_6 = new QLabel(frame_2);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(780, 10, 261, 41));
        label_6->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    color: white; /* Text color */\n"
"    background-color: rgb(211, 205, 255); /* Blue background */\n"
"    font-size: 20px; /* Text size */\n"
"    font-weight: bold; /* Bold text */\n"
"    padding: 10px; /* Space inside the label */\n"
"    border-radius: 8px; /* Rounded corners */\n"
"    text-align: center; /* Centers text */\n"
"}\n"
""));
        Sp_Label_Titre = new QLabel(frame_2);
        Sp_Label_Titre->setObjectName("Sp_Label_Titre");
        Sp_Label_Titre->setGeometry(QRect(360, 60, 391, 51));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Trebuchet MS")});
        font2.setPointSize(22);
        Sp_Label_Titre->setFont(font2);
        Sp_Label_Titre->setStyleSheet(QString::fromUtf8("background-color: rgba(35,10,152,50);\n"
"border-bottom-left-radius: 10px;\n"
"border-bottom-right-radius: 10px;"));
        Sp_Label_Titre->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_8 = new QLabel(frame_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(1070, 10, 41, 41));
        label_8->setStyleSheet(QString::fromUtf8("image :url(:/imgs/userr.jpg)"));
        pushButton = new QPushButton(frame_2);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(1030, 60, 111, 41));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ff6b6b, stop:1 #d63031);\n"
"    color: white;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    padding: 10px 20px;\n"
"    border-radius: 6px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ff5252, stop:1 #b71c1c);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: #b71c1c;\n"
"}\n"
""));
        Sp_Button_Modifier_2 = new QPushButton(frame_2);
        Sp_Button_Modifier_2->setObjectName("Sp_Button_Modifier_2");
        Sp_Button_Modifier_2->setGeometry(QRect(820, 100, 91, 20));
        Sp_Button_Modifier_2->setFont(font1);
        Sp_Button_Modifier_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));
        pushButton_2 = new QPushButton(frame_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(20, 50, 81, 41));
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ff6b6b, stop:1 #d63031);\n"
"    color: white;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    padding: 10px 20px;\n"
"    border-radius: 6px;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ff5252, stop:1 #b71c1c);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: #b71c1c;\n"
"}\n"
""));
        MainWindow->setCentralWidget(centralwidget);
        Bg_imzge->raise();
        tabWidget->raise();
        frame->raise();
        frame_2->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1376, 30));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Sp_GroupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Ajouter un Service", nullptr));
        Sp_Label_Nom_2->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        Sp_Label_DA_2->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        Sp_Label_ID_2->setText(QCoreApplication::translate("MainWindow", "Nom ", nullptr));
        Sp_Label_CIN_2->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        Sp_Label_Email_2->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        Sp_Button_Ajouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Vole", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Nettouyage", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Maintenance", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Perte", nullptr));

        Sp_Button_auto->setText(QCoreApplication::translate("MainWindow", "Auto", nullptr));
        Sp_Button_Fermer_2->setText(QCoreApplication::translate("MainWindow", "Fermer", nullptr));
        label->setText(QString());
        Sp_Button_Modifier_3->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QCoreApplication::translate("MainWindow", "Gestion des Services", nullptr));
        Sp_Button_Tri_Nom->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        Sp_Button_Tri_Type->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        Sp_Line_Recherche->setInputMask(QString());
        Sp_Line_Recherche->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche par type", nullptr));
        Sp_Label_TriPar->setText(QCoreApplication::translate("MainWindow", "Tri par :", nullptr));
        Sp_Button_Fermer->setText(QCoreApplication::translate("MainWindow", "Fermer", nullptr));
        Sp_Button_ExportPDF->setText(QCoreApplication::translate("MainWindow", "Export PDF", nullptr));
        label_2->setText(QString());
        Sp_Button_Modifier_4->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Liste des Services", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "Refresh stats", nullptr));
        Sp_Label_Stats->setText(QString());
        Sp_Button_Fermer_4->setText(QCoreApplication::translate("MainWindow", "Fermer", nullptr));
        label_4->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", " stats des Servicess", nullptr));
        Bg_imzge->setText(QString());
        label_5->setText(QString());
        label_9->setText(QCoreApplication::translate("MainWindow", "Dashboard", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Employ\303\251s", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Sponsors", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Fournisseurs", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "services", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Magasins", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Evenements", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "User@shopDevs.com", nullptr));
        Sp_Label_Titre->setText(QCoreApplication::translate("MainWindow", "GESTION DES SERVICES", nullptr));
        label_8->setText(QString());
        pushButton->setText(QCoreApplication::translate("MainWindow", "logout", nullptr));
        Sp_Button_Modifier_2->setText(QCoreApplication::translate("MainWindow", "Historique", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "SOS", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
