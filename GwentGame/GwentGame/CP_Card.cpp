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

void Card::Init(int num)
{
	//记录卡牌文件路径
	QString temp_string = QString::number(num, 10);
	path = "Resource\\Cards\\" + temp_string + ".jpg";

	//记录卡牌ID
	ID = num / 10000000;

	num %= 10000000;

	CreateLogic(num);

}

void Card::CreateLogic(int num)
{
	/*
	用7位有效数字(abcdefg)来记录卡牌信息（9位数字前两位为ID，即编号）
	第一位数字（a）记录卡牌种类（0-近战，1-远程，2-攻城，3-我方任意，4-敌方任意，5-功能牌）
	当（0<=a<=4）时
		第二、三位数字（bc）记录卡牌技能（00-无技能，01-对普通单位增益两点...)
		第四、五位数字（de）记录卡牌攻击力
		第六位数字（f）记录卡牌是否为英雄（f<5时是英雄）
		第七位数字（g）记录卡牌材质（0-金，1-银，2-铜）
	当（a=3）时
		第二、三位数字（bc）记录功能牌种类（0-晴朗，1-雨，2-雾，3-冰霜，4-稻草人，5-烧毁攻击力最高的单位...）
	*/

	isHero = false;
	isPickerActive = false;
	isSelected = false;
	isFielded = false;
	isGarbaged = false;

	int temp_num = 0;

	No = num;

	genre = num / 1000000;

	temp_num = num / 10000;
	skill = temp_num % 100;

	temp_num = num / 100;
	attack = num % 100;

	temp_num = (num/10) % 10;
	if (temp_num < 5)
	{
		isHero = true;
	}

	material = num % 10;
}
