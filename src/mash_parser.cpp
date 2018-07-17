/*
 * More Simple API for HTML/XML
 * 非常に効率は悪いが巨大なhtmlでも僅かなバッファでパースができる
 * It's very inefficient but you can parse with a small buffer even with huge html
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 */

#include "mash_parser.hpp"

namespace {
// エレメントとバリューを含めこのサイズに収まる分しか保持しない、超えた分は失われる
constexpr uint16_t buffer_max = 512;
}

MSAHParser::MSAHParser(element_handler_t element_handler) {
  element_handler_ = element_handler;
}

MSAHParser::~MSAHParser() {
}

size_t MSAHParser::write(uint8_t ch) {
  if (buf_.length() >= buffer_max) {
    // バッファーサイズを超えていたらバッファの先頭から一文字削除
    buf_.remove(0, 1);
    // 削除した分ロケーションをずらす
    if (pre_value_loc_ > 0)  { pre_value_loc_ --; }
    if (elem_begin_loc_ > 0)  { elem_begin_loc_ --; }
  }

  switch (ch)
  {
      case '<': {
        // element開始を記録
        elem_begin_loc_ = buf_.length();
        buf_ += static_cast<char>(ch);
        break;
      }

      case '>': {
        // elementの終了
        if (elem_begin_loc_ >= 0) {
          buf_ += static_cast<char>(ch);

          elem_end_loc_ = buf_.length();
          old_pre_value_loc_ = pre_value_loc_;
          // element終了からvalueの先頭として記録
          pre_value_loc_ = elem_end_loc_;

          auto elem_len = elem_end_loc_ - elem_begin_loc_;
          if (elem_len < 0) {elem_len = 0;}

          auto value_len = elem_begin_loc_ - old_pre_value_loc_;
          if (value_len < 0) {value_len = 0;}

          element_handler_(elem_count_, &(buf_[elem_begin_loc_]), elem_len, &(buf_[old_pre_value_loc_]), value_len);
          elem_count_ ++;

          buf_.remove(0, elem_begin_loc_);
          elem_end_loc_ -= elem_begin_loc_;
          pre_value_loc_ -= elem_begin_loc_;
          old_pre_value_loc_ -= elem_begin_loc_;
          elem_begin_loc_ = -1;
        }
        break;
      }

      default: {
        buf_ += static_cast<char>(ch);
        break;
      }
  }

  return buf_.length();
}
