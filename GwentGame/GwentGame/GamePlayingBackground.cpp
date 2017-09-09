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
#include<QCursor>

//����ȫ�ֱ���
const qreal CARD_DIS = 80;//���Ƽ��

const qreal CHOOSE_CARD_DIS = 300;
const qreal CHOOSE_CARD_HEIGHT = 400;

int SCREEN_WIDTH2 = 1800;//������
int SCREEN_HEIGHT2 = 960;//����߶�

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

//ʹ�ü��ܴ���
const int UNSEEN_ELDER_SKILL_TIMES = 2;
const int NORMAL_SKILL_TIMES = 1;

GamePlayingBackground::GamePlayingBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//��ʼ�����г���
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
	isUsingSkill = false;
	usingSkillTimes = 0;
	useMainScene = true;

	m_Melee_weather = 0;
	m_Archer_weather = 0;
	m_Siege_weather = 0;
	e_Melee_weather = 0;
	e_Archer_weather = 0;
	e_Siege_weather = 0;


	//���ı��л�ȡ����
	getFromText();

	view = new QGraphicsView(this);
	main_scene = new CardsScene();
	choose_scene = new CardsScene();

	//���ô������ԣ�û�й�������
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//����ͼ���ƶ����źŲ�(��Ϸ�����棩
	connect(main_scene, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	connect(main_scene, SIGNAL(isMoving(QPointF&)),
		this, SLOT(isMoving(QPointF&)));

	//�����ͷż��ܵ��źŲ�
	connect(this, SIGNAL(toUseSkills(Card *)), this, SLOT(useSkills(Card *)));

	CardsUI *temp_card;
	QPointF pos;
	//���ƿ�����Ϣ
	for (int i = 0; i < my_cardStackNo.size(); i++)
	{
		temp_card = new CardsUI(my_cardStackNo.at(i));
		pos = QPointF(COLUMN_POS_X + CARD_DIS * i, PREPARE_COLUMN_POS_Y);
		temp_card->setPos(pos);

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
			this, SLOT(CardisPressed()));
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()),
			this, SLOT(CardisReleased()));

		//�򳡾�����Ӳ���
		main_scene->addItem(card_temp);
		i++;
	}

	selected_card = cardUILists[0];//��ʼ��

	//�������Ƴߴ�
	cardUISizeAdjust();

	//�����ӿ�view������
	view->setScene(main_scene);//��ʼ��Ϊ��Ϸ������
	view->setRenderHints(QPainter::Antialiasing);
	view->setContextMenuPolicy(Qt::NoContextMenu);

}

void GamePlayingBackground::updateCoordinate()
{

	CARD_HEIGHT = SCREEN_HEIGHT2*0.123;
	CARD_WIDTH = SCREEN_WIDTH2*0.06;

	COLUMN_POS_X = 0.3*SCREEN_WIDTH2;

	PREPARE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.87;

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
	int num1 = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0, num6 = 0, num7 = 0;
	foreach(CardsUI* card_temp, cardUILists)
	{
		if (card_temp->operating_card->isGarbaged == true)//���ƽ����Ĺ
		{
			main_scene->removeItem(card_temp);
		}
		else if(card_temp->operating_card->isFielded==false)//���Ʋ��ڳ���
		{
			card_temp->setPos(COLUMN_POS_X + num1*CARD_DIS, PREPARE_COLUMN_POS_Y);
			num1++;
		}
		else if(card_temp->pos().y()==M_MELEE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num2*CARD_DIS,M_MELEE_COLUMN_POS_Y);
			num2++;
		}
		else if (card_temp->pos().y() == M_ARCHER_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num3*CARD_DIS, M_ARCHER_COLUMN_POS_Y);
			num3++;
		}
		else if (card_temp->pos().y() == M_SIEGE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num4*CARD_DIS, M_SIEGE_COLUMN_POS_Y);
			num4++;
		}
		else if (card_temp->pos().y() == E_MELEE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num5*CARD_DIS, E_MELEE_COLUMN_POS_Y);
			num5++;
		}
		else if (card_temp->pos().y() == E_ARCHER_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num6*CARD_DIS, E_ARCHER_COLUMN_POS_Y);
			num6++;
		}
		else if (card_temp->pos().y() == E_SIEGE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num7*CARD_DIS, E_SIEGE_COLUMN_POS_Y);
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

