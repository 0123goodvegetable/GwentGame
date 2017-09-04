//包括与选择游戏手牌相关的内容
#pragma once

#include <QWidget>
#include "ui_CardsSelectionBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include <QMap>
#include<QResizeEvent>

#include"CardsSelectionScene.h"
#include"CardsUI.h"

class CardsSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	CardsSelectionBackground(QWidget *parent = Q_NULLPTR);
	~CardsSelectionBackground();

	void resizeEvent(QResizeEvent*event);//画面调整事件

	CardsUI *selectedCardUI();//选取正在操作的卡牌
	bool isCardUIClicked();//判断是否点击卡牌图片
	void cardUISizeAdjust();//重新调整选牌界面坐标

private:
	Ui::CardsSelectionBackground ui;

	void init();//初始化界面

	QGraphicsView *view;
	CardsSelectionScene *scene;

	volatile bool Pressed;//（常用）点击鼠标的判断变量

	QList<CardsUI *> cardUILists;//卡牌列表
	QList<QPointF> cardUIPosLists;//卡牌位置列表
	QList<QPixmap> cardUIPixmapLists;//卡牌图片列表

	private slots:
	void isMoving(QPointF &pos);//鼠标移动函数
	void isPressed();//鼠标按下函数
	void isReleased();//鼠标释放函数
	void selectionChanged();//选择对象改变函数

};
