#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using std::cout;
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::string;

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
    return 1;
  }

  string line;
  string key = argv[2];
  string outfilename;
  ifstream indataset(argv[1]);

  if (!indataset.is_open()) {
    cerr << "error: no se pudo abrir \"" << argv[1] << "\"\n";
    return 1;
  }

  if (argc < 4) {
    outfilename = "dataset.txt";
  } else {
    outfilename = argv[3];
  }

  ofstream outdataset(outfilename);

  while (getline(indataset, line)) {
    json data = json::parse(line);
    outdataset << data[key] << '\n';
  }

  cout << "data guardada en \"" << outfilename << "\"\n";

  return 0;
}

