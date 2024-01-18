#include "Card.h"
#include "Card.cpp"

int main() {
	Card::ctype bomb = Card::bomb;
	Card::ctype reinforcement = Card::reinforcement;
	Card::ctype blockade = Card::blockade;
	Card::ctype airlift = Card::airlift;
	Card::ctype diplomacy = Card::diplomacy;

	Deck* deck = new Deck();

	Hand* handCard1 = new Hand();
	Hand* handCard2 = new Hand();

	Card* card = new Card(blockade);
	deck->addCard(card);
	deck->addCard(new Card(bomb));
	deck->addCard(new Card(blockade));
	deck->addCard(new Card(airlift));
	deck->addCard(new Card(reinforcement));
	deck->addCard(new Card(diplomacy));
	deck->addCard(new Card(bomb));
	deck->addCard(new Card(airlift));
	deck->addCard(new Card(diplomacy));
	cout << "card type: 0 = bomb, 1 = reinforcement, 2 = blockade, 3 = airlift, 4 = diplomacy" << endl;
	cout << *deck;
	cout << "number of cards in deck: " << deck->size() << endl;
	cout << "number of cards in first hand: " << handCard1->size() << endl;
	cout << "number of cards in second hand: " << handCard2->size() << endl;

	cout << "hand 1 draws a card" << endl;
	deck->draw(handCard1);
	cout << "number of cards in deck: " << deck->size() << endl;
	cout << "number of cards in first hand: " << handCard1->size() << endl;
	cout << "number of cards in second hand: " << handCard2->size() << endl;

	cout << "hand 1 draws a card" << endl;
	deck->draw(handCard1);
	cout << "hand 2 draws a card" << endl;
	deck->draw(handCard2);
	cout << "hand 1 draws a card" << endl;
	deck->draw(handCard1);
	cout << "number of cards in deck: " << deck->size() << endl;
	cout << "number of cards in first hand: " << handCard1->size() << endl;
	cout << "number of cards in second hand: " << handCard2->size() << endl;

	cout << endl << "Hand 1: " << endl;
	cout << *handCard1;

	cout << "hand 1 plays second card in deck (bomb)" << endl;
	//handCard1->getCards()[1]->play(1);
	cout << endl << "Hand 1: " << endl;
	cout << *handCard1;
	cout << "number of cards in deck: " << deck->size() << endl;

	cout << "hand 1 plays second card in deck (airlift)" << endl;
	//handCard1->getCards()[1]->play(1);
	cout << endl << "Hand 1: " << endl;
	cout << *handCard1;

	cout << endl << "deck: " << endl;
	cout << *deck;
	return 0;
}
