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
	int armer;//���ƻ���
	int skill;//���Ƽ��ܣ�������Ϊ�书�ܣ�
	int material;//���Ʋ��ʣ�������ͭ��
	
	bool isHero;//�����Ƿ���Ӣ����
	bool isShield;//�����Ƿ��л���
	bool isFriend;//�����Ƿ����ѷ�����
	bool isInGameCardsStack;//�����Ƿ�����Ϸ������
	bool isPickerActive;//�����ܷ�ѡ��
	bool isSelected;//�����Ƿ�ѡ��
	bool isFielded;//�����Ƿ���ս����
	bool isGarbaged;//�����Ƿ����Ĺ��

	int isWeatherControlled;//�����ܵ�������Ӱ��(0-�ޣ�1-����Ũ��2-�̹Ǳ�˪��3-������꣩

	int card_width;//���ƵĿ��
	int card_height;//���Ƶĸ߶�

	//������غ���
	Card();
	//��������������Ϣ��No����������
	Card(int num);
	//�������캯��
	Card(const Card &root_card);

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
