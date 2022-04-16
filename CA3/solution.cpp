#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>
#include <map>

const int INVALID = -1;
const char CSV_DELIMITER = ',';
const char TIME_DELIMITER = ':';
const char VERTICAL_SEPARATOR = '|';
const char HORIZENTAL_SEPERATOR = '-';
const char BOX_CORNER = '+';

const std::string CINEMA = "CinemaName";
const std::string MOVIE = "MovieName";
const std::string DAY = "Day";
const std::string START = "StartingTime";
const std::string FINISH = "FinishingTime";
const std::string PRICE = "Price";
const std::string ROOM =  "Hall";

const std::string COMMAND_GET_ALL = "GET ALL MOVIES";
const std::string COMMAND_GET_SCHEDULE = "GET SCHEDULE";
const std::string COMMAND_GET_HTML = "GET HTML";
const int LEN_COMMAND_GET_SCHEDULE = COMMAND_GET_SCHEDULE.size();
const int LEN_COMMAND_GET_HTML = COMMAND_GET_HTML.size();

const int TABLE_LINES = 16;
const int ROW_LENGTH = 175;
const int UNIT_TIME_LENGTH = 5;
const int UNIT_MINUTE = 30;
const int EARLIEST_HOUR = 8;
const std::string FIRST_ROW = "          08:00               10:00               12:00               14:00               16:00               18:00               20:00               22:00               00:00";

const int UNIT_TIME_IN_PX = 50;
const int LEFT_OFFSET = 100;
const int TOP_OFFSET = 60;
const int DAY_DIFF_PX = 50;

using namespace std;

const vector<string> day_names = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

struct Time{
    int h;
    int m;
};

struct Record{
    Time start;
    Time finish;
    string cinema;
    string movie;
    string day;
    int price;
    int room;
};

typedef map<string, vector<Record>> TimeSchedule;

vector<Record> get_records(char*);
void run_command(vector<Record>, string);
vector<Record> tokenize_table(vector<vector<string>>);
vector<vector<string>> read_csv(char*);
vector<string> split(string, char);
Record tokenize(vector<string>, vector<string> header);
Time get_time(string);
int find_index_in_vector(const vector<string>&, string);
string get_movie_name(string);

void run_get_all(const vector <Record>& table);
void run_get_schedule(vector <Record> table, string movie);
void run_get_html(vector <Record> table, string movie) {};


int main(int argc, char** argv){
    string command;
    vector<Record> table;
    vector<Record> result;
    table = get_records(argv[1]);
    while(getline(cin, command)){
        run_command(table, command);
    }
}

vector<string> sort_movies_by_name(vector<string> movies){
    string swap_var;
    for(int i = 0; i < movies.size(); i++){
        for(int j = 0 ; j < movies.size() - i - 1; j++){
            if(movies[j] > movies[j + 1]){
                swap_var = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = swap_var;
            }
        }
    }
    return movies;
}

vector<string> delete_repeated_movies(vector<string> movies){
    vector<string> movies_set;
    movies_set.push_back(movies[0]);
    for(int i = 1; i < movies.size(); i++)
        if(movies[i] != movies_set.back())
            movies_set.push_back(movies[i]);
    return movies_set;
}

void run_get_all(const vector<Record>& table){
    vector<string> movies;
    for (int i = 0 ; i < table.size() ; i++)
        movies.push_back(table[i].movie);
    
    movies = sort_movies_by_name(movies);
    movies = delete_repeated_movies(movies);
    for (auto it: movies)
        cout << it << endl;
}

void run_command(vector<Record> table, string command){
    if(command == COMMAND_GET_ALL){
        run_get_all(table);
    }else if(command.substr(0,LEN_COMMAND_GET_SCHEDULE) == COMMAND_GET_SCHEDULE){
        run_get_schedule(table, get_movie_name(command));
    }
}

string get_movie_name(string command){
    return command.substr(LEN_COMMAND_GET_SCHEDULE + 1);
}

vector<Record> get_records(char* file){
    vector<vector<string>> table;
    table = read_csv(file);
    return tokenize_table(table);
}

