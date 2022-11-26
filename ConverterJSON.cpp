#include "ConverterJSON.h"

#include <fstream>
#include <iostream>
#include <exception>

#include "nlohmann/json.hpp"

using std::ifstream;			using std::to_string;	using std::endl;
using std::ofstream;			using std::ios_base;	using std::ios;
using json = nlohmann::json;	using std::cout;
using std::istreambuf_iterator; 

string ConverterJSON::getNameProgramm() const
{
	ifstream config("config.json");

	try
	{
		if (!config)
			throw ios_base::failure("Error: config file is missing");
	}
	catch (const ios_base::failure& error)
	{
		cout << "\n" << error.what();
		return string();
	}

	json doksJson = json::parse(config);
	config.close();

	try
	{
		if (!doksJson.contains("config") || !doksJson.contains("files"))
			throw ios_base::failure("Error: config file is empty");
		if (!doksJson["config"].contains("name") || doksJson["config"]["name"].empty())
			throw ios_base::failure("Error: program name not found");
		if (!doksJson["config"].contains("version"))
			throw ios_base::failure("Error: program version not found");
	}
	catch (const ios_base::failure& error)
	{
		cout << "\n" << error.what();
		return string();
	}

	return doksJson["config"]["name"];
}

vector <string> ConverterJSON::getTextDocuments() const
{
	vector <string> textFiles;

	ifstream config("config.json");
	
	try
	{
		if (!config)
			throw ios_base::failure("Error: config file is missing");
	}
	catch (const ios_base::failure& error)
	{
		cout << "\n" << error.what();
		return textFiles;
	}

	json doksJson = json::parse(config);

	config.close();

	try
	{
		if (!doksJson.contains("files"))
			throw ios_base::failure("Error: config file is empty");
	}
	catch (const ios_base::failure& error)
	{
		cout << "\n" << error.what();
		return textFiles;
	}

	for(string filePath : doksJson["files"])
	{
		ifstream file(filePath);

		if (!file)
		{
			cout << "\nError: file " << filePath << " not found";
			textFiles.push_back("");
			continue;
		}

		string text{istreambuf_iterator<char>(file), istreambuf_iterator<char>()};
		file.close();

		textFiles.push_back(text);
	}

	return textFiles;
}

int ConverterJSON::getResponsesLimit() const
{
	ifstream config("config.json");

	try 
	{
		if (!config)
			throw ios_base::failure("Error: config file is missing");
	}
	catch (const ios_base::failure& error)
	{
		cout << "\n" << error.what();
		return -1;
	}

	json optionsJson = json::parse(config);
	int maxResponses;

	try
	{
		if (!optionsJson["config"].contains("max_responses"))
			throw ios_base::failure("Error: max responses not found");

		maxResponses = optionsJson["config"]["max_responses"];
		if (maxResponses < 1)
			throw (&maxResponses);
	}
	catch (int* _maxRespones)
	{
		cout << "\nError: max_responses cannot be less than 1";
		*_maxRespones = 1;
	}
	catch (const ios_base::failure& error)
	{
		cout << "\n" << error.what();
		return -1;
	}

	return maxResponses;
}

vector <string> ConverterJSON::getRequests() const
{
	vector <string> queryList;

	ifstream request("requests.json");

	if (!request)
	{
		cout << "\nError: request file is missing" << 
				"\nWarning: empty request list";

		return queryList;
	}

	json reqJson = json::parse(request);
	request.close();

	for (string query : reqJson["requests"])
		queryList.push_back(query);
	
	if (queryList.empty())
		cout << "\nWarning: empty request list";

	return queryList;
}

bool ConverterJSON::putAnswers(vector <vector <RelativeIndex> > answers)
{
	int maxResponses = getResponsesLimit();

	if (maxResponses < 1)
		return false;

	json answerJson;

	for (int index = 0; index < answers.size(); index++)
	{
		string numRequest = to_string(index + 1);
		string requestText = "request" + string(3 - numRequest.size(), '0') + numRequest;

		answerJson["answers"][requestText]["result"] = !answers[index].empty();

		json jsonRank;

		for (int relev = 0; relev < answers[index].size() && relev < maxResponses; ++relev)
		{
			jsonRank["docid"] = answers[index][relev].docId;
			jsonRank["rank"] = answers[index][relev].rank;
			answerJson["answers"][requestText]["relevance"].push_back(jsonRank);
		}
	}

	ofstream fileAnsw("answers.json", ios::trunc);
	fileAnsw << answerJson << endl;
	fileAnsw.close();

	return true;
}