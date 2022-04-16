#include <iostream>
#include <string>
#include <vector>

using namespace std;

const char IO_LINE_SEPARATOR = '\n';
const char IO_ELEMENT_SEPARATOR = ' ';
const int NOT_IN_PATH_VALUE = 0;
const string POSITIVE_OUTPUT = "Yes";
const string NEGATIVE_OUTPUT = "No";
const vector<int> MOVE_DELTA = {0, 1, -1};

typedef vector<int> PathRow;
typedef vector<PathRow> PathTable;
typedef vector<char> WordRow;
typedef vector<WordRow> WordTable;

WordRow read_word_row(int table_size);
WordTable read_word_table(int table_size);
PathTable find_path_table(const string& word, const WordTable& word_table);
PathTable find_path_table(const string& word, const WordTable& word_table, int row, int column);
PathTable find_path_table(const string& word, const WordTable& word_table, PathTable path_table, 
                          int row, int column, int word_index);
PathTable find_path_table_in_directions(const string& word, const WordTable& word_table, 
                                        PathTable path_table, int row, int column, int word_index);
PathTable get_empty_path_table(int table_size);
bool is_in_range(const WordTable& word_table, int row, int column);
bool is_in_valid_cell(const string& word, const WordTable& word_table, const PathTable& path_table,
                      int row, int column, int word_index);
void print_result(const PathTable& path_table);
void print_path_table(const PathTable& path_table);
void print_path_row_element(const PathRow& path_row, int element_index);

int main() {
    string word;
    cin >> word;

    int table_size;
    cin >> table_size;

    WordTable word_table = read_word_table(table_size);

    PathTable path_table = find_path_table(word, word_table);
    print_result(path_table);

    return 0;
}

WordTable read_word_table(int table_size) {
    WordTable word_table;
    for(int i = 0; i < table_size; i++) {
        WordRow word_row = read_word_row(table_size);
        word_table.push_back(word_row);
    }
    return word_table;
}

WordRow read_word_row(int table_size) {
    WordRow word_row;
    for(int i = 0; i < table_size; i++) {
        char character;
        cin >> character;
        word_row.push_back(character);
    }
    return word_row;
}

PathTable find_path_table(const string& word, const WordTable& word_table) {
    for(int i = 0; i < word_table.size(); i++) {
        for(int j = 0; j < word_table.size(); j++) {
            PathTable path_table = find_path_table(word, word_table, i, j);
            if(path_table.size() != 0)
                return path_table;
        }
    }
    return PathTable();
}

PathTable find_path_table(const string& word, const WordTable& word_table, int row, int column) {
    PathTable empty_path_table = get_empty_path_table(word_table.size());
    PathTable result_path_table = find_path_table(word, word_table, empty_path_table, row, column, 0);
    return result_path_table;
}

PathTable find_path_table(const string& word, const WordTable& word_table, PathTable path_table, 
                          int row, int column, int word_index) {
    if(!is_in_valid_cell(word, word_table, path_table, row, column, word_index))
        return PathTable();

    path_table[row][column] = word_index + 1;

    if(word_index == word.size() - 1)
        return path_table;

    PathTable next_path_table = find_path_table_in_directions(word, word_table, path_table,
                                                              row, column, word_index);
    if(next_path_table.size() != 0)
        return next_path_table;
     
    return PathTable();
}

PathTable find_path_table_in_directions(const string& word, const WordTable& word_table,
                                        PathTable path_table, int row, int column, int word_index) {
    for(int i = 0; i < MOVE_DELTA.size(); i++) {
        for(int j = 0; j < MOVE_DELTA.size(); j++) {
            if(MOVE_DELTA[i] == 0 && MOVE_DELTA[j] == 0)
                continue;
            
            PathTable next_path_table = find_path_table(word, word_table, path_table, 
                                                        row + MOVE_DELTA[i], column + MOVE_DELTA[j], 
                                                        word_index + 1);
            if(next_path_table.size() != 0)
                return next_path_table;
        }
    }
    return PathTable();
}

PathTable get_empty_path_table(int table_size) {
    return PathTable(table_size, PathRow(table_size, NOT_IN_PATH_VALUE));
}

bool is_in_valid_cell(const string& word, const WordTable& word_table, const PathTable& path_table,
                      int row, int column, int word_index) {
    return (is_in_range(word_table, row, column) &&
            path_table[row][column] == NOT_IN_PATH_VALUE &&
            word_table[row][column] == word[word_index]);
}

bool is_in_range(const WordTable& word_table, int row, int column) {
    int table_size = word_table.size();
    return (row >= 0 && column >= 0 && row < table_size && column < table_size);
}

void print_result(const PathTable& path_table) {
    if(path_table.size() == 0)
        cout << NEGATIVE_OUTPUT << IO_LINE_SEPARATOR;
    else {
        cout << POSITIVE_OUTPUT << IO_LINE_SEPARATOR;
        print_path_table(path_table);
    }
}

void print_path_table(const PathTable& path_table) {
    for(const PathRow& path_row : path_table)
        for(int i = 0; i < path_row.size(); i++)
            print_path_row_element(path_row, i);
}

void print_path_row_element(const PathRow& path_row, int element_index) {
    if(element_index > 0)
        cout << IO_ELEMENT_SEPARATOR;
    
    cout << path_row[element_index];

    if(element_index == path_row.size() - 1)
        cout << IO_LINE_SEPARATOR;
}
