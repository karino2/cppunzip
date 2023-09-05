#include "cppunzip.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>

using namespace cppunzip;

void testIStreamFile(File& f) {
  using namespace std;

  cout << f._size << endl;
  vector<uint8_t> buf(50);
  int res = f.readAt(f._size-50, buf.data(), 50);
  cout << res << endl;
  printf("%x, %x, %x, %x\n", buf[42], buf[43], buf[44], buf[45]);
}

void testInternalAPI(File& f) {
  using namespace std;
  using namespace cppunzip::impl;

  EOCDRReader unzipper(f);
  EOCDRecord eocdr = unzipper.readEOCDRecord();
  printf("entryNum=%d, size=%x, offset=%x\n", eocdr._cdEntryNum, eocdr._cdSize, eocdr._cdOffset);

  CDReader reader(f, eocdr);
  while(!reader.isEnd()) {
    CDRecord cdr = reader.readOne();
    printf("name=%s\n", cdr._fileName.c_str() );
    printf("   comp=%d, csize=%d, usize=%d\n", cdr._compressionMethod, cdr._compressedSize, cdr._uncompressedSize);
    if(!cdr.isDir()) {      
      printf("   not dir\n");
      CDRContentReader ereader(f, cdr);
      std::vector<uint8_t> cont = ereader.readContent();

      printf("   uncompressedsize=%zu\n", cont.size());
      // I assume uncompressed file must be larger than 4 bytes. because it just test!
      printf("   %c%c%c%c...\n", cont[0], cont[1], cont[2], cont[3]);
    }
  }
}


void printContent(std::vector<uint8_t>& content) {
  for(auto i : content)
    printf("%c", (char)i);
}

void testPublicAPI(File& f) {
  using namespace std;

  UnZipper unzipper(f);
  for(auto& fileEntry : unzipper.listFiles()) {
    cout << fileEntry.fileName() << ":" << endl;
    if (!fileEntry.isDir()) {
      auto content = fileEntry.readContent();
      cout << "   content size: " << content.size() << endl;
      cout << "   content: [" << endl;
      printContent(content);
      cout << endl;
      cout << "]" << endl;
    }
  }
}

int main() {
  std::ifstream is("test.zip", std::ios::binary);
  IStreamFile f(is);

  // testIStreamFile(f);
  // testInternalAPI(f);
  testPublicAPI(f);

  return 0;
}