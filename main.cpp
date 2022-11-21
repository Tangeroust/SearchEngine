#include "ConverterJSON.h"

#include "iostream"

using std::cout;
using std::cin;

int main()
{
    ConverterJSON converterJson;
    InvertedIndex invertIndex;
    SearchServer searchServer(invertIndex);

    cout << "The program " << converterJson.getNameProgramm();

    string answer;

    while (answer != "exit")
    {
        cout << "\nEnter \"start\" to start searching or \"exit\" to exit the program: ";
        cin >> answer;
        if (answer != "start")
            continue;

        invertIndex.updateDocumentBase(converterJson.getTextDocuments());
        converterJson.putAnswers(searchServer.search(converterJson.getRequests()));

        cout << "\nSearch is over!" 
            "\nResults written to file: answers.json";
    }
}