vector<vector<string>> read_csv(char* file){
    string line;
    fstream fin;
    fin.open(file, ios::in);
    vector<vector<string>> table;
    while(getline(fin, line)){
        table.push_back(split(line, CSV_DELIMITER));
    }
    return table;
}

vector<string> split(string line, char delimiter){
    stringstream stream(line);
    vector<string> out_list;
    string block;
    while(getline(stream, block, delimiter)){
        out_list.push_back(block);
    }
    return  out_list;
}

vector<Record> tokenize_table(vector<vector<string>> table){
    vector<Record> tokens;
    vector<string> header = table[0];
    for(int i = 1; i < table.size(); i++){
        tokens.push_back(tokenize(table[i], header));
    }
    return tokens;
}

Record tokenize(vector<string> raw,vector<string> header){
    Record record;
    record.cinema = raw[find_index_in_vector(header, CINEMA)];
    record.start = get_time(raw[find_index_in_vector(header, START)]);
    record.finish = get_time(raw[find_index_in_vector(header, FINISH)]);
    record.movie = raw[find_index_in_vector(header, MOVIE)];
    record.day = raw[find_index_in_vector(header, DAY)];
    record.price = stoi(raw[find_index_in_vector(header, PRICE)]);
    record.room = stoi(raw[find_index_in_vector(header, ROOM)]);
    return record;
}

int find_index_in_vector(const vector<string>& vec, string s){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] == s){
            return i;
        }
    }
    return INVALID;
}

Time get_time(string in_time){
    Time time;
    int col_pos = in_time.find(TIME_DELIMITER);
    time.h = stoi(in_time.substr(0, col_pos));
    time.m = stoi(in_time.substr(col_pos + 1, in_time.size() - col_pos));
    return time;
}

string convert_time_to_string(Time time){
    return to_string(time.h) + TIME_DELIMITER + to_string(time.m);
}

Time subtract(const Time& first_operand, const Time& second_operand){
    Time res;
    int diff_hour = first_operand.h - second_operand.h;
    int diff_minutes = first_operand.m - second_operand.m;
    if (diff_minutes < 0){
        diff_minutes = -diff_minutes;
        diff_hour -= 1;
    }
    res.h = diff_hour;
    res.m = diff_minutes;
    return res;
}

Time get_half_hour() {Time res; res.h = 0; res.m = UNIT_MINUTE; return res;};
Time get8AM() {Time res; res.h = EARLIEST_HOUR; res.m = 0; return res;}

int divide_by_half(const Time& time){
    return time.h * 2 + (time.m == UNIT_MINUTE);
}

bool less_than(const Time& operand1, const Time& operand2) {
    if (operand1.h == operand2.h)
        return operand1.m < operand2.m;
    return operand1.h < operand2.h;
}

bool eq(const Time& operand1, const Time& operand2){
    return (operand1.h == operand2.h && operand1.m == operand2.m);
}

int get_day_num_by_day_name(string day_name){
    auto it = find(day_names.begin(), day_names.end(), day_name);
    if(it != day_names.end())
       return distance(day_names.begin(), it);
    return INVALID;
}

vector<Record> filter_records_by_movie_name(vector<Record> records, string movie_name){
    vector<Record> result;
    for(auto record : records){
        if(record.movie == movie_name)
            result.push_back(record);
    }
    return result;
}

vector<Record> filter_records_by_day(vector<Record> records, string day){
    vector<Record> result;
    for(auto record : records){
        if(record.day == day)
            result.push_back(record);
    }
    return result;
}

bool less_than(Record operand1, Record operand2){
    if (eq(operand1.finish, operand2.finish)) {
        if (operand1.price == operand2.price)
            return operand1.cinema < operand2.cinema;
        else
            return operand1.price < operand2.price;
    }
    return less_than(operand1.finish, operand2.finish);
}

