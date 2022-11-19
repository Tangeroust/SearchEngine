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

	//ƒл€ тестовых сценариев
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
	* ќбновить или заполнить базу документов, по которой будем совершать
	поиск*
	@param texts_input содержимое документов
	*/
	void updateDocumentBase(const vector <string>& inputDocs);
	/**
	* ћетод определ€ет количество вхождений слова word в загруженной базе
	документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает подготовленный список с частотой слов
	*/
	vector <Entry> getWordCount(const string& word) const;

private:
	vector <string> docs; // список содержимого документов
	map <string, vector <Entry> > freqDictionary; // частотный словарь
	mutex blockDictionary;

private:
	void indexing(const string& docText, size_t id);
};