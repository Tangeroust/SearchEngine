#pragma once

#include <string>
#include <vector>

#include "SearchServer.h"

using std::vector;
using std::string;
using std::pair;

class ConverterJSON {
public:
	ConverterJSON() = default;

	string getNameProgramm() const;
	/**
	* Метод получения содержимого файлов
	* @return Возвращает список с содержимым файлов перечисленных
	* в config.json
	*/
	vector <string> getTextDocuments() const;
	/**
	* Метод считывает поле max_responses для определения предельного 
	* количества ответов на один запрос
	* @return
	*/
	int getResponsesLimit() const;
	/**
	* Метод получения запросов из файла requests.json
	* @return возвращает список запросов из файла requests.json
	*/
	vector <string> getRequests() const;
	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void putAnswers(vector <vector <RelativeIndex> > answers);
};