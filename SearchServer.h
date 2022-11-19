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
	* @param idx � ����������� ������ ��������� ������ �� �����
	InvertedIndex,
	* ����� SearchServer ��� ������ ������� ���� ����������� �
	�������
	*/
	SearchServer(InvertedIndex& idx) : index(idx)
	{	};

	/**
	* ����� ��������� ��������� ��������
	* @param queriesInput ��������� ������� ������ �� �����
	requests.json
	* @return ���������� ��������������� ������ ����������� ������� ���
	�������� ��������
	*/
	vector <vector <RelativeIndex> > search(const vector <string>& queriesInput);

private:
	const InvertedIndex& index;
	map <string, pair <size_t, vector <Entry> > > freqOccurrence;

	void sortUniqueWords(list <string>& uniqueWords);
	void findUniqueWords(const string& text, list <string>& uniqueWords);
	vector <RelativeIndex> getRelativeIndex(const list <string>& uniqueWords) const;
};