void GamePlayingBackground::updateStack(QList<CardsUI*> stack)
{
	cardUILists.clear();
	foreach(CardsUI *card, stack)
	{
		cardUILists.append(card);
	}
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
		cardUILists[num]->operating_card->isFielded = true;
		//���ݲ�ͬ���ల�Ų�ͬλ��
		switch (selected_card->operating_card->genre)
		{
		case 0:
			cardUILists[num]->setPos(pos_x, M_MELEE_COLUMN_POS_Y);
			cardUILists[num]->operating_card->isWeatherControlled = m_Melee_weather;
			break;
		case 1:
			cardUILists[num]->setPos(pos_x, M_ARCHER_COLUMN_POS_Y);
			cardUILists[num]->operating_card->isWeatherControlled = m_Archer_weather;

			break;
		case 2:
			cardUILists[num]->setPos(pos_x, M_SIEGE_COLUMN_POS_Y);
			cardUILists[num]->operating_card->isWeatherControlled = m_Siege_weather;
			break;
		case 3:
			if (pos_y < M_ARCHER_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, M_MELEE_COLUMN_POS_Y);
				cardUILists[num]->operating_card->isWeatherControlled = m_Melee_weather;
			}
			else if (pos_y >= M_ARCHER_COLUMN_POS_Y&&pos_y < M_SIEGE_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, M_ARCHER_COLUMN_POS_Y);
				cardUILists[num]->operating_card->isWeatherControlled = m_Archer_weather;
			}
			else
			{
				cardUILists[num]->setPos(pos_x, M_SIEGE_COLUMN_POS_Y);
				cardUILists[num]->operating_card->isWeatherControlled = m_Siege_weather;
			}
			break;
		case 4:
			if (pos_y < E_ARCHER_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, E_SIEGE_COLUMN_POS_Y);
				cardUILists[num]->operating_card->isWeatherControlled = e_Siege_weather;
			}
			else if (pos_y >= E_ARCHER_COLUMN_POS_Y&&pos_y < E_MELEE_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, E_ARCHER_COLUMN_POS_Y);
				cardUILists[num]->operating_card->isWeatherControlled = e_Archer_weather;
			}
			else
			{
				cardUILists[num]->setPos(pos_x, E_MELEE_COLUMN_POS_Y);
				cardUILists[num]->operating_card->isWeatherControlled = e_Melee_weather;
			}
			break;
		case 5:
			cardUILists[num]->operating_card->isGarbaged = true;
			cardUILists[num]->operating_card->isFielded = false;
			break;
		}
	}

}

//�ۺ�������cardUI��갴��ʱִ�У�������갴�±���Ϊ��
void GamePlayingBackground::CardisPressed()
{
	Pressed = true;
}

//�ۺ�������cardUI����ͷ�ʱִ��
//����ʱ�ڲ���һ�ſ��ƣ����ͷż���
void GamePlayingBackground::CardisReleased()
{
		if (operation == true && isUsingSkill == false)//���ڲ���һ����
		{
			//�������ͼ��
			QCursor cursor;
			QPixmap pixmap(":/cursors/Resources/cursor/cursor1_icon.png");
			pixmap.scaled(30, 30);
			cursor = QCursor(pixmap, -1, -1);
			this->setCursor(cursor);

			usingSkill_card = selected_card;
			isUsingSkill = true;

			switch (usingSkill_card->operating_card->ID)
			{
			case 2://���˶�Ť��֮��,����Ҫѡ�����
				emit toUseSkills(usingSkill_card->operating_card);//ʹ�ü���
				break;
			case 5://���,����Ҫѡ�����
				//emit toUseSkills(usingSkill_card->operating_card);//ʹ�ü���
				useChooseScene(usingSkill_card);
				break;
			}

			updateStatus();
		}
		else if (operation == false && isUsingSkill == true)
		{

			if (usingSkill_card->operating_card->ID == 1 &&
				usingSkillTimes<UNSEEN_ELDER_SKILL_TIMES)//ʹ�����μ��ܡ���д�ķ����Ƚ��鷳
			{
				emit toUseSkills(usingSkill_card->operating_card);//ʹ�ü���
			}
			else
			{
				emit toUseSkills(usingSkill_card->operating_card);//ʹ�ü���
			}
		}

}

