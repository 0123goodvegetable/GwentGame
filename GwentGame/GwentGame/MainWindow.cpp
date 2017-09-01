#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	init();//初始化各项变量

	connect(this, SIGNAL(changeBackgroundNo(int)), this, SLOT(updateBackground(int)));//随时更新场景

}

MainWindow::~MainWindow()
{
	delete BackgroundController;	
	delete beginBackground;
	delete gameSelectionBackground;
}

void MainWindow::init()
{
	//初始化各项变量
	BackgroundController = new QStackedWidget(this);

	beginBackground = new BeginBackground(this);
	gameSelectionBackground = new GameSelectionBackground(this);

	this->setCentralWidget(BackgroundController);//将页面设置为中心窗口

	//将界面加入到界面控制器中
	BackgroundController->addWidget(beginBackground);
	BackgroundController->addWidget(gameSelectionBackground);

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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (BackgroundNo == 0)
		toGameSelectionBackground();

	switch (event->key())
	{
	}

}
