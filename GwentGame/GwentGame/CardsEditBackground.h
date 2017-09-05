//实现牌组编辑功能
#pragma once

#include <QWidget>
#include "ui_CardsEditBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>

#include"CardsScene.h"
#include"CardsUI.h"
#include"CP_CardStack.h"


class CardsEditBackground : public QWidget
{
	Q_OBJECT

public:
	CardsEditBackground(QWidget *parent = Q_NULLPTR);
	~CardsEditBackground();

	void resizeEvent(QResizeEvent *event);//画面调整事件
	void cardUISizeAdjust();//重新调整卡牌编辑界面

private:
	Ui::CardsEditBackground ui;

	void init();//初始化卡牌编辑界面
	void updateCoordinate();//更新坐标

	QGraphicsView *view;
	CardsScene *scene;

	CardStack *stack;//牌组

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
