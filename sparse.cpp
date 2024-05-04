#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>

using namespace std;

//read the contents of a file into a vector of strings where each line is a seperate element
vector<string> readFile(int argc, char *argv[]) {
    vector<string> fileContents;
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input filename> [output filename]" << endl;
        return fileContents;
    }
    ifstream file(argv[1]);
    if(!file.is_open()) {
        cerr << "Cannot open file: " << argv[1] << endl;
        return fileContents;
    }
    
    //read each line seperately into fileContents vector
    string line;
    while(getline(file, line)) {
        fileContents.push_back(line);
    }
    
    file.close();
    return fileContents;
}

//parse each line from the file such that the parameters, results and signals are ignored
//syscalls are in the form: 'name(param1, param2) = result'
//signals are in the form: '+++signal+++' or '---signal---'
vector<string> parseLines(const vector<string>& fileContents) {
    vector<string> parsedLines;
    for(const auto& line : fileContents) {
        if(!line.empty() && line[0] != '+' && line[0] != '-') {     //line should not be a signal
            size_t bracketIdx = line.find('(');                     //find index of opening bracket: (
            if (bracketIdx != string::npos) {                       //if no opening bracket found, not a syscall
                string parsedLine = line.substr(0, bracketIdx);     //create new string out of only syscall name
                parsedLines.push_back(parsedLine);
            }
        }
    }
    return parsedLines;
}

void writeResult(const set<string>& resultSet, string fileName) {
    ofstream file(fileName);
    if(!file.is_open()) {
        cerr << "Cannot write to file: " << fileName << "\n";
        return;
    }
    for (const auto& syscall : resultSet) {
            file << syscall << endl;
    }
    file.close();
    cout << "Results written to file: " << fileName << endl;
    return;
}

int main(int argc, char *argv[])
{
    auto fileContents = readFile(argc, argv);
    if(fileContents.size() == 0) return 1;

    auto parsedLines = parseLines(fileContents);

    //add everything to a set to remove duplicates
    set<string> resultSet = {parsedLines.begin(), parsedLines.end()};
    
    //write result to file if output file is provided. Otherwise, print result
    if(argc == 3) {
        writeResult(resultSet, argv[2]);
    }
    else {
        // print set as result
        for (const auto& syscall : resultSet) {
            cout << syscall << endl;
        }
    }

    return 0;
}