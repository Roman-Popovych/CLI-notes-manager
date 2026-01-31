# CLI Notes Manager

CLI Notes Manager is a command-line application written in C++ that allows users to manage simple text notes directly from the terminal.
The application supports adding, listing, and removing notes, with all data stored locally in a plain text file.

This project is designed as a practical example of building a real-world CLI utility using C++, focusing on argument parsing, file I/O, and error handling.

---

## Features

- Add text notes from the command line
- List all saved notes
- Remove notes by their unique ID
- Automatic ID generation
- Persistent storage using a local text file
- Human-readable file format
- Simple and fast CLI workflow

---

## Commands Overview

The application is controlled entirely via command-line arguments.

CLI-notes-manager <command> [arguments]

Available commands:
- `add` — add a new note
- `list` — display all notes
- `remove` — delete a note by ID
- `help` — display usage information