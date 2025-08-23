/**
 * @file font.cpp
 *
 * フォント描画のプログラムを集めたファイル.
 */

#include "font.hpp"

// リンカが埋め込んだフォントバイナリのシンボルを参照
extern const uint8_t _binary_hankaku_bin_start; // 先頭アドレス
extern const uint8_t _binary_hankaku_bin_end; // 末尾の次
extern const uint8_t _binary_hankaku_bin_size; // 全体サイズ

// 文字cに対応するフォントデータ先頭へのポインタを返す関数
const uint8_t* GetFont(char c) {
  auto index = 16 * static_cast<unsigned int>(c); // 文字cのコード値×16バイト
  if (index >= reinterpret_cast<uintptr_t>(&_binary_hankaku_bin_size)) {
    return nullptr;
  }
  // 先頭アドレスにindexバイトを足して、その文字の先頭ポインタを返す
  return &_binary_hankaku_bin_start + index;
}

void WriteAscii(PixelWriter& writer, int x, int y, char c, const PixelColor& color) {
  const uint8_t* font = GetFont(c);
  if (font == nullptr) {
    return;
  }
  for (int dy = 0; dy < 16; ++dy) {
    for (int dx = 0; dx < 8; ++dx) {
      if ((font[dy] << dx) & 0x80u) {
        writer.Write(x + dx, y + dy, color);
      }
    }
  }
}
