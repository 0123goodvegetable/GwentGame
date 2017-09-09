/*
用7位有效数字(abcdefg)来记录卡牌信息（9位数字前两位为ID，即编号）
	第一位数字（a）记录卡牌种类（0-近战，1-远程，2-攻城，3-我方任意，4-敌方任意，5-功能牌）
	当（0<=a<=4）时
		第二、三位数字（bc）记录卡牌技能
			01-部署：吞噬1个友军单位，吸收它们的战力为自身的基础战力，重复两次。（暗影长者）
			05-部署：生成“刺骨冰霜”、“蔽日浓雾”或“倾盆大雨”。（达冈）
			06-部署：移除所在排的天气效果。将1张铜色牌从一方墓场置入另一方墓场。（大狮鹫）
			07-部署：从牌组顶端各抽1张金色牌和银色牌。打出1张，将另1张置于牌组顶端。(盖尔）
			08-部署：若对方同排总战力不低于25，则摧毁其上所有最强的单位。（杰洛特：伊格尼法印）
			09-部署：将1个敌军单位移至其所在半场同排，并在此排降下刺骨冰霜.(卡兰希尔)
			10-部署：使对方此排的“刺骨冰霜”伤害提高1点。(狂猎骑士)
			11-部署：从牌组打出“煮婆”和“织婆”(老巫妪：呢喃婆)
			12-部署：从牌组打出“煮婆”和“呢喃婆”(老巫妪：织婆)
			13-部署：从牌组打出“织婆”和“呢喃婆”(老巫妪：煮婆)
			15-部署：生成3只“暴怒的狼”，并在对方同排降下“蔽日浓雾。（林妖）
			16-无（暴怒的狼）
			17-己方每次从手牌打出金色牌时（领袖牌除外），便将牌组中的“萝卜”在其能力结算之前召唤至随机位置。（萝卜）
			20-部署：在自身左侧生成2个“鹰身女妖蛋”。（赛尔伊诺鹰身女妖）
			22-部署．获得护盾
			   遗愿：在该排末位生成2个“次级土元素”（土元素）
			23-部署：从牌组顶端抽1张牌。将所在战场其他排的1个单位移至自身的同一排。（畏惧者）
			24-部署：吞噬右侧单位。每2回合，己方回合开始时，吞噬右侧单位。（蜥蜴人战士）
			25-每当己方“蔽日浓雾”在敌排生效，便从牌组打出1个“小雾妖”至己方同排。
			   若牌组中没有“小雾妖”，则复活1个“小雾妖”至己方同排。
			   若场上不再存在“蔽日浓雾”效果，则摧毁所有“小雾妖”（小雾妖）
			26-部署：从牌组打出所有“蟹蜘蛛”。（蟹蜘蛛）
			27-每当有友军单位吞噬1张牌，便在随机排生成1只“蟹蜘蛛幼虫”，随后受到1点伤害（无视护甲）。
			   部署：获得2点护甲。（蟹蜘蛛巨兽）
			28-使吞噬自身的单位获得额外获得4点增益。
			   遗愿：在随机排生成1只鹰身女妖幼崽。（鹰身女妖蛋）

		第四、五位数字（de）记录卡牌攻击力
		第六位数字（f）记录卡牌是否为英雄（f=9时是英雄）否则其表示同种卡牌个数

	当（a=3）时
		第二、三位数字（bc）记录功能牌种类
			02-对场上最强的单位造成数值等同于其战力的伤害，但最多不超过10点（无视护甲），并使场上最弱的非同名单位获得相同数值的增益。（贝克尔扭曲之镜）
			03-在对方单排降下“蔽日浓雾”。蔽日浓雾：每当己方回合开始时，对所在排最强的单位造成2点伤害。（蔽日浓雾）
			04-在对方单排降下“刺骨冰霜”。刺骨冰霜：每当己方回合开始时，对所在排最弱的单位造成2点伤害。（刺骨冰霜）
			14-使 3 个相邻单位获得 2 点护甲和 3 点增益。(雷霆药水）
			18-生成“晴空”或“重整”。（破晓）
			19-在对方单排降下“倾盆大雨”。倾盆大雨：每当己方回合开始时，对所在排最多2个随机单位造成1点伤害（倾盆大雨）
			21-对单排所有单位造成3点伤害。（撕裂）
	第七位数字（g）记录卡牌材质（0-金，1-银，2-铜）
*/

