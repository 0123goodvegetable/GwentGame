//ʵ�ֿ��Ʋ���������߼�

#pragma once
#ifndef CP_PLAYINGLOGIC_H
#define CP_PLAYINGLOGIC_H
#include"CP_Card.h"
#include"CP_AllCards.h"
#include"CardsUI.h"

class PlayingLogic
{
public:
	PlayingLogic(QList<CardsUI*> &aim_stack);
	~PlayingLogic() {}

	//���ڿ��ƽ��в������ؼ�������
	//������card������Ŀ���
	//����ֵ�����Ʒ�������֮���������Ϣ
	QList<CardsUI*> operateCard(Card &card,int card_number);
	
private:
	QList<CardsUI*> cardStack;//���ڴ��濨�Ƶ�״̬����GamePlayingBackground�еĿ���״̬���и���

	AllCards allCards;

	//���п��Ƶļ��ܺ���
	void skill1(int number);
	void skill2(int number);
	void skill3(int number);
	void skill4(int number);
	void skill6(int number);
	void skill8(int number);
	void skill14(int number);
	/*
	void skill16();
	void skill17();
	void skill18();
	void skill19();
	void skill20();
	void skill21();
	void skill22();
	void skill23();
	void skill24();
	void skill25();
	void skill26();
	void skill27();
	void skill28();
	void skill29();*/

};

#endif // !CP_PLAYINGLOGIC_H
