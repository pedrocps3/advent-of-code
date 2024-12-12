#include <algorithm>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

enum Direction { Right, Left, Up, Down, UpRight, UpLeft, DownRight, DownLeft };

std::vector<std::vector<char>> get_two_d_vector(std::string &path) {
  std::ifstream file(path);
  if (!file)
    throw new std::runtime_error("error: error to load input.");

  std::vector<std::vector<char>> two_d_vector{};

  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::vector<char> char_vec{};

    char letter{};
    while (ss >> letter)
      char_vec.push_back(letter);

    two_d_vector.push_back(char_vec);
  }

  return two_d_vector;
}

bool word_is(const std::string &word, const std::string &word_to_compare) {
  return word.compare(word_to_compare) == 0;
}

bool search_word(size_t x_axis_pos, size_t y_axis_pos,
                 const std::vector<std::vector<char>> &two_d_vec,
                 int num_letters, Direction direction,
                 const std::string &word_to_compare) {
  std::string word{};

  switch (direction) {
  case Right: {
    for (size_t i = 0; i < num_letters; i++)
      word.push_back(two_d_vec.at(y_axis_pos).at(x_axis_pos + i));

    break;
  }
  case Left: {
    for (size_t i = 0; i < num_letters; i++)
      word.push_back(two_d_vec.at(y_axis_pos).at(x_axis_pos - i));

    break;
  }
  case Up: {
    for (size_t i = 0; i < num_letters; i++)
      word.push_back(two_d_vec.at(y_axis_pos - i).at(x_axis_pos));

    break;
  }
  case Down: {
    for (size_t i = 0; i < num_letters; i++)
      word.push_back(two_d_vec.at(y_axis_pos + i).at(x_axis_pos));

    break;
  }
  case UpRight: {
    for (size_t i = 0; i < num_letters; i++)
      word.push_back(two_d_vec.at(y_axis_pos - i).at(x_axis_pos + i));

    break;
  }
  case UpLeft: {
    for (size_t i = 0; i < num_letters; i++)
      word.push_back(two_d_vec.at(y_axis_pos - i).at(x_axis_pos - i));

    break;
  }
  case DownRight: {
    for (size_t i = 0; i < num_letters; i++)
      word.push_back(two_d_vec.at(y_axis_pos + i).at(x_axis_pos + i));

    break;
  }
  case DownLeft: {
    for (size_t i = 0; i < num_letters; i++)
      word.push_back(two_d_vec.at(y_axis_pos + i).at(x_axis_pos - i));

    break;
  }
  default:
    break;
  }

  std::string reverse_word{word_to_compare};
  std::reverse(reverse_word.begin(), reverse_word.end());

  return word_is(word, word_to_compare) || word_is(word, reverse_word);
}

bool search_word_in_x(size_t x_axis_pos, size_t y_axis_pos,
                      const std::vector<std::vector<char>> &two_d_vec,
                      int num_letters, const std::string &word_to_compare) {
  char initial_char{two_d_vec.at(y_axis_pos).at(x_axis_pos)};
  std::string left_diagonal_word{initial_char};
  std::string right_diagonal_word{initial_char};
  const int NUMBER_OF_CHARACTERS{1};

  for (size_t i = 1; i <= num_letters; i++) {
    left_diagonal_word.append(
        std::string(NUMBER_OF_CHARACTERS,
                    (two_d_vec.at(y_axis_pos - i).at(x_axis_pos + i))));

    left_diagonal_word.insert(
        0, std::string(NUMBER_OF_CHARACTERS,
                       (two_d_vec.at(y_axis_pos + i).at(x_axis_pos - i))));
  }

  for (size_t i = 1; i <= num_letters; i++) {
    right_diagonal_word.insert(
        0, std::string(NUMBER_OF_CHARACTERS,
                       (two_d_vec.at(y_axis_pos - i).at(x_axis_pos - i))));

    right_diagonal_word.append(
        std::string(NUMBER_OF_CHARACTERS,
                    (two_d_vec.at(y_axis_pos + i).at(x_axis_pos + i))));
  }

  std::string reverse_word{word_to_compare};
  std::reverse(reverse_word.begin(), reverse_word.end());

  bool is_left_diagonal_valid = word_is(left_diagonal_word, word_to_compare) ||
                                word_is(left_diagonal_word, reverse_word);
  bool is_right_diagonal_valid =
      word_is(right_diagonal_word, word_to_compare) ||
      word_is(right_diagonal_word, reverse_word);

  return (is_left_diagonal_valid && is_right_diagonal_valid);
}

