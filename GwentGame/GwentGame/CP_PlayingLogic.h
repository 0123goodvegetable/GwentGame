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
	QList<CardsUI*> operateCard(Card &card);
	
private:
	QList<CardsUI*> cardStack;//���ڴ��濨�Ƶ�״̬����GamePlayingBackground�еĿ���״̬���и���

	AllCards allCards;

	//���п��Ƶļ��ܺ���
	void skill1();
	void skill2();
	void skill3();
	void skill4();
	void skill5();
	void skill6();
	void skill7();
	void skill8();
	void skill9();
	/*void skill10();
	void skill11();
	void skill12();
	void skill13();
	void skill14();
	void skill15();
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
