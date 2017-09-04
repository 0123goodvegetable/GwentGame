//����Card�࣬���а������뿨����صĸ�������

#pragma once
#ifndef CP_CARD_H
#define CP_CARD_H
#include<Qstring>
#include<QGraphicsItem>
#include<QPainter>
#include<QGraphicsPixmapItem>

class Card:public QGraphicsItem
{
public:

	//������Ϣ
	int ID;//���ڿ��Ƶı��
	int No;//��������Ϣ������
	int number;//�������м��Ÿÿ���
	QString explainText;//������Ϣ����
	QString name;//��������
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

	int card_width;//���ƵĿ��
	int card_height;//���Ƶĸ߶�

	//������غ���
	Card();
	//��������������Ϣ��No����������
	Card(int num);

	//���Ƶĳ�ʼ��
	void Init(int num);
	//��ӿ�������
	void AddName(int num);

	//�������Ƶ��߼�����
	void CreateLogic(int num);

	//�������صĺ���
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);//���ƺ���																						   //�̳��Ի�����麯��
	virtual QRectF boundingRect() const;

};


#endif // !CP_CARD_H
