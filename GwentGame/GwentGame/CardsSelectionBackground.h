//包括与选择游戏手牌相关的内容
#pragma once

#include <QWidget>
#include "ui_CardsSelectionBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QPushButton>

#include"CardsScene.h"
#include"CardsUI.h"

class CardsSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	CardsSelectionBackground(int turn = 0,QWidget *parent = Q_NULLPTR);
	~CardsSelectionBackground();

	void resizeEvent(QResizeEvent*event);//画面调整事件

	CardsUI *selectedCardUI();//选取正在操作的卡牌
	bool isCardUIClicked();//判断是否点击卡牌图片
	void cardUISizeAdjust();//重新调整选牌界面坐标

	QPushButton *cardsSelectionFinished_button;//手牌调度完毕按钮

signals:
	void toChange();//更换卡牌

private:
	Ui::CardsSelectionBackground ui;

	void init();//初始化界面

	QGraphicsView *view;
	CardsScene *scene;

	volatile bool Pressed;//（常用）点击鼠标的判断变量

	QList<CardsUI *> cardUILists;//卡牌图片列表
	QList<QPointF> cardUIPosLists;//卡牌图片位置列表
	QList<QPixmap> cardUIPixmapLists;//卡牌图片图像列表
	QList<int> cardStackNo;//可选择的牌组

	int my_turn;//轮次
	int cardsToAdjust_number;//可以调整的卡牌数

	private slots:
	void isMoving(QPointF &pos);//鼠标移动函数
	void isPressed();//鼠标按下函数
	void isReleased();//鼠标释放函数
	void selectionChanged();//选择对象改变函数
	void getFromText();//从文本文件中获取牌组信息
	void putInText();//将牌组信息存储到文本文件中
	void changeCard();//更换卡牌

};
