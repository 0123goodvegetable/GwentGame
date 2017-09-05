//这是CardStack类，实现各种与牌组相关的功能

#pragma once
#ifndef CP_CARDSTACK_H
#define CP_CARDSTACK_H

#include"CP_Card.h"
#include<QList>

class CardStack
{
public:
	CardStack();
	~CardStack() {}

	void addGameCard(int No);//向进行游戏的卡组中插入卡牌
	void deleteGameCard(int No);//移除游戏卡组中的卡牌

	static QList<Card> game_cardStack;//游戏卡组

};


#endif // !CP_CARDSTACK_H
