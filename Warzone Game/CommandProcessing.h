#pragma once
#include "GameEngine.h"
#include "LoggingObserver.h"
#include <list>
#include <fstream>
#include <iostream>
#include <string>

class GameEngine;

class Command : public ILoggable, public Subject {
public:
    //-------------- Constructors --------------//
    Command(string,Observer*);
    Command(string,string,Observer*);
    Command(const Command& obj);

    //-------------- Overloads --------------//
    Command& operator =(const Command& obj);
    friend std::ostream& operator <<(std::ostream& output, const Command& obj);

    //-------------- Destructor --------------//
    ~Command();

    //-------------- Getters --------------//
    string getCommand();
    string* getEffect();
    string getToAdd();

    //-------------- Setters --------------//
    void setEffect(string*);

    //-------------- Other Methods --------------// 
    void saveEffect(Command*);
    string stringToLog();
    
private:
    string* command;
    string* effect;
    string* toAdd;
};

class CommandProcessor: public ILoggable, public Subject {
public:
    //-------------- Constructors --------------//
    CommandProcessor(Observer*);  
    CommandProcessor(const CommandProcessor& obj);

    //-------------- Overloads --------------//
    CommandProcessor& operator =(const CommandProcessor& obj);
    
    friend std::ostream& operator <<(std::ostream& output, const CommandProcessor& obj);
    //-------------- Destructor --------------//
    ~CommandProcessor();
    
    //-------------- Getters --------------//
    vector<Command*> getCommandList();
    vector<Command*> getValidCommandList();
    bool getExitProgram();
    
    //-------------- Other Methods --------------// 
    void validate(Command*, GameEngine*);
    void getCommand(GameEngine*,CommandProcessor*);
    virtual Command* readCommand();
    void saveCommand(Command*);
    string stringToLog();


private:
    Observer* logger;

    vector <Command*> commandList;
    // Boolean to see if the program should exit
	bool* exitProgram;

	// Boolean to see if the commandProcessor should pause
	//bool cmdProPause = false;


};

class FileLineReader {
public:
    //-------------- Constructors --------------//  
    FileLineReader();
    FileLineReader(const FileLineReader& f);

    //-------------- Overloads --------------//
    friend ostream& operator<<(ostream& output, FileLineReader& F);
    FileLineReader& operator=(const FileLineReader& F);

    //-------------- Destructor --------------//
    ~FileLineReader();

    //-------------- Getters --------------//
    vector <string*> getFileContent();

    //-------------- Other Methods --------------//
    void readLineFromFile(string);
    
private:
    vector <string*> fileContent;
    
};


class FileCommandProcessorAdapter : public CommandProcessor {
public:
    //-------------- Constructors --------------//
    FileCommandProcessorAdapter(FileLineReader* flr, Observer*);
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& f);

    //-------------- Overloads --------------//
    friend ostream& operator<<(ostream& output, FileCommandProcessorAdapter& F);
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& F);

    //-------------- Destructor --------------//
    ~FileCommandProcessorAdapter();

    //-------------- Getters --------------//
    FileLineReader* getFileLineReader();

    //-------------- Other Methods --------------//
    Command* readCommand();

private:
    Observer* logger;
    FileLineReader* flr;
    vector <string*> commands;
    int* index;
};
