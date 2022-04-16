#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

const int DAY_SUMMARY_CHARACTER_COUNT = 20;
const int DATE_INDEX_IN_LINE = 1;
const string CONTINUED_SUMMARY_SIGN = "...";
const string POSITIVE_WORDS_FILENAME = "positive-words.txt";

enum Command
{
	START_DAY,
	PRINT_DAY,
	LONGEST_DAY,
	BEST_DAY,
	NO_COMMAND
};

struct JournalPage
{
	string date;
	string content;
};

typedef vector<JournalPage> Journal;

vector<string> split(string text)
{
	vector<string> words;
	stringstream textStream(text);
	string word;
	while (textStream >> word)
	{
		words.push_back(word);
	}
	return words;
}

string getWordAt(string line, int wordIndex)
{
	return split(line)[wordIndex];
}

Command getCommand(string line)
{
	string commandText = getWordAt(line, 0);
	if (commandText == "start_day")
	{
		return START_DAY;
	}
	else if (commandText == "show_day")
	{
		return PRINT_DAY;
	}
	else if (commandText == "show_the_longest_day")
	{
		return LONGEST_DAY;
	}
	else if (commandText == "show_the_best_day")
	{
		return BEST_DAY;
	}
	else
	{
		return NO_COMMAND;
	}
}

string getDate(string line)
{
	return getWordAt(line, DATE_INDEX_IN_LINE);
}

string getJournalContent(const Journal &journal, string date)
{
	for (int i = 0; i < journal.size(); i++)
	{
		if (journal[i].date == date)
		{
			return journal[i].content;
		}
	}
	return "";
}

string getFileContents(string filename)
{
	ifstream file(filename);
	string contents = "";
	string line;
	while (getline(file, line))
	{
		contents += line + "\n";
	}
	file.close();
	return contents;
}

int getIndexWithMax(const vector<int> &values)
{
	int maxIndex = 0;
	int maxValue = values[0];
	for (int i = 1; i < values.size(); i++)
	{
		if (values[i] > maxValue)
		{
			maxValue = values[i];
			maxIndex = i;
		}
	}
	return maxIndex;
}

string getTheLongestDay(const Journal &journal)
{
	vector<int> journalLengths;
	for (const auto &page : journal)
	{
		journalLengths.push_back(page.content.size());
	}
	return journal[getIndexWithMax(journalLengths)].date;
}

bool isIn(string word, const vector<string> &keywords)
{
	for (const auto &keyword : keywords)
	{
		if (word == keyword)
		{
			return true;
		}
	}
	return false;
}

int getOccuranceCount(string text, const vector<string> &keywords)
{
	int count = 0;
	for (const auto &word : split(text))
	{
		if (isIn(word, keywords))
		{
			count++;
		}
	}
	return count;
}

string getTheBestDay(const Journal &journal, const vector<string> &positiveWords)
{
	vector<int> positiveWordCounts;
	for (const auto &page : journal)
	{
		positiveWordCounts.push_back(
			getOccuranceCount(page.content, positiveWords));
	}
	return journal[getIndexWithMax(positiveWordCounts)].date;
}

string getDaySummary(const Journal &journal, string date)
{
	string journalContent = getJournalContent(journal, date);
	if (journalContent.size() < DAY_SUMMARY_CHARACTER_COUNT)
	{
		return date + "\n" + journalContent;
	}
	return date + "\n" + journalContent.substr(0, DAY_SUMMARY_CHARACTER_COUNT) + CONTINUED_SUMMARY_SIGN + "\n";
}

void print(const string& content)
{
	cout << content;
}

void readAndRunCommands(const vector<string> &positiveWords)
{
	string line;
	Journal journal;
	while (getline(cin, line))
	{
		Command command = getCommand(line);
		switch (command)
		{
		case START_DAY:
			journal.push_back({getDate(line), ""});
			break;
		case PRINT_DAY:
			print(getJournalContent(journal, getDate(line)));
			break;
		case LONGEST_DAY:
			print(getDaySummary(journal, getTheLongestDay(journal)));
			break;
		case BEST_DAY:
			print(getDaySummary(journal, getTheBestDay(journal, positiveWords)));
			break;
		case NO_COMMAND:
			journal.back().content += line + "\n";
			break;
		}
	}
}

vector<string> loadPositiveWords()
{
	return split(getFileContents(POSITIVE_WORDS_FILENAME));
}

int main()
{
	vector<string> positiveWords = loadPositiveWords();
	readAndRunCommands(positiveWords);
	return 0;
}