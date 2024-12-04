#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>

std::string get_corrupted_memory_section(std::string path) {
  std::string corrupted_memory_section{};
  std::ifstream file(path);
  if (!file)
    throw std::runtime_error("error to load input");

  std::string line;

  while (std::getline(file, line))
    corrupted_memory_section.append(line);

  return corrupted_memory_section;
}

std::vector<std::string>
get_mul_occurrences(const std::string &corrupted_memory_section) {
  std::regex pattern("mul\\(\\d+,\\d+\\)");

  auto words_begin =
      std::sregex_iterator(corrupted_memory_section.begin(),
                           corrupted_memory_section.end(), pattern);

  auto words_end = std::sregex_iterator();
  std::cout << "found: " << std::distance(words_begin, words_end) << " muls:\n";

  std::vector<std::string> muls_list{};

  for (auto i = words_begin; i != words_end; ++i) {
    std::smatch match = *i;
    std::string match_str = match.str();

    muls_list.push_back(match_str);
  }

  return muls_list;
}

std::vector<std::string>
get_instructions_occurrences(const std::string &corrupted_memory_section) {
  std::regex pattern("mul\\(\\d+,\\d+\\)|do\\(\\)|don\\'t\\(\\)");
  const std::string DONT_INTRUCTION{"don't()"};
  const std::string DO_INSTRUCTION{"do()"};
  bool valid_instruction{true};

  auto words_begin =
      std::sregex_iterator(corrupted_memory_section.begin(),
                           corrupted_memory_section.end(), pattern);

  auto words_end = std::sregex_iterator();
  std::cout << "found: " << std::distance(words_begin, words_end)
            << " occurrences:\n";

  std::vector<std::string> muls_list{};

  for (auto i = words_begin; i != words_end; ++i) {
    std::smatch match = *i;
    std::string match_str = match.str();

    if (match_str.compare(DONT_INTRUCTION) == 0) {
      valid_instruction = false;
      continue;
    }
    if (match_str.compare(DO_INSTRUCTION) == 0) {
      valid_instruction = true;
      continue;
    }

    if (valid_instruction)
      muls_list.push_back(match_str);
  }

  return muls_list;
}

std::vector<std::tuple<int, int>>
get_muls_to_calculate(const std::vector<std::string> &muls_list) {
  std::regex pattern("\\d+");

  std::vector<std::tuple<int, int>> mults_to_calculate{};

  for (const std::string &mul : muls_list) {
    auto begin = std::sregex_iterator(mul.begin(), mul.end(), pattern);
    auto end = std::sregex_iterator();

    int first_operand{0};
    int second_operand{0};

    bool first_occurrency = true;

    for (auto i = begin; i != end; ++i) {
      std::smatch match = *i;

      if (first_occurrency) {
        first_operand = std::stoi(match.str());
        first_occurrency = false;

        continue;
      }

      second_operand = std::stoi(match.str());
    }

    mults_to_calculate.push_back(
        std::make_tuple(first_operand, second_operand));
  }

  return mults_to_calculate;
}

void calculate_occurrences(
    const std::vector<std::tuple<int, int>> &mults_to_calculate) {
  int total{0};

  for (const std::tuple<int, int> &mult : mults_to_calculate) {
    auto [first_operand, second_operand] = mult;

    total += first_operand * second_operand;
  }

  std::cout << "result of multiplications is: " << total << '\n' << std::endl;
}

int main(int argv, char *argc[]) {
  if (argv < 2)
    throw std::runtime_error("error to load input");

  std::string path = argc[1];
  std::string corrupted_memory_section{get_corrupted_memory_section(path)};

  std::vector<std::string> muls_list{
      get_mul_occurrences(corrupted_memory_section)};

  std::vector<std::tuple<int, int>> muls_to_calculate{
      get_muls_to_calculate(muls_list)};

  std::cout << "PART ONE: \n";
  calculate_occurrences(muls_to_calculate);

  std::vector<std::string> instructions_list{
      get_instructions_occurrences(corrupted_memory_section)};

  std::vector<std::tuple<int, int>>
      muls_to_calculate_without_other_instructions{
          get_muls_to_calculate(instructions_list)};

  std::cout << "PART TWO: \n";
  calculate_occurrences(muls_to_calculate_without_other_instructions);

  return 0;
}
