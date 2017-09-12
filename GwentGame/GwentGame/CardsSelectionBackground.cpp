#include "CardsSelectionBackground.h"
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
#include<stdlib.h>
#include<time.h>


#include"CP_AllCards.h"

//����ȫ�ֱ���
const qreal CARD_POS_Y = 250;//����y���꣨����ˮƽ����Ϊ������
const qreal CARD_DIS = 300;//���Ƽ��
const qreal CARD_STA = 100;//��һ�ſ��ƿ�ʼʱ��λ��
int   SCREEN_SIZE = 2000;//���ڿ��

CardsSelectionBackground::CardsSelectionBackground(int turn,QWidget *parent)
	: QWidget(parent),
	my_turn(turn)
{
	ui.setupUi(this);

	//��ʼ������
	init();

	
}

CardsSelectionBackground::~CardsSelectionBackground()
{
}

void CardsSelectionBackground::init()
{
	//��ʼ��
	Pressed = false;

	view = new QGraphicsView(this);
	scene = new CardsScene();
	cardsSelectionFinished_button = new QPushButton(this);
	textLabel = new QLabel(this);
	cardsToAdjust_number = 3;

	if (my_turn == 1)
	{
		cardsToAdjust_number = 3;
	}
	if (my_turn == 2)
	{
		cardsToAdjust_number = 2;
	}
	if (my_turn == 3)
	{
		cardsToAdjust_number = 1;
	}

	//��ʼ����ťͼ��
	QIcon cardsSelectionFinished_button_icon;
	cardsSelectionFinished_button_icon.addFile(":/buttons/Resources/buttons/cardsSelectionFinished_button.png");
	cardsSelectionFinished_button->setIcon(cardsSelectionFinished_button_icon);
	cardsSelectionFinished_button->setGeometry(855, 895, 155, 55);
	cardsSelectionFinished_button->setIconSize(QSize(150, 80));
	cardsSelectionFinished_button->setFlat(true);

	//���ı��л�ȡ����
	getFromText();

	//���ô�������
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//����ͼ���ƶ����źŲ�
	connect(scene, SIGNAL(selectionChanged()), 
		this, SLOT(selectionChanged()));    
	connect(scene, SIGNAL(isMoving(QPointF&)), 
		this, SLOT(isMoving(QPointF&))); 

	//��������������Ƶ��źŲ�
	connect(this, SIGNAL(toChange()), this, SLOT(changeCard()));

	CardsUI *temp_card;
	QPointF pos;
	AllCards all_cards;

	//�����ӿ���
	for (int i = 0; i < 10; i++)
	{
		srand((unsigned)time(NULL));
		int n = (rand() % cardStackNo.size());

		temp_card = new CardsUI(cardStackNo.at(n));
		pos = QPointF(CARD_STA + CARD_DIS * i, CARD_POS_Y);
		temp_card->setPos(pos);
		cardUILists.append(temp_card);
		cardUIPosLists.append(pos);
		cardUIPixmapLists.append(temp_card->pixmap());
		cardStackNo.removeAt(n);
	}

	putInText();

	int i = 0;
	foreach(CardsUI* card_temp, cardUILists)
	{

		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		card_temp->using_background = 2;

		//���ڿ��Ƶĵ��
		connect(cardUILists.at(i), SIGNAL(cardIsPressed()), 
			this, SLOT(isPressed()));  
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()), 
			this, SLOT(isReleased()));
		//�򳡾�����Ӳ���
		scene->addItem(card_temp);

		i++;
	}

	//����ı� 
	textLabel->setText(tr("remain times to use: %1/3 ").arg(cardsToAdjust_number));
	QFont font;
	font.setPixelSize(30); // ���ش�С 
	textLabel->setFont(font);
	QPalette palette;
	palette.setColor(QPalette::WindowText, Qt::white);
	textLabel->setPalette(palette);
	textLabel->setGeometry(750, 110, 400, 50);

	//���¼���UI�ߴ�
	cardUISizeAdjust();

	//�����ӿ�view������
	view->setScene(scene);
	view->setRenderHints(QPainter::Antialiasing);
	view->setContextMenuPolicy(Qt::NoContextMenu);


}

//�ۺ����������קsceneʱִ��
//����UIˮƽ
void  CardsSelectionBackground::isMoving(QPointF &pos)
{
	int i = 0;

	int min_pos, max_pos;
	min_pos = cardUIPosLists.at(0).x() + pos.x();
	max_pos = cardUIPosLists.at(cardUIPosLists.size() - 1).x() + pos.x() + cardUIPixmapLists.at(cardUIPosLists.size() - 1).width();
	if (Pressed)
	{
		if (min_pos > 40||max_pos < SCREEN_SIZE)
		{
			return;
		}
		else
		{
			foreach(CardsUI* card_temp, cardUILists)
			{
				card_temp->setPos(cardUIPosLists.at(i).x() + pos.x(), CARD_POS_Y);
				i++;
			}
		}

		cardUISizeAdjust();
	}
}

//�ۺ�������cardUI��갴��ʱִ�У�������갴�±���Ϊ��
void CardsSelectionBackground::isPressed()
{
	Pressed = true;
}

