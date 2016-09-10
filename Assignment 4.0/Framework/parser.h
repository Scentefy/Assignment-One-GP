// COMP710 GP 2D Framework
#ifndef __PARSER_H__
#define __PARSER_H__

#include "rapidjson/document.h"
#pragma warning (disable : 4996)
using namespace std;
using namespace rapidjson;

class Parser
{
	//Member Methods:
public:
	static Parser& GetInstance();
	~Parser();
	void loadInFile(string file);

protected:

private:
	Parser();
	Parser(const Parser& parser);
	Parser& operator=(const Parser& parser);

	//Member Data:
public:
	const char* json;
	string stringValue;
	Document document;
protected:
	static Parser* sm_pInstance;

private:

};

#endif //__PARSER_H__



string readFile(string file);