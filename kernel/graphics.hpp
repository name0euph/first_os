#pragma once

#include "frame_buffer_config.hpp"

struct PixelColor {
  uint8_t r, g, b;
};

// ピクセル描画インターフェースを作成
class PixelWriter {
  public:
    // コンストラクタ：インスタンスをメモリ上に生成、クラス名と同じ名前になる
    // フレームバッファ構成情報を受け取り、クラスのメンバー変数config_にコピー
    PixelWriter(const FrameBufferConfig& config) : config_{config} {
    }
    // デストラクタ：インスタンスを破棄、~をつける
    virtual ~PixelWriter() = default;

    // =0とすることで実装を持たない（純粋仮想関数）
    virtual void Write(int x, int y, const PixelColor& c) = 0;

  protected:
    uint8_t* PixelAt(int x, int y) {
      return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
    }

  private:
    const FrameBufferConfig& config_;
};

// PixelWriterインターフェースを継承
class RGBResv8BitPerColorPixelWriter: public PixelWriter {
  public:
    using PixelWriter::PixelWriter; // コンストラクタを引き継ぐ

    virtual void Write(int x, int y, const PixelColor& c) override;
};

class BGRResv8BitPerColorPixelWriter: public PixelWriter {
  public:
    using PixelWriter::PixelWriter; // コンストラクタを引き継ぐ

    virtual void Write(int x, int y, const PixelColor& c) override;
};
