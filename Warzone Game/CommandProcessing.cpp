#include "CommandProcessing.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std; 

//to make lowercase
string toLower(string arr) {
	char a = 'a';
	string lowerArr = "";

	for (int i = 0; i < arr.length(); i++) {
		a = arr[i];
		lowerArr.push_back(char(tolower(a)));
	}

	return lowerArr;
}

//----------------------------Command Class----------------------------//

//-------------- Constructors --------------//

Command::Command(string cmd,Observer* _obs) {
	this->command = new string(cmd);
	this->effect = new string("Not a valid command");
	this->toAdd = new string("");

	this->Attach(_obs);
}

Command::Command(string cmd, string toAdd, Observer* _obs) {
	this->command = new string(cmd);
	this->toAdd = new string(toAdd);

	this->Attach(_obs);
}

Command::Command(const Command& obj) {
	this->command = obj.command;
	this->toAdd = obj.command;
	this->effect = obj.effect;
}

//-------------- Overloads --------------//
Command& Command::operator = (const Command& obj) {
	this->command = obj.command;
	this->toAdd = obj.command;
	this->effect = obj.effect;
	return *this;
}
std::ostream& operator <<(std::ostream& output, const Command& obj) {
	output << "The command is " << *obj.command << " with effect: " << *obj.effect << std::endl;
	return output;
}

//-------------- destructors --------------//
Command::~Command() {
	delete command;
	delete effect;

}

//-------------- Getters --------------//
string Command::getCommand(){
	return *this->command;
}

string* Command::getEffect(){
	return this->effect;
}

string Command::getToAdd(){
	return *this->toAdd;
}

//-------------- Setters --------------//

void Command::setEffect(string* effect){
	this->effect = effect;
}

//-------------- Other Methods --------------//
void Command::saveEffect(Command* cmd) {
	
	if(cmd->getCommand() == "loadmap")
		cmd->setEffect(new string(cmd->getToAdd() + " has been succesfully loaded"));
	
	if(cmd->getCommand() == "validatemap")
		cmd->setEffect(new string("The map has been validated"));
	
	if(cmd->getCommand() == "addplayer")
		cmd->setEffect(new string("Player " + cmd->getToAdd() + " has been successfully added to the game"));
	
	if(cmd->getCommand() == "gamestart")
		cmd->setEffect(new string("The game has started"));
	
	if(cmd->getCommand() == "replay")
		cmd->setEffect(new string("Replaying the game"));
	
	if(cmd->getCommand() == "quit")
		cmd->setEffect(new string("Quitting the game"));

	if(cmd->getCommand() == "gameend")
		cmd->setEffect(new string("Command for testing purposes that skips the main game loop"));

	Notify(this);
}

string Command::stringToLog() {
	string command = "Command issued: " + getCommand()
		+ "\nCommand's effect: " + *getEffect() + "\n";
	return command;
}

//----------------------------CommandProcessor Class----------------------------//

//-------------- Constructors --------------//

CommandProcessor::CommandProcessor(Observer* _obs){
	this->logger = _obs;
	this->exitProgram = new bool(false);

	this->Attach(_obs);
}

CommandProcessor::CommandProcessor(const CommandProcessor& obj) {
	this->logger = obj.logger;
	
	for (auto i = 0; i < obj.commandList.size() ; i++)
    {
        commandList.push_back(new Command(*obj.commandList[i]));
    }

	this->exitProgram = obj.exitProgram;
}

//-------------- Overloads --------------//
CommandProcessor& CommandProcessor::operator = (const CommandProcessor& obj) {
	this->logger = obj.logger;
	for (auto i = 0; i < obj.commandList.size() ; i++)
    {
        commandList.push_back(new Command(*obj.commandList[i]));
    }
	this->exitProgram = obj.exitProgram;
	return *this;
}

std::ostream& operator <<(std::ostream& output, const CommandProcessor& obj) {
	
	output << "The command list contains the following:" << endl;
	for(Command* cmd : obj.commandList)
		output << *cmd << endl;

	return output;
}

//-------------- destructors --------------//
CommandProcessor::~CommandProcessor() {
	commandList.clear();
	delete exitProgram;
	this->Detach(logger);
}

//-------------- Getters --------------//

vector<Command*> CommandProcessor::getCommandList(){
	return this->commandList;
}

vector<Command*> CommandProcessor::getValidCommandList(){
	vector<Command*> validCommands;

	for(Command* cmd : commandList){
		if (*cmd->getEffect() != "Not a valid command") 
			validCommands.push_back(cmd);
	}

	return validCommands;
}

bool CommandProcessor::getExitProgram(){
	return *this->exitProgram;
}

//-------------- Other Methods --------------// 
void CommandProcessor::getCommand(GameEngine* game,CommandProcessor* cp) {
	Command* cmd = cp->readCommand();
	validate(cmd,game);
	saveCommand(cmd);
}

