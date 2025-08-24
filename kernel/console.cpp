/**
 * @file console.cpp
 *
 * コンソール描画のプログラムを集めたファイル．
 */

#include "console.hpp"

#include <cstring>
#include "font.hpp"

// コンストラクタ
// : 以下で初期化する
// witer_ ピクセル描画
// buffer_ 文字を保持
// cursor_row_/cursor_column_ 位置管理
Console::Console(PixelWriter& writer,
  const PixelColor& fg_color, const PixelColor& bg_color)
  : writer_{writer}, fg_color_{fg_color}, bg_color_{bg_color},
    buffer_{}, cursor_row_{0}, cursor_column_{0} {
}

// 1文字ずつ先頭から文字列を処理する
void Console::PutString(const char* s) {
  while (*s) {
    if (*s == '\n' ) {
      Newline(); // \nのとき改行をする
    } else if (cursor_column_ < kColumns -1) {
      WriteAscii(writer_, 8 * cursor_column_, 16 * cursor_row_, *s, fg_color_);
      buffer_[cursor_row_][cursor_column_] = *s;
      ++cursor_column_;
    }
    ++s;
  }
}

void Console::Newline() {
  cursor_column_ = 0; // 行頭に戻る

  if (cursor_row_ < kRows - 1) {
    ++cursor_row_; // カーソル行が最下行でなければ1行進めるだけ
  } else {
    // 表示領域をbg_color_で塗りつぶし
    for (int y = 0; y < 16 * kRows; ++y) {
      for (int x = 0; x < 8 * kColumns; ++x) {
        writer_.Write(x, y, bg_color_);
      }
    }
    // row + 1行目をrow行にコピーして、再描画
    for (int row = 0; row < kRows - 1; ++row) {
      memcpy(buffer_[row], buffer_[row + 1], kColumns + 1);
      WriteString(writer_, 0, 16 * row, buffer_[row], fg_color_);
    }
    memset(buffer_[kRows - 1], 0, kColumns + 1); // 最下行をnull文字で埋める
  }
}
