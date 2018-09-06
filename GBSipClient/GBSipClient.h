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

private:
	Ui::GBSipClientClass ui;
};
