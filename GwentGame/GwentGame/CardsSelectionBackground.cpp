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

#include"CP_AllCards.h"

//����ȫ�ֱ���
const qreal CARD_POS_Y = 250;//����y���꣨����ˮƽ����Ϊ������
const qreal CARD_DIS = 300;//���Ƽ��
const qreal CARD_STA = 100;//��һ�ſ��ƿ�ʼʱ��λ��
int   SCREEN_SIZE = 2250;//���ڿ��

CardsSelectionBackground::CardsSelectionBackground(QWidget *parent)
	: QWidget(parent)
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
	scene = new CardsSelectionScene();


	//����ͼ���ƶ����źŲ�
	connect(scene, SIGNAL(selectionChanged()), 
		this, SLOT(selectionChanged()));    
	connect(scene, SIGNAL(isMoving(QPointF&)), 
		this, SLOT(isMoving(QPointF&))); 

	CardsUI *temp_card;
	QPointF pos;

	//��ӿ��ƣ���Ҫ�Ľ�����ӿ���ѡ������
	//��һ���ƣ���Ӱ����
	temp_card = new CardsUI(Unseen_Elder_No);
	pos = QPointF(CARD_STA, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//�ڶ����ƣ����˶���Ť��֮��
	temp_card = new CardsUI(Bekker_Twister_Mirror_No);
	pos = QPointF(CARD_STA + CARD_DIS * 1, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//�������ƣ�����Ũ��
	temp_card = new CardsUI(Impenetrable_Fog_No);
	pos = QPointF(CARD_STA + CARD_DIS * 2, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//�������ƣ��̹Ǳ�˪
	temp_card = new CardsUI(Biting_Frost_No);
	pos = QPointF(CARD_STA + CARD_DIS * 3, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());


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
		if (min_pos < 0||max_pos > SCREEN_SIZE)
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
	if (isCardUIClicked())
		qDebug() << "clicked";
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

		/*QPixmap pixmap = cardUIPixmapLists.at(i);
		pixmap = pixmap.scaled(350, 400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		cardUILists[i]->setPixmap(pixmap);*/

		//�ı����˿��ƴ�С
		if (pos.x() >= 0 && pos.x()<= SCREEN_SIZE / 2)
		{			
			quint16 width = pos.x() / 5 + 200;
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(width, width*1.1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
		}

		if (pos.x()>SCREEN_SIZE / 2 && pos.x() <= SCREEN_SIZE )
		{
			quint16 width = (SCREEN_SIZE - pos.x()) / 5 + 200;
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(width, width*1.1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
		}
		i++;
	}
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

