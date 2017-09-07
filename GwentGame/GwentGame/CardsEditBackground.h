//实现牌组编辑功能
#pragma once

#include <QWidget>
#include "ui_CardsEditBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QPushButton>

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
	bool isCardUIClicked();//判断是否点击卡牌图片

	QPushButton *saveAndQuit_button;//保存并退出按钮

signals:
	void toEdit();//编辑牌组

private:
	Ui::CardsEditBackground ui;

	void init();//初始化卡牌编辑界面
	void updateCoordinate();//更新坐标
	void updatePictures();//更新卡牌图像位置
	void putInText();//将牌组信息存储到文本文件中
	void getFromText();//从文本文件中获取牌组信息

	QGraphicsView *view;//视口
	CardsScene *scene;//场景
	CardStack *stack;//牌组

	volatile bool Pressed;//（常用）点击鼠标的判断变量
	CardsUI* selected_card;//目前正在选择的卡牌

	QList<int> cardStackNo;//在游戏牌组中的卡牌号码
	QList<CardsUI *> cardUILists;//卡牌图片列表
	QList<QPointF> cardUIPosLists;//卡牌图片位置列表
	QList<QPixmap> cardUIPixmapLists;//卡牌图片图像列表

	private slots:
	void isMoving(QPointF &pos);//鼠标移动函数
	void isPressed();//鼠标按下函数
	void isReleased();//鼠标释放函数
	void selectionChanged();//选择对象改变函数
	void editStacks();//编辑牌组


};
