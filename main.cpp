#include "ConverterJSON.h"

#include "iostream"

using std::cout;
using std::cin;

int main()
{
    ConverterJSON converterJson;
    InvertedIndex invertIndex;
    SearchServer searchServer(invertIndex);

    string nameProgram = converterJson.getNameProgramm();
    if (nameProgram.empty())
        return 1;

    cout << "The program " << nameProgram;

    string answer;

    while (answer != "exit")
    {
        cout << "\nEnter \"start\" to start searching or \"exit\" to exit the program: ";
        cin >> answer;
        if (answer != "start")
            continue;

        vector <string> textDocuments = converterJson.getTextDocuments();
        if(textDocuments.empty())
        {
            cout << "\nFill in the config file!";
            continue;
        }

        vector <string> request = converterJson.getRequests();
        if(request.empty())
        {
            cout << "\nFill in the request file!";
            continue;
        }

        invertIndex.updateDocumentBase(textDocuments);
        if (!converterJson.putAnswers(searchServer.search(request)))
        {
            cout << "\nIncorrect answer limit parameter in config file.";
        }

        cout << "\nSearch is over!" 
            "\nResults written to file: answers.json";
    }
}