int get_number_of_x_max_occurrences(
    const std::vector<std::vector<char>> &two_d_vec) {
  const int MISSING_LETTERS_NUM{1};
  int ocorrs{0};

  for (size_t y = 0; y < two_d_vec.size(); ++y) {
    for (size_t x = 0; x < two_d_vec.at(y).size(); ++x) {
      if (two_d_vec.at(y).at(x) != 'A')
        continue;

      bool can_go_right = (x + MISSING_LETTERS_NUM < two_d_vec.at(y).size());
      bool can_go_left = (x >= MISSING_LETTERS_NUM);
      bool can_go_down = (y + MISSING_LETTERS_NUM < two_d_vec.size());
      bool can_go_up = (y >= MISSING_LETTERS_NUM);

      bool can_go_downright = can_go_right && can_go_down;
      bool can_go_downleft = can_go_left && can_go_down;
      bool can_go_upright = can_go_right && can_go_up;
      bool can_go_upleft = can_go_left && can_go_up;

      if (can_go_downright && can_go_downleft && can_go_upright &&
          can_go_upleft)
        ocorrs += search_word_in_x(x, y, two_d_vec, MISSING_LETTERS_NUM, "MAS");
    }
  }

  return ocorrs;
}

int get_number_of_xmas_occurrences(
    const std::vector<std::vector<char>> &two_d_vec) {
  const int MISSING_LETTERS_NUM{3};
  const std::string WORD_TO_COMPARE{"XMAS"};
  int ocorrs{0};

  for (size_t y = 0; y < two_d_vec.size(); ++y) {
    for (size_t x = 0; x < two_d_vec.at(y).size(); ++x) {
      if (two_d_vec.at(y).at(x) != 'X')
        continue;

      bool can_go_right = (x + MISSING_LETTERS_NUM < two_d_vec.at(y).size());
      bool can_go_left = (x >= MISSING_LETTERS_NUM);
      bool can_go_down = (y + MISSING_LETTERS_NUM < two_d_vec.size());
      bool can_go_up = (y >= MISSING_LETTERS_NUM);

      bool can_go_downright = can_go_right && can_go_down;
      bool can_go_downleft = can_go_left && can_go_down;
      bool can_go_upright = can_go_right && can_go_up;
      bool can_go_upleft = can_go_left && can_go_up;

      if (can_go_right)
        ocorrs += search_word(x, y, two_d_vec, MISSING_LETTERS_NUM + 1,
                              Direction::Right, WORD_TO_COMPARE);

      if (can_go_left)
        ocorrs += search_word(x, y, two_d_vec, MISSING_LETTERS_NUM + 1,
                              Direction::Left, WORD_TO_COMPARE);

      if (can_go_down)
        ocorrs += search_word(x, y, two_d_vec, MISSING_LETTERS_NUM + 1,
                              Direction::Down, WORD_TO_COMPARE);

      if (can_go_up)
        ocorrs += search_word(x, y, two_d_vec, MISSING_LETTERS_NUM + 1,
                              Direction::Up, WORD_TO_COMPARE);

      if (can_go_downright)
        ocorrs += search_word(x, y, two_d_vec, MISSING_LETTERS_NUM + 1,
                              Direction::DownRight, WORD_TO_COMPARE);

      if (can_go_downleft)
        ocorrs += search_word(x, y, two_d_vec, MISSING_LETTERS_NUM + 1,
                              Direction::DownLeft, WORD_TO_COMPARE);

      if (can_go_upright)
        ocorrs += search_word(x, y, two_d_vec, MISSING_LETTERS_NUM + 1,
                              Direction::UpRight, WORD_TO_COMPARE);

      if (can_go_upleft)
        ocorrs += search_word(x, y, two_d_vec, MISSING_LETTERS_NUM + 1,
                              Direction::UpLeft, WORD_TO_COMPARE);
    }
  }

  return ocorrs;
}

void solve_part_one(const std::vector<std::vector<char>> &two_d_vec) {
  std::cout << "part one: \n" << std::endl;

  int ocorrs{get_number_of_xmas_occurrences(two_d_vec)};

  std::cout << "total ocorrs: " << ocorrs << std::endl;
}

void solve_part_two(const std::vector<std::vector<char>> &two_d_vec) {
  std::cout << "part two: \n" << std::endl;

  int ocorrs{get_number_of_x_max_occurrences(two_d_vec)};

  std::cout << "total ocorrs: " << ocorrs << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    throw new std::runtime_error("error: no input as found in parameters.");

  std::string path = std::string{argv[1]};

  std::vector<std::vector<char>> two_d_vector{get_two_d_vector(path)};

  solve_part_one(two_d_vector);

  std::cout << '\n' << std::endl;

  solve_part_two(two_d_vector);

  return 0;
}
