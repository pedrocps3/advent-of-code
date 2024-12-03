#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

std::vector<std::vector<int>> get_lists_of_reports(std::string path) {
  std::ifstream file(path);
  if (!file) {
    throw std::runtime_error("error to load input");
  }

  std::vector<std::vector<int>> reports_lists{};

  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::vector<int> reports{};

    while (ss && !ss.eof()) {
      int report_number{};
      ss >> report_number;

      reports.push_back(report_number);
    }

    reports_lists.push_back(reports);
  }

  return reports_lists;
}

bool validate_safe_reports(const std::vector<int> &reports_list) {
  const int MAX_DIFFER = 3;
  const int MIN_DIFFER = 1;

  bool increasing = reports_list[0] < reports_list[1];

  for (size_t i = 0; i < reports_list.size() - 1; i++) {
    int differ = reports_list[i] - reports_list[i + 1];
    int abs_differ = std::abs(differ);
    bool is_increase = reports_list[i] < reports_list[i + 1];

    if (abs_differ < MIN_DIFFER || abs_differ > MAX_DIFFER ||
        is_increase != increasing)
      return false;
  }
  return true;
}

bool validate_safe_reports_with_problem_dampener(
    const std::vector<int> &reports_list) {
  bool is_safe = validate_safe_reports(reports_list);

  if (is_safe)
    return true;

  for (size_t i = 0; i < reports_list.size(); ++i) {
    std::vector<int> temp_list = reports_list;
    temp_list.erase(temp_list.begin() + i);

    if (validate_safe_reports(temp_list))
      return true;
  }

  return false;
}

void solve_part_one(const std::vector<std::vector<int>> &reports_lists) {
  int safe_reports{0};

  for (auto const &reports_list : reports_lists) {
    if (validate_safe_reports(reports_list))
      safe_reports++;
  }

  std::cout << "Part one: \n" << std::endl;
  std::cout << "safe reports: " << safe_reports << std::endl;
}

void solve_part_two(const std::vector<std::vector<int>> &reports_lists) {
  int safe_reports{0};

  for (auto const &reports_list : reports_lists)
    if (validate_safe_reports_with_problem_dampener(reports_list))
      safe_reports++;

  std::cout << "Part two: \n" << std::endl;
  std::cout << "safe reports: " << safe_reports << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "error: no input as found in parameters";

    return 1;
  }

  std::string path = argv[1];

  auto reports_lists = get_lists_of_reports(path);

  solve_part_one(reports_lists);

  std::cout << std::endl;

  solve_part_two(reports_lists);

  return 0;
}
