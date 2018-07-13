#pragma once

/*
 * More Simple API for HTML
 * 非常に効率は悪いが巨大なhtmlでも僅かなバッファでパースができる
 * It's very inefficient but you can parse with a small buffer even with huge html
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 */

#include <Arduino.h>
#include <functional>

class MSAHParser {
using element_handler_t = std::function<void(uint32_t element_count, const char* element_name, uint16_t element_name_length, const char* part_of_value, uint16_t part_of_value_length)>;
public:
  MSAHParser(element_handler_t element_handler);
  ~MSAHParser();
  size_t write(uint8_t ch);

private:
  element_handler_t element_handler_;
  String buf_;
  uint16_t elem_count_ = 0;
  int16_t elem_begin_loc_ = -1;
  int16_t elem_end_loc_ = -1;
  int16_t pre_value_loc_ = -1;
  int16_t old_pre_value_loc_ = -1;
};
