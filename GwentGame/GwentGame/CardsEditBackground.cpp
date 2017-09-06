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
#include<QFile>
#include<QTextStream>

#include"CP_AllCards.h"

#define min(x,y) x<y?x:y//��Сֵ
#define max(x,y) x>y?x:y//���ֵ

//����ȫ�ֱ���
const qreal CARD_DIS = 150;//���Ƽ��
const int CARDS_NUM = 29;//���п�������

int SCREEN_WIDTH = 1800;//������
int SCREEN_HEIGHT = 961;//����߶�

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

	view = new QGraphicsView(this);
	scene = new CardsScene();
	stack = new CardStack();
	selected_card = new CardsUI();

	//���ļ��ж�ȡ����
	getFromText();

	//����ͼ���ƶ����źŲ�
	connect(scene, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	connect(scene, SIGNAL(isMoving(QPointF&)),
		this, SLOT(isMoving(QPointF&)));

	//������ӿ��Ƶ��źŲ�
	connect(this, SIGNAL(toEdit()), this, SLOT(editStacks()));

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
		for (int i = 0; i < cardStackNo.size(); i++)
		{
			if (cardStackNo[i] == card_temp->operating_card->No)
			{
				card_temp->operating_card->isInGameCardsStack = true;
			}
		}

		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		card_temp->using_background = 3;

		//���ڿ��Ƶĵ��
		connect(cardUILists.at(i), SIGNAL(cardIsPressed()),
			this, SLOT(isPressed()));
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()),
			this, SLOT(isReleased()));

		//�򳡾�����Ӳ���
		if (card_temp->pos().x() >= BASIS_COLUMN_POS_X&&
			card_temp->pos().x() + NONSELECTED_CARD_WIDTH <= BASIS_COLUMN_POS_X + BASIS_COLUMN_LENGTH)
		{
			scene->addItem(card_temp);
		}
		i++;
	}

	//��ʼ������ͼ��
	updatePictures();
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
	 SELECTED_CARD_WIDTH = SCREEN_WIDTH*0.065;//��ѡ��Ŀ��ƿ��
	 SELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.145;//��ѡ���Ƹ߶�

	 //������x����
	 FIGHT_COLUMN_POS_X = SCREEN_WIDTH*0.231;//��ս��Զ�̡�����������x����
	 FUNCTION_COLUMN_POS_X = SCREEN_WIDTH*0.248;//��������x����
	 BASIS_COLUMN_POS_X = SCREEN_WIDTH*0.222;//δ��ѡ������x����


	
	 BASIS_COLUMN_POS_Y = SCREEN_HEIGHT*0.734;//δ��ѡ������y����
	 MELEE_COLUMN_POS_Y = SCREEN_HEIGHT*0.115;//��ս��y����
	 ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT*0.235;//Զ����y����
	 SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT*0.361;//������y����
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
		//�ҵ�δѡ�����е�һ�������һ����,���ƿ�����ק��Χ
		int j, k, min_pos = selected_card->pos().x(), max_pos = selected_card->pos().x();
		for (j = 0; j < cardUILists.size(); j++)
		{
			if (cardUILists.at(j)->pos().y() == selected_card->pos().y())
			{
				min_pos = min(cardUILists.at(j)->pos().x(), min_pos);
				max_pos = max(cardUILists.at(j)->pos().x(), max_pos);
			}
		}

		if (selected_card->pos().y() == BASIS_COLUMN_POS_Y)
		{
			max_pos = max_pos + pos.x() + NONSELECTED_CARD_WIDTH;
			if (min_pos + pos.x() > BASIS_COLUMN_POS_X) //)|| max_pos < BASIS_COLUMN_POS_X + BASIS_COLUMN_LENGTH)
			{
				pos = QPointF(BASIS_COLUMN_POS_X - min_pos, pos.y());

			}
			else if (max_pos + pos.x() + NONSELECTED_CARD_WIDTH < BASIS_COLUMN_POS_X + BASIS_COLUMN_LENGTH)
			{
				pos = QPointF(BASIS_COLUMN_POS_X + BASIS_COLUMN_LENGTH - max_pos-NONSELECTED_CARD_WIDTH, pos.y());
			}
		}
		else if (selected_card->pos().y() == FUNCTION_COLUMN_POS_Y)
		{

			if (min_pos + pos.x() > FUNCTION_COLUMN_POS_X )
			{
				pos = QPointF(FUNCTION_COLUMN_POS_X - min_pos, pos.y());

			}

			else if (max_pos + pos.x() + SELECTED_CARD_WIDTH < FUNCTION_COLUMN_POS_X + FUNCTION_COLUMN_LENGTH)
			{
				pos = QPointF(FUNCTION_COLUMN_POS_X + FUNCTION_COLUMN_LENGTH - max_pos - SELECTED_CARD_WIDTH, pos.y());
			}
		}
		else
		{
			if (min_pos + pos.x() > FIGHT_COLUMN_POS_X)
			{
				pos = QPointF(FIGHT_COLUMN_POS_X - min_pos, pos.y());

			}

			else if (max_pos + pos.x() + SELECTED_CARD_WIDTH < FIGHT_COLUMN_POS_X + FIGHT_COLUMN_LENGTH)
			{
				pos = QPointF(FIGHT_COLUMN_POS_X + FIGHT_COLUMN_LENGTH - max_pos - SELECTED_CARD_WIDTH, pos.y());
			}

		}

		//���п�����ק(�����жϿ�������һ����
		if (selected_card->operating_card->isInGameCardsStack == false)
		{
			foreach(CardsUI* card_temp, cardUILists)
			{
				if (card_temp->operating_card->isInGameCardsStack == false)
				{
					card_temp->setPos(cardUIPosLists.at(i).x() + pos.x(),
						cardUIPosLists.at(i).y());
				}
				i++;
			}
		}
		else
		{
			foreach(CardsUI* card_temp, cardUILists)
			{
				if (card_temp->operating_card->isInGameCardsStack == true&&
					card_temp->operating_card->genre== selected_card->operating_card->genre)
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
	{
		emit toEdit();
	}
}

//�ۺ�������scene��selectedItem�仯ʱ������ͬ���źŵ��˲�
void CardsEditBackground::selectionChanged()
{
	int i = 0, j = 0, k = 0, num = 0;

	QList<QGraphicsItem *> items = scene->selectedItems();
	if (items.count() == 1)
	{
		//��ǰ��ѡ��Ŀ��Ƶ�����
		QPointF pos = items.first()->pos();
		CardsUI* card_temp = dynamic_cast<CardsUI*>(items.first());

		foreach(CardsUI* card, cardUILists)
		{
			if (card == card_temp)
			{
				selected_card = card;
				break;
			}
			i++;
		}
		j = i;
		i = 0;

		//���ÿ���λ��
		foreach(QPointF card_pos, cardUIPosLists)
		{
			Q_UNUSED(card_pos);
			if (i <= j)
			{
				for (k = i, num = 0; k < j; k++)
				{
					if (cardUILists[k]->pos().y()==selected_card->pos().y())
					{
						num++;
					}
				}
				cardUIPosLists[i].setX(-num*NONSELECTED_CARD_WIDTH + pos.x());
			}
			else
			{
				for (k = j, num = 0; k < i; k++)
				{
					if (cardUILists[k]->pos().y() == selected_card->pos().y())
					{
						num++;
					}
				}
				cardUIPosLists[i].setX(num*NONSELECTED_CARD_WIDTH + pos.x());
			}

			i++;
		}
	}
	else
	{
		return;
	}
}

//�ۺ���������������¼�
void CardsEditBackground::editStacks()
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

	//�����������δѡ��������
	if (cardUILists[No]->operating_card->isInGameCardsStack == false)
	{
		cardUILists[No]->operating_card->isInGameCardsStack = true;//��������Ϊ����Ϸ������
	}

	//�������ڳ�����Ϸ������
	else
	{	
		cardUILists[No]->operating_card->isInGameCardsStack = false;//�����˻�δѡ������
	}
	updatePictures();
	cardUISizeAdjust();

	//����Ϣ�����ı��ļ���
	putInText();
}

