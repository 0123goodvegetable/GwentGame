//这是Card类，其中包含了与卡牌相关的各种属性
//头文件包含所有卡牌

#pragma once
#ifndef CP_CARD_H
#define CP_CARD_H
#include<Qstring>
#include<QGraphicsItem>
#include<QPainter>
#include<QGraphicsPixmapItem>

class Card:public QGraphicsPixmapItem
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
	int skill;//卡牌技能（功能牌为其功能）
	int material;//卡牌材质（金、银、铜）
	
	bool isHero;//卡牌是否是英雄牌
	bool isPickerActive;//卡牌能否被选择
	bool isSelected;//卡牌是否被选择
	bool isFielded;//卡牌是否在战场上
	bool isGarbaged;//卡牌是否进入墓地

	int card_width;//卡牌的宽度
	int card_height;//卡牌的高度
	int card_height;

	//卡牌相关函数
	Card();
	//根据所给卡牌信息（No）创建卡牌
	Card(int num);
	//卡牌的初始化
	void Init(int num);
	//添加卡牌名字
	void AddName(int num);
	//添加卡牌图片
	void AddPicture();
	//创建卡牌的逻辑函数
	void CreateLogic(int num);

	//与绘制相关的函数
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);//绘制函数																						   //继承自基类的虚函数
	virtual QRectF boundingRect() const;

};


//所有卡牌
Card Unseen_Elder(13010530);//暗影长者
Card Bekker_Twister_Mirror(25010091);//贝克尔的扭曲之镜
Card Impenetrable_Fog(35020092);//蔽日浓雾
Card Biting_Frost(45030092);//刺骨冰霜
Card Dagon(53020630);//达冈
Card Archgriffin(63030882);//大狮鹫


#endif // !CP_CARD_H
