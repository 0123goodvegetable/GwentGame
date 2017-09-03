//����Card�࣬���а������뿨����صĸ�������

#pragma once
#ifndef CP_CARD_H
#define CP_CARD_H
#include<Qstring>
#include<QGraphicsItem>

class Card:public QGraphicsItem
{
public:

	//������Ϣ
	int ID;//���ڿ��Ƶı��
	int No;//��������Ϣ������
	int number;//�������м��Ÿÿ���
	QString path;//����ͼƬ�����ļ�·��

	int genre;//�������ࣨ��ս��Զ�̡����ǡ��ҷ����⡢�з����⡢�����ƣ�
	int attack;//���ƵĹ�����
	int skill;//���Ƽ��ܣ�������Ϊ�书�ܣ�
	int material;//���Ʋ��ʣ�������ͭ��
	
	bool isHero;//�����Ƿ���Ӣ����
	bool isPickerActive;//�����ܷ�ѡ��
	bool isSelected;//�����Ƿ�ѡ��
	bool isFielded;//�����Ƿ���ս����
	bool isGarbaged;//�����Ƿ����Ĺ��

	//������غ���
	Card();
	//��������������Ϣ��No����������
	Card(int num);
	//���Ƶĳ�ʼ��
	void Init(int num);
	//�������Ƶ��߼�����
	void CreateLogic(int num);

};


#endif // !CP_CARD_H
