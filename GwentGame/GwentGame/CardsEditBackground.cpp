#include "CardsEditBackground.h"

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
const qreal CARD_DIS = 150;//���Ƽ��
const int CARDS_NUM = 29;//���п�������

int SCREEN_WIDTH = 1800;//������
int SCREEN_HEIGHT = 1000;//����߶�

int NONSELECTED_CARD_WIDTH;//���·�����δѡ��Ŀ��ƿ��
int NONSELECTED_CARD_HEIGHT;//���·�����δѡ��Ŀ��Ƹ߶�
int SELECTED_CARD_WIDTH;//��ѡ��Ŀ��ƿ��
int SELECTED_CARD_HEIGHT;//��ѡ���Ƹ߶�

int FIGHT_COLUMN_POS_X;// ��ս��Զ�̡�����������x����
int FUNCTION_COLUMN_POS_X;//��������x����
int BASIS_COLUMN_POS_X;//δ��ѡ������x����
int MELEE_COLUMN_POS_Y;//��ս��y����
int ARCHER_COLUMN_POS_Y;//Զ����y����
int SIEGE_COLUMN_POS_Y;//������y����
int FUNCTION_COLUMN_POS_Y;//��������y����
int BASIS_COLUMN_POS_Y;//δ��ѡ������y����

int FIGHT_COLUMN_LENGTH;//��ս��Զ�̡�������������
int FUNCTION_COLUMN_LENGTH;//������������
int BASIS_COLUMN_LENGTH;//δ��ѡ����������

CardsEditBackground::CardsEditBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	updateCoordinate();//��ʼ����������

	init();//��ʼ������

}

CardsEditBackground::~CardsEditBackground()
{
}

void CardsEditBackground::init()
{
	//��ʼ��
	Pressed = false;
	change_size = false;

	view = new QGraphicsView(this);
	scene = new CardsScene();
	stack = new CardStack();

	//����ͼ���ƶ����źŲ�
	connect(scene, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	connect(scene, SIGNAL(isMoving(QPointF&)),
		this, SLOT(isMoving(QPointF&)));

	CardsUI *temp_card;
	QPointF pos;
	AllCards all_cards;
	
	//������༭���м������п���
	for (int i = 1; i <= CARDS_NUM; i++)
	{
		temp_card = new CardsUI(all_cards.AllCardsNo[i]);
		pos = QPointF(BASIS_COLUMN_POS_X+(i-1)*NONSELECTED_CARD_WIDTH, BASIS_COLUMN_POS_Y);
		temp_card->setPos(pos);
		cardUILists.append(temp_card);
		cardUIPosLists.append(pos);
		cardUIPixmapLists.append(temp_card->pixmap());

	}

	int i = 0;
	foreach(CardsUI* card_temp, cardUILists)
	{

		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		card_temp->using_background = 3;

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

void  CardsEditBackground::updateCoordinate()
{
	//���Ƴߴ�
	 NONSELECTED_CARD_WIDTH = SCREEN_WIDTH*0.082;//���·�����δѡ��Ŀ��ƿ��
	 NONSELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.17;//���·�����δѡ��Ŀ��Ƹ߶�
	 SELECTED_CARD_WIDTH = SCREEN_WIDTH*0.048;//��ѡ��Ŀ��ƿ��
	 SELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.114;//��ѡ���Ƹ߶�

	 //������x����
	 FIGHT_COLUMN_POS_X = SCREEN_WIDTH*0.231;//��ս��Զ�̡�����������x����
	 FUNCTION_COLUMN_POS_X = SCREEN_WIDTH*0.248;//��������x����
	 BASIS_COLUMN_POS_X = SCREEN_WIDTH*0.222;//δ��ѡ������x����


	
	 BASIS_COLUMN_POS_Y = (SCREEN_HEIGHT*0.72)+geometry().y();//δ��ѡ������y����
	 MELEE_COLUMN_POS_Y = SCREEN_HEIGHT*0.1;//��ս��y����
	 ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT*0.224;//Զ����y����
	 SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT*0.346;//������y����
	 FUNCTION_COLUMN_POS_Y = SCREEN_HEIGHT*0.5;//��������y����


	 //����������
	 FIGHT_COLUMN_LENGTH = SCREEN_WIDTH*0.5364;//��ս��Զ�̡�������������
	 FUNCTION_COLUMN_LENGTH = SCREEN_WIDTH*0.5; //������������
	 BASIS_COLUMN_LENGTH = SCREEN_WIDTH*0.5593;//δ��ѡ����������
}

//�ۺ����������קsceneʱִ��
//��������ˮƽ�ƶ�
void  CardsEditBackground::isMoving(QPointF &pos)
{
	int i = 0;

	if (Pressed)
	{
		//�ҵ�δѡ�����е�һ�������һ����,���ƿ���λ��
		int j, k;
		for (j = 0; j < cardUIPosLists.size(); j++)
		{
			if (cardUILists[j]->operating_card->isInGameCardsStack == false)
			{
				break;
			}
		}
		for (k = cardUIPosLists.size()-1; k>=0; k--)
		{
			if (cardUILists[k]->operating_card->isInGameCardsStack == false)
			{
				break;
			}
		}
		int min_pos, max_pos;
		min_pos = cardUIPosLists.at(j).x() + pos.x();
		max_pos = cardUIPosLists.at(k).x()
			+ pos.x() + cardUIPixmapLists.at(k).width();
		if (min_pos > BASIS_COLUMN_POS_X || max_pos < BASIS_COLUMN_POS_X+ BASIS_COLUMN_LENGTH)
		{
			return;
		}

		//���п����ƶ�
		else
		{
			foreach(CardsUI* card_temp, cardUILists)
			{
				if(card_temp->operating_card->isInGameCardsStack==false)
				{
					card_temp->setPos(cardUIPosLists.at(i).x() + pos.x(),
						cardUIPosLists.at(i).y());
				}
				i++;
			}
		}

		cardUISizeAdjust();
	}
}

//�ۺ�������cardUI��갴��ʱִ�У�������갴�±���Ϊ��
void CardsEditBackground::isPressed()
{
	Pressed = true;
}

//�ۺ�������cardUI����ͷ�ʱִ��
//Ӧ�����ñ�־λ����UIͼƬֹͣ������϶��¼�����Ӧ
void CardsEditBackground::isReleased()
{
	if (isCardUIClicked())
		qDebug() << "clicked";
}

//�ۺ�������scene��selectedItem�仯ʱ������ͬ���źŵ��˲�
void CardsEditBackground::selectionChanged()
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
			cardUIPosLists[i].setX((i - j)*NONSELECTED_CARD_WIDTH + pos.x());
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
bool CardsEditBackground::isCardUIClicked()
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

void CardsEditBackground::cardUISizeAdjust()
{
	quint16 i = 0;
	foreach(CardsUI* card, cardUILists)
	{
		QPointF pos = card->pos();

		pos.setX(card->pos().x() + card->pixmap().width());
		pos.setY(card->pos().y());

		QPixmap pixmap = cardUIPixmapLists.at(i);
		pixmap = pixmap.scaled(NONSELECTED_CARD_WIDTH, NONSELECTED_CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		cardUILists[i]->setPixmap(pixmap);
		i++;
	}
}

void CardsEditBackground::resizeEvent(QResizeEvent *event)
{

	//��ӱ���
	QPixmap background(":/backgrounds/Resources/backgrounds/cardsEdit_background.jpg");//���ر���ͼƬ
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
	SCREEN_WIDTH = this->width();
	SCREEN_HEIGHT = this->height();
	updateCoordinate();


}
