//ʵ����Ϸ��������������ʾ�Ͳ�����

#pragma once

#include <QWidget>
#include "ui_GameEndBackground.h"
#include<QPushButton>
#include<QLabel>

class GameEndBackground : public QWidget
{
	Q_OBJECT

public:
	GameEndBackground(QWidget *parent = Q_NULLPTR);
	~GameEndBackground();

	void init();//��ʼ������
	void resizeEvent(QResizeEvent*event);//ʹ����ͼƬ�����������

	QPushButton *close_button;//�رհ�ť

	public slots:
	void receiveFinal(int m_final1, int m_final2, int m_final3, int e_final1, int e_final2, int e_final3);//���ձȷֵĲۺ���

private:
	Ui::GameEndBackground ui;
	QLabel *WinOrLoseLabel;//��֪ʤ����ʧ�ܵĴ���

	//��ʾ�ȷֵĴ���
	QLabel *my_Final1;
	QLabel *my_Final2;
	QLabel *my_Final3;
	QLabel *enemy_Final1;
	QLabel *enemy_Final2;
	QLabel *enemy_Final3;

	QString WinOrLose;//��ʾʤ������ʧ�ܵ��ַ���

	//��¼�ȷֵ�����
	int my_final[3] = { 0 };
	int enemy_final[3] = { 0 };

	void updateScore();//���±ȷ�

};
