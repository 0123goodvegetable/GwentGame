//��BeginBackground������ʼ������ص�����

#pragma once
#pragma execution_character_set("utf-8")

#include <QWidget>
#include "ui_BeginBackground.h"
#include<QResizeEvent>
#include<QLabel>

class BeginBackground : public QWidget
{
	Q_OBJECT

public:
	BeginBackground(QWidget *parent = Q_NULLPTR);
	~BeginBackground();

	void resizeEvent(QResizeEvent*event);//ʹ����ͼƬ�����������

private:
	Ui::BeginBackground ui;

	QLabel *showLabel;//��ʾһ������
};
