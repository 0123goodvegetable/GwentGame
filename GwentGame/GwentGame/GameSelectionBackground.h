//��GameSelectionBackground����ѡ��������Ϸ��������Ϸ������༭��

#pragma once
#pragma execution_character_set("utf-8")

#include <QWidget>
#include "ui_GameSelectionBackground.h"
#include<QPushButton>
#include<QResizeEvent>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLabel>
#include<QFont>

class GameSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	GameSelectionBackground(QWidget *parent = Q_NULLPTR);
	~GameSelectionBackground();

	void init();//��ʼ������
	void resizeEvent(QResizeEvent*event);//ʹ����ͼƬ�����������
	void updateIconImage();//����ͼƬ��ť

	QPushButton *playWithPlayer_button;//������Ϸ��ť
	QPushButton *playWithAI_button;//������Ϸ��ť����AI��ս��
	QPushButton *editCardsDeck_button;//����༭����ť
	QPushButton *quit_button;//�뿪��ť

private:
	Ui::GameSelectionBackground ui;

	QLabel *playWithPlayer_label;//������Ϸ��ʾ�Ի���
	QLabel *playWithAI_label;//������Ϸ��ʾ�Ի���
	QLabel *editCardsDeck_label;//����༭����ʾ�Ի���

	QVBoxLayout *PWP_layout;//������Ϸ��ť�Ĳ��ֹ�����
	QVBoxLayout *PWA_layout;//������Ϸ��ť�Ĳ��ֹ�����
	QVBoxLayout *ECD_layout;//����༭����ť�Ĳ��ֹ�����
	QHBoxLayout *main_layout;//�ܲ��ֹ����������������ֲ��ֹ�������

	QWidget *centralWidget;//���ڿ�����������Ĳ���

};
