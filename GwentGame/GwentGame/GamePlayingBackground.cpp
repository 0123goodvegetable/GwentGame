#include "GamePlayingBackground.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPixmap>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QDebug>
#include<QFile>
#include<QTextStream>

//����ȫ�ֱ���
const qreal CARD_DIS = 80;//���Ƽ��

int SCREEN_WIDTH2 = 1800;//������
int SCREEN_HEIGHT2 = 1000;//����߶�

int CARD_WIDTH;//���ƿ��
int CARD_HEIGHT;//���Ƹ߶�

int COLUMN_POS_X;//������x����

int PREPARE_COLUMN_POS_Y;//δ��������ڿ�����y����

int M_MELEE_COLUMN_POS_Y;//�ҷ���ս��y����
int M_ARCHER_COLUMN_POS_Y;//�ҷ�Զ����y����
int M_SIEGE_COLUMN_POS_Y;//�ҷ�������y����

int E_MELEE_COLUMN_POS_Y;//�з���ս��y����
int E_ARCHER_COLUMN_POS_Y;//�з�Զ����y����
int E_SIEGE_COLUMN_POS_Y;//�з�������y����

int COLUMN_LENGTH;//����������



GamePlayingBackground::GamePlayingBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//��ʼ������ȫ�ֱ���
	updateCoordinate();

	//��ʼ������
	init();
}

GamePlayingBackground::~GamePlayingBackground()
{
}

void GamePlayingBackground::init()
{
	//��ʼ��������Ա����
	Pressed = false;
	operation = false;


	//���ı��л�ȡ����
	getFromText();

	view = new QGraphicsView(this);
	scene1 = new CardsScene();
	scene2 = new CardsScene();

	//���ô������ԣ�û�й�������
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//����ͼ���ƶ����źŲ�(��Ϸ�����棩
	connect(scene1, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	connect(scene1, SIGNAL(isMoving(QPointF&)),
		this, SLOT(isMoving(QPointF&)));

	//�����ͷż��ܵ��źŲ�
	connect(this, SIGNAL(toUseSkills()), this, SLOT(useSkills()));

	CardsUI *temp_card;
	QPointF pos;
	//���ƿ�����Ϣ
	for (int i = 0; i < my_cardStackNo.size(); i++)
	{
		temp_card = new CardsUI(my_cardStackNo.at(i));
		pos = QPointF(COLUMN_POS_X + CARD_DIS * i, PREPARE_COLUMN_POS_Y);
		temp_card->setPos(pos);

		PlayingCard.append(*temp_card->operating_card);
		cardUILists.append(temp_card);
		cardUIPosLists.append(pos);
		cardUIPixmapLists.append(temp_card->pixmap());
	}

	int i = 0;
	foreach(CardsUI* card_temp, cardUILists)
	{
		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		card_temp->using_background = 4;

		//���ڿ��Ƶĵ��
		connect(cardUILists.at(i), SIGNAL(cardIsPressed()),
			this, SLOT(isPressed()));
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()),
			this, SLOT(isReleased()));

		//�򳡾�����Ӳ���
		scene1->addItem(card_temp);
		i++;
	}

	//�������Ƴߴ�
	cardUISizeAdjust();

	//�����ӿ�view������
	view->setScene(scene1);//��ʼ��Ϊ��Ϸ������
	view->setRenderHints(QPainter::Antialiasing);
	view->setContextMenuPolicy(Qt::NoContextMenu);

}

void GamePlayingBackground::updateCoordinate()
{


	CARD_HEIGHT = SCREEN_HEIGHT2*0.123;
	CARD_WIDTH = SCREEN_WIDTH2*0.06;

	COLUMN_POS_X = 0.3*SCREEN_WIDTH2;

	PREPARE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.84;

	M_MELEE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.51;
	M_ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT2*0.62;
	M_SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.73;

	E_MELEE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.37;
	E_ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT2*0.26;
	E_SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.16;

	COLUMN_LENGTH = SCREEN_WIDTH2*0.414;
}

void GamePlayingBackground::updateStatus()
{

}

//�ۺ����������ק���Ƶ��Ƴ���
void GamePlayingBackground::isMoving(QPointF &pos)
{
	int num = cardUILists.indexOf(selected_card);
	int pos_x, pos_y;
	pos_x = cardUIPosLists[num].x() + pos.x();
	pos_y = cardUIPosLists[num].y() + pos.y();


	if (Pressed == true && operation == true &&
		pos_x >= COLUMN_POS_X&&pos_x <= COLUMN_POS_X + COLUMN_LENGTH&&
		pos_y >= E_SIEGE_COLUMN_POS_Y&&pos_y < PREPARE_COLUMN_POS_Y)//ȷ���ڲ������ҽ����ϵ��Ƴ���
	{
		//���ÿ���״̬Ϊ�ڳ���
		PlayingCard[num].isFielded = true;
		cardUILists[num]->operating_card->isFielded = true;
		//���ݲ�ͬ���ల�Ų�ͬλ��
		switch (selected_card->operating_card->genre)
		{
		case 0:
			cardUILists[num]->setPos(pos_x, M_MELEE_COLUMN_POS_Y);
			break;
		case 1:
			cardUILists[num]->setPos(pos_x, M_ARCHER_COLUMN_POS_Y);
			break;
		case 2:
			cardUILists[num]->setPos(pos_x, M_SIEGE_COLUMN_POS_Y);
			break;
		case 3:
			if (pos_y < M_ARCHER_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, M_MELEE_COLUMN_POS_Y);
			}
			else if (pos_y >= M_ARCHER_COLUMN_POS_Y&&pos_y < M_SIEGE_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, M_ARCHER_COLUMN_POS_Y);
			}
			else
			{
				cardUILists[num]->setPos(pos_x, M_SIEGE_COLUMN_POS_Y);
			}
			break;
		case 4:
			if (pos_y < E_ARCHER_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, E_SIEGE_COLUMN_POS_Y);
			}
			else if (pos_y >= E_ARCHER_COLUMN_POS_Y&&pos_y < E_MELEE_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, E_ARCHER_COLUMN_POS_Y);
			}
			else
			{
				cardUILists[num]->setPos(pos_x, E_MELEE_COLUMN_POS_Y);
			}
			break;
		case 5:
			cardUILists[num]->operating_card->isGarbaged = true;
		}
	}

}

