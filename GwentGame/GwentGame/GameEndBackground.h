//实现游戏结束界面的相关显示和操作。

#pragma once

#include <QWidget>
#include "ui_GameEndBackground.h"
#include<QPushButton>
#include<QLabel>

class GameEndBackground : public QWidget
{
	Q_OBJECT

public:
	GameEndBackground(QWidget *parent = Q_NULLPTR);
	~GameEndBackground();

	void init();//初始化界面
	void resizeEvent(QResizeEvent*event);//使背景图片填充整个界面

	QPushButton *close_button;//关闭按钮

	public slots:
	void receiveFinal(int m_final1, int m_final2, int m_final3, int e_final1, int e_final2, int e_final3);//接收比分的槽函数

private:
	Ui::GameEndBackground ui;
	QLabel *WinOrLoseLabel;//告知胜利或失败的窗体

	//显示比分的窗体
	QLabel *my_Final1;
	QLabel *my_Final2;
	QLabel *my_Final3;
	QLabel *enemy_Final1;
	QLabel *enemy_Final2;
	QLabel *enemy_Final3;

	QString WinOrLose;//表示胜利或者失败的字符串

	//记录比分的数组
	int my_final[3] = { 0 };
	int enemy_final[3] = { 0 };

	void updateScore();//更新比分

};
