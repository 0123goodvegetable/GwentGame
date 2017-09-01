#include "BeginBackground.h"

BeginBackground::BeginBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	showLabel = new QLabel(this);

	QFont font;//�����ı�����
	font.setPointSize(16);
	font.setFamily("����");
	showLabel->setFont(font);
	showLabel->setStyleSheet("color:white");//�����ı���ɫ

	showLabel->setText(tr("�������������..."));

}

BeginBackground::~BeginBackground()
{
}

void BeginBackground::resizeEvent(QResizeEvent*event)
{
	Q_UNUSED(event)//�������������
	this->setAutoFillBackground(true);//�Զ���䱳��

	//��ӱ���
	QImage image;
	QPalette palette;
	image.load(":/backgrounds/Resources/backgrounds/begin_background.jpg");//���ر���ͼƬ

	palette.setBrush(this->backgroundRole(), 
		QBrush(image.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));//���ñ�����ˢ
		
	this->setPalette(palette);

	//ʹ�ı�������Ӧ
	showLabel->setGeometry(geometry().x() + geometry().width() / 2 - 100, geometry().y() + geometry().height() - 35, 250, 25);
}