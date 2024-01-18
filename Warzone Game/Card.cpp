#include "Card.h"

#include <random>
#include <algorithm>

using namespace std;

Deck Card::deck;

Card::Card() {
	cardType = EMPTY;
	handR = NULL;
	deckR = NULL;
}

Card::Card(ctype type) {
	cardType = type;
	handR = NULL;
	deckR = NULL;
}

Card::Card(ctype type, Hand* hand, Deck* deck) {
	cardType = type;
	this->handR = hand;
	this->deckR = deck;
}

Card::Card(const Card& card) {
	cardType = card.cardType;
	handR = card.handR;
	deckR = card.deckR;
}

Card::~Card() {}

Card::ctype Card::getCardType() {
	return cardType;
}

void Card::setDeck(Deck* deck) {
	this->deckR = deck;
}

void Card::setHand(Hand* hand) {
	this->handR = hand;
}

void Card::play(Card* card,int* numArmy, Player* p1, Player* p2, Territory* source, Territory* target, GameEngine* game) {
	
	switch (card->getCardType()) {
	case Card::ctype::bomb:
		p1->getOrders()->addOrder(new Bomb(p1,target, game->getMap()));
		break;
	case Card::ctype::reinforcement:
        cout << "Reinforcement card played immediately" << endl;
        cout << "Old reinforcement pool army count : " << *p1->getReinforcementPool() << endl;
        p1->setReinforcementPool(*p1->getReinforcementPool()+ 5);
        cout << "New reinforcement pool army count : " << *p1->getReinforcementPool() << endl;
		break;
	case Card::ctype::blockade:
        p1->getOrders()->addOrder(new Blockade(p1,target));
		break;
	case Card::ctype::airlift:
        p1->getOrders()->addOrder(new Airlift(p1, source, target, numArmy));
		break;
	case Card::ctype::diplomacy:
        p1->getOrders()->addOrder(new Negotiate(p1,p2));
		break;
	}

	//Removes card played from the hand
	p1->getHand()->removeCard(card);

	//Adds the card played to the deck
	game->getDeck()->addCard(card);

}

void createRandomCards(Deck* deck, Hand* hand, int amount) {
	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<std::mt19937::result_type> dist(0, 4);
	for (int i = 0; i < amount; i++) {
		Card* card = new Card((Card::ctype)(dist(rng)), hand, deck);
		deck->addCard(card);
		if (hand) {
			hand->addCard(card);
		}
	}
}

ostream& operator << (ostream& out, const Card& c) {
	switch (c.cardType)
	{
	case Card::ctype::bomb:
		out << "Card type: Bomb.";
		break;
	case Card::ctype::reinforcement:
		out << "Card type: Reinforcement.";
		break;
	case Card::ctype::airlift:
		out << "Card type: Airlift.";
		break;
	case Card::ctype::blockade:
		out << "Card type: Blockade.";
		break;
	case Card::ctype::diplomacy:
		out << "Card type: Diplomacy";
		break;
	default:
		out << "Wrong choice of options";
	}
	return out;
}



Hand::Hand() {

}

Hand::Hand(const Hand& hand) {
	cards = hand.cards;
}

vector<Card*> Hand::getCards() {
	return cards;
}

int Hand::size() {
	return cards.size();
}

void Hand::removeCard(Card* c)
{
	cards.erase(remove(cards.begin(), cards.end(), c), cards.end());
}

void Hand::addCard(Card* card) {
	cards.push_back(card);
	card->setHand(this);
}

Card* Hand::getCard(int idx) {
	return cards[idx];
}

ostream& operator << (ostream& out, const Hand& h) {
	out << "Hand size: " << h.cards.size() << "." << endl;
	out << "Cards in hand: " << endl;
	for (int i = 0; i < h.cards.size(); i++) {
		out << "Card " << i << "=" << *(h.cards[i]) << endl;
	}
	return out;
}


Deck::Deck() {
	cards.push_back(new Card(Card::ctype::bomb));
	cards.push_back(new Card(Card::ctype::reinforcement));
	cards.push_back(new Card(Card::ctype::blockade));
	cards.push_back(new Card(Card::ctype::airlift));
	cards.push_back(new Card(Card::ctype::diplomacy));
}

Deck::Deck(int size) {

	int sizeCorrected{ size - 5 };

	//Making sure the deck has a card of each type

	cards.push_back(new Card(Card::ctype::bomb));
	cards.push_back(new Card(Card::ctype::reinforcement));
	cards.push_back(new Card(Card::ctype::blockade));
	cards.push_back(new Card(Card::ctype::airlift));
	cards.push_back(new Card(Card::ctype::diplomacy));

	for (int i = 0; i < sizeCorrected; i++) {

		//Random int from 0 to 4
		int typeNumber{ rand() % 4 };

		switch (typeNumber) {
		case 0:
			cards.push_back(new Card(Card::ctype::bomb));
			break;
		case 1:
			cards.push_back(new Card(Card::ctype::reinforcement));
			break;
		case 2:
			cards.push_back(new Card(Card::ctype::blockade));
			break;
		case 3:
			cards.push_back(new Card(Card::ctype::airlift));
			break;
		case 4:
			cards.push_back(new Card(Card::ctype::diplomacy));
			break;
		}
	}
}

Deck::Deck(const Deck& deck) {
	cards = deck.cards;
}

Deck::~Deck() {
	for (Card* c : cards) delete c;
}

int Deck::size() {
	return cards.size();
}

vector<Card*> Deck::getCards() {
	return cards;
}

void Deck::removeCard(int i) {
	if (cards.size() == 0) {
		cout << "Deck is empty" << endl;
		return;
	}
	cards.erase(cards.begin() + i);
}


void Deck::addCard(Card* card) {
	cards.push_back(card);
	card->setDeck(this);
}

void Deck::draw(Hand* hand) {
	Card* drawnCard = new Card(*cards.front());
	this->removeCard(0);
	hand->addCard(drawnCard);
}

ostream& operator << (ostream& out, const Deck& d) {
	out << "Deck size: " << d.cards.size() << "." << endl;
	out << "Cards in deck: " << endl;
	for (int i = 0; i < d.cards.size(); i++) {
		out << "Card " << i << "=" << *(d.cards[i]) << endl;
	}
	return out;
}
