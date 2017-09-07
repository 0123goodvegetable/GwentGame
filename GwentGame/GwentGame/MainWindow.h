//MainWindow用于控制不同界面的转换（界面控制中枢）

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

//加入各界面头文件
#include"BeginBackground.h"
#include"GameSelectionBackground.h"
#include"CardsSelectionBackground.h"
#include"CardsEditBackground.h"
#include"GamePlayingBackground.h"

#include<QStackedWidget>
#include<QKeyEvent>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

private slots:
	void updateBackground(int number);//刷新游戏界面进行更新

	void toBeginBackground();//将界面切换到开始界面
	void toGameSelectionBackground();//将界面切换到游戏选择界面
	void toCardsSelectionBackground();//将界面切换到游戏开始后卡牌选择界面
	void toCardsEditBackground();//将界面切换到牌组编辑界面
	void toGamePlayingBackground();//将界面切换到游戏主界面

protected:
	void keyPressEvent(QKeyEvent* event);//处理各窗口的按键信息

signals:
	void changeBackgroundNo(int number);//更新界面编号

private:
	Ui::MainWindowClass ui;
	void init();//初始化各项数据

	QStackedWidget *BackgroundController;//控制界面的转换
	int BackgroundNo;//所选界面的编号（开始界面-0，游戏选择界面-1，卡牌选择界面-2，牌组编辑界面-3，游戏主界面-4）
	
	BeginBackground *beginBackground;//创建开始界面
	GameSelectionBackground *gameSelectionBackground;//创建游戏选择界面
	CardsSelectionBackground *cardsSelectionBackground;//创建游戏开始后卡牌选择界面
	CardsEditBackground *cardsEditBackground;//创建牌组编辑界面
	GamePlayingBackground *gamePlayingBackground;//创建游戏主界面界面
};
