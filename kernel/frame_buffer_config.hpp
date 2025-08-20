#pragma once

#include <stdint.h>

// ピクセルのデータ形式
enum PixelFormat {
  kPixelRGBResv8BitPerColor,
  kPixelBGRResv8BitPerColor,
};

struct FrameBufferConfig {
  uint8_t* frame_buffer; // フレームバッファ領域へのポインタ
  uint32_t pixels_per_scan_line; // フレームバッファの余白を含めた横方向のピクセル数
  uint32_t horizontal_resolution; // 水平の解像度
  uint32_t vertical_resolution; // 垂直の解像度
  enum PixelFormat pixel_format; // ピクセルのデータ形式
};
