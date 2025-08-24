/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたファイル．
 */

#include <cstdint>
#include <cstddef>
#include <cstdio>

#include "frame_buffer_config.hpp"
#include "graphics.hpp"
#include "font.hpp"
#include "console.hpp"

// 配置newを定義して、クラスのインスタンスを生成可能にする
void* operator new(size_t size, void* buf) {
  return buf;
}
void operator delete(void* obj) noexcept {
}

char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_writer;

// コンソールクラスをグローバル変数で参照できるようにする
char console_buf[sizeof(Console)];
Console* console;

// そのコンソールを出力先にするprintk関数を作成
// ...で可変長引数を受け取る宣言
int printk(const char* format, ...) {
  va_list ap;
  int result;
  char s[1024]; // 1024バイトの固定長

  // va_list型のapで受け取って、sに書式展開
  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap); // 後始末

  console->PutString(s);
  return result;
}

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {

  switch (frame_buffer_config.pixel_format) {
    case kPixelRGBResv8BitPerColor:
      // new(メモリ領域) クラス名(引数) でインスタンス生成
      pixel_writer = new(pixel_writer_buf)
        RGBResv8BitPerColorPixelWriter(frame_buffer_config);
      break;
    case kPixelBGRResv8BitPerColor:
      pixel_writer = new(pixel_writer_buf)
        BGRResv8BitPerColorPixelWriter(frame_buffer_config);
      break;
  }

  for (int x = 0; x < frame_buffer_config.horizontal_resolution; ++x) {
    for (int y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
      pixel_writer->Write(x, y, {255, 255, 255});
    }
  }

  // Consoleのメモリを確保してインスタンス生成
  console = new(console_buf) Console(*pixel_writer, {0, 0, 0}, {255, 255, 255});

  // コンソール表示
  for (int i = 0; i < 27; i++) {
    printk("printk: %d\n", i);
  }


  while (1) __asm__("hlt");  // __asm__()はアセンブリ言語の命令を埋め込むときに使う
}
