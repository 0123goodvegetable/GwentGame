//与进行游戏相关的界面
#pragma once

#include <QWidget>
#include "ui_GamePlayingBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QKeyEvent>
#include<QTimerEvent>
#include<QPushButton>
#include<QMouseEvent>
#include<QStringList>
#include<QLabel>
#include<QTimer>
#include<QLCDNumber>


#include"CP_Card.h"
#include"CP_PlayingLogic.h"
#include"CP_AllCards.h"
#include"CardsUI.h"
#include"CardsScene.h"


class GamePlayingBackground : public QWidget
{
	Q_OBJECT

public:
	GamePlayingBackground(int round = 0, QWidget *parent = Q_NULLPTR);
	~GamePlayingBackground();

	void resizeEvent(QResizeEvent*event);//画面调整事件
	void keyPressEvent(QKeyEvent *event);//按键事件

	CardsUI *selectedCardUI();//选取正在操作的卡牌
	bool isCardUIClicked();//判断是否点击卡牌图片
	void cardUISizeAdjust();//重新调整选牌界面坐标

	public slots:
	void putInEnemyText();//将我方数据存入文件进行传送
	void getFromEnemyText();//从文件中获取敌方信息
	void changeMyTurn();//到了我方轮次
	void enemyEnd() { enemy_end = true; }//敌方结束
	void timing();//计时

signals:
	void toUseSkills(Card *card);//释放技能的信号

	void toSendFile(QString filename);//将存储我方数据的文件传输到另一边

	void chooseEnd();//选择结束

	void sendFinal(int i, int me_final_num,int enemy_final_num);//输出结果

private:
	Ui::GamePlayingBackground ui;

	void init();//初始化界面
	void updateStatus();//更新游戏状态
	void updateCoordinate();//更新游戏画面常量
	void updateStack(QList<CardsUI *> stack);//更新牌组信息

	void useChooseScene(CardsUI *root_card);//根据root_card设置场景

	QGraphicsView *view;
	CardsScene *main_scene;//正常游戏画面
	CardsScene *choose_scene;//卡牌操作游戏画面
	QLabel *turnTextLabel;//显示轮次的文本
	QLabel *myAllAttackLabel;//我方战力总值
	QLabel *enemyAllAttackLabel;//敌方战力总值
	QLCDNumber *timeShowLCD;//显示计时的屏幕
	QTimer *clock;//定时器

	QLabel *m_Melee_weather_Label;
	QLabel *m_Archer_weather_Label;
	QLabel *m_Siege_weather_Label;
	QLabel *e_Melee_weather_Label;
	QLabel *e_Archer_weather_Label;
	QLabel *e_Siege_weather_Label;

	volatile bool Pressed;//（常用）点击鼠标的判断变量
	int second_number;//经过多少秒
	int my_round;//第几回合
	bool my_turn;//我方轮次
	bool me_end;//我方选择结束
	bool enemy_end;//敌方结束
	bool useMainScene;//使用主界面
	bool operation;//是否在某张卡牌的移动中
	bool isUsingSkill;//是否在使用技能阶段
	bool cardExist;//存在要选取的卡牌
	int usingSkillTimes;//使用技能次数
	bool isGoldCardOut;//打出一张金色牌（萝卜技能）

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
	QList<CardsUI *> cardUILists;//所有在场上卡牌图片列表
	QList<CardsUI *> allCardUILists;//我方牌组中所有卡牌列表
	QList<QPointF> cardUIPosLists;//卡牌图片位置列表
	QList<QPixmap> cardUIPixmapLists;//卡牌图片图像列表
	QList<int> my_cardStackNo;//游戏中友方手牌信息
	QList<int> all_cardStackNo;//所有牌组中卡牌信息

	private slots:
	void isMoving(QPointF &pos);//鼠标移动函数
	void CardisPressed();//鼠标按下卡牌函数
	void CardisReleased();//鼠标释放卡牌函数
	void selectionChanged();//选择对象改变函数
	void putInText();//将牌组信息存储到文本文件中
	void getFromText();//从文本文件中获取牌组信息
	void useSkills(Card *card);//使用技能
	void whetherUseActiveSkill();//判断是否可以发动主动技能
	void whetherUseFollowSkill();//判断是否可以发出被动技能

};
