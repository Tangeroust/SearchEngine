#include "SearchServer.h"

#include <unordered_map>
#include <algorithm>

using std::unordered_map;
using std::sort;
using std::greater;

vector < vector <RelativeIndex> > SearchServer::search(const vector <string>& queriesInput)
{
	vector < vector <RelativeIndex> > relativeIndex;

	for (auto& request : queriesInput)
	{
		list <string> uniqueWords;
		freqOccurrence.clear();
		findUniqueWords(request, uniqueWords);
		sortUniqueWords(uniqueWords);
		relativeIndex.push_back(getRelativeIndex(uniqueWords));
	}

	return relativeIndex;
}

void SearchServer::findUniqueWords(const string& text, list <string>& uniqueWords)
{
	size_t size = text.size();
	size_t start = 0;
	size_t len = 0;

	for (size_t pos = 0; pos < size; ++pos)
	{
		if (text[pos] >= 'a' && text[pos] <= 'z')
		{
			if (len == 0)
				start = pos;

			len++;

			if (pos == size - 1)
			{
				uniqueWords.push_back(string(text, start, len));
				len = 0;
			}
		}
		else if (text[pos] == ' ' && len > 0)
		{
			uniqueWords.push_back(string(text, start, len));
			len = 0;
		}

		else
			len = 0;
	}
}

void SearchServer::sortUniqueWords(list <string>& uniqueWords)
{
	auto countCalculation = [this](const string& word)
	{
		map <string, pair <size_t, vector <Entry> > > ::iterator it;
		size_t allCount = 0;

		it = freqOccurrence.find(word);
		if (it == freqOccurrence.end())
		{
			vector <Entry> entryList = index.getWordCount(word);
			for (const auto& entry : entryList)
				allCount += entry.count;

			freqOccurrence[word] = pair <size_t, vector <Entry> > (allCount, entryList);
		}
		else
			allCount = it->second.first;

		return allCount;
	};

	auto comp = [countCalculation](const string& ltWord, const string& rtWord)
	{
		return countCalculation(ltWord) < countCalculation(rtWord);
	};

	uniqueWords.sort(comp);
}

vector <RelativeIndex> SearchServer::getRelativeIndex(const list <string>& uniqueWords) const
{
	if (uniqueWords.empty())
		return vector <RelativeIndex>();

	unordered_map <size_t, size_t> absoluteRelevance;
	list <string> ::const_iterator wordIter = uniqueWords.cbegin();

	auto freqIt = freqOccurrence.find(*wordIter);
	if (freqIt != freqOccurrence.cend())
	{
		for (const auto& entry : freqIt->second.second)
			absoluteRelevance[entry.docId] = entry.count;
	}

	size_t maxNumbMathes = 0;

	while (++wordIter != uniqueWords.cend())
	{
		freqIt = freqOccurrence.find(*wordIter);
		if (freqIt == freqOccurrence.cend())
			continue;

		unordered_map <size_t, size_t> dockList;
		for (const auto& entry : freqIt->second.second)
			dockList[entry.docId] = entry.count;

		unordered_map <size_t, size_t>::iterator it;
		vector <size_t> deletedDocs;
		for (auto& freq : absoluteRelevance)
		{
			it = dockList.find(freq.first);
			if (it != dockList.end())
			{
				freq.second += it->second;
				if (freq.second > maxNumbMathes)
					maxNumbMathes = freq.second;
			}
			else
				deletedDocs.push_back(freq.first);
		}

		for(auto& id : deletedDocs)
			absoluteRelevance.erase(id);
	}

	if (absoluteRelevance.empty())
		return vector <RelativeIndex>();

	vector <RelativeIndex> relIndex;
	for (const auto& freq : absoluteRelevance)
		relIndex.push_back(RelativeIndex(freq.first, (float)freq.second / maxNumbMathes));

	sort(relIndex.begin(), relIndex.end(), greater<RelativeIndex>());

	return relIndex;
}