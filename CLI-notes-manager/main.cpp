#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Note {
    int id{};
    std::string date; // "dd-mm-YYYY"
    std::string text;
};

std::vector<Note> loadNotes(const std::string& filename) {
    std::vector<Note> notes;

    std::ifstream file(filename);
    if (!file) {
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

std::string getDateStringDDMMYYYY() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &t);
#else
    local = *std::localtime(&t);
#endif

    std::ostringstream ss;
    ss << std::put_time(&local, "%d-%m-%Y");
    return ss.str();
}

void updateNotes(const std::vector<Note>& notes, const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file) {
        std::cerr << "Error: cannot write to file: " << filename << "\n";
        return;
    }

    for (const auto& note : notes) {
        file << note.id << '|'
            << note.date << '|'
            << note.text << '\n';
    }
}

int getNextIdRobust(const std::vector<Note>& notes) {
    int nextId = 1;
    for (const auto& n : notes) {
        nextId = std::max(nextId, n.id + 1);
    }
    return nextId;
}

void renumberIds(std::vector<Note>& notes) {
    for (size_t i = 0; i < notes.size(); ++i) {
        notes[i].id = static_cast<int>(i) + 1;
    }
}

void printHelp() {
    std::cout
        << "Notes Application Help:\n"
        << "Commands:\n"
        << "  add <text>    - Add a new note with the specified text.\n"
        << "  list          - List all notes.\n"
        << "  remove <id>   - Remove the note with the specified id.\n"
        << "  clear         - Clear all notes (empties the file).\n"
        << "  help          - Show this help message.\n";
}

int main(int argc, char* argv[]) {
    const std::string filename = "notes.txt";

    if (argc < 2) {
        std::cout << "No arguments provided.\n";
        printHelp();
        return 1;
    }

    std::vector<Note> notes = loadNotes(filename);
    std::string command = argv[1];

    if (command == "add") {
        if (argc < 3) {
            std::cerr << "Usage: notes add <text>\n";
            return 1;
        }

        std::string text;
        for (int i = 2; i < argc; ++i) {
            if (i > 2) text += " ";
            text += argv[i];
        }

        const int nextId = getNextIdRobust(notes);

        Note n;
        n.id = nextId;
        n.date = getDateStringDDMMYYYY();
        n.text = text;
        notes.push_back(n);

        updateNotes(notes, filename);

        std::cout << "Added note: " << text << "\n";
        return 0;
    }

    if (command == "list") {
        std::cout << "Listing notes...\n";
        for (const auto& n : notes) {
            std::cout << n.id << "|" << n.date << "|" << n.text << "\n";
        }
        return 0;
    }

    if (command == "remove") {
        if (argc < 3) {
            std::cerr << "Usage: notes remove <id>\n";
            return 1;
        }

        int id = 0;
        try {
            id = std::stoi(argv[2]);
        }
        catch (...) {
            std::cerr << "Invalid id (must be a number)\n";
            return 1;
        }

        bool found = false;
        for (size_t i = 0; i < notes.size(); ++i) {
            if (notes[i].id == id) {
                notes.erase(notes.begin() + static_cast<std::ptrdiff_t>(i));
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "Note with id " << id << " not found\n";
            return 0;
        }

        renumberIds(notes);

        updateNotes(notes, filename);
        std::cout << "Removed note with id " << id << "\n";
        return 0;
    }

    if (command == "clear") {
        std::cout << "Clearing all notes...\n";
        std::ofstream file(filename, std::ios::trunc);
        if (!file) {
            std::cerr << "Error: cannot clear file: " << filename << "\n";
            return 1;
        }
        return 0;
    }

    if (command == "help") {
        printHelp();
        return 0;
    }

    std::cout << "Unknown command.\n";
    printHelp();
    return 1;
}