Command* CommandProcessor::readCommand() {
	
	string cmdString;
	string toAdd;

	cout << "\nEnter Command: ";

	cin >> cmdString;
	cmdString = toLower(cmdString);

	if(cmdString == "loadmap"){
		cout << "Which map to load? ";
		cin >> toAdd;
		cout << endl;
	}

	if(cmdString == "addplayer"){
		cout << "Which player to load? ";
		cin >> toAdd;
		cout << endl;
	}

	
	vector<string> validCommands = {"loadmap", "validatemap", "addplayer", "gamestart", "replay", "quit", "gameend"};

	if(find(validCommands.begin(), validCommands.end(), cmdString) != validCommands.end())
		return new Command(cmdString,toAdd,logger);
	else
		return new Command(cmdString,logger);

}

void CommandProcessor::saveCommand(Command* cmd) {
	commandList.push_back(cmd);
	Notify(this);
}

void CommandProcessor::validate(Command* command, GameEngine* game) {

		command ->saveEffect(command);

	if (game->checkState(command->getCommand())) {

		if(command->getCommand() == "quit"){
			cout << "Quitting the game.\n";

			// Setting the exitProgram boolean to true in order to stop the program
			this->exitProgram = new bool(true);
		}

		cout << "It is a valid command. \n" << endl;
	}
	else	
		cout << "It is not a valid command \n" << endl;
		

}

string CommandProcessor::stringToLog(){
	return "Inserted command: " + commandList.back()->getCommand() + " into the list.\n";
}

//----------------------------FileLineReader Class----------------------------//

//-------------- Constructors --------------//
FileLineReader::FileLineReader() {}

FileLineReader::FileLineReader(const FileLineReader& f) {
	this->fileContent = f.fileContent;
}

//-------------- Overloads --------------//
ostream& operator<<(ostream& output, FileLineReader& f) {
	for(string* line : f.fileContent)
		output << *line << endl;
	return output;
}

FileLineReader& FileLineReader::operator=(const FileLineReader& f) {
	this->fileContent = f.fileContent;
	return *this;
}

//-------------- destructors --------------//
FileLineReader::~FileLineReader(){
	fileContent.clear();
}


//-------------- Getters --------------//
vector <string*> FileLineReader::getFileContent(){
	return this->fileContent;
}
//-------------- Other Methods --------------// 
void FileLineReader::readLineFromFile(string fileName) {
	
	vector<string> lines;
	ifstream myfile("./commandProcessorFiles/" + fileName);

	if (myfile.is_open()) {
		string line;

		// While the file contains something we add each line to the lines vector
		while (getline(myfile, line)) {
			fileContent.push_back(new string(line));
		}
		// Closing the file
		myfile.close();

	}
	else {
		cout << "Couldn't find the mentioned file." << '\n';
	}
}

//----------------------------FileCommandProcessorAdapter Class----------------------------//

//-------------- Constructors --------------//


FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileLineReader* reader, Observer* _obs) :CommandProcessor(_obs) {
	
	this->flr = reader;
	for (int i = 0; i < reader->getFileContent().size(); i++) {
		commands.push_back(reader->getFileContent().at(i));
	}
	this->index = new int(-1);
	this->logger = _obs;

}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& f) : CommandProcessor(f.logger){
	this->logger = f.logger;

	this->flr = f.flr;
	for (int i = 0; i < f.flr->getFileContent().size(); i++) {
		commands.push_back(f.flr->getFileContent().at(i));
	}
	this->index = f.index;	
}
//-------------- Overloads --------------//
ostream& operator<<(ostream& output, FileCommandProcessorAdapter& F) {
	output << F.readCommand();
	return output;
}

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& f) {
	this->flr = f.flr;
	for (int i = 0; i < f.flr->getFileContent().size(); i++) {
		commands.push_back(f.flr->getFileContent().at(i));
	}
	this->index = f.index;	
	return *this;
}
//-------------- destructors --------------//
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
	delete flr;
	delete logger;
	commands.clear();
}

//-------------- Getters --------------//
FileLineReader* FileCommandProcessorAdapter::getFileLineReader() {
	return this->flr;
}

//-------------- Other Methods --------------// 
Command* FileCommandProcessorAdapter::readCommand() {
	
	// Using a space delimiter when reading from a file for lines such as "loadmap mapname"
	string delimiter{ " " };

	// The position of the delimiter
	size_t pos{};

	// The word before the delimiter
	string firstWord{ "" };

	// The word after the delimiter which is the toAdd part
	string secondWord{ "" };

	// Increasing the index of the line
	int currentIdx = *index + 1;
	this->index = new int(currentIdx);

	//current line from vector
	string line = *commands[currentIdx];

	// If the delimiter is found and is not at the end of the string
	if (line.find(delimiter) != string::npos) {

		// Separating every line at the delimiter " " into two words
		pos = line.find(delimiter);
		string firstWord(line.substr(0, pos));
		string secondWord(line.substr(pos + 1));

		if (firstWord == "loadmap" || firstWord == "addplayer") 
		return new Command(firstWord, secondWord, logger);
	}

	// If the delimiter is not found then we can read the line normally
	vector<string> otherValidCommands = {"validatemap", "gamestart", "replay", "quit", "gameend"};
	
	if(find(otherValidCommands.begin(), otherValidCommands.end(), line) != otherValidCommands.end())
		return new Command(line," ",logger);
	else
		return new Command(line, logger);
}














