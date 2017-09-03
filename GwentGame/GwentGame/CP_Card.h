//这是Card类，其中包含了与卡牌相关的各种属性

#pragma once
#ifndef CP_CARD_H
#define CP_CARD_H
#include<Qstring>
#include<QGraphicsItem>

class Card:public QGraphicsItem
{
public:

	//卡牌信息
	int ID;//对于卡牌的编号
	int No;//代表卡牌信息的数字
	int number;//牌组中有几张该卡牌
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

	//卡牌相关函数
	Card();
	//根据所给卡牌信息（No）创建卡牌
	Card(int num);
	//卡牌的初始化
	void Init(int num);
	//创建卡牌的逻辑函数
	void CreateLogic(int num);

};


#endif // !CP_CARD_H
