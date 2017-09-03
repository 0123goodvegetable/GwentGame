#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	init();//初始化各项变量

	this->resize(1800, 1000);

	connect(this, SIGNAL(changeBackgroundNo(int)), this, SLOT(updateBackground(int)));//随时更新场景

	connect(gameSelectionBackground->playWithPlayer_button, SIGNAL(clicked()), this, SLOT(toCardsSelectionBackground()));

}

MainWindow::~MainWindow()
{
	delete BackgroundController;	
	delete beginBackground;
	delete gameSelectionBackground;
	delete cardsSelectionBackground;
}

void MainWindow::init()
{
	//初始化各项变量
	BackgroundController = new QStackedWidget(this);

	beginBackground = new BeginBackground(this);
	gameSelectionBackground = new GameSelectionBackground(this);
	cardsSelectionBackground = new CardsSelectionBackground(this);

	this->setCentralWidget(BackgroundController);//将页面设置为中心窗口

	//将界面加入到界面控制器中
	BackgroundController->addWidget(beginBackground);
	BackgroundController->addWidget(gameSelectionBackground);
	BackgroundController->addWidget(cardsSelectionBackground);

	BackgroundNo = 0;

	//将最初的界面设置为开始界面
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (BackgroundNo == 0)
		toGameSelectionBackground();

	switch (event->key())
	{
	}

}