//�ۺ�������scene��selectedItem�仯ʱ������ͬ���źŵ��˲�
void GamePlayingBackground::selectionChanged()
{
	if (useMainScene)
	{
		int i = 0;
		QList<QGraphicsItem *> items = main_scene->selectedItems();
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

			if (i > cardUILists.size())
			{
				return;
			}

			if (operation == false && isUsingSkill == false)
			{
				selected_card = cardUILists[i];//ѡȡ��������
				operation = true;
			}
			else if (isCardUIClicked() == true && isUsingSkill == true)//ʹ�ü���
			{
				selected_card = cardUILists[i];
				operation = false;
			}

		}
		else
		{
			return;
		}
	}
	else if(useMainScene==false)
	{
		QList<QGraphicsItem *> items = choose_scene->selectedItems();
		CardsUI* card_temp = dynamic_cast<CardsUI*>(items.first());
	
		if (isUsingSkill == true)
		{
			selected_card = card_temp;
			operation = false;
		}

	}

}


//�ж�cardUI���յ��Ƿ��ǵ����źš�
//�ж������ǵ�ǰ������cardUI�����pos��洢��cardUIPosListsd��λ�ñȽϣ������Ϊ������
//����ѡȡ���õĿ���
bool GamePlayingBackground::isCardUIClicked()
{
	int i = -1;
	if (useMainScene)
	{
		QList<QGraphicsItem *> items = main_scene->selectedItems();

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
	}
	return false;

}

