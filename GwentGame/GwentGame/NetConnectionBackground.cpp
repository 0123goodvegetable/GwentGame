#include "NetConnectionBackground.h"


NetConnectionBackground::NetConnectionBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//初始化
	init();

	//当下拉列表中的值变化时，改变其他控件选项
	connect(chooseClientOrServerBox, &QComboBox::currentTextChanged, this, &NetConnectionBackground::changeUI);
}

NetConnectionBackground::~NetConnectionBackground()
{
}

void NetConnectionBackground::init()
{
	//设置各控件的属性及位置
	chooseClientOrServerBox = new QComboBox(this);
	IPLabel = new QLabel("IP:", this);
	hostLabel = new QLabel("host:", this);
	IPLineEdit = new QLineEdit(this);
	hostLineEdit = new QLineEdit(this);
	toConnectButton = new QPushButton(this);
	layout = new QGridLayout(this);
	centralWidget = new QWidget(this);

	layout->setColumnStretch(0, 1);
	layout->setColumnStretch(1, 3);

	IPLabel->setFrameShape(QFrame::Panel);
	IPLabel->setFrameShadow(QFrame::Sunken);
	IPLabel->setStyleSheet("background-color:rgb(255,255,255)");
	hostLabel->setFrameShape(QFrame::Panel);
	hostLabel->setFrameShadow(QFrame::Sunken);
	hostLabel->setStyleSheet("background-color:rgb(255,255,255)");

	toConnectButton->setStyleSheet("background-color:rgb(255,255,255)");
	toConnectButton->setText("waiting for connect...");
	
	chooseClientOrServerBox->addItem(tr("Client"));
	chooseClientOrServerBox->addItem(tr("Server"));

	layout->addWidget(chooseClientOrServerBox,0,1);
	layout->addWidget(IPLabel, 1, 0);
	layout->addWidget(hostLabel, 2, 0);
	layout->addWidget(IPLineEdit, 1, 1);
	layout->addWidget(hostLineEdit, 2, 1);
	layout->addWidget(toConnectButton,3,1);
	
	centralWidget->setLayout(layout);
	centralWidget->setGeometry(700, 300, 400, 400);
	centralWidget->setStyleSheet(QString::fromUtf8("border:5px solid white"));
	centralWidget->raise();

}

void NetConnectionBackground::changeUI()
{
	if (chooseClientOrServerBox->currentText() == "Client")
	{
		toConnectButton->setText("waiting for connect...");
	}
	else
	{
		toConnectButton->setText("to connect");
	}
}
