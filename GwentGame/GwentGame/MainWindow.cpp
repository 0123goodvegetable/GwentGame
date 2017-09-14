#include "MainWindow.h"

#define max(a,b) a>b?a:b

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	init();//��ʼ���������

	//��������������
	this->resize(1800, 1000);
	this->setWindowOpacity(1);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowTitle("Gwent Game");

	connect(this, SIGNAL(changeBackgroundNo(int)), this, SLOT(updateBackground(int)));//��ʱ���³���

	//��ť����Ļ�л��źŲ�
	connect(gameSelectionBackground->playWithAI_button, SIGNAL(clicked()), 
		this, SLOT(toCardsSelectionBackground()));
	connect(gameSelectionBackground->editCardsDeck_button, SIGNAL(clicked()), 
		this, SLOT(toCardsEditBackground()));
	connect(cardsEditBackground->saveAndQuit_button, SIGNAL(clicked()), 
		this, SLOT(toGameSelectionBackground()));
	connect(gameSelectionBackground->playWithPlayer_button, SIGNAL(clicked()), 
		this, SLOT(toNetConnectionBackground()));
	connect(gameEndBackground->close_button, SIGNAL(clicked()),
		this, SLOT(toGameSelectionBackground()));
	connect(gameSelectionBackground->quit_button, SIGNAL(clicked()),
		this, SLOT(close()));

	connect(cardsSelectionBackground->cardsSelectionFinished_button, &QPushButton::clicked,
		myClient, &MyClient::MeReady);
	connect(cardsSelectionBackground->cardsSelectionFinished_button, &QPushButton::clicked,
		myServer, &MyServer::MeReady);

	//������������Ļ�л��źŲ�
	connect(myServer, &MyServer::changeBackground, 
		this, &MainWindow::toCardsSelectionBackground);
	connect(myClient, &MyClient::changeBackground, 
		this, &MainWindow::toCardsSelectionBackground);
	connect(myServer, &MyServer::toPlayBackground,
		this, &MainWindow::toGamePlayingBackground);
	connect(myClient, &MyClient::toPlayBackground,
		this, &MainWindow::toGamePlayingBackground);
	connect(myServer, &MyServer::toCardsSelectionBackground,
		this, &MainWindow::toCardsSelectionBackground);
	connect(myClient, &MyClient::toCardsSelectionBackground,
		this, &MainWindow::toCardsSelectionBackground);
	connect(myServer, &MyServer::toGameEndBackground,
		this, &MainWindow::toGameEndBackground);
	connect(myClient, &MyClient::toGameEndBackground,
		this, &MainWindow::toGameEndBackground);

	connect(myClient, &MyClient::toTellFinal,
		gameEndBackground, &GameEndBackground::receiveFinal);
	connect(myServer, &MyServer::toTellFinal,
		gameEndBackground, &GameEndBackground::receiveFinal);

	//������������
	connect(netConnectionBackground, &NetConnectionBackground::connectToClient, 
		myServer, &MyServer::startlisten);
	connect(netConnectionBackground, &NetConnectionBackground::connectToServer, 
		myClient, &MyClient::tryToConnect);

}

MainWindow::~MainWindow()
{

}

void MainWindow::init()
{
	//��ʼ���������
	BackgroundController = new QStackedWidget(this);

	isServer = false;
	isClient = false;
	myServer = new MyServer();
	myClient = new MyClient();

	beginBackground = new BeginBackground(this);
	gameSelectionBackground = new GameSelectionBackground(this);
	cardsSelectionBackground = new CardsSelectionBackground(max(myClient->turn, myServer->turn),this);
	cardsEditBackground = new CardsEditBackground(this);
	gamePlayingBackground = new GamePlayingBackground(max(myClient->turn, myServer->turn), this);
	netConnectionBackground = new NetConnectionBackground(this);
	gameEndBackground = new GameEndBackground(this);



	this->setCentralWidget(BackgroundController);//��ҳ������Ϊ���Ĵ���

	//��������뵽�����������
	BackgroundController->addWidget(beginBackground);
	BackgroundController->addWidget(gameSelectionBackground);
	BackgroundController->addWidget(cardsSelectionBackground);
	BackgroundController->addWidget(cardsEditBackground);
	BackgroundController->addWidget(gamePlayingBackground);
	BackgroundController->addWidget(netConnectionBackground);
	BackgroundController->addWidget(gameEndBackground);

	BackgroundNo = 0;

	//������Ľ�������Ϊ��ʼ����
	BackgroundController->setCurrentIndex(BackgroundNo);

}

