#include <iostream>
#include <string>

using namespace std;

string encode_word(const string& word);
string encode_word(const string& word, int start_index, int end_index);
string encode_valid_word(const string& word, int start_index, int end_index);

int main() {
    string word;
    cin >> word;

    cout << encode_word(word) << endl;

    return 0;
}

string encode_word(const string& word) {
    return encode_word(word, 0, word.size() - 1);
}

string encode_word(const string& word, int start_index, int end_index) {
    if(start_index > end_index)
        return "";
    else
        return encode_valid_word(word, start_index, end_index);
}

string encode_valid_word(const string& word, int start_index, int end_index) {
    int middle_index = (start_index + end_index) / 2;
    char middle_character = word[middle_index];

    string left_encoded = encode_word(word, start_index, middle_index - 1);
    string right_encoded = encode_word(word, middle_index + 1, end_index);

    return (middle_character + left_encoded + right_encoded);
}
