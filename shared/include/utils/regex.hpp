#pragma once

#include <string>

#include <utils/fast_pimpl.hpp>

namespace utils {

/// Small alias for boost::regex / std::regex without huge includes
class regex final {
 public:
  explicit regex(std::string_view pattern);

  ~regex();

 private:
  struct Impl;
  utils::FastPimpl<Impl, 16, 8> impl_;

  friend bool regex_match(std::string_view str, const regex& pattern);
};

/// Determines whether the regular expression matches the entire target
/// character sequence
bool regex_match(std::string_view str, const regex& pattern);

}  // namespace utils