/*
This file is part of DSpellCheck Plug-in for Notepad++
Copyright (C)2013 Sergey Semushin <Predelnik@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#pragma once
#include <locale>
#include "MainDef.h"
#include "Plugin.h"

struct NppData;

std::wstring to_wstring (std::string_view source);
std::string to_string (std::wstring_view source);
std::string toUtf8String (std::string_view source);
std::string toUtf8String (std::wstring_view source);
std::wstring utf8_to_wstring (const char *source);
std::string utf8_to_string (const char *source);

std::pair<std::wstring_view, bool> applyAlias(std::wstring_view str);

void SetParsedString(wchar_t *&Dest, const wchar_t* Source);
std::wstring parseString(const wchar_t* source);

LRESULT SendMsgToNpp(const NppData *NppDataArg, UINT Msg,
                     WPARAM wParam = 0, LPARAM lParam = 0);

char *Utf8Dec(const char *string, const char *current);
char *Utf8chr(const char *s, const char *sfc);
int Utf8Gewchar_tSize(char c);
char *Utf8strtok(char *s1, const char *Delimit, char **Context);
char *Utf8Inc(const char *string);
char *Utf8pbrk(const char *s, const char *set);
size_t Utf8Length(const char *String);
bool Utf8IsLead(char c);
bool Utf8IsCont(char c);

bool SortCompare(wchar_t *a, wchar_t *b);
bool Equivwchar_tStrings(wchar_t *a, wchar_t *b);
size_t Hashwchar_tString(wchar_t *a);
bool EquivCharStrings(char *a, char *b);
size_t HashCharString(char *a);
bool SortCompareChars(char *a, char *b);

bool CheckForDirectoryExistence(std::wstring path, bool Silent = true,
                                HWND NppWindow = nullptr);
wchar_t* GetLastSlashPosition(wchar_t* Path);

// trim from start (in place)
inline void ltrim(std::wstring &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wchar_t ch) {
        return !iswspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::wstring &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch) {
        return !iswspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::wstring &s) {
    ltrim(s);
    rtrim(s);
}

template <typename T>
std::weak_ptr<T> weaken (std::shared_ptr<T> ptr) { return ptr; }

template <typename... ArgTypes>
std::wstring wstring_printf (const wchar_t *format, ArgTypes &&... args) {
    auto size = _snwprintf (nullptr, 0, format, args...);
    std::vector<wchar_t> buf (size + 1);
    _snwprintf (buf.data (), size + 1, format, args...);
    return buf.data ();
}

void replaceAll(std::string& str, const std::string& from, const std::string& to);
std::wstring readIniValue(const wchar_t* appName, const wchar_t* keyName, const wchar_t* defaultValue,
                          const wchar_t* fileName);

class move_only_flag {
    using self = move_only_flag;
public:
    move_only_flag () {}
    static self create_valid () { self out; out.valid = true; return out; }
    void make_valid () { valid = true; }
    move_only_flag (self &&other) noexcept : valid (other.valid) { other.valid = false;}
    self &operator= (self &&other) noexcept { valid = other.valid; other.valid = false; return *this; }
    move_only_flag (const self &) = delete;
    self &operator= (const self &other) = delete;
    bool is_valid () const { return valid; }
private:
    bool valid = false;
};
