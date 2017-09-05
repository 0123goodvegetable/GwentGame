#include"CP_CardStack.h"
CardStack::CardStack()
{
	Card init_card(13010530);	
	game_cardStack << init_card;
}

void CardStack::addGameCard(int No)
{
	Card newCard(No);
	game_cardStack.append(newCard);
}

void CardStack::deleteGameCard(int No)
{
	for (int i = 0; i < game_cardStack.size(); i++)
	{
		if (game_cardStack.at(i).No == No)
		{
			game_cardStack.removeAt(i);
		}
	}
}