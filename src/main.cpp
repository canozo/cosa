#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using std::cout;
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;

string tempname(string);
string resname(string, uint64_t);

int main(int argc, char *argv[]) {
  bool error = false;

  if (argc < 2) {
    cerr << "falta argumento: archivo de dataset (ej: \"dataset.json\")\n";
    error = true;
  }

  if (argc < 3) {
    cerr << "falta argumento: key de dataset (ej: \"reviewText\")\n";
    error = true;
  }

  if (error) {
    cerr << "args: " << argv[0] << " <archivo entrada> <key> [<limite (MB)>]\n";
    cerr << "ej: " << argv[0] << " entrada.json key 100\n";
    return 1;
  }

  uint64_t size = 0;
  uint64_t limit = 500000000;
  string strlimit = "500";
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

  string outfilename = tempname(argv[1]);
  ofstream outdataset(outfilename + "_temp.txt");

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

    string text = data[key].get<string>();
    outdataset << text << '\n';
    size += text.size() + 1;
  }

  string resfile = resname(outfilename, size);
  rename((outfilename + "_temp.txt").c_str(), resfile.c_str());

  cout << "data guardada en \"" << resfile << "\", total de " << size << " bytes\n";

  return 0;
}

string tempname(string infilename) {
  // dataset.json => dataset.txt
  int hasta = infilename.find('.');
  if (hasta == string::npos) {
    // no tiene punto
    return infilename;
  }
  // si tiene punto
  return infilename.substr(0, hasta);
}

string resname(string oldname, uint64_t size) {
  stringstream result;
  result << oldname << '_' << size / 1000000 << "MB.txt";
  return result.str();
}
