#include"CardsUI.h"
#include <QPixmap>
#include <iostream>
#include <QDebug>

CardsUI::CardsUI()
{
}

CardsUI::CardsUI(int cardNo)
{
	operating_card = new Card(cardNo);

	//加载卡牌对应图片
	QPixmap pixmap;
	pixmap.load(operating_card->path);
	pixmap = pixmap.scaled(operating_card->card_width, operating_card->card_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	setPixmap(pixmap);

	//初始化使用背景
	using_background = -1;
}

QRectF CardsUI::boundingRect() const
{
	QRect rect = this->pixmap().rect();
	return QRectF(0, 0, rect.width(), rect.height() + 15);
}

void CardsUI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
	QPixmap pixmap = this->pixmap();
	QRect rect = pixmap.rect();

	painter->drawPixmap(rect, pixmap);

	if (using_background == 2)//在卡牌选择界面
	{
		//卡牌下方附带卡牌相关信息
		QPen pen(Qt::white);
		painter->setPen(pen);
		painter->setRenderHint(QPainter::Antialiasing);
		QFont font("Verdana", 12, QFont::Normal);
		painter->setFont(font);
		painter->drawText(QRectF(0, rect.height()-10, rect.width(), 25),
							Qt::AlignCenter,
							operating_card->name);
	}
	

}

QPainterPath  CardsUI::shape() const
{
	QRectF rect = boundingRect();

	QPainterPath path;
	path.addRoundRect(rect, 5, 5);
	return path;
}

void  CardsUI::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit cardIsPressed();
	QGraphicsItem::mousePressEvent(event);
}
void  CardsUI::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update(boundingRect());
	emit cardIsReleased();  
	QGraphicsItem::mouseReleaseEvent(event);
}

void  CardsUI::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF point = event->pos();

}
