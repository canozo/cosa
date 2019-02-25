#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using std::cout;
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::string;

string outname(string, string);

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

  string outfilename = outname(argv[1], strlimit);
  ofstream outdataset(outfilename);

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

  cout << "data guardada en \"" << outfilename << "\", total de " << size << " bytes\n";

  return 0;
}

string outname(string infilename, string strlimit) {
  // dataset.json => dataset.txt
  int hasta = infilename.find('.');
  if (hasta == string::npos) {
    // no tiene punto
    return infilename + "_" + strlimit + "MB.txt";
  }
  // si tiene punto
  return infilename.substr(0, hasta) + "_" + strlimit + "MB.txt";
}
