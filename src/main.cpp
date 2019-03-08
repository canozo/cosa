#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <regex>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using std::cout;
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;
using std::find;
using std::vector;
using std::transform;
using std::regex;
using std::regex_replace;

string tempname(string);
string resname(string, uint64_t);
string process(vector<string>, string);

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cerr << "error, args: " << argv[0] << " <archivo entrada> <key> [<limite (MB)> <stopwords>]\n";
    return 1;
  }

  uint64_t size = 0;
  uint64_t limit = 1000000000;
  string strlimit = "1000";
  string key = argv[2];
  ifstream indataset(argv[1]);

  if (!indataset.is_open()) {
    cerr << "error: no se pudo abrir \"" << argv[1] << "\"\n";
    return 1;
  }

  if (argc >= 4) {
    strlimit = argv[3];
    limit = strtoull(argv[3], NULL, 10) * 1000000;
  }

  string swfile = "../stopwords/default.txt";
  ifstream instopwords;

  if (argc >= 5) {
    swfile = argv[4];
  }
  instopwords.open(swfile, ifstream::in);

  string stopword;
  vector<string> stopwords;
  while (getline(instopwords, stopword)) {
    stopwords.push_back(" " + stopword + " ");
  }

  string outfilename = tempname(argv[1]);
  ofstream outdataset(outfilename + ".temp");

  string line;
  while (getline(indataset, line) && size <= limit) {
    json data = json::parse(line);

    if (data[key].is_null()) {
      cerr << "error: key \"" << key << "\" contiene \"null\"\n";
      return 1;
    }

    if (!data[key].is_string()) {
      cerr << "error: key \"" << key << "\" no contiene \"string\", contiene \"" << data[key].type_name() << "\"\n";
      return 1;
    }

    string text = process(stopwords, data[key].get<string>());
    outdataset << text << '\n';
    size += text.size() + 1;
  }

  string resfile = resname(outfilename, size);
  rename((outfilename + ".temp").c_str(), resfile.c_str());

  cout << "data guardada en \"" << resfile << "\"\n";
  return 0;
}

string tempname(string infilename) {
  int desde = infilename.find_last_of('/') + 1;
  int hasta = infilename.find_last_of('.');
  return infilename.substr(desde, hasta - desde);
}

string resname(string oldname, uint64_t size) {
  stringstream result;
  result << oldname << '_' << size / 1000000 << "MB.txt";
  return result.str();
}

string process(vector<string> stopwords, string text) {
  string processed = regex_replace(text, regex(R"([^a-zA-Z0-9 '])"), "");
  for (int i = 0; i < stopwords.size(); i += 1) {
    string::size_type find_pos;
    while ((find_pos = processed.find(stopwords[i])) != string::npos) {
      processed.erase(find_pos, stopwords[i].length() - 1);
    }
  }
  transform(processed.begin(), processed.end(), processed.begin(), ::tolower);
  return processed;
}