vector<Record> sort_records(vector <Record> records){
    vector<Record> sorted_records(records);
    int n = sorted_records.size();
    for (int i = 0 ; i < n - 1 ; i++){
        for (int j = 0 ; j < n - i - 1 ; j++){
            if (less_than(sorted_records[j + 1], sorted_records[j])){
                swap(sorted_records[j], sorted_records[j + 1]);
            }
        }
    }
    return sorted_records;
}

bool has_conflict(Record sans, Time time){
    if(less_than(sans.start, time) || eq(time, sans.finish))
        return true;
    return false;
}

vector <Record> get_max_intervals(vector <Record> records){
    vector<Record> sorted_records = sort_records(records);
    vector <Record> result;
    if(sorted_records.size() == 0)
        return result;
    result.push_back(sorted_records[0]);
    Time last_choosen_end_time = sorted_records[0].finish;

    for (int i = 1 ; i < sorted_records.size() ; i++) {
        if(!has_conflict(sorted_records[i], last_choosen_end_time)){
            result.push_back(sorted_records[i]);
            last_choosen_end_time = sorted_records[i].finish;
        }
    }
    return result;
}

TimeSchedule get_table_data(vector<Record> records, string movie_name){
    TimeSchedule table_data;
    vector<Record> filtered_records;

    for(auto day_name : day_names){
        filtered_records = filter_records_by_movie_name(records, movie_name);
        filtered_records = filter_records_by_day(filtered_records, day_name);
        table_data[day_name] = get_max_intervals(filtered_records);
    }
    return table_data;
}


string generate_html_head_code(){
    return string() +  
    "<head>\n" +
        "<title></title>\n" +
        "<link rel=\"stylesheet\" href=\"./style.css\" />\n" +
    "</head>";
}

string generate_html_times_row_code(){
    string time_boxes_html = "";
    const vector<string> time_box_texts = {"08:00", "10:00", "12:00", "14:00", "16:00", "18:00", "20:00", "22:00", "00:00"};
    const int time_texts_diff_in_px = 200;
    int left_offset;
    for(int i = 0; i < time_box_texts.size(); i++){
        left_offset = LEFT_OFFSET + time_texts_diff_in_px * i;
        time_boxes_html += "<div class=\"time-box\" style=\"left: " + to_string(left_offset) + 
                           "px;\"><p>" + time_box_texts[i] + "</p></div>\n";
    }
    return time_boxes_html;
}

string generate_html_vertical_lines_code(){
    const int distance_in_px = 100, num_of_lines = 17; 
    string vecrical_lines_code = "";

    for(int i = 0; i < num_of_lines; i++){
        int left_offset = LEFT_OFFSET + distance_in_px * i;
        vecrical_lines_code += "<div class=\"vertical-line\" style=\"left: " + to_string(left_offset) + "px;\"></div>\n";
    }
    return vecrical_lines_code;
}

string generate_html_days_column_code(){
    string days_column_code = "";
    for(int i = 0; i < day_names.size(); i++){
        int top_offset = TOP_OFFSET + DAY_DIFF_PX * i;
        days_column_code += "<div class=\"day-box\" style=\"top: " + to_string(top_offset) + 
                            "px;\">" + day_names[i] + "</div>\n";
    }
    return days_column_code;
}

string generate_html_records_code(map<string, vector<Record> > table_data){
    string records_code = "";
    for(auto it = table_data.begin(); it != table_data.end(); it++){
        int top_offset = TOP_OFFSET + get_day_num_by_day_name(it->first) * DAY_DIFF_PX;
        for(auto record: it->second){
            int left_offset = LEFT_OFFSET + divide_by_half(subtract(record.start, get8AM())) * UNIT_TIME_IN_PX;
            int width_in_px = divide_by_half(subtract(record.finish, record.start)) * UNIT_TIME_IN_PX;
            records_code += "<div class=\"record-box\" style=\"width: " + to_string(width_in_px) + 
                            "px; left: " + to_string(left_offset) + "px; top:" + to_string(top_offset) + 
                            "px; \">" + record.cinema + "</div>\n"; 
        }
    }
    return records_code;
}

