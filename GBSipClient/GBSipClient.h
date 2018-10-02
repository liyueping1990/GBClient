#pragma once
#include "SipMgr.h"
#include "PlayM4.h"
#include <QtWidgets/QWidget>
#include "ui_GBSipClient.h"

#pragma comment(lib, "PlayCtrl.lib")
class GBSipClient : public QWidget
{
	Q_OBJECT

public:
	GBSipClient(QWidget *parent = Q_NULLPTR);

	SipDBServer* mSipDB;
		
private slots:
	void showSelectedImage(QTreeWidgetItem * item, int column); //点击树节点事件;

	// TreeView
	void treeDoubleCllicked(const QModelIndex &index); //点击树节点事件;
	void treeCllicked(const QModelIndex &index); //点击树节点事件;
	void queryCatalog();

private:
	Ui::GBSipClientClass ui;
	SipMgr* mSipMgr;
	void CreateCatalogTree();
	void CreatePlatformTable();
};
