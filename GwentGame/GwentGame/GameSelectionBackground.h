//��GameSelectionBackground����ѡ��������Ϸ��������Ϸ������༭��

#pragma once

#include <QWidget>
#include "ui_GameSelectionBackground.h"
#include<QPushButton>

class GameSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	GameSelectionBackground(QWidget *parent = Q_NULLPTR);
	~GameSelectionBackground();

private:
	Ui::GameSelectionBackground ui;
	QPushButton*button;

};
