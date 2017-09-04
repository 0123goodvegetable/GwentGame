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
	//记录名字
	AddName(num);

	//记录卡牌文件路径
	path = ":/cards/" + name;

	//添加卡牌图片
	//AddPicture();

	//记录卡牌ID
	ID = num / 10000000;
	num %= 10000000;

	//创建卡牌信息
	CreateLogic(num);

	//初始化卡牌长度与宽度
	card_width = 300;
	card_height = 400;

}

void Card::CreateLogic(int num)
{
	/*
	用7位有效数字(abcdefg)来记录卡牌信息（9位数字前两位为ID，即编号）
	第一位数字（a）记录卡牌种类（0-近战，1-远程，2-攻城，3-我方任意，4-敌方任意，5-功能牌）
	当（0<=a<=4）时
	第二、三位数字（bc）记录卡牌技能
	01-部署：吞噬1个友军单位，吸收它们的战力为自身的基础战力，重复两次。（暗影长者）
	02-部署：生成“刺骨冰霜”、“蔽日浓雾”或“倾盆大雨”。（达冈）
	03-部署：移除所在排的天气效果。将1张铜色牌从一方墓场置入另一方墓场。（大狮鹫）
	第四、五位数字（de）记录卡牌攻击力
	第六位数字（f）记录卡牌是否为英雄（f<5时是英雄）

	当（a=3）时
	第二、三位数字（bc）记录功能牌种类
	01-对场上最强的单位造成数值等同于其战力的伤害，但最多不超过10点（无视护甲），并使场上最弱的非同名单位获得相同数值的增益。（贝克尔扭曲之镜）
	02-在对方单排降下“蔽日浓雾”。蔽日浓雾：每当己方回合开始时，对所在排最强的单位造成2点伤害。（蔽日浓雾）
	03-在对方单排降下“刺骨冰霜”。刺骨冰霜：每当己方回合开始时，对所在排最弱的单位造成2点伤害。（刺骨冰霜）

	第七位数字（g）记录卡牌材质（0-金，1-银，2-铜）
	*/

	isHero = false;
	isPickerActive = false;
	isSelected = false;
	isFielded = false;
	isGarbaged = false;
	number = 1;

	int temp_num = 0;

	No = num;//卡牌编号

	genre = num / 1000000;//卡牌种类

	temp_num = num / 10000;
	skill = temp_num % 100;//卡牌技能

	temp_num = num / 100;
	attack = num % 100;//卡牌战斗力

	temp_num = (num / 10) % 10;
	if (temp_num < 5)
	{
		isHero = true;
	}//是否为英雄牌

	material = num % 10;//卡牌材质
}

void  Card::AddName(int num)
{
	switch (num)
	{
	case 13010530:
		name = "Unseen_Elder";
		break;
	case 25010091:
		name = "Bekker_Twister_Mirror";
		break;
	case 35020092:
		name = "Impenetrable_Fog";
		break;
	case 45030092:
		name = "Biting_Frost";
		break;
	case 53020630:
		name = "Dagon";
		break;
	case 63030882:
		name = "Archgriffin";
		break;
	}
}

/*void Card::AddPicture()
{
	QPixmap pixmap;
	pixmap.load(path);
	pixmap = pixmap.scaled(card_width, card_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	setPixmap(pixmap);
}*/

QRectF Card::boundingRect() const
{

	return QRectF(0, 0, card_width, card_height);
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPixmap pixmap(path);

	QRect rect = pixmap.rect();
	painter->drawPixmap(rect, pixmap);
}