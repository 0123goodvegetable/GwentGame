#include "GameSelectionBackground.h"
#include<QDebug>

GameSelectionBackground::GameSelectionBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	init();

}

GameSelectionBackground::~GameSelectionBackground()
{
}

void GameSelectionBackground::init()
{

	playWithPlayer_button = new QPushButton(this);
	playWithAI_button = new QPushButton(this);
	editCardsDeck_button = new QPushButton(this);
	quit_button = new QPushButton(this);

	playWithPlayer_label = new QLabel(this);
	playWithAI_label = new QLabel(this);
	editCardsDeck_label = new QLabel(this);

	PWP_layout = new QVBoxLayout;
	PWA_layout = new QVBoxLayout;
	ECD_layout = new QVBoxLayout;
	main_layout = new QHBoxLayout;

	centralWidget = new QWidget(this);


	//���ò���
	PWP_layout->addWidget(playWithPlayer_label);
	PWP_layout->addWidget(playWithPlayer_button);
	PWA_layout->addWidget(playWithAI_label);
	PWA_layout->addWidget(playWithAI_button);
	ECD_layout->addWidget(editCardsDeck_label);
	ECD_layout->addWidget(editCardsDeck_button);



	//�ı�ؼ�������
	playWithPlayer_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	playWithPlayer_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	playWithAI_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	playWithAI_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	editCardsDeck_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	editCardsDeck_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	//���ò��ֹ������пؼ����������
	PWP_layout->setStretchFactor(playWithPlayer_label, 1);
	PWP_layout->setStretchFactor(playWithPlayer_button, 5);
	PWA_layout->setStretchFactor(playWithAI_label, 1);
	PWA_layout->setStretchFactor(playWithAI_button, 5);
	ECD_layout->setStretchFactor(editCardsDeck_label, 1);
	ECD_layout->setStretchFactor(editCardsDeck_button, 5);
	main_layout->addLayout(PWP_layout, 4);
	main_layout->addLayout(PWA_layout, 3);
	main_layout->addLayout(ECD_layout, 4);

	PWP_layout->setContentsMargins(0, 0, 20, 0);
	PWA_layout->setContentsMargins(0, 0, 20, 0);
	ECD_layout->setContentsMargins(0, 0, 20, 0);

	centralWidget->setLayout(main_layout);


	//����ť���ͼƬ(��ʼ����
	QIcon playWithPlayer_button_icon;
	playWithPlayer_button_icon.addFile(":/buttons/Resources/buttons/playWithPlayer_button.png");
	playWithPlayer_button->setIcon(playWithPlayer_button_icon);
	playWithPlayer_button->setGeometry(0, 0, 500,370);
	playWithPlayer_button->setIconSize(QSize(520,350));
	playWithPlayer_button->setFlat(true);

	QIcon playWithAI_button_icon;
	playWithAI_button_icon.addFile(":/buttons/Resources/buttons/playWithAI_button.png");
	playWithAI_button->setIcon(playWithAI_button_icon);
	playWithAI_button->setGeometry(0,0,250, 370);
	playWithAI_button->setIconSize(QSize(250, 350));
	playWithAI_button->setFlat(true);

	QIcon editCardsDeck_button_icon;
	editCardsDeck_button_icon.addFile(":/buttons/Resources/buttons/editCardsDeck_button.png");
	editCardsDeck_button->setIcon(editCardsDeck_button_icon);
	editCardsDeck_button->setGeometry(0, 0, 510, 370);
	editCardsDeck_button->setIconSize(QSize(520, 350));
	editCardsDeck_button->setFlat(true);

	QIcon quit_button_icon;
	quit_button_icon.addFile(":/buttons/Resources/buttons/quit_button.png");
	quit_button->setIcon(quit_button_icon);
	quit_button->setGeometry(855, 880,90, 45);
	quit_button->setIconSize(QSize(100, 40));
	quit_button->setFlat(true);

}

