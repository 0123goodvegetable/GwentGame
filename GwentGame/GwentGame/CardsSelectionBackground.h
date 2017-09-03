//包括与选择游戏手牌相关的内容
#pragma once

#include <QWidget>
#include "ui_CardsSelectionBackground.h"

class CardsSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	CardsSelectionBackground(QWidget *parent = Q_NULLPTR);
	~CardsSelectionBackground();

private:
	Ui::CardsSelectionBackground ui;
};
