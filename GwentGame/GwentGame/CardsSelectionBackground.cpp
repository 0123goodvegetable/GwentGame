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
const qreal CARD_POS_Y = 150;
const qreal CARD_DIS = 300;
const qreal CARD_STA = 100;
const int   CARD_SIZE = 200;
int   SCREEN_SIZE = 1800;

CardsSelectionBackground::CardsSelectionBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	init();
}

CardsSelectionBackground::~CardsSelectionBackground()
{
}

void CardsSelectionBackground::init()
{
	//��ʼ��
	Pressed = false;

	//��ȡ��Ļ�ߴ�
	GetScreenInfo();

	view = new QGraphicsView(this);
	scene = new CardsSelectionScene();


	//����ͼ���ƶ����źŲ�
	connect(scene, SIGNAL(selectionChanged()), 
		this, SLOT(selectionChanged()));    
	connect(scene, SIGNAL(isMoving(QPointF&)), 
		this, SLOT(isMoving(QPointF&))); 

	CardsSelectionUI *temp_card;
	QPointF pos;

	//��ӿ���
	//��һ���ƣ���Ӱ����
	temp_card = new CardsSelectionUI(Unseen_Elder_No);
	pos = QPointF(CARD_STA, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//�ڶ����ƣ����˶���Ť��֮��
	temp_card = new CardsSelectionUI(Bekker_Twister_Mirror_No);
	pos = QPointF(CARD_STA + CARD_DIS * 1, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//�������ƣ�����Ũ��
	temp_card = new CardsSelectionUI(Impenetrable_Fog_No);
	pos = QPointF(CARD_STA + CARD_DIS * 2, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//�������ƣ��̹Ǳ�˪
	temp_card = new CardsSelectionUI(Biting_Frost_No);
	pos = QPointF(CARD_STA + CARD_DIS * 3, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//���¼���UI�ߴ�
	cardUISizeAdjust();

	int i = 0;
	foreach(CardsSelectionUI* card_temp, cardUILists)
	{

		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

		//���ڿ��Ƶĵ��
		connect(cardUILists.at(i), SIGNAL(cardIsPressed()), 
			this, SLOT(isPressed()));  
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()), 
			this, SLOT(isReleased()));
		//�򳡾�����Ӳ���
		scene->addItem(card_temp);

		i++;
	}

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
			foreach(CardsSelectionUI* card_temp, cardUILists)
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
		CardsSelectionUI* card_temp = dynamic_cast<CardsSelectionUI*>(items.first());

		foreach(CardsSelectionUI* card, cardUILists)
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
		CardsSelectionUI* card_temp = dynamic_cast<CardsSelectionUI*>(items.first());
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
	foreach(CardsSelectionUI* card, cardUILists)
	{
		QPointF pos = card->pos();

		pos.setX(card->pos().x()+card->pixmap().width());

		//quint16 width = 400;
		//quint16 height = 400;
		QPixmap pixmap = cardUIPixmapLists.at(i);
		pixmap = pixmap.scaled(350, 400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		cardUILists[i]->setPixmap(pixmap);

		//�ı����˿��ƴ�С
		/*if (pos.x() >= 0 && pos.x() <= SCREEN_SIZE / 2)
		{
			
			quint16 size = pos.x() / 5 + 200;
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
		}

		if (pos.x()>SCREEN_SIZE / 2 && pos.x() <= SCREEN_SIZE + 10)
		{
			quint16 size = (SCREEN_SIZE - pos.x()) / 5 + 200;
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
		}*/
		i++;
	}
}

//��ȡ�豸�ֱ�����Ϣ
void CardsSelectionBackground::GetScreenInfo()
{
	QDesktopWidget* desktopWidget = QApplication::desktop();
	//��ȡ���������С
	//QRect deskRect = desktopWidget->availableGeometry();
	//��ȡ�豸��Ļ��С
	QRect screenRect = desktopWidget->screenGeometry();

	sceenSizeX = screenRect.width();
	sceenSizeY = screenRect.height();

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
