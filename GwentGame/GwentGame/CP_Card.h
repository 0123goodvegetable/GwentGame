//这是Card类，其中包含了与卡牌相关的各种属性

#pragma once
#ifndef CP_CARD_H
#define CP_CARD_H
#include<Qstring>
#include<QGraphicsItem>
#include<QPainter>
#include<QGraphicsPixmapItem>

class Card:public QGraphicsItem
{
public:

	//卡牌信息
	int ID;//对于卡牌的编号
	int No;//代表卡牌信息的数字
	int number;//牌组中有几张该卡牌
	QString explainText;//卡牌信息解释
	QString name;//卡牌名称
	QString path;//卡牌图片所在文件路径

	int genre;//卡牌种类（近战、远程、攻城、我方任意、敌方任意、功能牌）
	int attack;//卡牌的攻击力
	int armer;//卡牌护甲
	int skill;//卡牌技能（功能牌为其功能）
	int material;//卡牌材质（金、银、铜）
	
	bool isHero;//卡牌是否是英雄牌
	bool isShield;//卡牌是否有护盾
	bool isFriend;//卡牌是否是友方卡牌
	bool isInGameCardsStack;//卡牌是否在游戏牌组中
	bool isPickerActive;//卡牌能否被选择
	bool isSelected;//卡牌是否被选择
	bool isFielded;//卡牌是否在战场上
	bool isGarbaged;//卡牌是否进入墓地

	int isWeatherControlled;//卡牌受到天气的影响(0-无，1-蔽日浓雾，2-刺骨冰霜，3-倾盆大雨）

	int card_width;//卡牌的宽度
	int card_height;//卡牌的高度

	//卡牌相关函数
	Card();
	//根据所给卡牌信息（No）创建卡牌
	Card(int num);
	//拷贝构造函数
	Card(const Card &root_card);

	//卡牌的初始化
	void Init(int num);
	//添加卡牌名字
	void AddName(int num);

	//创建卡牌的逻辑函数
	void CreateLogic(int num);

	//与绘制相关的函数
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);//绘制函数																						   //继承自基类的虚函数
	virtual QRectF boundingRect() const;

};


#endif // !CP_CARD_H
