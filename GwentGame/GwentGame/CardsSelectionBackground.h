//包括与选择游戏手牌相关的内容
#pragma once
#ifndef CARDSSELECTIONBACKGROUND_H
#define CARDSSELECTIONBACKGROUND_H

#include <QWidget>
#include "ui_CardsSelectionBackground.h"
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QPointF>
#include"CardsSelectionDirector.h"
#include"CardsSelectionScene.h"
#include<QMap>
#include"CP_CardStack.h"

class CardsSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	CardsSelectionBackground(QWidget *parent = Q_NULLPTR);
	~CardsSelectionBackground();

	void init();//初始化函数

	CardsSelectionDirector *selectCard();//选取卡牌（节点形式）
	bool isCardClicked();//是否点击卡牌
	void cardSizeAdjust();//调整卡牌大小

private slots:
	//判断鼠标状态
	void isMoving(QPointF &pos);
	void isPressed();
	void isReleased();

	void selectionChanged();//改变选择对象

signals:
	void cardClicked(CardsSelectionDirector *card);

private:
	Ui::CardsSelectionBackground ui;

	void GetScreenInfo();//获取屏幕信息
	
	QGraphicsView *cardsSelectionView;
	CardsSelectionScene *cardsSelectionScene;
	CardsSelectionDirector *operation_card;
	QPointF cardPos;
	volatile bool Pressed;

	QList<CardsSelectionDirector*> cardsLists;
	QList<QPointF> cardsPosLists;
	QList<QPixmap> cardsPixmapLists;

	//屏幕尺寸信息
	qint16 screenSizeX;
	qint16 screenSizeY;


};

#endif // !CARDSSELECTIONBACKGROUND_H


