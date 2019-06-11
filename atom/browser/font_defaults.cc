// Copyright (c) 2018 Slack Technologies, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "atom/browser/font_defaults.h"

#include <string>
#include <unordered_map>

#include "base/stl_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/common/pref_names.h"
#include "chrome/grit/platform_locale_settings.h"
#include "content/public/common/web_preferences.h"
#include "ui/base/l10n/l10n_util.h"

namespace {

// The following list of font defaults was copied from
// https://chromium.googlesource.com/chromium/src/+/69.0.3497.106/chrome/browser/ui/prefs/prefs_tab_helper.cc#152
//
// The only updates that should be made to this list are copying updates that
// were made in Chromium.
//
// vvvvv DO NOT EDIT vvvvv

struct FontDefault {
  const char* pref_name;
  int resource_id;
};

// Font pref defaults.  The prefs that have defaults vary by platform, since not
// all platforms have fonts for all scripts for all generic families.
// TODO(falken): add proper defaults when possible for all
// platforms/scripts/generic families.
const FontDefault kFontDefaults[] = {
    {prefs::kWebKitStandardFontFamily, IDS_STANDARD_FONT_FAMILY},
    {prefs::kWebKitFixedFontFamily, IDS_FIXED_FONT_FAMILY},
    {prefs::kWebKitSerifFontFamily, IDS_SERIF_FONT_FAMILY},
    {prefs::kWebKitSansSerifFontFamily, IDS_SANS_SERIF_FONT_FAMILY},
    {prefs::kWebKitCursiveFontFamily, IDS_CURSIVE_FONT_FAMILY},
    {prefs::kWebKitFantasyFontFamily, IDS_FANTASY_FONT_FAMILY},
    {prefs::kWebKitPictographFontFamily, IDS_PICTOGRAPH_FONT_FAMILY},
#if defined(OS_CHROMEOS) || defined(OS_MACOSX) || defined(OS_WIN)
    {prefs::kWebKitStandardFontFamilyJapanese,
     IDS_STANDARD_FONT_FAMILY_JAPANESE},
    {prefs::kWebKitFixedFontFamilyJapanese, IDS_FIXED_FONT_FAMILY_JAPANESE},
    {prefs::kWebKitSerifFontFamilyJapanese, IDS_SERIF_FONT_FAMILY_JAPANESE},
    {prefs::kWebKitSansSerifFontFamilyJapanese,
     IDS_SANS_SERIF_FONT_FAMILY_JAPANESE},
    {prefs::kWebKitStandardFontFamilyKorean, IDS_STANDARD_FONT_FAMILY_KOREAN},
    {prefs::kWebKitSerifFontFamilyKorean, IDS_SERIF_FONT_FAMILY_KOREAN},
    {prefs::kWebKitSansSerifFontFamilyKorean,
     IDS_SANS_SERIF_FONT_FAMILY_KOREAN},
    {prefs::kWebKitStandardFontFamilySimplifiedHan,
     IDS_STANDARD_FONT_FAMILY_SIMPLIFIED_HAN},
    {prefs::kWebKitSerifFontFamilySimplifiedHan,
     IDS_SERIF_FONT_FAMILY_SIMPLIFIED_HAN},
    {prefs::kWebKitSansSerifFontFamilySimplifiedHan,
     IDS_SANS_SERIF_FONT_FAMILY_SIMPLIFIED_HAN},
    {prefs::kWebKitStandardFontFamilyTraditionalHan,
     IDS_STANDARD_FONT_FAMILY_TRADITIONAL_HAN},
    {prefs::kWebKitSerifFontFamilyTraditionalHan,
     IDS_SERIF_FONT_FAMILY_TRADITIONAL_HAN},
    {prefs::kWebKitSansSerifFontFamilyTraditionalHan,
     IDS_SANS_SERIF_FONT_FAMILY_TRADITIONAL_HAN},
#endif
#if defined(OS_MACOSX) || defined(OS_WIN)
    {prefs::kWebKitCursiveFontFamilySimplifiedHan,
     IDS_CURSIVE_FONT_FAMILY_SIMPLIFIED_HAN},
    {prefs::kWebKitCursiveFontFamilyTraditionalHan,
     IDS_CURSIVE_FONT_FAMILY_TRADITIONAL_HAN},
#endif
#if defined(OS_CHROMEOS)
    {prefs::kWebKitStandardFontFamilyArabic, IDS_STANDARD_FONT_FAMILY_ARABIC},
    {prefs::kWebKitSerifFontFamilyArabic, IDS_SERIF_FONT_FAMILY_ARABIC},
    {prefs::kWebKitSansSerifFontFamilyArabic,
     IDS_SANS_SERIF_FONT_FAMILY_ARABIC},
    {prefs::kWebKitFixedFontFamilyKorean, IDS_FIXED_FONT_FAMILY_KOREAN},
    {prefs::kWebKitFixedFontFamilySimplifiedHan,
     IDS_FIXED_FONT_FAMILY_SIMPLIFIED_HAN},
    {prefs::kWebKitFixedFontFamilyTraditionalHan,
     IDS_FIXED_FONT_FAMILY_TRADITIONAL_HAN},
#elif defined(OS_WIN)
    {prefs::kWebKitFixedFontFamilyArabic, IDS_FIXED_FONT_FAMILY_ARABIC},
    {prefs::kWebKitSansSerifFontFamilyArabic,
     IDS_SANS_SERIF_FONT_FAMILY_ARABIC},
    {prefs::kWebKitStandardFontFamilyCyrillic,
     IDS_STANDARD_FONT_FAMILY_CYRILLIC},
    {prefs::kWebKitFixedFontFamilyCyrillic, IDS_FIXED_FONT_FAMILY_CYRILLIC},
    {prefs::kWebKitSerifFontFamilyCyrillic, IDS_SERIF_FONT_FAMILY_CYRILLIC},
    {prefs::kWebKitSansSerifFontFamilyCyrillic,
     IDS_SANS_SERIF_FONT_FAMILY_CYRILLIC},
    {prefs::kWebKitStandardFontFamilyGreek, IDS_STANDARD_FONT_FAMILY_GREEK},
    {prefs::kWebKitFixedFontFamilyGreek, IDS_FIXED_FONT_FAMILY_GREEK},
    {prefs::kWebKitSerifFontFamilyGreek, IDS_SERIF_FONT_FAMILY_GREEK},
    {prefs::kWebKitSansSerifFontFamilyGreek, IDS_SANS_SERIF_FONT_FAMILY_GREEK},
    {prefs::kWebKitFixedFontFamilyKorean, IDS_FIXED_FONT_FAMILY_KOREAN},
    {prefs::kWebKitCursiveFontFamilyKorean, IDS_CURSIVE_FONT_FAMILY_KOREAN},
    {prefs::kWebKitFixedFontFamilySimplifiedHan,
     IDS_FIXED_FONT_FAMILY_SIMPLIFIED_HAN},
    {prefs::kWebKitFixedFontFamilyTraditionalHan,
     IDS_FIXED_FONT_FAMILY_TRADITIONAL_HAN},
#endif
};
const size_t kFontDefaultsLength = base::size(kFontDefaults);

// ^^^^^ DO NOT EDIT ^^^^^

std::string GetDefaultFontForPref(const char* pref_name) {
  for (size_t i = 0; i < kFontDefaultsLength; ++i) {
    FontDefault pref = kFontDefaults[i];
    if (strcmp(pref.pref_name, pref_name) == 0) {
      return l10n_util::GetStringUTF8(pref.resource_id);
    }
  }
  return std::string();
}

// Map from script to font.
// Key comparison uses pointer equality.
using ScriptFontMap = std::unordered_map<const char*, base::string16>;

// Map from font family to ScriptFontMap.
// Key comparison uses pointer equality.
using FontFamilyMap = std::unordered_map<const char*, ScriptFontMap>;

// A lookup table mapping (font-family, script) -> font-name
// e.g. ("sans-serif", "Zyyy") -> "Arial"
FontFamilyMap g_font_cache;

base::string16 FetchFont(const char* script, const char* map_name) {
  FontFamilyMap::const_iterator it = g_font_cache.find(map_name);
  if (it != g_font_cache.end()) {
    ScriptFontMap::const_iterator it2 = it->second.find(script);
    if (it2 != it->second.end())
      return it2->second;
  }

  std::string pref_name = base::StringPrintf("%s.%s", map_name, script);
  std::string font = GetDefaultFontForPref(pref_name.c_str());
  base::string16 font16 = base::UTF8ToUTF16(font);

  ScriptFontMap& map = g_font_cache[map_name];
  map[script] = font16;
  return font16;
}

void FillFontFamilyMap(const char* map_name,
                       content::ScriptFontFamilyMap* map) {
  for (size_t i = 0; i < prefs::kWebKitScriptsForFontFamilyMapsLength; ++i) {
    const char* script = prefs::kWebKitScriptsForFontFamilyMaps[i];
    base::string16 result = FetchFont(script, map_name);
    if (!result.empty()) {
      (*map)[script] = result;
    }
  }
}

}  // namespace

namespace atom {

void SetFontDefaults(content::WebPreferences* prefs) {
  FillFontFamilyMap(prefs::kWebKitStandardFontFamilyMap,
                    &prefs->standard_font_family_map);
  FillFontFamilyMap(prefs::kWebKitFixedFontFamilyMap,
                    &prefs->fixed_font_family_map);
  FillFontFamilyMap(prefs::kWebKitSerifFontFamilyMap,
                    &prefs->serif_font_family_map);
  FillFontFamilyMap(prefs::kWebKitSansSerifFontFamilyMap,
                    &prefs->sans_serif_font_family_map);
  FillFontFamilyMap(prefs::kWebKitCursiveFontFamilyMap,
                    &prefs->cursive_font_family_map);
  FillFontFamilyMap(prefs::kWebKitFantasyFontFamilyMap,
                    &prefs->fantasy_font_family_map);
  FillFontFamilyMap(prefs::kWebKitPictographFontFamilyMap,
                    &prefs->pictograph_font_family_map);
}

}  // namespace atom