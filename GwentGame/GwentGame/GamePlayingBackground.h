//与进行游戏相关的界面
#pragma once

#include <QWidget>
#include "ui_GamePlayingBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QPushButton>
#include<QMouseEvent>


#include"CP_Card.h"
#include"CP_PlayingLogic.h"
#include"CP_AllCards.h"
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

signals:
	void toUseSkills(Card *card);//释放技能的信号

private:
	Ui::GamePlayingBackground ui;

	void init();//初始化界面
	void updateStatus();//更新游戏状态
	void updateCoordinate();//更新游戏画面常量
	void updateStack(QList<CardsUI *> stack);//更新牌组信息

	QGraphicsView *view;
	CardsScene *scene1;//正常游戏画面
	CardsScene *scene2;//卡牌操作游戏画面

	volatile bool Pressed;//（常用）点击鼠标的判断变量
	bool operation;//是否在某张卡牌的轮次中
	bool isUsingSkill;//是否在使用技能阶段
	int usingSkillTimes;//使用技能次数
	PlayingLogic *conductor;//释放技能时的指挥
	AllCards allCards;//所有卡牌信息
	QPoint cursor_point;//光标坐标

	int m_Melee_weather;//我方近战栏受到天气影响（0-无，1-蔽日浓雾，2-刺骨冰霜，3-倾盆大雨）
	int m_Archer_weather;//我方远程栏受到天气影响（0-无，1-蔽日浓雾，2-刺骨冰霜，3-倾盆大雨）
	int m_Siege_weather;//我方攻城栏受到天气影响（0-无，1-蔽日浓雾，2-刺骨冰霜，3-倾盆大雨）
	int e_Melee_weather;//敌方近战栏受到天气影响（0-无，1-蔽日浓雾，2-刺骨冰霜，3-倾盆大雨）
	int e_Archer_weather;//敌方远程栏受到天气影响（0-无，1-蔽日浓雾，2-刺骨冰霜，3-倾盆大雨）
	int e_Siege_weather;//敌方攻城栏受到天气影响（0-无，1-蔽日浓雾，2-刺骨冰霜，3-倾盆大雨）

	CardsUI  *selected_card;//当前选择的卡牌
	CardsUI *usingSkill_card;//将要释放技能的卡牌
	QList<CardsUI *> cardUILists;//卡牌图片列表
	QList<QPointF> cardUIPosLists;//卡牌图片位置列表
	QList<QPixmap> cardUIPixmapLists;//卡牌图片图像列表
	QList<int> my_cardStackNo;//游戏中友方手牌信息

	private slots:
	void isMoving(QPointF &pos);//鼠标移动函数
	void CardisPressed();//鼠标按下卡牌函数
	void CardisReleased();//鼠标释放卡牌函数
	void selectionChanged();//选择对象改变函数
	void putInText();//将牌组信息存储到文本文件中
	void getFromText();//从文本文件中获取牌组信息
	void useSkills(Card *card);//使用技能

};
