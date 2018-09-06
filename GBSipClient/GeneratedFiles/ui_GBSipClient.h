/********************************************************************************
** Form generated from reading UI file 'GBSipClient.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GBSIPCLIENT_H
#define UI_GBSIPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GBSipClientClass
{
public:
    QOpenGLWidget *openGLWidget;
    QTreeWidget *treeWidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *GBSipClientClass)
    {
        if (GBSipClientClass->objectName().isEmpty())
            GBSipClientClass->setObjectName(QStringLiteral("GBSipClientClass"));
        GBSipClientClass->resize(1200, 900);
        openGLWidget = new QOpenGLWidget(GBSipClientClass);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(280, 10, 651, 531));
        treeWidget = new QTreeWidget(GBSipClientClass);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(10, 11, 261, 531));
        widget = new QWidget(GBSipClientClass);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(500, 570, 158, 25));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        retranslateUi(GBSipClientClass);

        QMetaObject::connectSlotsByName(GBSipClientClass);
    } // setupUi

    void retranslateUi(QWidget *GBSipClientClass)
    {
        GBSipClientClass->setWindowTitle(QApplication::translate("GBSipClientClass", "GBSipClient", Q_NULLPTR));
        pushButton->setText(QApplication::translate("GBSipClientClass", "\347\233\256\345\275\225\346\237\245\350\257\242", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("GBSipClientClass", "\346\222\255\346\224\276", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GBSipClientClass: public Ui_GBSipClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GBSIPCLIENT_H
