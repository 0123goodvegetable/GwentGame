//类GameSelectionBackground负责选择联网游戏、单人游戏、牌组编辑器

#pragma once
#pragma execution_character_set("utf-8")

#include <QWidget>
#include "ui_GameSelectionBackground.h"
#include<QPushButton>
#include<QResizeEvent>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLabel>
#include<QFont>

class GameSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	GameSelectionBackground(QWidget *parent = Q_NULLPTR);
	~GameSelectionBackground();

	void init();//初始化界面
	void resizeEvent(QResizeEvent*event);//使背景图片填充整个界面
	void updateIconImage();//更新图片按钮

	QPushButton *playWithPlayer_button;//联机游戏按钮
	QPushButton *playWithAI_button;//单人游戏按钮（与AI作战）
	QPushButton *editCardsDeck_button;//牌组编辑器按钮
	QPushButton *quit_button;//离开按钮

private:
	Ui::GameSelectionBackground ui;

	QLabel *playWithPlayer_label;//联机游戏提示对话框
	QLabel *playWithAI_label;//单人游戏提示对话框
	QLabel *editCardsDeck_label;//牌组编辑器提示对话框

	QVBoxLayout *PWP_layout;//联机游戏按钮的布局管理器
	QVBoxLayout *PWA_layout;//单人游戏按钮的布局管理器
	QVBoxLayout *ECD_layout;//牌组编辑器按钮的布局管理器
	QHBoxLayout *main_layout;//总布局管理器（管理三个分布局管理器）

	QWidget *centralWidget;//用于控制中央组件的布局

};
