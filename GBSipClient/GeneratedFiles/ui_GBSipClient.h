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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GBSipClientClass
{
public:
    QOpenGLWidget *openGLWidget;
    QTreeWidget *treeWidget;
    QTreeView *treeView;
    QTableView *tableView;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *GBSipClientClass)
    {
        if (GBSipClientClass->objectName().isEmpty())
            GBSipClientClass->setObjectName(QStringLiteral("GBSipClientClass"));
        GBSipClientClass->resize(954, 900);
        openGLWidget = new QOpenGLWidget(GBSipClientClass);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(280, 10, 651, 531));
        openGLWidget->setStyleSheet(QLatin1String("background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(0, 0, 0, 255), stop:0.19397 rgba(0, 0, 0, 255), stop:0.202312 rgba(122, 97, 0, 255), stop:0.495514 rgba(76, 58, 0, 255), stop:0.504819 rgba(255, 255, 255, 255), stop:0.79 rgba(255, 255, 255, 255), stop:1 rgba(255, 158, 158, 255));\n"
"selection-background-color: rgb(255, 56, 7);"));
        treeWidget = new QTreeWidget(GBSipClientClass);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(690, 560, 121, 51));
        treeWidget->setStyleSheet(QStringLiteral(""));
        treeView = new QTreeView(GBSipClientClass);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setGeometry(QRect(10, 10, 241, 531));
        tableView = new QTableView(GBSipClientClass);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(0, 690, 941, 192));
        pushButton = new QPushButton(GBSipClientClass);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(480, 570, 75, 23));
        pushButton_2 = new QPushButton(GBSipClientClass);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(582, 571, 75, 23));

        retranslateUi(GBSipClientClass);
        QObject::connect(treeView, SIGNAL(doubleClicked(QModelIndex)), GBSipClientClass, SLOT(treeDoubleCllicked(QModelIndex)));
        QObject::connect(treeView, SIGNAL(clicked(QModelIndex)), GBSipClientClass, SLOT(treeCllicked(QModelIndex)));
        QObject::connect(pushButton, SIGNAL(clicked()), GBSipClientClass, SLOT(queryCatalog()));

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
