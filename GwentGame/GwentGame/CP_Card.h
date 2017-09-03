//����Card�࣬���а������뿨����صĸ�������
//ͷ�ļ��������п���

#pragma once
#ifndef CP_CARD_H
#define CP_CARD_H
#include<Qstring>
#include<QGraphicsItem>
#include<QPainter>
#include<QGraphicsPixmapItem>

class Card:public QGraphicsPixmapItem
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
	int card_height;

	//������غ���
	Card();
	//��������������Ϣ��No����������
	Card(int num);
	//���Ƶĳ�ʼ��
	void Init(int num);
	//��ӿ�������
	void AddName(int num);
	//��ӿ���ͼƬ
	void AddPicture();
	//�������Ƶ��߼�����
	void CreateLogic(int num);

	//�������صĺ���
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);//���ƺ���																						   //�̳��Ի�����麯��
	virtual QRectF boundingRect() const;

};


//���п���
Card Unseen_Elder(13010530);//��Ӱ����
Card Bekker_Twister_Mirror(25010091);//���˶���Ť��֮��
Card Impenetrable_Fog(35020092);//����Ũ��
Card Biting_Frost(45030092);//�̹Ǳ�˪
Card Dagon(53020630);//���
Card Archgriffin(63030882);//��ʨ��


#endif // !CP_CARD_H
