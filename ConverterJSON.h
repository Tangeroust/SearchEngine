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
	* ����� ��������� ����������� ������
	* @return ���������� ������ � ���������� ������ �������������
	* � config.json
	*/
	vector <string> getTextDocuments() const;
	/**
	* ����� ��������� ���� max_responses ��� ����������� ����������� 
	* ���������� ������� �� ���� ������
	* @return
	*/
	int getResponsesLimit() const;
	/**
	* ����� ��������� �������� �� ����� requests.json
	* @return ���������� ������ �������� �� ����� requests.json
	*/
	vector <string> getRequests() const;
	/**
	* �������� � ���� answers.json ���������� ��������� ��������
	*/
	void putAnswers(vector <vector <RelativeIndex> > answers);
};