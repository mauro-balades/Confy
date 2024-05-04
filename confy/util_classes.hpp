
/**
 * DO NOT INCLUDE THIS FILE DIRECTLY!
 *
 * This is the implementation file for the confy library.
 * If you want to use the library, include the confy.hpp file.
 *
 */

template <int N1>
class MinNumType : public NumType {
public:
  static const int value = N1;

  std::optional<std::string> validate(double value) const override {
    if (value < N1) {
      return "Value must be greater than or equal to " + std::to_string(N1);
    }
    return std::nullopt;
  }

  static std::shared_ptr<NumType> create() {
    return std::make_shared<MinNumType<N1>>();
  }
};

template <int N1>
class MaxNumType : public NumType {
public:
  static const int value = N1;

  std::optional<std::string> validate(double value) const override {
    if (value > N1) {
      return "Value must be less than or equal to " + std::to_string(N1);
    }
    return std::nullopt;
  }

  static std::shared_ptr<NumType> create() {
    return std::make_shared<MaxNumType<N1>>();
  }
};

template <int N1, int N2>
class RangeNumType : public NumType {
public:
  static const int min = N1;
  static const int max = N2;

  std::optional<std::string> validate(double value) const override {
    if (value < N1 || value > N2) {
      return "Value must be between " + std::to_string(N1) + " and " + std::to_string(N2);
    }
    return std::nullopt;
  }

  static std::shared_ptr<NumType> create() {
    return std::make_shared<RangeNumType<N1, N2>>();
  }
};

template <int S1>
class MinStrType : public StringType {
public:
  static const int value = S1;

  std::optional<std::string> validate(const std::string& value) const override {
    if (value.size() < S1) {
      return "String must be at least " + std::to_string(S1) + " characters long";
    }
    return std::nullopt;
  }

  static std::shared_ptr<StringType> create() {
    return std::make_shared<MinStrType<S1>>();
  }
};

class StrRegexType : public StringType {
  std::string regex;
public:
  StrRegexType(const std::string& regex) : regex(regex) {}

  std::optional<std::string> validate(const std::string& value) const override {
    if (!std::regex_match(value, std::regex(regex))) {
      return "String must match the regex: " + regex;
    }
    return std::nullopt;
  }

  static std::shared_ptr<StringType> create(const std::string& regex) {
    return std::make_shared<StrRegexType>(regex);
  }
};

