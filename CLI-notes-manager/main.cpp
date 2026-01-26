#include <iostream>
#include <string>

#include <fstream>
#include <sstream>
#include <vector>

#include <chrono>
#include <ctime>
#include <iomanip>

struct Note {
	int id;
	std::string date;
	std::string text;
};

std::vector<Note> loadNotes(const std::string& filename)
{
	std::vector<Note> notes;

	std::ifstream file(filename);
	
	if (!file.is_open()) {
		return notes;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) continue;

		std::stringstream ss(line);
		std::string part;

		Note note;

		// 1) id
		if (!std::getline(ss, part, '|') || part.empty()) continue;
		try {
			note.id = std::stoi(part);
		}
		catch (...) {
			continue;
		}

		// 2) date
		if (!std::getline(ss, note.date, '|') || note.date.empty()) continue;

		// 3) text
		if (!std::getline(ss, note.text) || note.text.empty()) continue;

		notes.push_back(note);
	}

	return notes;
}

int main(int argc, char* argv[]) {
	if (argc < 2)
	{
		std::cout << "No arguments provided." << std::endl;
		return 1;
	}

	const std::string filename = "notes.txt";

	std::vector<Note> notes = loadNotes(filename);
	
	std::string command = argv[1];



	if (command == "add") {
		// add
		if (argc < 3) {
			std::cerr << "Usage: notes add \"text\"\n";
			return 1;
		}

		std::string text;
		for (int i = 2; i < argc; ++i) {
			if (i > 2) text += " ";
			text += argv[i];
		}

		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm local{};
		localtime_s(&local, &t);


		std::fstream file(filename, std::ios::app);
		file << (notes.size() + 1) << '|' << std::put_time(&local, "%d-%m-%Y") << '|' << text << std::endl;


		return 0;
	}
	else if (command == "list") {
		// list
		std::cout << "Listing notes..." << std::endl;

		for (int i = 0; i < notes.size(); i++)
		{
			std::cout << notes[i].id << "|" << notes[i].date << "|" << notes[i].text << std::endl;
		}
		return 0;
	}
	else if (command == "remove") {
		// remove
		if (argc < 3) {
			std::cerr << "Usage: notes remove <id>\n";
			return 1;
		}

		try {
			int id = std::stoi(argv[2]);
			std::cout << "Removing note with id " << id << "\n";
		}
		catch (...) {
			std::cerr << "Invalid id (must be a number)\n";
			return 1;
		}
	}
	else {
		std::cout << "Unknown command." << std::endl;
		return 1;
	}

	return 0;
}