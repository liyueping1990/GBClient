#include "GBSipClient.h"
#include <QStandardItemModel>
#include <QList>
#include <QMessageBox>
//#pragma execution_character_set("utf-8")

#define ORG_PNG ":/GBSipClient/Resources/org.png"
#define QIU_PNG ":/GBSipClient/Resources/qiu.png"

GBSipClient::GBSipClient(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	mSipDB = new SipDBServer;
	mSipMgr = SipMgr::GetInstance();

	CreateCatalogTree();
	CreatePlatformTable();

	//connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(showSelectedImage(QTreeWidgetItem*, int)));

	//// ����sip
	//mSipMgr = SipMgr::GetInstance();
	//mSipMgr->Start();
}


void GBSipClient::CreateCatalogTree()
{
	GroupInfoList groupInfoList;
	mSipDB = new SipDBServer;
	mSipDB->GetCatalogRootNode(groupInfoList);

	QStandardItemModel* rootModel = new QStandardItemModel(ui.treeView);
	rootModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("��Դ��")/* << QStringLiteral("��Դ��")*/);
	ui.treeView->setModel(rootModel);

	for (auto item : groupInfoList)
	{
		QString name = QString::fromUtf8(item->name.c_str());
		QStandardItem* itemName = new QStandardItem(QIcon(ORG_PNG), name);
		itemName->setEditable(false);
		itemName->setCheckable(true);
		itemName->setToolTip(item->deviceID.c_str());
		rootModel->appendRow(itemName);
		rootModel->setItem(rootModel->indexFromItem(itemName).row(), 1, new QStandardItem(QString(item->deviceID.c_str())));
		rootModel->setItem(rootModel->indexFromItem(itemName).row(), 2, new QStandardItem(QString(item->groupID)));
	}
	ui.treeView->hideColumn(1);
	ui.treeView->hideColumn(2);
}

void GBSipClient::CreatePlatformTable()
{
	QStandardItemModel* rootModel = new QStandardItemModel(ui.tableView);
	rootModel->setHorizontalHeaderLabels(QStringList()
		<< QStringLiteral("ID")
		<< QStringLiteral("����")
		<< QStringLiteral("����") 
		<< QStringLiteral("IP��ַ") 
		<< QStringLiteral("�˿ں�")
		<< QStringLiteral("����״̬")
		<< QStringLiteral("״̬"));
	
	PlatformInfoList platformInfoList;
	mSipDB->GetAllPlatfromInfo(platformInfoList);
	for each (auto item in platformInfoList)
	{
		QStandardItem* idItem = new QStandardItem(QString::number(item->id));
		rootModel->appendRow(idItem);

		rootModel->setItem(idItem->row(), 1, new QStandardItem(item->name.c_str()));
		rootModel->setItem(idItem->row(), 2, new QStandardItem(item->deviceID.c_str()));
		rootModel->setItem(idItem->row(), 3, new QStandardItem(item->ip.c_str()));
		rootModel->setItem(idItem->row(), 4, new QStandardItem(QString::number(item->port)));
		rootModel->setItem(idItem->row(), 5, new QStandardItem(QString::number(item->upOrDown)));
		rootModel->setItem(idItem->row(), 6, new QStandardItem(item->status.c_str()));
	}
	
	ui.tableView->setModel(rootModel);
}


void GBSipClient::showSelectedImage(QTreeWidgetItem * item, int column)
{
	QTreeWidgetItem *parent = item->parent();
	if (NULL == parent) //ע�⣺�������û�и��ڵ�ģ�˫����Щ��ʱע��(����)
		return;
	int col = parent->indexOfChild(item); //item�ڸ����еĽڵ��к�(��0��ʼ)
	item->setText(1, "dd");
	QString ss = item->text(1);
	if (0 == col) //Band1
	{
		//ִ�ж�Ӧ����
	}
	if (1 == col) //Band2
	{
		//ִ�ж�Ӧ����
	}
}

void GBSipClient::treeDoubleCllicked(const QModelIndex &index)
{
	QStandardItemModel* m = (QStandardItemModel*)index.model();
	QStandardItem* currentItem = m->itemFromIndex(index);
	QStandardItem* parentItem = currentItem->parent();

	QStandardItem* deviceIDItem = nullptr;
	if (parentItem)
	{
		deviceIDItem = parentItem->child(currentItem->row(), 1);
	}
	else
	{
		deviceIDItem = m->item(currentItem->row(), 1);
	}


	QString deviceID = deviceIDItem->text();
	if (deviceID.length() < 10 || (deviceID.length() == 20 && (deviceID.mid(10, 3) == "215" || deviceID.mid(10, 3) == "216")))
	{
		bool hasChildren = currentItem->hasChildren();
		if (!hasChildren)
		{
			// �������
			GroupInfoList groupInfoList;
			mSipDB->GetGroupChildren(deviceID.toStdString(), groupInfoList);
			for each (auto item in groupInfoList)
			{				
				QString name = QString::fromUtf8(item->name.c_str());
				QStandardItem* itemName = new QStandardItem(QIcon(ORG_PNG), name);
				itemName->setEditable(false);
				itemName->setCheckable(true);
				currentItem->appendRow(itemName);
				
				currentItem->setChild(itemName->row(), 1, new QStandardItem(QString(item->deviceID.c_str())));
				currentItem->setChild(itemName->row(), 2, new QStandardItem(QString(item->groupID)));
			}

			// ��������
			CameraInfoList cameraInfoList;
			mSipDB->GetCameraChildren(deviceID.toStdString(), cameraInfoList);
			for each (auto item in cameraInfoList)
			{
				QString name = QString::fromUtf8(item->name.c_str());
				QString pngName = QIU_PNG;
				if (item->status == "ON")
				{
					switch (item->ptzType)
					{
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						break;
					default:
						break;
					}
				} 
				else
				{
					switch (item->ptzType)
					{
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						break;
					default:
						break;
					}
				}
				QStandardItem* itemName = new QStandardItem(QIcon(pngName), name);
				itemName->setEditable(false);
				itemName->setCheckable(true);
				currentItem->appendRow(itemName);

				currentItem->setChild(itemName->row(), 1, new QStandardItem(QString(item->deviceID.c_str())));
				currentItem->setChild(itemName->row(), 2, new QStandardItem(QString(item->status.c_str())));
			}
		}
	}
	else // ������Ƶ
	{
		mSipMgr->Play(deviceID.toStdString());
	}
}

void GBSipClient::treeCllicked(const QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle(QStringLiteral("��ʾ"));
	msgBox.setText(QStringLiteral("��δѡ����Ҫ��ѯĿ¼ƽ̨���豸"));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}

void GBSipClient::queryCatalog()
{
	// ��ȡ��ǰѡ�е���
	QModelIndex curentIndex = ui.tableView->currentIndex();
	QStandardItemModel* model = (QStandardItemModel*)curentIndex.model();
	if (model)
	{
		QStandardItem* deviceIdItem = model->item(curentIndex.row(), 2);

		// ��ȡƽ̨DeviceID�����¼�����Ŀ¼��ѯ
		QString deviceID = deviceIdItem->text();
		mSipMgr->QueryCatalog(deviceID.toStdString());
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QStringLiteral("��ʾ"));
		msgBox.setText(QStringLiteral("��δѡ����Ҫ��ѯĿ¼ƽ̨���豸"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.exec();
	}
}

void GBSipClient::refreshCatalog()
{
	QStandardItemModel* model = (QStandardItemModel*)ui.treeView->model();
	model->clear();
	delete model;
	model = nullptr;

	CreateCatalogTree();
}
