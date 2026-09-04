#include <cstring>
#include <iostream>
#include <vector>

#include <metaUtils.h>

// A chunk boundary that falls inside the gzip trailer leaves the trailer in
// the next input piece after the output buffer is already full.
static int
TestTrailerInLaterChunk(const std::vector<unsigned char> & raw, const std::vector<unsigned char> & compressed)
{
  const std::streamoff savedChunkSize = MET_GetMaxChunkSize();
  MET_SetMaxChunkSize(static_cast<std::streamoff>(compressed.size()) - 4);

  std::vector<unsigned char> destination(raw.size(), 0);
  const bool                 accepted = MET_PerformUncompression(compressed.data(),
                                                 static_cast<std::streamoff>(compressed.size()),
                                                 destination.data(),
                                                 static_cast<std::streamoff>(raw.size()));
  MET_SetMaxChunkSize(savedChunkSize);

  if (!accepted)
  {
    std::cerr << "FAILED: valid stream rejected when the trailer lands in a later input chunk\n";
    return 1;
  }
  if (destination != raw)
  {
    std::cerr << "FAILED: decompressed content does not match the original\n";
    return 1;
  }
  return 0;
}

static int
TestCorruptTrailerStillRejected(const std::vector<unsigned char> & raw, std::vector<unsigned char> compressed)
{
  compressed[compressed.size() - 1] ^= 0xFF;

  std::vector<unsigned char> destination(raw.size(), 0);
  std::cerr << "--- expect an uncompression failure message below ---\n";
  const bool accepted = MET_PerformUncompression(compressed.data(),
                                                 static_cast<std::streamoff>(compressed.size()),
                                                 destination.data(),
                                                 static_cast<std::streamoff>(raw.size()));
  if (accepted)
  {
    std::cerr << "FAILED: stream with a corrupt CRC trailer was accepted\n";
    return 1;
  }
  return 0;
}

int
main(int, char *[])
{
  std::vector<unsigned char> raw(64 * 1024);
  for (size_t i = 0; i < raw.size(); ++i)
  {
    raw[i] = static_cast<unsigned char>((i * 7 + (i >> 3)) & 0xFF);
  }

  std::streamoff  compressedSize = 0;
  unsigned char * compressedBuffer =
    MET_PerformCompression(raw.data(), static_cast<std::streamoff>(raw.size()), &compressedSize, 6);
  if (compressedBuffer == nullptr || compressedSize <= 8)
  {
    std::cerr << "FAILED: compression did not produce a usable stream\n";
    delete[] compressedBuffer;
    return 1;
  }
  const std::vector<unsigned char> compressed(compressedBuffer, compressedBuffer + compressedSize);
  delete[] compressedBuffer;

  int result = 0;
  result += TestTrailerInLaterChunk(raw, compressed);
  result += TestCorruptTrailerStillRejected(raw, compressed);

  if (result == 0)
  {
    std::cout << "testMeta15UncompressChunkBoundary passed\n";
  }
  return result;
}