string generate_html_body_code(map<string, vector<Record> > file_data){
    string body_code = "";
    body_code += "<body>\n";
    body_code += generate_html_times_row_code() + "\n";
    body_code += generate_html_vertical_lines_code() + "\n";
    body_code += generate_html_days_column_code() + "\n";
    body_code += generate_html_records_code(file_data) + "\n";
    body_code += "</body>";
    return body_code;
}

void generate_html_file(map<string, vector<Record> > file_data, string movie_name){
    ofstream html_file(movie_name + ".html");
    string html_head_code = generate_html_head_code();
    string html_body_code = generate_html_body_code(file_data);
    html_file << "<html lang=\"en\"> \n";
    html_file << html_head_code + "\n";
    html_file << html_body_code + "\n";
    html_file << "</html>";
    html_file.close();
}


int get_row_index_by_day_name(string day_name){
    return (get_day_num_by_day_name(day_name) + 1) * 2;
}

vector<string> initialize_table_lines(){
    vector<string> table_lines;
    table_lines.push_back(FIRST_ROW);
    for(int i = 1; i < TABLE_LINES; i++)
        table_lines.push_back(string(ROW_LENGTH, ' '));
    return table_lines;
}

string fill_box_vertical_side(string row, int box_start_pos, int box_length, int box_end_pos){
    for (int i = box_start_pos + 1; i < box_start_pos + box_length + 1; i++)
        if(row[i] != BOX_CORNER)
            row[i] = HORIZENTAL_SEPERATOR;
    row[box_start_pos] = BOX_CORNER;
    row[box_end_pos] = BOX_CORNER;
    return row;
}

string fill_box_horizental_sides(string record_row, int left_side, int right_side){
    record_row[left_side] = VERTICAL_SEPARATOR;
    record_row[right_side] = VERTICAL_SEPARATOR;
    return record_row;
}

string add_string_to_row(string row, int start_pos, string str){
    row.replace(start_pos, str.size(), str);
    return row;
}

vector<string> add_record_box_to_table(vector<string> table, Record record, int record_row_index){
    int box_start_pos = 11 + divide_by_half(subtract(record.start, get8AM())) * UNIT_TIME_LENGTH - 1;
    int box_length = divide_by_half(subtract(record.finish, record.start)) * UNIT_TIME_LENGTH - 1; 
    int box_end_pos = box_start_pos + box_length + 1;
    string before_record_row = table[record_row_index - 1];
    string record_row = table[record_row_index];
    string after_record_row = table[record_row_index + 1];

    before_record_row = fill_box_vertical_side(before_record_row, box_start_pos, box_length, box_end_pos);
    after_record_row = fill_box_vertical_side(after_record_row, box_start_pos, box_length, box_end_pos);
    record_row = fill_box_horizental_sides(record_row, box_start_pos, box_end_pos);
    record_row = add_string_to_row(record_row, box_start_pos + 1, record.cinema);

    table[record_row_index - 1] = before_record_row;
    table[record_row_index] = record_row;
    table[record_row_index + 1] = after_record_row;
    return table; 
}

vector<string> get_table_lines(TimeSchedule table_data){
    vector<string> table_lines = initialize_table_lines();
    for(auto it = table_data.begin(); it != table_data.end(); it++){
        int row_index = get_row_index_by_day_name(it->first);
        table_lines[row_index] = add_string_to_row(table_lines[row_index], 0, it->first);
        for(auto data : it->second){
            table_lines = add_record_box_to_table(table_lines, data, row_index);
        }
    }
    return table_lines;
}

void print(vector<string> table_lines){
    for(auto line : table_lines)
        cout<<line<<"\n";
}

bool is_table_empty(TimeSchedule table_data){
    bool is_empty = true;
    for(auto it = table_data.begin(); it != table_data.end(); it++){
        if(it->second.size() != 0)
            is_empty = false;
    }
    return is_empty;
}

void run_get_schedule(vector <Record> records, string movie_name){
    auto table_data = get_table_data(records, movie_name);
    if(is_table_empty(table_data))
        return;
    auto table_lines = get_table_lines(table_data);
    print(table_lines);
    generate_html_file(table_data, movie_name);
}