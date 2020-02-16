// This file is part of DSpellCheck Plug-in for Notepad++
// Copyright (C)2019 Sergey Semushin <Predelnik@gmail.com>
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

#include "npp/EditorInterface.h"

#include "utils/enum_array.h"
#include <vector>

class MockedIndicatorInfo {
public:
  int style;
  int foreground;
  std::vector<bool> set_for;
};

class MockedDocumentInfo {
public:
  std::wstring path;
  EditorCodepage codepage = EditorCodepage::utf8;
  std::vector<MockedIndicatorInfo> indicator_info;
  int lexer = 0;
  int hotspot_style = 123;
  int current_indicator = 0;
  std::array<TextPosition, 2> visible_lines = {0, 30};
  struct State {
  std::string data;
  std::array<TextPosition, 2> selection = {};
  std::vector<int> style;
  } cur;
  std::vector<State> past;

  void set_data(const std::wstring &data_arg);
  void set_data_raw(const std::string& data_arg);
  void erase(TextPosition start, TextPosition length);
  void save_state ();
  TextPosition cursor_pos;
};

class MockEditorInterface : public EditorInterface {
public:
  void move_active_document_to_other_view() override;
  void add_toolbar_icon(int cmd_id,
                        const toolbarIcons *tool_bar_icons_ptr) override;
  void force_style_update(TextPosition from, TextPosition to) override;
  void set_selection(TextPosition from, TextPosition to) override;
  void replace_selection(const char *str) override;
  void set_indicator_style(int indicator_index,
                           int style) override;
  void set_indicator_foreground(int indicator_index,
                                int style) override;
  void set_current_indicator(int indicator_index) override;
  void indicator_fill_range(TextPosition from, TextPosition to) override;
  void indicator_clear_range(TextPosition from, TextPosition to) override;
  EditorCodepage get_encoding() const override;
  TextPosition get_current_pos() const override;
  int get_current_line_number() const override;
  int get_text_height(int line) const override;
  int line_from_position(TextPosition position) const override;
  TextPosition get_line_start_position(TextPosition line) const override;
  TextPosition get_line_end_position(TextPosition line) const override;
  int get_lexer() const override;
  TextPosition get_selection_start() const override;
  TextPosition get_selection_end() const override;
  int get_style_at(TextPosition position) const override;
  bool is_style_hotspot(int style) const override;
  TextPosition get_active_document_length() const override;
  TextPosition get_line_length(int line) const override;
  int get_point_x_from_position(
    TextPosition position) const override;
  int get_point_y_from_position(
    TextPosition position) const override;
  TextPosition get_first_visible_line() const override;
  TextPosition get_lines_on_screen() const override;
  TextPosition get_document_line_from_visible(
    TextPosition visible_line) const override;
  TextPosition get_document_line_count() const override;
  bool open_document(std::wstring filename) override;
  void activate_document(int index) override;
  void activate_document(const std::wstring &filepath) override;
  void switch_to_file(const std::wstring &path) override;
  bool is_opened(const std::wstring &filename) const override;
  std::wstring active_document_path() const override;
  std::wstring active_file_directory() const override;
  std::wstring plugin_config_dir() const override;
  std::string selected_text() const override;
  std::string get_current_line() const override;
  std::string get_line(TextPosition line_number) const override;
  std::optional<TextPosition> char_position_from_global_point(int x,
                                                              int y) const override;
  HWND get_editor_hwnd() const override;
  HWND get_view_hwnd() const override;
  std::wstring get_full_current_path() const override;
  std::string get_text_range(TextPosition from,
                             TextPosition to) const override;
  std::string get_active_document_text() const override;
  TextPosition char_position_from_point(const POINT& pnt) const override;
  RECT editor_rect() const override;
  MockEditorInterface();
  ~MockEditorInterface();
  void open_virtual_document(NppViewType view, const std::wstring &path,
                             const std::wstring &data);
  void set_active_document_text(NppViewType view, const std::wstring &text);
  void set_active_document_text_raw(NppViewType view, const std::string& text);
  std::vector<std::string> get_underlined_words (NppViewType view, int indicator_id) const;
  void make_all_visible (NppViewType view);
  void set_lexer (NppViewType view, int lexer);
  void set_whole_text_style (NppViewType view, int style);
  void set_codepage(NppViewType view, EditorCodepage codepage);
  void delete_range(TextPosition start, TextPosition length) override;
  void begin_undo_action() override;
  void end_undo_action() override;
  void undo() override;
  bool is_line_visible(TextPosition line) const override;
  TextPosition find_next(TextPosition from_position, const char* needle) override;
  void replace_text(TextPosition from, TextPosition to, std::string_view replacement) override;
  void add_bookmark(TextPosition line) override;
  int get_view_count() const override;
  std::vector<std::wstring> get_open_filenames() const override;
  std::vector<std::wstring> get_open_filenames_all_views() const override;

private:
  void set_target_view(int view_index) const override;
  int get_target_view() const override;
  const MockedDocumentInfo *active_document() const;
  MockedDocumentInfo *active_document(NppViewType view);
private:
  int active_view() const override;
  enum_array<NppViewType, std::vector<MockedDocumentInfo>> m_documents;
  enum_array<NppViewType, int> m_active_document_index;
  enum_array<NppViewType, bool> m_save_undo;
  NppViewType m_active_view = NppViewType::primary;
  mutable NppViewType m_target_view = NppViewType::COUNT;
  static constexpr auto text_width = 13;
  static constexpr auto text_height = 13;
};
