#include<iostream>
#include<cstdlib>
#include<sstream>
#include<fstream>
#include<string>
#include"TextQuery.h"

using namespace std;

/*==============================================================================*/

ifstream& open_file(ifstream &in, const string &file)
{
    in.close();
    in.clear();
    in.open(file.c_str());
    return in;
}

/*==============================================================================*/

void TextQuery::store_file(ifstream &is) {
    string line;
    while(getline(is, line)) {
        lines_of_text.push_back(line);
    }
}

string TextQuery::text_line(TextQuery::line_no i) const{
    if(i < lines_of_text.size())
        return lines_of_text[i];
}

void TextQuery::build_map() {
    for(line_no i = 0; i < lines_of_text.size(); i++) {
        string word;
        istringstream line(lines_of_text[i]);
        while(line >> word) {
            word_map[word].insert(i);
        }
    }
}

set<TextQuery::line_no> TextQuery::run_query(const string keyWord) const {
    map<string, set<line_no> >::const_iterator loc = word_map.find(keyWord);
    if(loc == word_map.end()) {
        return set<line_no>();
    }
    else {
        return loc -> second;
    }
}

int main() {
    string fileName, keyWord;
    ifstream is;

    cin >> fileName;
    if(!cin || !open_file(is, fileName)) {
        cerr << "None such File." << endl;
        return 0;
    }

    TextQuery tq;
    tq.read_file(is);
    while(1) {
        cout << "Input a key word: ";
        cin >> keyWord;
        if(!cin || keyWord == "q") break;
        set<TextQuery::line_no> loc = tq.run_query(keyWord);
        for(set<TextQuery::line_no>::iterator it = loc.begin(); it != loc.end(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    }


    return 0;
}
