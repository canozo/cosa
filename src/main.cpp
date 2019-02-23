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
    cerr << "args: " << argv[0] << " <archivo entrada> <key> [<archivo salida>]\n";
    cerr << "ej: " << argv[0] << " entrada.json key salida.txt\n";
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
  } else if (strcmp(argv[1], argv[3]) == 0) {
    cerr << "error: archivos de entrada y salida son el mismo\n";
    return 1;
  } else {
    outfilename = argv[3];
  }

  ofstream outdataset(outfilename);

  while (getline(indataset, line)) {
    json data = json::parse(line);

    if (data[key].is_null()) {
      cerr << "error: key \"" << key << "\" no valida para todo json en \"" << argv[1] << "\"\n";
      return 1;
    }

    outdataset << data[key].get<string>() << '\n';
  }

  cout << "data guardada en \"" << outfilename << "\"\n";

  return 0;
}

