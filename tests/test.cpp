
class TFile {
public:
  static TFile* open(const char* path);
};

int main() {
  TFile* F = TFile::open("file.root");
}
