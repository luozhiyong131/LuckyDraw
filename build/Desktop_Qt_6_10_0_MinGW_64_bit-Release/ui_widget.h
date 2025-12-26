/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *leftPanel;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *prizeLevelLabel;
    QLabel *joinCountLabel;
    QSpacerItem *spacerItem;
    QWidget *prizeContainer;
    QVBoxLayout *prizeVLayout;
    QLabel *giftLabel;
    QLabel *prizeNameLabel;
    QSpacerItem *spacerItem1;
    QLabel *roundLabel;
    QSpacerItem *verticalSpacer;
    QPushButton *startDrawButton;
    QPushButton *btnSettings;
    QSpacerItem *horizontalSpacer;
    QWidget *rightPanel;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *rightTitle;
    QGroupBox *groupBox;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(1366, 1026);
        Widget->setStyleSheet(QString::fromUtf8("\n"
"    /* \350\277\231\351\207\214\347\232\204\346\270\220\345\217\230\346\230\257\346\225\264\344\270\252\347\252\227\345\217\243\347\232\204\346\234\200\345\272\225\345\261\202\350\203\214\346\231\257 */\n"
"    QWidget#Widget {\n"
"        background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"            stop:0 #ff6b6b, stop:0.5 #f06595, stop:1 #c44569);\n"
"    }\n"
"   "));
        horizontalLayout = new QHBoxLayout(Widget);
        horizontalLayout->setSpacing(30);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(40, 40, 40, 40);
        leftPanel = new QWidget(Widget);
        leftPanel->setObjectName("leftPanel");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(4);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leftPanel->sizePolicy().hasHeightForWidth());
        leftPanel->setSizePolicy(sizePolicy);
        leftPanel->setStyleSheet(QString::fromUtf8("\n"
"       QWidget#leftPanel {\n"
"           /* \346\224\271\344\270\272\345\215\212\351\200\217\346\230\216\347\231\275\350\211\262\357\274\214\344\270\216\345\217\263\344\276\247\344\277\235\346\214\201\344\270\200\350\207\264 */\n"
"           background: rgba(255, 255, 255, 0.1);\n"
"           /* \346\267\273\345\212\240\347\273\206\345\276\256\347\232\204\345\215\212\351\200\217\346\230\216\350\276\271\346\241\206\345\242\236\345\212\240\351\253\230\347\272\247\346\204\237 */\n"
"           border: 1px solid rgba(255, 255, 255, 0.15);\n"
"           border-radius: 40px;\n"
"       }\n"
"      "));
        gridLayout = new QGridLayout(leftPanel);
        gridLayout->setObjectName("gridLayout");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        prizeLevelLabel = new QLabel(leftPanel);
        prizeLevelLabel->setObjectName("prizeLevelLabel");
        prizeLevelLabel->setMinimumSize(QSize(0, 88));
        prizeLevelLabel->setStyleSheet(QString::fromUtf8("\n"
"            font-family: \"Microsoft YaHei\"; font-size: 48px; font-weight: 900; color: #ffffff;\n"
"           "));
        prizeLevelLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(prizeLevelLabel);

        joinCountLabel = new QLabel(leftPanel);
        joinCountLabel->setObjectName("joinCountLabel");
        joinCountLabel->setMinimumSize(QSize(0, 66));
        joinCountLabel->setStyleSheet(QString::fromUtf8("\n"
"            font-size: 22px; color: rgba(255,255,255,0.8); font-weight: bold;\n"
"           "));
        joinCountLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(joinCountLabel);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 3);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(spacerItem, 1, 0, 1, 3);

        prizeContainer = new QWidget(leftPanel);
        prizeContainer->setObjectName("prizeContainer");
        prizeVLayout = new QVBoxLayout(prizeContainer);
        prizeVLayout->setSpacing(10);
        prizeVLayout->setObjectName("prizeVLayout");
        giftLabel = new QLabel(prizeContainer);
        giftLabel->setObjectName("giftLabel");
        giftLabel->setMinimumSize(QSize(320, 320));
        giftLabel->setStyleSheet(QString::fromUtf8("background: transparent;"));
        giftLabel->setPixmap(QPixmap(QString::fromUtf8(":/img/gift.png")));
        giftLabel->setScaledContents(false);
        giftLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        prizeVLayout->addWidget(giftLabel);

        prizeNameLabel = new QLabel(prizeContainer);
        prizeNameLabel->setObjectName("prizeNameLabel");
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 0));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush1);
        QBrush brush2(QColor(255, 0, 0, 128));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush2);
