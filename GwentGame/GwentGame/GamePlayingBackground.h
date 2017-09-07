//与进行游戏相关的界面
#pragma once

#include <QWidget>
#include "ui_GamePlayingBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QPushButton>


#include"CP_Card.h"
#include"CP_PlayingLogic.h"
#include"CardsUI.h"
#include"CardsScene.h"


class GamePlayingBackground : public QWidget
{
	Q_OBJECT

public:
	GamePlayingBackground(QWidget *parent = Q_NULLPTR);
	~GamePlayingBackground();

private:
	Ui::GamePlayingBackground ui;
};