void CardsEditBackground::updatePictures()
{
	int num1 = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0, num6 = 0, num7 = 0;
	//����ÿһ����������ͼ��λ��
	foreach(CardsUI *card, cardUILists)
	{
		if (card->operating_card->isInGameCardsStack == false)
		{
			card->setPos(BASIS_COLUMN_POS_X + num1*NONSELECTED_CARD_WIDTH, BASIS_COLUMN_POS_Y);
			num1++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 0)
		{
			card->setPos(FIGHT_COLUMN_POS_X + num2*SELECTED_CARD_WIDTH, MELEE_COLUMN_POS_Y);
			num2++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 1)
		{
			card->setPos(FIGHT_COLUMN_POS_X + num3*SELECTED_CARD_WIDTH, ARCHER_COLUMN_POS_Y);
			num3++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 2)
		{
			card->setPos(FIGHT_COLUMN_POS_X + num4*SELECTED_CARD_WIDTH, SIEGE_COLUMN_POS_Y);
			num4++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 3)
		{
			card->setPos(FIGHT_COLUMN_POS_X + num5*SELECTED_CARD_WIDTH, ARCHER_COLUMN_POS_Y);
			num5++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 4)
		{
			card->setPos(FIGHT_COLUMN_POS_X + num6*SELECTED_CARD_WIDTH, ARCHER_COLUMN_POS_Y);
			num6++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 5)
		{
			card->setPos(FUNCTION_COLUMN_POS_X + num7*SELECTED_CARD_WIDTH, FUNCTION_COLUMN_POS_Y);
			num7++;
		}
	}

	int i = 0;
	foreach(CardsUI *card, cardUILists)
	{
		cardUIPosLists[i] = card->pos();
		i++;
	}
}

void CardsEditBackground::putInText()
{
	QFile file("gameCardStack.txt");

	if (file.open(QFile::WriteOnly | QIODevice::Truncate| QIODevice::Text))
	{
		QTextStream outPut(&file);

		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isInGameCardsStack == true)
			{
				outPut << card->operating_card->No;
				outPut << "\n";
			}
		}
	}
	file.close();
}

void CardsEditBackground::getFromText()
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


		//�򳡾�����ӿ���
		if (card->pos().x() >= BASIS_COLUMN_POS_X-10  &&
			card->pos().x() + NONSELECTED_CARD_WIDTH <= BASIS_COLUMN_POS_X + BASIS_COLUMN_LENGTH+10)
		{
			scene->addItem(card);
		}
		else
		{
			scene->removeItem(card);
		}

		QPixmap pixmap = cardUIPixmapLists.at(i);
		if (card->operating_card->isInGameCardsStack == false)
		{
			pixmap = pixmap.scaled(NONSELECTED_CARD_WIDTH, NONSELECTED_CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		}
		else
		{
			pixmap = pixmap.scaled(SELECTED_CARD_WIDTH, SELECTED_CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		}
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
