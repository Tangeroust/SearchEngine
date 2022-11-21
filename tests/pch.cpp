#include "pch.h"

void TestInvertedIndexFunctionality(
    const vector<string>& docs,
    const vector<string>& requests,
    const vector<vector<Entry> >& expected
) {
    vector< vector<Entry> > result;
    InvertedIndex idx;
    idx.updateDocumentBase(docs);
    for (auto& request : requests) {
        vector <Entry> word_count = idx.getWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}

void TestCaseSearchServerFunctionality(
    const vector<string>& docs,
    const vector<string>& requests,
    const vector<vector<RelativeIndex>>& expected
) {
    InvertedIndex idx;
    idx.updateDocumentBase(docs);
    SearchServer srv(idx);
    vector <vector <RelativeIndex> > result = srv.search(requests);
    ASSERT_EQ(result, expected);
}