void GamePlayingBackground::cardUISizeAdjust()
{
	//���Ƴߴ����
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

void GamePlayingBackground::useChooseScene(CardsUI *root_card)
{
	choose_scene->clear();
	useMainScene = false;
	switch (root_card->operating_card->ID)
	{
	case 5://���
		CardsUI *temp_card[3];
		QPointF pos;
		//���ÿ���
		int No[3] = { allCards.Biting_Frost_No,allCards.Impenetrable_Fog_No,allCards.Torrential_Rain_No };
		for (int i = 0; i < 3; i++)
		{
			temp_card[i] = new CardsUI(No[i]);
			pos = QPointF(400 + CHOOSE_CARD_DIS * i, 200);
			temp_card[i]->setPos(pos);
			temp_card[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
			temp_card[i]->using_background = 4;

			//���ڿ��Ƶĵ��
			connect(temp_card[i], SIGNAL(cardIsPressed()),
				this, SLOT(CardisPressed()));
			connect(temp_card[i], SIGNAL(cardIsReleased()),
				this, SLOT(CardisReleased()));

			//�򳡾�����Ӳ���
			choose_scene->addItem(temp_card[i]);

			QPixmap pixmap = temp_card[i]->pixmap();
			pixmap = pixmap.scaled(CHOOSE_CARD_HEIGHT*0.85, CHOOSE_CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			temp_card[i]->setPixmap(pixmap);
		}
		break;

	}
	//����ͼ���ƶ����źŲ�(��Ϸ�����棩
	connect(choose_scene, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	view->setScene(choose_scene);

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
	main_scene->setSceneRect(geometry().x(), geometry().y(),
		geometry().width(), geometry().height());

	//����ȫ�ֱ���
	SCREEN_WIDTH2 = this->width();
	SCREEN_HEIGHT2 = this->height();
	updateCoordinate();

}


//�ۺ��������ڿ���ʩ�ż���
void GamePlayingBackground::useSkills(Card *card)
{
	bool cardExist = false;///����Ŀ�꿨��
	switch (card->skill)
	{
	case 1://��Ӱ����
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend ==true && 
				//card->operating_card->isFielded == true &&
				card->operating_card->ID != 1)
			{
				cardExist = true;
				break;
			}
		}
		//�������������Ŀ���
		if (cardExist)
		{
			if (//selected_card->operating_card->isFielded == true &&
				selected_card->operating_card->isFriend == true &&
				selected_card->operating_card->ID != 1)//ѡȡ��������Ѿ���
			{
				int num = cardUILists.indexOf(selected_card);
				cardUILists[num]->operating_card->isSelected = true;
				Card card(allCards.Unseen_Elder_No);
				conductor = new PlayingLogic(cardUILists);
				updateStack(conductor->operateCard(card));
				updateStatus();
				delete conductor;
				usingSkillTimes++;
			}
		}
		//��ԭ
		if (usingSkillTimes == UNSEEN_ELDER_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//�ָ�ԭ���
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			cardExist = false;
		}
		break;

	case 2://���˶�Ť��֮��
		conductor = new PlayingLogic(cardUILists);
		updateStack(conductor->operateCard(*usingSkill_card->operating_card));
		updateStatus();
		delete conductor;
		//��ԭ
		setCursor(QCursor(Qt::ArrowCursor));//�ָ�ԭ���
		usingSkillTimes = 0;
		operation = false;
		isUsingSkill = false;
		break;

	case 3://����Ũ��
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == false &&
				card->operating_card->isFielded == true)//�ڳ��ϵĵз�Ӣ��
			{
				cardExist = true;
				break;
			}
		}
		//�������������Ŀ���
		if (cardExist)
		{
			if (selected_card->operating_card->isFielded == true&&
				selected_card->operating_card->isFriend == false )
			{
				//���ø����������
				if (selected_card->pos().y() == E_MELEE_COLUMN_POS_Y)
				{
					e_Melee_weather = 1;
				}
				else if (selected_card->pos().y() == E_ARCHER_COLUMN_POS_Y)
				{
					e_Archer_weather = 1;
				}
				else if (selected_card->pos().y() == E_SIEGE_COLUMN_POS_Y)
				{
					e_Siege_weather = 1;
				}

				int i = 0;
				foreach(CardsUI *card, cardUILists)//ѡ��ͬ����������
				{
					if (card->pos().y() == selected_card->pos().y() &&
						card->operating_card->isFielded == true)
					{
						cardUILists[i]->operating_card->isSelected = true;
					}
					i++;
				}
				conductor = new PlayingLogic(cardUILists);
				updateStack(conductor->operateCard(*usingSkill_card->operating_card));
				updateStatus();
				delete conductor;
				usingSkillTimes++;
			}
		}
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//�ָ�ԭ���
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			cardExist = false;
		}
		break;
		
	case 4://�̹Ǳ�˪
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == false &&
				card->operating_card->isFielded == true)//�ڳ��ϵĵз�Ӣ��
			{
				cardExist = true;
				break;
			}
		}
		//�������������Ŀ���
		if (cardExist)
		{
			if (selected_card->operating_card->isFielded == true&&
				selected_card->operating_card->isFriend == true )
			{
				//���ø����������
				if (selected_card->pos().y() == E_MELEE_COLUMN_POS_Y)
				{
					e_Melee_weather = 2;
				}
				else if (selected_card->pos().y() == E_ARCHER_COLUMN_POS_Y)
				{
					e_Archer_weather = 2;
				}
				else if (selected_card->pos().y() == E_SIEGE_COLUMN_POS_Y)
				{
					e_Siege_weather = 2;
				}

				int i = 0;
				foreach(CardsUI *card, cardUILists)//ѡ��ͬ����������
				{
					if (card->pos().y() == selected_card->pos().y() &&
						card->operating_card->isFielded == true)
					{
						cardUILists[i]->operating_card->isSelected = true;
					}
					i++;
				}
				conductor = new PlayingLogic(cardUILists);
				updateStack(conductor->operateCard(*usingSkill_card->operating_card));
				updateStatus();
				delete conductor;
				usingSkillTimes++;
			}
		}
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//�ָ�ԭ���
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			cardExist = false;
		}
		break;

	case 5://���
		//�ָ�ԭ�趨
		if (selected_card->operating_card->ID != usingSkill_card->operating_card->ID)
		{
			usingSkillTimes++;
		}
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			isUsingSkill = false;
			usingSkillTimes = 0;
			operation = true;
			useMainScene = true;
			usingSkill_card = selected_card;
			view->setScene(main_scene);
			CardisReleased();
		}		
		break;

	}

}