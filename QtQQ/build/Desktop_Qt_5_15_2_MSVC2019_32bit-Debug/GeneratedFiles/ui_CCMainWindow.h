/********************************************************************************
** Form generated from reading UI file 'CCMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCMAINWINDOW_H
#define UI_CCMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCMainWindowClass
{
public:
    QVBoxLayout *verticalLayout_6;
    QWidget *titileWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *titileIcon;
    QSpacerItem *horizontalSpacer;
    QPushButton *placehold1;
    QPushButton *placehold2;
    QPushButton *sysmin;
    QPushButton *sysclose;
    QHBoxLayout *horizontalLayout_3;
    QLabel *headLabel;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *nameLabel;
    QPushButton *statusBtn;
    QPushButton *levelBtn;
    QLineEdit *lineEdit;
    QWidget *appWidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QWidget *searchWidget;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *searchLineEdit;
    QPushButton *searchBtn;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *treeWidget;
    QWidget *bottomWidget;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *bottomLayout_up;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *bottomBtn_1;
    QPushButton *bottomBtn_2;
    QPushButton *bottomBtn_3;
    QPushButton *bottomBtn_4;
    QPushButton *bottomBtn_5;
    QPushButton *bottomBtn_6;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *bottomBtn_7;

    void setupUi(QWidget *CCMainWindowClass)
    {
        if (CCMainWindowClass->objectName().isEmpty())
            CCMainWindowClass->setObjectName(QString::fromUtf8("CCMainWindowClass"));
        CCMainWindowClass->resize(280, 680);
        CCMainWindowClass->setMinimumSize(QSize(280, 680));
        verticalLayout_6 = new QVBoxLayout(CCMainWindowClass);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        titileWidget = new QWidget(CCMainWindowClass);
        titileWidget->setObjectName(QString::fromUtf8("titileWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(titileWidget->sizePolicy().hasHeightForWidth());
        titileWidget->setSizePolicy(sizePolicy);
        titileWidget->setProperty("titleskin", QVariant(true));
        verticalLayout_2 = new QVBoxLayout(titileWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(12, -1, -1, -1);
        titileIcon = new QLabel(titileWidget);
        titileIcon->setObjectName(QString::fromUtf8("titileIcon"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(titileIcon->sizePolicy().hasHeightForWidth());
        titileIcon->setSizePolicy(sizePolicy1);
        titileIcon->setMinimumSize(QSize(42, 18));
        titileIcon->setMaximumSize(QSize(42, 18));

        horizontalLayout->addWidget(titileIcon);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        placehold1 = new QPushButton(titileWidget);
        placehold1->setObjectName(QString::fromUtf8("placehold1"));
        placehold1->setMinimumSize(QSize(24, 24));
        placehold1->setMaximumSize(QSize(24, 24));

        horizontalLayout->addWidget(placehold1);

        placehold2 = new QPushButton(titileWidget);
        placehold2->setObjectName(QString::fromUtf8("placehold2"));
        placehold2->setMinimumSize(QSize(24, 24));
        placehold2->setMaximumSize(QSize(24, 24));

        horizontalLayout->addWidget(placehold2);

        sysmin = new QPushButton(titileWidget);
        sysmin->setObjectName(QString::fromUtf8("sysmin"));
        sysmin->setMinimumSize(QSize(24, 24));
        sysmin->setMaximumSize(QSize(24, 24));

        horizontalLayout->addWidget(sysmin);

        sysclose = new QPushButton(titileWidget);
        sysclose->setObjectName(QString::fromUtf8("sysclose"));
        sysclose->setMinimumSize(QSize(24, 24));
        sysclose->setMaximumSize(QSize(24, 24));

        horizontalLayout->addWidget(sysclose);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(12, 6, 12, 6);
        headLabel = new QLabel(titileWidget);
        headLabel->setObjectName(QString::fromUtf8("headLabel"));
        sizePolicy1.setHeightForWidth(headLabel->sizePolicy().hasHeightForWidth());
        headLabel->setSizePolicy(sizePolicy1);
        headLabel->setMinimumSize(QSize(68, 68));
        headLabel->setMaximumSize(QSize(68, 68));

        horizontalLayout_3->addWidget(headLabel);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        nameLabel = new QLabel(titileWidget);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(nameLabel->sizePolicy().hasHeightForWidth());
        nameLabel->setSizePolicy(sizePolicy2);
        nameLabel->setMinimumSize(QSize(0, 22));
        nameLabel->setMaximumSize(QSize(16777215, 22));
        nameLabel->setStyleSheet(QString::fromUtf8("color:rgb(255,255,255);\n"
"font:75 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
""));

        horizontalLayout_2->addWidget(nameLabel);

        statusBtn = new QPushButton(titileWidget);
        statusBtn->setObjectName(QString::fromUtf8("statusBtn"));
        sizePolicy1.setHeightForWidth(statusBtn->sizePolicy().hasHeightForWidth());
        statusBtn->setSizePolicy(sizePolicy1);
        statusBtn->setMinimumSize(QSize(34, 22));
        statusBtn->setMaximumSize(QSize(34, 22));
        statusBtn->setProperty("hasborder", QVariant(true));

        horizontalLayout_2->addWidget(statusBtn);

        levelBtn = new QPushButton(titileWidget);
        levelBtn->setObjectName(QString::fromUtf8("levelBtn"));
        sizePolicy1.setHeightForWidth(levelBtn->sizePolicy().hasHeightForWidth());
        levelBtn->setSizePolicy(sizePolicy1);
        levelBtn->setMinimumSize(QSize(22, 16));
        levelBtn->setMaximumSize(QSize(22, 16));
        levelBtn->setProperty("hasborder", QVariant(true));

        horizontalLayout_2->addWidget(levelBtn);


        verticalLayout->addLayout(horizontalLayout_2);

        lineEdit = new QLineEdit(titileWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 22));
        lineEdit->setMaximumSize(QSize(16777215, 22));
        lineEdit->setFrame(false);
        lineEdit->setReadOnly(false);

        verticalLayout->addWidget(lineEdit);

        appWidget = new QWidget(titileWidget);
        appWidget->setObjectName(QString::fromUtf8("appWidget"));
        sizePolicy2.setHeightForWidth(appWidget->sizePolicy().hasHeightForWidth());
        appWidget->setSizePolicy(sizePolicy2);
        appWidget->setMinimumSize(QSize(0, 22));
        appWidget->setMaximumSize(QSize(16777215, 22));

        verticalLayout->addWidget(appWidget);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_6->addWidget(titileWidget);

        widget = new QWidget(CCMainWindowClass);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy3);
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(1, 0, 1, 0);
        searchWidget = new QWidget(widget);
        searchWidget->setObjectName(QString::fromUtf8("searchWidget"));
        sizePolicy2.setHeightForWidth(searchWidget->sizePolicy().hasHeightForWidth());
        searchWidget->setSizePolicy(sizePolicy2);
        searchWidget->setMinimumSize(QSize(0, 30));
        searchWidget->setMaximumSize(QSize(16777215, 30));
        horizontalLayout_4 = new QHBoxLayout(searchWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(6, 0, 6, 0);
        searchLineEdit = new QLineEdit(searchWidget);
        searchLineEdit->setObjectName(QString::fromUtf8("searchLineEdit"));
        searchLineEdit->setMinimumSize(QSize(0, 30));
        searchLineEdit->setMaximumSize(QSize(16777215, 30));
        searchLineEdit->setFrame(false);

        horizontalLayout_4->addWidget(searchLineEdit);

        searchBtn = new QPushButton(searchWidget);
        searchBtn->setObjectName(QString::fromUtf8("searchBtn"));
        sizePolicy1.setHeightForWidth(searchBtn->sizePolicy().hasHeightForWidth());
        searchBtn->setSizePolicy(sizePolicy1);
        searchBtn->setMinimumSize(QSize(16, 16));
        searchBtn->setMaximumSize(QSize(16, 16));

        horizontalLayout_4->addWidget(searchBtn);


        verticalLayout_4->addWidget(searchWidget);

        scrollArea = new QScrollArea(widget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 278, 470));
        sizePolicy3.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy3);
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(scrollAreaWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setFrameShape(QFrame::NoFrame);
        treeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        treeWidget->setIndentation(0);
        treeWidget->setHeaderHidden(true);
        treeWidget->setExpandsOnDoubleClick(false);

        verticalLayout_3->addWidget(treeWidget);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_4->addWidget(scrollArea);


        verticalLayout_6->addWidget(widget);

        bottomWidget = new QWidget(CCMainWindowClass);
        bottomWidget->setObjectName(QString::fromUtf8("bottomWidget"));
        sizePolicy2.setHeightForWidth(bottomWidget->sizePolicy().hasHeightForWidth());
        bottomWidget->setSizePolicy(sizePolicy2);
        bottomWidget->setMinimumSize(QSize(0, 60));
        bottomWidget->setMaximumSize(QSize(16777215, 60));
        bottomWidget->setProperty("bottomskin", QVariant(true));
        verticalLayout_5 = new QVBoxLayout(bottomWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(6, 6, 6, 6);
        bottomLayout_up = new QHBoxLayout();
        bottomLayout_up->setSpacing(6);
        bottomLayout_up->setObjectName(QString::fromUtf8("bottomLayout_up"));

        verticalLayout_5->addLayout(bottomLayout_up);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        bottomBtn_1 = new QPushButton(bottomWidget);
        bottomBtn_1->setObjectName(QString::fromUtf8("bottomBtn_1"));
        sizePolicy1.setHeightForWidth(bottomBtn_1->sizePolicy().hasHeightForWidth());
        bottomBtn_1->setSizePolicy(sizePolicy1);
        bottomBtn_1->setMinimumSize(QSize(24, 24));
        bottomBtn_1->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_1);

        bottomBtn_2 = new QPushButton(bottomWidget);
        bottomBtn_2->setObjectName(QString::fromUtf8("bottomBtn_2"));
        sizePolicy1.setHeightForWidth(bottomBtn_2->sizePolicy().hasHeightForWidth());
        bottomBtn_2->setSizePolicy(sizePolicy1);
        bottomBtn_2->setMinimumSize(QSize(24, 24));
        bottomBtn_2->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_2);

        bottomBtn_3 = new QPushButton(bottomWidget);
        bottomBtn_3->setObjectName(QString::fromUtf8("bottomBtn_3"));
        sizePolicy1.setHeightForWidth(bottomBtn_3->sizePolicy().hasHeightForWidth());
        bottomBtn_3->setSizePolicy(sizePolicy1);
        bottomBtn_3->setMinimumSize(QSize(24, 24));
        bottomBtn_3->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_3);

        bottomBtn_4 = new QPushButton(bottomWidget);
        bottomBtn_4->setObjectName(QString::fromUtf8("bottomBtn_4"));
        sizePolicy1.setHeightForWidth(bottomBtn_4->sizePolicy().hasHeightForWidth());
        bottomBtn_4->setSizePolicy(sizePolicy1);
        bottomBtn_4->setMinimumSize(QSize(24, 24));
        bottomBtn_4->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_4);

        bottomBtn_5 = new QPushButton(bottomWidget);
        bottomBtn_5->setObjectName(QString::fromUtf8("bottomBtn_5"));
        sizePolicy1.setHeightForWidth(bottomBtn_5->sizePolicy().hasHeightForWidth());
        bottomBtn_5->setSizePolicy(sizePolicy1);
        bottomBtn_5->setMinimumSize(QSize(24, 24));
        bottomBtn_5->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_5);

        bottomBtn_6 = new QPushButton(bottomWidget);
        bottomBtn_6->setObjectName(QString::fromUtf8("bottomBtn_6"));
        sizePolicy1.setHeightForWidth(bottomBtn_6->sizePolicy().hasHeightForWidth());
        bottomBtn_6->setSizePolicy(sizePolicy1);
        bottomBtn_6->setMinimumSize(QSize(24, 24));
        bottomBtn_6->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_6);

        horizontalSpacer_2 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        bottomBtn_7 = new QPushButton(bottomWidget);
        bottomBtn_7->setObjectName(QString::fromUtf8("bottomBtn_7"));
        sizePolicy1.setHeightForWidth(bottomBtn_7->sizePolicy().hasHeightForWidth());
        bottomBtn_7->setSizePolicy(sizePolicy1);
        bottomBtn_7->setMinimumSize(QSize(24, 24));
        bottomBtn_7->setMaximumSize(QSize(86, 24));

        horizontalLayout_5->addWidget(bottomBtn_7);


        verticalLayout_5->addLayout(horizontalLayout_5);

        verticalLayout_5->setStretch(0, 1);
        verticalLayout_5->setStretch(1, 1);

        verticalLayout_6->addWidget(bottomWidget);


        retranslateUi(CCMainWindowClass);

        QMetaObject::connectSlotsByName(CCMainWindowClass);
    } // setupUi

    void retranslateUi(QWidget *CCMainWindowClass)
    {
        CCMainWindowClass->setWindowTitle(QCoreApplication::translate("CCMainWindowClass", "CCMainWindow", nullptr));
        titileIcon->setText(QString());
        placehold1->setText(QString());
        placehold2->setText(QString());
        sysmin->setText(QString());
        sysclose->setText(QString());
        headLabel->setText(QString());
        nameLabel->setText(QCoreApplication::translate("CCMainWindowClass", "\346\263\260\346\260\217\351\233\206\345\233\242", nullptr));
        statusBtn->setText(QString());
        levelBtn->setText(QString());
        lineEdit->setPlaceholderText(QCoreApplication::translate("CCMainWindowClass", "\344\270\252\346\200\247\347\255\276\345\220\215", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("CCMainWindowClass", "\346\220\234\347\264\242\357\274\232\350\201\224\347\263\273\344\272\272\343\200\201\347\276\244", nullptr));
        searchBtn->setText(QString());
        bottomBtn_1->setText(QString());
        bottomBtn_2->setText(QString());
        bottomBtn_3->setText(QString());
        bottomBtn_4->setText(QString());
        bottomBtn_5->setText(QString());
        bottomBtn_6->setText(QString());
        bottomBtn_7->setText(QCoreApplication::translate("CCMainWindowClass", "\345\272\224\347\224\250\345\256\235", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CCMainWindowClass: public Ui_CCMainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCMAINWINDOW_H