void MainWindow::updateBackground(int number)
{
	BackgroundController->setCurrentIndex(number);
}

void MainWindow::toBeginBackground()
{
	BackgroundNo = 0;
	emit changeBackgroundNo(BackgroundNo);
}

void MainWindow::toGameSelectionBackground()
{
	BackgroundNo = 1;
	emit changeBackgroundNo(BackgroundNo);
}

void MainWindow::toCardsSelectionBackground()
{
	delete cardsSelectionBackground;
	cardsSelectionBackground = new CardsSelectionBackground(max(myClient->turn, myServer->turn),this);
	BackgroundController->insertWidget(2, cardsSelectionBackground);

	if (myClient->isChoosed == true)
	{
		connect(cardsSelectionBackground->cardsSelectionFinished_button, &QPushButton::clicked,
			myClient, &MyClient::MeReady);
	}
	if (myServer->isChoosed == true)
	{
		connect(cardsSelectionBackground->cardsSelectionFinished_button, &QPushButton::clicked,
			myServer, &MyServer::MeReady);
	}
	BackgroundNo = 2;
	emit changeBackgroundNo(BackgroundNo);
}

void MainWindow::toCardsEditBackground()
{

	BackgroundNo = 3;
	emit changeBackgroundNo(BackgroundNo);
}

void MainWindow::toGamePlayingBackground()
{
	delete gamePlayingBackground;
	gamePlayingBackground = new GamePlayingBackground(max(myClient->turn, myServer->turn), this);
	BackgroundController->insertWidget(4,gamePlayingBackground);
	if (myClient->isChoosed == true)
	{
		connect(gamePlayingBackground, &GamePlayingBackground::toSendFile,
			myClient, &MyClient::sendFile);
		connect(myClient, &MyClient::receiveFinished,
			gamePlayingBackground, &GamePlayingBackground::getFromEnemyText);
		connect(myClient, &MyClient::changeTurn,
			gamePlayingBackground, &GamePlayingBackground::changeMyTurn);
		connect(gamePlayingBackground, &GamePlayingBackground::chooseEnd,
			myClient, &MyClient::MeEnd);
		connect(myClient, &MyClient::toKnowEnemyEnd,
			gamePlayingBackground, &GamePlayingBackground::enemyEnd);
		connect(gamePlayingBackground, &GamePlayingBackground::sendFinal,
			myClient, &MyClient::getFinal);

	}
	if (myServer->isChoosed == true)
	{
		connect(gamePlayingBackground, &GamePlayingBackground::toSendFile,
			myServer, &MyServer::sendFile);
		connect(myServer, &MyServer::receiveFinished,
			gamePlayingBackground, &GamePlayingBackground::getFromEnemyText);
		connect(myServer, &MyServer::changeTurn,
			gamePlayingBackground, &GamePlayingBackground::changeMyTurn);
		connect(gamePlayingBackground, &GamePlayingBackground::chooseEnd,
			myServer, &MyServer::MeEnd);
		connect(myServer, &MyServer::toKnowEnemyEnd,
			gamePlayingBackground, &GamePlayingBackground::enemyEnd);
		connect(gamePlayingBackground, &GamePlayingBackground::sendFinal,
			myServer, &MyServer::getFinal);
	}
	srand((unsigned)time(NULL));
	if (rand() % 100 < 50)
	{
		emit myServer->changeTurn();
	}
	else
	{
		emit myClient->changeTurn();
	}
	BackgroundNo = 4;
	emit changeBackgroundNo(BackgroundNo);
}

void MainWindow::toNetConnectionBackground()
{

	BackgroundNo = 5;
	emit changeBackgroundNo(BackgroundNo);
}

void MainWindow::toGameEndBackground()
{

	BackgroundNo = 6;
	emit changeBackgroundNo(BackgroundNo);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (BackgroundNo == 0)
		toGameSelectionBackground();

	switch (event->key())
	{
	}

}