#endif
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush2);
#endif
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush2);
#endif
        prizeNameLabel->setPalette(palette);
        prizeNameLabel->setStyleSheet(QString::fromUtf8("\n"
"             font-size: 42px; color: #FF0000; font-weight: bold;\n"
"             background: transparent;\n"
"            "));
        prizeNameLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        prizeVLayout->addWidget(prizeNameLabel);


        gridLayout->addWidget(prizeContainer, 3, 0, 1, 3);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(spacerItem1, 4, 0, 1, 3);

        roundLabel = new QLabel(leftPanel);
        roundLabel->setObjectName("roundLabel");
        roundLabel->setStyleSheet(QString::fromUtf8("\n"
"          font-size: 28px; color: #ffffff; font-weight: bold;\n"
"         "));
        roundLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(roundLabel, 5, 0, 1, 3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer, 6, 0, 1, 1);

        startDrawButton = new QPushButton(leftPanel);
        startDrawButton->setObjectName("startDrawButton");
        startDrawButton->setMinimumSize(QSize(0, 90));
        startDrawButton->setStyleSheet(QString::fromUtf8("\n"
"          QPushButton {\n"
"              background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #feca57, stop:1 #ff9f43);\n"
"              color: #2d3436; font-size: 36px; font-weight: bold;\n"
"              border-radius: 45px; border-bottom: 5px solid #e67e22;\n"
"          }\n"
"          QPushButton:hover { background: #ffbd4a; }\n"
"          QPushButton:pressed { border-bottom: 1px solid #e67e22; margin-top: 4px; }\n"
"         "));

        gridLayout->addWidget(startDrawButton, 7, 0, 1, 3);

        btnSettings = new QPushButton(leftPanel);
        btnSettings->setObjectName("btnSettings");
        btnSettings->setMinimumSize(QSize(68, 38));
        btnSettings->setMaximumSize(QSize(16777215, 16777215));
        btnSettings->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgba(255, 255, 255, 100);\n"
"    border: 1px solid rgba(255, 255, 255, 150);\n"
"    border-radius: 5px;\n"
"    color: #FFFFFF;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgba(255, 255, 255, 150);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: rgba(200, 200, 200, 100);\n"
"}"));

        gridLayout->addWidget(btnSettings, 8, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(391, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 8, 2, 1, 1);


        horizontalLayout->addWidget(leftPanel);

        rightPanel = new QWidget(Widget);
        rightPanel->setObjectName("rightPanel");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(6);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(rightPanel->sizePolicy().hasHeightForWidth());
        rightPanel->setSizePolicy(sizePolicy1);
        rightPanel->setStyleSheet(QString::fromUtf8("\n"
"       QWidget#rightPanel {\n"
"           background: rgba(255, 255, 255, 0.1);\n"
"           border: 1px solid rgba(255, 255, 255, 0.15);\n"
"           border-radius: 40px;\n"
"       }\n"
"      "));
        gridLayout_2 = new QGridLayout(rightPanel);
        gridLayout_2->setObjectName("gridLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
        rightTitle = new QLabel(rightPanel);
        rightTitle->setObjectName("rightTitle");
        rightTitle->setMinimumSize(QSize(0, 88));
        rightTitle->setStyleSheet(QString::fromUtf8("\n"
"            font-family: \"Microsoft YaHei\"; font-size: 48px; font-weight: 900; color: #ffffff;\n"
"           "));
        rightTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(rightTitle);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        groupBox = new QGroupBox(rightPanel);
        groupBox->setObjectName("groupBox");
        groupBox->setMinimumSize(QSize(0, 770));

        gridLayout_2->addWidget(groupBox, 1, 0, 1, 1);


        horizontalLayout->addWidget(rightPanel);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "\345\271\270\350\277\220\346\212\275\345\245\226", nullptr));
        prizeLevelLabel->setText(QCoreApplication::translate("Widget", "\342\200\224 \344\270\200\347\255\211\345\245\226 \342\200\224", nullptr));
        joinCountLabel->setText(QCoreApplication::translate("Widget", "\345\217\202\344\270\216\344\272\272\346\225\260: 6,748", nullptr));
        prizeNameLabel->setText(QCoreApplication::translate("Widget", "\350\213\271\346\236\234\346\211\213\346\234\272", nullptr));
        roundLabel->setText(QCoreApplication::translate("Widget", "\347\254\254 2 / 3 \350\275\256", nullptr));
        startDrawButton->setText(QCoreApplication::translate("Widget", "\347\253\213\345\215\263\345\274\200\345\245\226", nullptr));
        btnSettings->setText(QCoreApplication::translate("Widget", "\345\245\226\351\241\271\351\205\215\347\275\256", nullptr));
        rightTitle->setText(QCoreApplication::translate("Widget", "\345\200\231\351\200\211\345\220\215\345\215\225", nullptr));
        groupBox->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
