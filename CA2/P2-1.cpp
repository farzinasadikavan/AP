#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const char IO_ELEMENT_SEPARATOR = ' ';

struct HeightInfo {
    int height;
    int tallest_height_in_front;
};

int find_tallest_heights_in_front(const vector<HeightInfo *>& height_infos, int start_index) {
    if(start_index == height_infos.size())
        return -1;

    int next_tallest_height = find_tallest_heights_in_front(height_infos, start_index + 1);
    height_infos[start_index]->tallest_height_in_front = next_tallest_height;
    return max(next_tallest_height, height_infos[start_index]->height);
}

void set_tallest_heights_in_front(const vector<HeightInfo *>& height_infos) {
    find_tallest_heights_in_front(height_infos, 0);
}

vector<HeightInfo* > get_height_infos() {
    string input_line;
    getline(cin, input_line);

    std::istringstream iss(input_line);
    vector<HeightInfo *> height_infos;
    int height;
    while(iss >> height) {
        HeightInfo *height_info = new HeightInfo({height, -1});
        height_infos.push_back(height_info);
    }
    return height_infos;
}

void print_heights(const vector<HeightInfo *>& height_infos) {
    for(int i = 0; i < height_infos.size(); i++) {
        if(i > 0)
            cout << IO_ELEMENT_SEPARATOR;
        cout << height_infos[i]->tallest_height_in_front;
    }
    cout << endl;
}

int main() {
    vector<HeightInfo *> height_infos = get_height_infos();
    set_tallest_heights_in_front(height_infos);
    print_heights(height_infos);
    return 0;
}
