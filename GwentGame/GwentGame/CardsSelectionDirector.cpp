#include"CardsSelectionDirector.h"
#include<QPixmap>
#include<iostream>
#include<QDebug>

CardsSelectionDirector::CardsSelectionDirector()
{
}

//在头文件中规定imagesize
CardsSelectionDirector::CardsSelectionDirector(QString &file, QString &text, int height , int width )
{
	//setMyText(text);
	setMyPixmap(file, height, width);
}

/*void CardsSelectionDirector::setMyText(QString &text)
{
	myText = text;
}*/

void CardsSelectionDirector::setMyPixmap(QString &file, int height , int width )
{
 
	QPixmap pixmap;
	pixmap.load(file);
	pixmap = pixmap.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	setPixmap(pixmap);
}

QRectF CardsSelectionDirector::boundingRect() const
{
	QRect rect = this->pixmap().rect();

	return QRectF(0, 0, rect.width(), rect.width() + 15);
}

void CardsSelectionDirector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPixmap pixmap = this->pixmap();
	QRect rect = pixmap.rect();

	painter->drawPixmap(rect, pixmap);

	//输出名字
	QPen pen(Qt::white);
	painter->setPen(pen);
	painter->setRenderHint(QPainter::Antialiasing);
	QFont font("Verdana", 8, QFont::Normal);
	painter->setFont(font);
	painter->drawText(QRectF(0, rect.height(), rect.width(), 15), Qt::AlignCenter, operation_card.name);

	if (option->state & QStyle::State_Sunken)
	{
		QRectF rect1 = boundingRect();
		//QPen pen(Qt::darkGreen);  
		painter->setPen(QPen(Qt::darkGreen));
	}
}

QPainterPath CardsSelectionDirector::shape() const
{
	QRectF rect = boundingRect();

	QPainterPath path;
	path.addRoundedRect(rect, 5, 5);
	return path;
}

void CardsSelectionDirector::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit  nodeIsPressed();
	//qDebug()<<"pressed";
	QGraphicsItem::mousePressEvent(event);
}

void CardsSelectionDirector::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update(boundingRect());
	QGraphicsItem::mouseReleaseEvent(event);
}

/*QString CardsSelectionDirector::getMyText()
{
	return myText;
}*/