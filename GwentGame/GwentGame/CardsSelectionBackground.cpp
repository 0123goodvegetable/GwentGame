#include "CardsSelectionBackground.h"
#include<QDesktopWidget>
#include<QApplication>
#include<QPixmap>
#include<QGraphicsItem>
#include<QMouseEvent>
#include<QGraphicsPixmapItem>
#include<QMessageBox>
#include<QDebug>

//定义一些常量
const qreal CARD_POS_Y = 200;
const qreal CARD_DIS = 110;
const qreal CARD_STA = 90;
const int CARD_SIZE = 100;
const int CARD_M = 20;
const int SCREEN_SIZE = 800;

CardsSelectionBackground::CardsSelectionBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	init();
}

CardsSelectionBackground::~CardsSelectionBackground()
{
}

void CardsSelectionBackground::init()
{
	Pressed = false;
	GetScreenInfo();

	cardsSelectionView = new QGraphicsView;
	cardsSelectionScene = new CardsSelectionScene();
	cardsSelectionScene->setSceneRect(0, 0, 800, 400);
}
