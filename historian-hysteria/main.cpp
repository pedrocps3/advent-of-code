#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <vector>

void merge(std::vector<int> &arr, size_t left, size_t middle, size_t rigth) {
  size_t n1 = middle - left + 1;
  size_t n2 = rigth - middle;

  std::vector<int> left_arr(n1);
  std::vector<int> rigth_arr(n2);

  for (size_t i = 0; i < n1; i++)
    left_arr[i] = arr[left + i];

  for (size_t i = 0; i < n2; i++)
    rigth_arr[i] = arr[middle + 1 + i];

  size_t i = 0, j = 0, k = left;

  while (i < n1 && j < n2) {
    if (left_arr[i] <= rigth_arr[j]) {
      arr[k] = left_arr[i];
      i++;
    } else {
      arr[k] = rigth_arr[j];
      j++;
    }

    k++;
  }

  while (i < n1) {
    arr[k] = left_arr[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = rigth_arr[j];
    j++;
    k++;
  }
}

void merge_sort(std::vector<int> &arr, size_t left, size_t right) {
  if (left < right) {
    size_t middle = (left + right) / 2;

    merge_sort(arr, left, middle);
    merge_sort(arr, middle + 1, right);

    merge(arr, left, middle, right);
  }
};

void display_lists(std::vector<int> const &first_list,
                   std::vector<int> const &second_list) {
  size_t length = first_list.size() - 1;

  for (size_t i = 0; i <= length; i++)
    std::cout << first_list[i] << " " << "|" << " " << second_list[i]
              << std::endl;

  std::cout << std::endl;
}

std::tuple<std::vector<int>, std::vector<int>>
get_location_ids_lists(std::string path) {
  std::ifstream file(path);
  if (!file) {
    throw std::runtime_error("error to load input");
  }

  std::vector<int> first_list{};
  std::vector<int> second_list{};

  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    int first_number{0};
    int second_number{0};
    ss >> first_number >> second_number;

    first_list.push_back(first_number);
    second_list.push_back(second_number);
  }

  return std::make_tuple(first_list, second_list);
};

int calculate_distance(std::vector<int> const &first_list,
                       std::vector<int> const &second_list) {
  int total{0};

  size_t size_of_lists = first_list.size() - 1;

  for (size_t i = 0; i <= size_of_lists; i++) {
    int abs{std::abs(first_list[i] - second_list[i])};

    total += abs;
  }

  return total;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "error: no input as found in parameters";

    return 1;
  }

  std::string path = argv[1];

  auto [first_list_os_ids, second_list_of_ids] = get_location_ids_lists(path);

  display_lists(first_list_os_ids, second_list_of_ids);

  std::cout << first_list_os_ids.size() << " " << second_list_of_ids.size()
            << std::endl;

  merge_sort(first_list_os_ids, 0, first_list_os_ids.size() - 1);
  merge_sort(second_list_of_ids, 0, second_list_of_ids.size() - 1);

  std::cout << "total distance is: "
            << calculate_distance(first_list_os_ids, second_list_of_ids)
            << std::endl;

  return 0;
}
