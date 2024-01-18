#include "LoggingObserver.h"
using namespace std;

/// <summary>
/// ILoggable Class
/// </summary>
ILoggable::ILoggable() {

}

ILoggable::~ILoggable() {

}

ILoggable::ILoggable(const ILoggable& logObj) {

};
ILoggable& ILoggable::operator = (const ILoggable& s) {
	return *this;
}

/// <summary>
/// Observer Class
/// </summary>
Observer::Observer() {

}

Observer::~Observer() {

}

Observer::Observer(const Observer& observer) {

};
Observer& Observer::operator = (const Observer& observer) {
	return *this;
}

/// <summary>
/// Subject Class
/// </summary>
Subject::Subject() {
	
}

Subject::~Subject() {
	observers.clear();
}

void Subject::Attach(Observer* observer) {
	observers.push_back(observer);
}

void Subject::Detach(Observer* observer) {
	observers.remove(observer);
}

void Subject::Notify(ILoggable* loggable) {
	list < Observer* > :: iterator i = observers.begin();
	for (; i != observers.end(); i++) {
		(*i)->Update(loggable);
	}
}

/// <summary>
/// LogObserver Class
/// </summary>
LogObserver::LogObserver() {

}
void LogObserver::Update(ILoggable* loggable) {
	
	ofstream filestream;

	filestream.open("gamelog.txt", ofstream::app);
	filestream << loggable->stringToLog() << endl;
	filestream.close();
}

LogObserver::LogObserver(const LogObserver& s) {

}
LogObserver& LogObserver::operator = (const LogObserver& s) {
	return *this;
}