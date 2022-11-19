#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>

using std::string;	using std::map;
using std::vector;	using std::mutex;

struct Entry
{
	size_t docId;
	size_t count;

	//��� �������� ���������
	bool operator == (const Entry& other) const
	{
		return (docId == other.docId &&
				count == other.count);
	}
};

class InvertedIndex
{
public:
	InvertedIndex() = default;
	InvertedIndex(const InvertedIndex& index) : docs(index.docs), freqDictionary(index.freqDictionary) {};
	/**
	* �������� ��� ��������� ���� ����������, �� ������� ����� ���������
	�����*
	@param texts_input ���������� ����������
	*/
	void updateDocumentBase(const vector <string>& inputDocs);
	/**
	* ����� ���������� ���������� ��������� ����� word � ����������� ����
	����������
	* @param word �����, ������� ��������� �������� ���������� ����������
	* @return ���������� �������������� ������ � �������� ����
	*/
	vector <Entry> getWordCount(const string& word) const;

private:
	vector <string> docs; // ������ ����������� ����������
	map <string, vector <Entry> > freqDictionary; // ��������� �������
	mutex blockDictionary;

private:
	void indexing(const string& docText, size_t id);
};