#include"CP_Card.h"

Card::Card():
	ID(-1),
	attack(0)
{
}

Card::Card(const Card &root_card)
{

	ID = root_card.ID;
	No = root_card.No;
	number = root_card.number;
	explainText = root_card.explainText;
	name = root_card.name;

	genre = root_card.genre;
	attack = root_card.attack;
	skill = root_card.skill;
	material = root_card.material;
	armer = root_card.armer;

	isHero = root_card.isHero;
	isFriend = root_card.isFriend;
	isInGameCardsStack = root_card.isInGameCardsStack;
	isPickerActive = root_card.isPickerActive;
	isSelected = root_card.isSelected;
	isFielded = root_card.isFielded;
	isGarbaged = root_card.isGarbaged;
	isShield = root_card.isShield;

	isWeatherControlled = root_card.isWeatherControlled;

	card_height = root_card.card_height;
	card_width = root_card.card_width;

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

	//记录卡牌ID
	ID = num / 10000000;

	//卡牌编号
	No = num;

	num %= 10000000;

	//创建卡牌信息
	CreateLogic(num);

	//设置卡牌的长宽
	card_height = 400; 
	card_width = 300;

}

void Card::CreateLogic(int num)
{

	isHero = false;
	isPickerActive = false;
	isSelected = false;
	isFielded = false;
	isGarbaged = false;
	isInGameCardsStack = false;
	isFriend = true;
	isShield = false;
	armer = 0;
	isWeatherControlled = 0;
	number = 1;

	int temp_num = 0;

	genre = num / 1000000;//卡牌种类

	temp_num = num / 10000;
	skill = temp_num % 100;//卡牌技能

	temp_num = num / 100;
	attack = num % 100;//卡牌战斗力

	temp_num = (num / 10) % 10;
	if (temp_num ==9 )
	{
		isHero = true;
	}//是否为英雄牌
	else
	{
		isHero = false;
		number = temp_num;
	}

	material = num % 10;//卡牌材质
}

void  Card::AddName(int num)
{
	switch (num)
	{
	case 13010590:
		name = "Unseen_Elder";
		break;
	case 25020011:
		name = "Bekker_Twister_Mirror";
		break;
	case 35030012:
		name = "Impenetrable_Fog";
		break;
	case 45040012:
		name = "Biting_Frost";
		break;
	case 53050690:
		name = "Dagon";
		break;
	case 63060812:
		name = "Archgriffin";
		break;
	case 63060822:
		name = "Archgriffin";
		break;
	case 63060832:
		name = "Archgriffin";
		break;
	case 73070110:
		name = "Ge_Els";
		break;
	case 83080510:
		name = "Geralt_Igni";
		break;
	case 93090810:
		name = "Caranthir";
		break;
	case 103100812:
		name = "Wild_Hunt_Rider";
		break;
	case 112110611:
		name = "Crone_Whispess";
		break;
	case 122120611:
		name = "Crone_Weavess";
		break;
	case 132130811:
		name = "Crone_Brewess";
		break;
	case 145140012:
		name = "Thunderbolt_potion";
		break;
	case 153150510:
		name = "Woodland_Spirit";
		break;
	case 160160112:
		name = "Raging_Wolf";
		break;
	case 173170411:
		name = "Roach";
		break;
	case 185180012:
		name = "First_Light";
		break;
	case 195190012:
		name = "Torrential_Rain";
		break;
	case 203200512:
		name = "Celaeno_Harpy";
		break;
	case 215210012:
		name = "Lacerate";
		break;
	case 220220612:
		name = "Earth_Elemental";
		break;
	case 234231211:
		name = "Frightener";
		break;
	case 243240612:
		name = "Vran_Warrior";
		break;
	case 253250412:
		name = "Foglet";
		break;
	case 261260312:
		name = "Arachas";
		break;
	case 272270612:
		name = "Arachas_Behemoth";
		break;
	case 283280112:
		name = "Harpy_Egg";
		break;
	case 295290011:
		name = "Commander_Horn";
		break;
	}
}


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