//�ۺ�������cardUI��갴��ʱִ�У�������갴�±���Ϊ��
void GamePlayingBackground::isPressed()
{
	Pressed = true;
}

//�ۺ�������cardUI����ͷ�ʱִ��
//����ʱ�ڲ���һ�ſ��ƣ����ͷż���
void GamePlayingBackground::isReleased()
{
	if (operation==true)//���ڲ���һ����
	{
		emit toUseSkills();
		operation= false;
	}

	int i = 0;
	foreach(QPointF card_pos, cardUIPosLists)
	{
		Q_UNUSED(card_pos);
		cardUIPosLists[i] = cardUILists[i]->pos();
		i++;
	}

}

//�ۺ�������scene��selectedItem�仯ʱ������ͬ���źŵ��˲�
void GamePlayingBackground::selectionChanged()
{
	int i = 0;

	QList<QGraphicsItem *> items = scene1->selectedItems();
	if (items.count() == 1)
	{
		//��ǰ��ѡ���UIͼ�������
		QPointF pos = items.first()->pos();
		CardsUI* card_temp = dynamic_cast<CardsUI*>(items.first());

		foreach(CardsUI* card, cardUILists)
		{
			if (card == card_temp)
				break;
			i++;
		}

		if (operation == false)//û���ڲ�������
		{
			selected_card = cardUILists[i];
			operation = true;
		}
	}
	else
	{
		return;
	}
}

//�ۺ��������ڿ���ʩ�ż���
void GamePlayingBackground::useSkills()
{

}

//�ж�cardUI���յ��Ƿ��ǵ����źš�
//�ж������ǵ�ǰ������cardUI�����pos��洢��cardUIPosListsd��λ�ñȽϣ������Ϊ������
//����ѡȡ���õĿ���
bool GamePlayingBackground::isCardUIClicked()
{
	int i = -1;
	QList<QGraphicsItem *> items = scene1->selectedItems();

	if (items.count() == 1)
	{
		QPointF pos = items.first()->pos();
		CardsUI* card_temp = dynamic_cast<CardsUI*>(items.first());
		i = cardUILists.indexOf(card_temp);
		if (pos == cardUIPosLists.at(i))
		{
			return true;
		}
	}
	return false;
}

void GamePlayingBackground::cardUISizeAdjust()
{
	quint16 i = 0;
	foreach(CardsUI* card, cardUILists)
	{

		QPixmap pixmap = cardUIPixmapLists.at(i);
		pixmap = pixmap.scaled(CARD_WIDTH, CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		cardUILists[i]->setPixmap(pixmap);
		i++;
	}
}

void GamePlayingBackground::getFromText()
{
	QFile file("playingCardStack.txt");

	if (file.open(QFile::ReadOnly))
	{
		QTextStream inPut(&file);
		QString temp_No;
		while (!inPut.atEnd())
		{
			temp_No = inPut.readLine();
			my_cardStackNo.append(temp_No.toInt());
		}
	}

	file.close();
}

void GamePlayingBackground::putInText()
{
	QFile file("playingCardStack.txt");

	if (file.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		QTextStream outPut(&file);

		foreach(CardsUI *card, cardUILists)
		{
			outPut << card->operating_card->No;
			outPut << "\n";
		}
	}
	file.close();
}

void GamePlayingBackground::resizeEvent(QResizeEvent *event)
{
	//��ӱ���
	QPixmap background(":/backgrounds/Resources/backgrounds/gamePlaying_background.jpg");//���ر���ͼƬ
	QPalette palette;
	view->setGeometry(geometry().x(), geometry().y(),
		geometry().width(), geometry().height());//�����ӿڳߴ��봰��һ����
	palette.setBrush(view->backgroundRole(),
		QBrush(background.scaled(view->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));//���ñ�����ˢ
	view->setPalette(palette);

	//��������Ĵ�С
	scene1->setSceneRect(geometry().x(), geometry().y(),
		geometry().width(), geometry().height());

	//����ȫ�ֱ���
	SCREEN_WIDTH2 = this->width();
	SCREEN_HEIGHT2 = this->height();
	updateCoordinate();

}
