#include <benchmark/benchmark.h>

#include <formats/json/parser/parser.hpp>
#include <formats/json/serialize.hpp>
#include <formats/parse/common_containers.hpp>

namespace {

std::string Build(size_t len) {
  std::string r = "[";
  for (size_t i = 0; i < len; i++) {
    if (i > 0) r += ',';
    r += '1';
  }
  r += ']';
  return r;
}

std::string BuildArray(size_t len) {
  std::string r = "[";
  for (size_t i = 0; i < len; i++) {
    if (i > 0) r += ',';
    r += Build(len);
  }
  r += ']';
  return r;
}

auto ParseDom(const formats::json::Value& value) {
  return value.As<std::vector<std::vector<int64_t>>>();
}

}  // namespace

void JsonParseArrayDom(benchmark::State& state) {
  const auto input = BuildArray(state.range(0));
  for (auto _ : state) {
    auto json = formats::json::FromString(input);
    const auto res = ParseDom(json);
    benchmark::DoNotOptimize(res);
  }
}
BENCHMARK(JsonParseArrayDom)->RangeMultiplier(4)->Range(1, 1024);

void JsonParseArraySax(benchmark::State& state) {
  const auto input = BuildArray(state.range(0));
  for (auto _ : state) {
    std::vector<std::vector<int64_t>> result{};
    using IntParser = formats::json::parser::IntParser;
    IntParser int_parser;
    using ArrayParser = formats::json::parser::ArrayParser<int64_t, IntParser>;

    ArrayParser array_parser(int_parser);
    formats::json::parser::ArrayParser<std::vector<int64_t>, ArrayParser>
        parser(array_parser);
    parser.Reset(result);

    formats::json::parser::ParserState state;
    state.PushParserNoKey(parser);
    state.ProcessInput(input);
  }
}
BENCHMARK(JsonParseArraySax)->RangeMultiplier(4)->Range(1, 1024);
