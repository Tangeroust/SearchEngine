#include "InvertedIndex.h"

#include <thread>

using std::thread;

void InvertedIndex::updateDocumentBase(const vector <string>& inputDocs)
{
	docs = inputDocs;

	for (int docId = 0; docId < docs.size(); ++docId)
	{
		thread updateDock(&InvertedIndex::indexing, this, docs[docId], docId);
		updateDock.join();
	}
}

void InvertedIndex::indexing(const string& docText, size_t id)
{
	size_t size = docText.size();
	size_t start = 0;
	size_t len = 0;

	map <string, size_t> globalPosWord;

	auto& listEntry = [this, &globalPosWord, &start, &len, docText, id]()
	{
		string word(docText, start, len);

		map <string, vector <Entry> >::iterator it = freqDictionary.find(word);

		if (it == freqDictionary.end())
		{
			Entry newEntry;
			newEntry.docId = id;
			newEntry.count = 1;

			vector <Entry> newVector{ newEntry };

			blockDictionary.lock();
			freqDictionary[word] = newVector;
			blockDictionary.unlock();

			globalPosWord[word] = 0;
		}
		else
		{
			map <string, size_t>::iterator posIt = globalPosWord.find(word);

			if (posIt == globalPosWord.end())
			{
				Entry newEntry;
				newEntry.docId = id;
				newEntry.count = 1;

				blockDictionary.lock();
				it->second.emplace_back(newEntry);
				size_t posDocId = it->second.size() - 1;
				blockDictionary.unlock();

				globalPosWord[word] = posDocId;
			}

			else
				it->second[posIt->second].count++;
		}

		len = 0;
	};

	for (size_t pos = 0; pos < size; ++pos)
	{
		if (docText[pos] >= 'a' && docText[pos] <= 'z')
		{
			if (len == 0)
				start = pos;

			len++;

			if (pos == size - 1)
				listEntry();
		}
		else if (docText[pos] == ' ' && len > 0)
			listEntry();
			
		else
			len = 0;
	}
}

vector <Entry> InvertedIndex::getWordCount(const string& word) const
{
	map <string, vector <Entry> >::const_iterator it = freqDictionary.find(word);

	if (it == freqDictionary.end())
		return vector <Entry>();

	else
		return it->second;
}