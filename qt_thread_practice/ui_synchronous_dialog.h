/********************************************************************************
** Form generated from reading UI file 'synchronous_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYNCHRONOUS_DIALOG_H
#define UI_SYNCHRONOUS_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Synchronous_Dialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QLCDNumber *tick_count;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *button_start;

    void setupUi(QDialog *Synchronous_Dialog)
    {
        if (Synchronous_Dialog->objectName().isEmpty())
            Synchronous_Dialog->setObjectName(QStringLiteral("Synchronous_Dialog"));
        Synchronous_Dialog->resize(414, 228);
        layoutWidget = new QWidget(Synchronous_Dialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 190, 371, 26));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        tick_count = new QLCDNumber(layoutWidget);
        tick_count->setObjectName(QStringLiteral("tick_count"));
        tick_count->setAutoFillBackground(false);
        tick_count->setDigitCount(9);

        horizontalLayout_2->addWidget(tick_count);

        horizontalSpacer_2 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        button_start = new QPushButton(layoutWidget);
        button_start->setObjectName(QStringLiteral("button_start"));

        horizontalLayout->addWidget(button_start);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(Synchronous_Dialog);

        QMetaObject::connectSlotsByName(Synchronous_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Synchronous_Dialog)
    {
        Synchronous_Dialog->setWindowTitle(QApplication::translate("Synchronous_Dialog", "Dialog", Q_NULLPTR));
        button_start->setText(QApplication::translate("Synchronous_Dialog", "Start", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Synchronous_Dialog: public Ui_Synchronous_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYNCHRONOUS_DIALOG_H
