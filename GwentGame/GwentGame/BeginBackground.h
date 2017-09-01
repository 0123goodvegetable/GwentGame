//类BeginBackground包括开始界面相关的内容

#pragma once
#pragma execution_character_set("utf-8")

#include <QWidget>
#include "ui_BeginBackground.h"
#include<QResizeEvent>
#include<QLabel>

class BeginBackground : public QWidget
{
	Q_OBJECT

public:
	BeginBackground(QWidget *parent = Q_NULLPTR);
	~BeginBackground();

	void resizeEvent(QResizeEvent*event);//使背景图片填充整个界面

private:
	Ui::BeginBackground ui;

	QLabel *showLabel;//显示一行文字
};
