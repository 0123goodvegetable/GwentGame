#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	init();//��ʼ���������

	connect(this, SIGNAL(changeBackgroundNo(int)), this, SLOT(updateBackground(int)));//��ʱ���³���

}

MainWindow::~MainWindow()
{
	delete BackgroundController;	
	delete beginBackground;
	delete gameSelectionBackground;
}

void MainWindow::init()
{
	//��ʼ���������
	BackgroundController = new QStackedWidget(this);

	beginBackground = new BeginBackground(this);
	gameSelectionBackground = new GameSelectionBackground(this);

	this->setCentralWidget(BackgroundController);//��ҳ������Ϊ���Ĵ���

	//��������뵽�����������
	BackgroundController->addWidget(beginBackground);
	BackgroundController->addWidget(gameSelectionBackground);

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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (BackgroundNo == 0)
		toGameSelectionBackground();

	switch (event->key())
	{
	}

}
