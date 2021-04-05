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
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLCDNumber *tick_count;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *button_start;
    QPushButton *button_stop;

    void setupUi(QDialog *Synchronous_Dialog)
    {
        if (Synchronous_Dialog->objectName().isEmpty())
            Synchronous_Dialog->setObjectName(QStringLiteral("Synchronous_Dialog"));
        Synchronous_Dialog->resize(414, 228);
        widget = new QWidget(Synchronous_Dialog);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(30, 190, 371, 26));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        tick_count = new QLCDNumber(widget);
        tick_count->setObjectName(QStringLiteral("tick_count"));

        horizontalLayout_2->addWidget(tick_count);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        button_start = new QPushButton(widget);
        button_start->setObjectName(QStringLiteral("button_start"));

        horizontalLayout->addWidget(button_start);

        button_stop = new QPushButton(widget);
        button_stop->setObjectName(QStringLiteral("button_stop"));

        horizontalLayout->addWidget(button_stop);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(Synchronous_Dialog);

        QMetaObject::connectSlotsByName(Synchronous_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Synchronous_Dialog)
    {
        Synchronous_Dialog->setWindowTitle(QApplication::translate("Synchronous_Dialog", "Dialog", Q_NULLPTR));
        button_start->setText(QApplication::translate("Synchronous_Dialog", "Start", Q_NULLPTR));
        button_stop->setText(QApplication::translate("Synchronous_Dialog", "Stop", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Synchronous_Dialog: public Ui_Synchronous_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYNCHRONOUS_DIALOG_H