void GameSelectionBackground::resizeEvent(QResizeEvent*event)
{
	Q_UNUSED(event)//�������������
	
	this->setAutoFillBackground(true);//�Զ���䱳��

	//��ӱ���
	QImage image;
	QPalette palette;
	image.load(":/backgrounds/Resources/backgrounds/gameSelection_background.jpg");//���ر���ͼƬ

	palette.setBrush(this->backgroundRole(),
		QBrush(image.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));//���ñ�����ˢ

	this->setPalette(palette);

	//����ÿ����ʾ�������
	QFont font;//�����ı�����
	int font_size;
	font_size = geometry().height() / 35;
	font.setPointSize(font_size);
	font.setFamily("����");
	playWithPlayer_label->setFont(font);
	playWithAI_label->setFont(font);
	editCardsDeck_label->setFont(font);

	//�����ı���ɫ
	playWithPlayer_label->setStyleSheet("color:white");
	playWithAI_label->setStyleSheet("color:white");
	editCardsDeck_label->setStyleSheet("color:white");

	playWithPlayer_label->setText(tr("������ս"));
	playWithAI_label->setText(tr("���Զ�ս"));
	editCardsDeck_label->setText(tr("����༭��"));

	//�ı䲼�ֹ��������ڵĳߴ�
	centralWidget->setGeometry(QRect(geometry().x() + geometry().width() / 40,
		geometry().y() + geometry().height() / 5,
		geometry().width()*0.8,
		geometry().height()*0.5));


	//����ͼƬ��ť
	updateIconImage();

}

void GameSelectionBackground::updateIconImage()
{
	//����playWithPlayer_button��ͼƬ
	int playWithPlayer_button_height = playWithPlayer_button->height();
	int playWithPlayer_button_width = playWithPlayer_button_height*1.42;
	playWithPlayer_button->setGeometry(playWithPlayer_button->x(),
		playWithPlayer_button->y(),
		playWithPlayer_button_width,
		playWithPlayer_button_height);

	QIcon playWithPlayer_button_icon;
	playWithPlayer_button_icon.addFile(":/buttons/Resources/buttons/playWithPlayer_button.png");
	playWithPlayer_button->setIcon(playWithPlayer_button_icon);
	playWithPlayer_button->setIconSize(QSize(playWithPlayer_button->width()-10, playWithPlayer_button->height()-20));

	//����playWithAI_button��ͼƬ
	int playWithAI_button_height = playWithAI_button->height();
	int playWithAI_button_width = playWithAI_button_height/1.4;
	playWithAI_button->setGeometry(playWithAI_button->x(),
		playWithAI_button->y(),
		playWithAI_button_width,
		playWithAI_button_height);

	QIcon playWithAI_button_icon;
	playWithAI_button_icon.addFile(":/buttons/Resources/buttons/playWithAI_button.png");
	playWithAI_button->setIcon(playWithAI_button_icon);
	playWithAI_button->setIconSize(QSize(playWithAI_button->width()-10, playWithAI_button->height()-10));

	//����editCardsDeck_button��ͼƬ
	int editCardsDeck_button_height = editCardsDeck_button->height();
	int editCardsDeck_button_width = editCardsDeck_button_height * 1.42;
	editCardsDeck_button->setGeometry(editCardsDeck_button->x(),
		editCardsDeck_button->y(),
		editCardsDeck_button_width,
		editCardsDeck_button_height);

	QIcon editCardsDeck_button_icon;
	editCardsDeck_button_icon.addFile(":/buttons/Resources/buttons/editCardsDeck_button.png");
	editCardsDeck_button->setIcon(editCardsDeck_button_icon);
	editCardsDeck_button->setIconSize(QSize(editCardsDeck_button->width()-10, editCardsDeck_button->height()-20));

	//����quit_button��ͼƬ
	QIcon quit_button_icon;
	quit_button_icon.addFile(":/buttons/Resources/buttons/quit_button.png");
	quit_button->setIcon(quit_button_icon);
	quit_button->setGeometry(geometry().width()/2-50, geometry().height()-90, 90, 45);
	quit_button->setIconSize(QSize(100, 40));

}
