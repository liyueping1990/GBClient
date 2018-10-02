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
    QPushButton *btn_queryCatalog;
    QPushButton *btn_refreshCatalog;

    void setupUi(QWidget *GBSipClientClass)
    {
        if (GBSipClientClass->objectName().isEmpty())
            GBSipClientClass->setObjectName(QStringLiteral("GBSipClientClass"));
        GBSipClientClass->resize(958, 900);
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
        treeWidget->setGeometry(QRect(690, 560, 121, 31));
        treeWidget->setStyleSheet(QStringLiteral(""));
        treeView = new QTreeView(GBSipClientClass);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setGeometry(QRect(10, 10, 241, 531));
        tableView = new QTableView(GBSipClientClass);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(40, 640, 851, 192));
        btn_queryCatalog = new QPushButton(GBSipClientClass);
        btn_queryCatalog->setObjectName(QStringLiteral("btn_queryCatalog"));
        btn_queryCatalog->setGeometry(QRect(30, 560, 75, 23));
        btn_refreshCatalog = new QPushButton(GBSipClientClass);
        btn_refreshCatalog->setObjectName(QStringLiteral("btn_refreshCatalog"));
        btn_refreshCatalog->setGeometry(QRect(130, 560, 75, 23));

        retranslateUi(GBSipClientClass);
        QObject::connect(treeView, SIGNAL(doubleClicked(QModelIndex)), GBSipClientClass, SLOT(treeDoubleCllicked(QModelIndex)));
        QObject::connect(treeView, SIGNAL(clicked(QModelIndex)), GBSipClientClass, SLOT(treeCllicked(QModelIndex)));
        QObject::connect(btn_queryCatalog, SIGNAL(clicked()), GBSipClientClass, SLOT(queryCatalog()));
        QObject::connect(btn_refreshCatalog, SIGNAL(clicked()), GBSipClientClass, SLOT(refreshCatalog()));

        QMetaObject::connectSlotsByName(GBSipClientClass);
    } // setupUi

    void retranslateUi(QWidget *GBSipClientClass)
    {
        GBSipClientClass->setWindowTitle(QApplication::translate("GBSipClientClass", "GBSipClient", Q_NULLPTR));
        btn_queryCatalog->setText(QApplication::translate("GBSipClientClass", "\347\233\256\345\275\225\346\237\245\350\257\242", Q_NULLPTR));
        btn_refreshCatalog->setText(QApplication::translate("GBSipClientClass", "\345\210\267\346\226\260\347\233\256\345\275\225", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GBSipClientClass: public Ui_GBSipClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GBSIPCLIENT_H
