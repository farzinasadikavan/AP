#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const char IO_ELEMENT_SEPARATOR = ' ';

vector<int> get_tallest_heights_in_front(const vector<int>& heights, int start_index) {
    if(start_index == heights.size() - 1)
        return vector<int>(1, -1);
    
    vector<int> result;
    vector<int> next_result = get_tallest_heights_in_front(heights, start_index + 1);
    result.push_back(max(next_result[0], heights[start_index + 1]));
    result.insert(result.end(), next_result.begin(), next_result.end());
    return result;
}

vector<int> get_tallest_heights_in_front(const vector<int>& heights) {
    return get_tallest_heights_in_front(heights, 0);
}

vector<int> get_heights() {
    string input_line;
    getline(cin, input_line);

    std::istringstream iss(input_line);
    vector<int> heights;
    int height;
    while(iss >> height) {
        heights.push_back(height);
    }
    return heights;
}

void print_heights(const vector<int>& heights) {
    for(int i = 0; i < heights.size(); i++) {
        if(i > 0)
            cout << IO_ELEMENT_SEPARATOR;
        cout << heights[i];
    }
    cout << endl;
}

int main() {
    vector<int> heights = get_heights();
    vector<int> tallest_heights_in_front = get_tallest_heights_in_front(heights);
    print_heights(tallest_heights_in_front);
    return 0;
}
