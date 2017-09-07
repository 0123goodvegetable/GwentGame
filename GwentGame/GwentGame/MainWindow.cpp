#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	init();//��ʼ���������

	//��������������
	this->resize(1800, 1000);
	this->setWindowOpacity(1);
	this->setAttribute(Qt::WA_TranslucentBackground);

	connect(this, SIGNAL(changeBackgroundNo(int)), this, SLOT(updateBackground(int)));//��ʱ���³���

	connect(gameSelectionBackground->playWithPlayer_button, SIGNAL(clicked()), this, SLOT(toCardsSelectionBackground()));

	connect(gameSelectionBackground->editCardsDeck_button, SIGNAL(clicked()), this, SLOT(toCardsEditBackground()));

	connect(cardsEditBackground->saveAndQuit_button, SIGNAL(clicked()), this, SLOT(toGameSelectionBackground()));

	//��������޸�
	connect(cardsSelectionBackground->cardsSelectionFinished_button, SIGNAL(clicked()), this, SLOT(toGamePlayingBackground()));

}

MainWindow::~MainWindow()
{
	delete BackgroundController;	
	delete beginBackground;
	delete gameSelectionBackground;
	delete cardsSelectionBackground;
	delete cardsEditBackground;
	delete gamePlayingBackground;
}

void MainWindow::init()
{
	//��ʼ���������
	BackgroundController = new QStackedWidget(this);

	beginBackground = new BeginBackground(this);
	gameSelectionBackground = new GameSelectionBackground(this);
	cardsSelectionBackground = new CardsSelectionBackground(this);
	cardsEditBackground = new CardsEditBackground(this);
	gamePlayingBackground = new GamePlayingBackground(this);

	this->setCentralWidget(BackgroundController);//��ҳ������Ϊ���Ĵ���

	//��������뵽�����������
	BackgroundController->addWidget(beginBackground);
	BackgroundController->addWidget(gameSelectionBackground);
	BackgroundController->addWidget(cardsSelectionBackground);
	BackgroundController->addWidget(cardsEditBackground);
	BackgroundController->addWidget(gamePlayingBackground);

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
	gamePlayingBackground = new GamePlayingBackground(this);
	BackgroundController->addWidget(gamePlayingBackground);
	BackgroundNo = 4;
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
