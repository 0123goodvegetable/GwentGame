//与进行游戏相关的界面
#pragma once

#include <QWidget>
#include "ui_GamePlayingBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QPushButton>


#include"CP_Card.h"
#include"CP_PlayingLogic.h"
#include"CardsUI.h"
#include"CardsScene.h"


class GamePlayingBackground : public QWidget
{
	Q_OBJECT

public:
	GamePlayingBackground(QWidget *parent = Q_NULLPTR);
	~GamePlayingBackground();

	void resizeEvent(QResizeEvent*event);//画面调整事件

	CardsUI *selectedCardUI();//选取正在操作的卡牌
	bool isCardUIClicked();//判断是否点击卡牌图片
	void cardUISizeAdjust();//重新调整选牌界面坐标

	QList<int> my_cardStackNo;//游戏中友方手牌信息

signals:
	void toUseSkills();//释放技能的信号

public slots:
	void getFromText();//从文本文件中获取牌组信息


private:
	Ui::GamePlayingBackground ui;

	void init();//初始化界面
	void updateStatus();//更新游戏状态
	void updateCoordinate();//更新游戏画面常量

	QGraphicsView *view;
	CardsScene *scene1;//正常游戏画面
	CardsScene *scene2;//卡牌操作游戏画面

	volatile bool Pressed;//（常用）点击鼠标的判断变量
	bool operation;//是否在某张卡牌的轮次中

	CardsUI  *selected_card;//当前选择的卡牌
	QList<CardsUI *> cardUILists;//卡牌图片列表
	QList<QPointF> cardUIPosLists;//卡牌图片位置列表
	QList<QPixmap> cardUIPixmapLists;//卡牌图片图像列表
	QList<Card> PlayingCard;//排场上所有卡牌（用于更新界面）



	private slots:
	void isMoving(QPointF &pos);//鼠标移动函数
	void isPressed();//鼠标按下函数
	void isReleased();//鼠标释放函数
	void selectionChanged();//选择对象改变函数
	void putInText();//将牌组信息存储到文本文件中
	void useSkills();//使用技能

};
