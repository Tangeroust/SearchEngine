#pragma once
#include "../search_engine/SearchServer.cpp"
#include "../search_engine/InvertedIndex.cpp"
#include "gtest/gtest.h"

using namespace std;

void TestInvertedIndexFunctionality(
    const vector <string>& docs,
    const vector <string>& requests,
    const vector <vector <Entry> >& expected
);

void TestCaseSearchServerFunctionality(
    const vector <string>& docs,
    const vector <string>& requests,
    const vector<vector<RelativeIndex>>& expected
);