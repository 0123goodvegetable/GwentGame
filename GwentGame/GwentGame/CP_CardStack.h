//����CardStack�࣬ʵ�ָ�����������صĹ���

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

	void addGameCard(int No);//�������Ϸ�Ŀ����в��뿨��
	void deleteGameCard(int No);//�Ƴ���Ϸ�����еĿ���

	static QList<Card> game_cardStack;//��Ϸ����

};


#endif // !CP_CARDSTACK_H
