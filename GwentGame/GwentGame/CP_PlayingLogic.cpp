#include"CP_PlayingLogic.h"
#include<QFile>
#include<QTextStream>

#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b

PlayingLogic::PlayingLogic(QList<Card> &aim_stack)
{
	//初始化牌组状态信息
	for(int i=0;i<aim_stack.size();i++)
	{
		Card temp_card(aim_stack[i]);
		cardStack.append(temp_card);
	}

}

QList<Card> PlayingLogic::operateCard(Card &card)
{
	//对于卡牌不同的skill进行不同的响应
	switch (card.skill)
	{
	case 1:
		skill1();
		break;
	case 2:
		skill2();
		break;
	case 3:
		skill3();
		break;
	case 4:
		skill4();
		break;
	case 5:
		skill5();
		break;
	case 6:
		skill6();
		break;
	case 7:
		skill7();
		break;
	case 8:
		skill8();
		break;
	case 9:
		skill9();
		break;
	/*case 10:
		skill10();
		break;
	case 11:
		skill11();
		break;
	case 12:
		skill12();
		break;
	case 13:
		skill13();
		break;
	case 14:
		skill14();
		break;
	case 15:
		skill15();
		break;
	case 16:
		skill16();
		break;
	case 17:
		skill17();
		break;
	case 18:
		skill18();
		break;
	case 19:
		skill19();
		break;
	case 20:
		skill20();
		break;
	case 21:
		skill21();
		break;
	case 22:
		skill22();
		break;
	case 23:
		skill23();
		break;
	case 24:
		skill24();
		break;
	case 25:
		skill25();
		break;
	case 26:
		skill26();
		break;
	case 27:
		skill27();
		break;
	case 28:
		skill28();
		break;
	case 29:
		skill29();
		break;*/
	}

	//返回改变后的牌组状态
	return cardStack;
}

/*
*实现所有卡牌的技能
*/
//1、暗影长者
void PlayingLogic::skill1()
{
	//搜寻打出卡牌，设置其属性为在场上
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No== allCards.Unseen_Elder_No)
		{
			break;
		}
	}
	cardStack[num].isFielded = true;

	//搜寻打出卡牌后的后续卡牌
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			cardStack[i].isGarbaged = true;//该卡牌被吞噬
			cardStack[num].attack += cardStack[i].attack;//战斗力被暗影长者吸收
			cardStack[i].isSelected = false;
		}
	}
}

//2、贝克尔扭曲之镜
void PlayingLogic::skill2()
{
	//搜寻打出卡牌，设置其属性为在墓地
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Bekker_Twister_Mirror_No)
		{
			break;
		}
	}
	cardStack[num].isGarbaged = true;

	//搜寻打出卡牌后的后续卡牌,交换战斗力
	int max_num, min_num, max_attack = cardStack[0].attack, min_attack = cardStack[0].attack;
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isFielded==true)
		{
			if (cardStack[i].attack > max_attack)
			{
				max_attack = cardStack[i].attack;
				max_num = i;
			}
			else if (cardStack[i].attack < min_attack)
			{
				min_attack = cardStack[i].attack;
				min_num = i;
			}
		}
	}

	if (cardStack[max_num].attack <= 10)
	{
		cardStack[min_num].attack += cardStack[max_num].attack;
		cardStack[max_num].isGarbaged = true;
	}
	else
	{
		cardStack[min_num].attack += 10;
		cardStack[max_num].attack -= 10;
	}

}

//3、蔽日浓雾
void PlayingLogic::skill3()
{
	//搜寻打出卡牌，设置其属性为在墓地
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Impenetrable_Fog_No)
		{
			break;
		}
	}
	cardStack[num].isGarbaged = true;

	//搜寻打出卡牌后的后续卡牌
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			cardStack[i].isWeatherControlled = 1;
			cardStack[i].isSelected = false;
		}
	}
}

//4、刺骨冰霜
void PlayingLogic::skill4()
{
	//搜寻打出卡牌，设置其属性为在墓地
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Biting_Frost_No)
		{
			break;
		}
	}
	cardStack[num].isGarbaged = true;

	//搜寻打出卡牌后的后续卡牌
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			cardStack[i].isWeatherControlled = 2;
			cardStack[i].isSelected = false;
		}
	}
}

//5、*达冈(待完成）
void PlayingLogic::skill5()
{
	//搜寻打出卡牌，设置其属性为在场上
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Dagon_No)
		{
			break;
		}
	}
	cardStack[num].isFielded = true;
}

//6、*大狮鹫(待完成）
void PlayingLogic::skill6()
{
	//搜寻打出卡牌，设置其属性为在场上
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Archgriffin_No)
		{
			break;
		}
	}
	cardStack[num].isFielded = true;

	//搜寻打出卡牌后的后续卡牌
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isFielded==true&&
			cardStack[i].isFriend==true&&
			cardStack[i].genre== cardStack[num].genre)
		{
			cardStack[i].isWeatherControlled = 0;//清空天气影响
			
		}
	}

}

//7、盖尔
void PlayingLogic::skill7()
{
	//搜寻打出卡牌，设置其属性为在场上
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Ge_Els_No)
		{
			break;
		}
	}
	cardStack[num].isFielded = true;

	//搜寻打出卡牌后的后续卡牌
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			cardStack[i].isFielded = true;//打出一张金色或者银色牌
		}
	}

}

//8、杰洛特：伊格尼法印
void PlayingLogic::skill8()
{
	//搜寻打出卡牌，设置其属性为在场上
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Geralt_Igni_No)
		{
			break;
		}
	}

	//搜寻打出卡牌后的后续卡牌
	QList<int> record_num;
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			record_num.append(i);
		}
	}

	int attack_all = 0;//卡牌总战力
	int max_attack = cardStack[record_num[0]].attack;
	for (int j = 0; j < record_num.size(); j++)
	{
		attack_all += cardStack[record_num[j]].attack;
		max_attack = max(max_attack, cardStack[record_num[j]].attack);
	}
	if (attack_all >= 25)
	{
		for (int j = 0; j < record_num.size(); j++)
		{
			if (cardStack[record_num[j]].attack == max_attack)
			{
				cardStack[record_num[j]].isGarbaged = true;
			}
		}
	}
}

//9、卡兰希尔
void PlayingLogic::skill9()
{
	//搜寻打出卡牌，设置其属性为在场上
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Caranthir_No)
		{
			break;
		}
	}

	//搜寻打出卡牌后的后续卡牌
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			cardStack[i].genre = cardStack[num].genre;//将敌方单位移至同排
		}
	}
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isFriend == false&& cardStack[i].genre==cardStack[num].genre)
		{
			cardStack[i].isWeatherControlled = 2;//在该排降下刺骨冰霜
		}
	}
}