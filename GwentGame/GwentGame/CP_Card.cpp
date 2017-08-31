#include"CP_Card.h"

Card::Card():
	ID(-1),
	attack(0)
{
}

Card::Card(int num)
{
	Init(num);
}

int Card::Init(int num)
{
	//记录卡牌文件路径
	QString temp_string = QString::number(num, 10);
	path = "Resource\\Cards\\" + temp_string + ".jpg";

	return CreateLogic(num);
}

int Card::CreateLogic(int num)
{
	/*
	用6位数字(abcdef)来记录卡牌信息
	第一位数字（a）记录卡牌种类（0-近战，1-远程，2-攻城，3-功能牌）
	当（0<=a<=2）时
		第二、三位数字（bc）记录卡牌技能（00-无技能，01-对普通单位增益两点...)
		第四、五位数字（de）记录卡牌攻击力
		第六位数字（f）记录卡牌是否为英雄
	当（a=3）时
		第二位数字记录功能牌种类（0-晴朗，1-雨，2-雾，3-冰霜，4-稻草人，5-烧毁攻击力最高的单位...）
	*/

	isHero = false;
	isPickerActive = false;
	isSelected = false;
	isFielded = false;
	isGarbaged = false;

	int temp_num = 0;

	No = num;
}