//�ۺ�������cardUI����ͷ�ʱִ��
//Ӧ�����ñ�־λ����UIͼƬֹͣ������϶��¼�����Ӧ
void CardsSelectionBackground::isReleased()
{
	if (isCardUIClicked()&&cardsToAdjust_number>0)
	{
		emit toChange();
	}
}

//�ۺ�������scene��selectedItem�仯ʱ������ͬ���źŵ��˲�
void CardsSelectionBackground::selectionChanged()
{
	int i = 0, j = 0;

	QList<QGraphicsItem *> items = scene->selectedItems();
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
		j = i;
		i = 0;

		foreach(QPointF card_pos, cardUIPosLists)
		{
			Q_UNUSED(card_pos);
			cardUIPosLists[i].setX((i - j)*CARD_DIS + pos.x());
			cardUIPosLists[i].setY(CARD_POS_Y);
			i++;
		}

	}
	else
	{
		return;
	}
}

//�ۺ��������������ʱ��������
void CardsSelectionBackground::changeCard()
{
	//ѡ������Ŀ���
	int No = 0;
	QList<QGraphicsItem *> items = scene->selectedItems();
	if (items.count() == 1)
	{
		//��ǰ��ѡ��Ŀ��Ƶ�����
		QPointF pos = items.first()->pos();
		CardsUI* card_temp = dynamic_cast<CardsUI*>(items.first());

		foreach(CardsUI* card, cardUILists)
		{
			if (card == card_temp)
				break;
			No++;
		}
	}

	if (cardsToAdjust_number > 0)
	{
		CardsUI *temp_card;

		srand((unsigned)time(NULL));
		int n = (rand() % cardStackNo.size());
		temp_card = new CardsUI(cardStackNo.at(n));
		temp_card->setPos(cardUILists[No]->pos());
		cardStackNo.removeAt(n);

		//ȥ��ԭ������
		scene->removeItem(cardUILists[No]);
		cardUILists.removeAt(No);
		cardUIPosLists.removeAt(No);
		cardUIPixmapLists.removeAt(No);

		//��������
		cardUIPosLists.insert(No, temp_card->pos());
		cardUIPixmapLists.insert(No, temp_card->pixmap());
		cardUILists.insert(No, temp_card);

		putInText();

		//�������ƵĲ���
		cardUILists[No]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		cardUILists[No]->using_background = 2;

		//���ڿ��Ƶĵ��
		connect(cardUILists[No], SIGNAL(cardIsPressed()),
			this, SLOT(isPressed()));
		connect(cardUILists[No], SIGNAL(cardIsReleased()),
			this, SLOT(isReleased()));

		//�����Ʋ��볡��
		scene->addItem(cardUILists[No]);

		cardsToAdjust_number--;

		textLabel->setText(tr("remain times to use: %1/3 ").arg(cardsToAdjust_number));
	}

	cardUISizeAdjust();

}

//�ж�cardUI���յ��Ƿ��ǵ����źš�
//�ж������ǵ�ǰ������cardUI�����pos��洢��cardUIPosListsd��λ�ñȽϣ������Ϊ������
//���ڸ�������
bool CardsSelectionBackground::isCardUIClicked()
{
	int i = -1;
	QList<QGraphicsItem *> items = scene->selectedItems();

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

void CardsSelectionBackground::cardUISizeAdjust()
{
	quint16 i = 0;
	foreach(CardsUI* card, cardUILists)
	{
		QPointF pos = card->pos();

		pos.setX(card->pos().x()+card->pixmap().width());

		//�ı����˿��ƴ�С
		if ( pos.x()<= SCREEN_SIZE / 2)
		{			
			quint16 width = pos.x() / 5 + 200;
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(width, width*1.1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
		}

		if (pos.x()> SCREEN_SIZE / 2)
		{
			quint16 width = (SCREEN_SIZE - pos.x()) / 5 + 200;
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(width, width*1.1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
		}
		i++;
	}
}

void CardsSelectionBackground::getFromText()
{
	QFile file("gameCardStack.txt");

	if (file.open(QFile::ReadOnly))
	{
		QTextStream inPut(&file);
		QString temp_No;
		while (!inPut.atEnd())
		{
			temp_No = inPut.readLine();
			cardStackNo.append(temp_No.toInt());
		}
	}

	file.close();
}

void CardsSelectionBackground::putInText()
{
	QFile file("my_playingCardStack.txt");

	if (file.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		QTextStream outPut(&file);

		foreach(CardsUI* temp_card, cardUILists)
		{
			outPut << temp_card->operating_card->No;
			outPut << "\n";
		}
	}
	file.close();
}

void CardsSelectionBackground::resizeEvent(QResizeEvent *event)
{
	//��ӱ���
	QPixmap background(":/backgrounds/Resources/backgrounds/cardsSelection_background.jpg");//���ر���ͼƬ
	QPalette palette;
	view->setGeometry(geometry().x(), geometry().y(), 
		geometry().width(), geometry().height());//�����ӿڳߴ��봰��һ����
	palette.setBrush(view->backgroundRole(),
		QBrush(background.scaled(view->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));//���ñ�����ˢ
	view->setPalette(palette);

	//��������Ĵ�С
	scene->setSceneRect(geometry().x(), geometry().y(),
		geometry().width(), geometry().height());

	//��ȡ���ڿ��
	SCREEN_SIZE = this->width();
	
}

