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

	int genre;//�������ࣨ��ս��Զ�̡����ǡ������ƣ�
	int attack;//���ƵĹ�����
	
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
	int Init(int num);
	//�������Ƶ��߼�����
	int CreateLogic(int num);

};


#endif // !CP_CARD_H
