#pragma once

#include "InvertedIndex.h"

#include <list>

using std::list;
using std::pair;

struct RelativeIndex
{
	size_t docId;
	float rank;

	RelativeIndex(size_t id, float r) : docId(id), rank(r) {};

	bool operator == (const RelativeIndex& other) const
	{
		return (docId == other.docId && rank == other.rank);
	}

	bool operator > (const RelativeIndex& other) const
	{
		return rank > other.rank;
	}
};

class SearchServer
{
public:
	/**
	* @param idx в конструктор класса передаётся ссылка на класс
	InvertedIndex,
	* чтобы SearchServer мог узнать частоту слов встречаемых в
	запросе
	*/
	SearchServer(InvertedIndex& idx) : index(idx)
	{	};

	/**
	* Метод обработки поисковых запросов
	* @param queriesInput поисковые запросы взятые из файла
	requests.json
	* @return возвращает отсортированный список релевантных ответов для
	заданных запросов
	*/
	vector <vector <RelativeIndex> > search(const vector <string>& queriesInput);

private:
	const InvertedIndex& index;
	map <string, pair <size_t, vector <Entry> > > freqOccurrence;

	void sortUniqueWords(list <string>& uniqueWords);
	void findUniqueWords(const string& text, list <string>& uniqueWords);
	vector <RelativeIndex> getRelativeIndex(const list <string>& uniqueWords) const;
};

