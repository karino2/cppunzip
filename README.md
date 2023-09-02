# CppUnzip

- Only depend on zlib and STL(no minizip dependency)
- No file write, in memory unzip
- Header only
- C++03 compatible(but not yet tested)
- Small enough to be able to modify for custom usecase

## Usage

Provide File interface to UnZipper, then listFiles return FileEntry which you can read content.
There is default File implementation of std::istream called IStreamFile.

Basic usage is like this:

```
  using namespace cppunzip;

  std::ifstream is("test.zip");
  IStreamFile f(is);

  UnZipper unzipper(f);

  for(auto& fileEntry: unzipper.listFiles()) {
    std::string fileName = fileEntry.fileName();

    if (!fileEntry.isDir()) {
      std::vector<uint8_t> content = fileEntry.readContent();
      // use content as you want.
    }
  }
```