#include "NetConnectionBackground.h"


NetConnectionBackground::NetConnectionBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//初始化
	init();

	//当下拉列表中的值变化时，改变其他控件选项
	connect(chooseClientOrServerBox, &QComboBox::currentTextChanged, this, &NetConnectionBackground::changeUI);

	//点击连接按钮后进行连接
	connect(toConnectButton, SIGNAL(clicked()), this, SLOT(createConnection()));
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
	
	chooseClientOrServerBox->addItem(tr("waiting choose..."));
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

	//搜索IP地址
	/* QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses(); 
	 for (int i = 0; i< ipAddressesList.size(); ++i) 
	 {     
		 // use the first non-localhostIPv4 address 
		 if (ipAddressesList.at(i) != QHostAddress::LocalHost&& 
			 ipAddressesList.at(i).toIPv4Address()) 
		 { 
			 ipAddress= ipAddressesList.at(i).toString();
			 break; 
		 } 
	 } 
	 if (ipAddress.isEmpty())     // if we did not find one, use IPv4 localhost 
		 ipAddress= QHostAddress(QHostAddress::LocalHost).toString(); */

	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	foreach(QHostAddress address, ipAddressesList)
	{
		if (address.protocol() == QAbstractSocket::IPv4Protocol&&
			address != QHostAddress::Null&&
			address != QHostAddress::LocalHost)
		{
			if (address.toString().contains("127.0."))
			{
				continue;
			}
		}
		ipAddress = address.toString();
		break;
	}

	QString localHostName = QHostInfo::localHostName();
	QHostInfo info = QHostInfo::fromName(localHostName);
	info.addresses();//QHostInfo的address函数获取本机ip地址  
	//如果存在多条ip地址ipv4和ipv6：  
	foreach(QHostAddress address, info.addresses())
	{
		if (address.protocol() == QAbstractSocket::IPv4Protocol) 
		{
			//只取ipv4协议的地址  
			ipAddress = address.toString();
		}
	}

}

void NetConnectionBackground::changeUI()
{
	if (chooseClientOrServerBox->currentText() == "Client")
	{
		toConnectButton->setText("to connect");
		IPLineEdit->clear();
		hostLineEdit->clear();
		IPLineEdit->setFocusPolicy(Qt::ClickFocus);
		hostLineEdit->setFocusPolicy(Qt::ClickFocus);
		IPLineEdit->setStyleSheet("background-color:rgb(255,255,255)");
		hostLineEdit->setStyleSheet("background-color:rgb(255,255,255)");
	}
	else if(chooseClientOrServerBox->currentText() == "Server")
	{
		toConnectButton->setText("waiting for connect...");
		IPLineEdit->setText(ipAddress);
		hostLineEdit->setText(QString::number(6666));
		IPLineEdit->setFocusPolicy(Qt::NoFocus);
		hostLineEdit->setFocusPolicy(Qt::NoFocus);
		IPLineEdit->setStyleSheet("background-color:rgb(200,200,200)");
		hostLineEdit->setStyleSheet("background-color:rgb(200,200,200)");
	}
}

void NetConnectionBackground::createConnection()
{
	if (chooseClientOrServerBox->currentText() == "Client")
	{
		emit connectToServer(IPLineEdit->text(), hostLineEdit->text().toInt());
	}

	if (chooseClientOrServerBox->currentText() == "Server")
	{
		emit connectToClient(IPLineEdit->text(), hostLineEdit->text().toInt());
	}

	toConnectButton->setStyleSheet("background-color:rgb(200,200,200)");
}

void NetConnectionBackground::resizeEvent(QResizeEvent*event)
{
	Q_UNUSED(event)//避免编译器警告

	this->setAutoFillBackground(true);//自动填充背景

	//添加背景
	QImage image;
	QPalette palette;
	image.load(":/backgrounds/Resources/backgrounds/netConnection_background.jpg");//加载背景图片

	palette.setBrush(this->backgroundRole(),
		QBrush(image.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));//设置背景画刷

	this->setPalette(palette);
}
