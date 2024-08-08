# SpellChecker GUI

Welcome to my SpellChecker GUI! This application is a simple spell checker with a graphical user interface built using wxWidgets, written for extra credit in my data structures course. The spell checking functionality is powered by a custom hashtable implementation, ensuring efficient handling of the dictionary operations and constant time lookups.

## Features

- **Load Dictionary**: Load a pre-defined dictionary from a text file.
- **Spell Check**: Real-time spell checking as you type.
- **Insert Word**: Add new words to the dictionary.
- **Find Word**: Search for words in the dictionary.
- **Remove Word**: Delete words from the dictionary.
- **Write Dictionary**: Save the current dictionary to a file.
- **Clear Dictionary**: Remove all words from the dictionary.
- **Dump Dictionary**: Display all words in the dictionary.
- **Show Dictionary Size**: Display the number of words in the dictionary.
- **Load New Dictionary**: Load a different dictionary file.
- **About**: Information about the application.

## Getting Started

### Prerequisites

- **wxWidgets**: Ensure you have wxWidgets installed on your system. For installation instructions, visit the [wxWidgets website](https://www.wxwidgets.org/downloads/).

### Installation

1. **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/spellchecker-gui.git
    cd spellchecker-gui
    ```

2. **Build the project**:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

3. **Run the application**:
    ```bash
    ./SpellChecker
    ```

## Usage

1. **Load Dictionary**: Upon starting the application, it attempts to load a dictionary from `dictionary.txt`. If not found, an error message will be displayed.

2. **Spell Checking**: Type text in the provided text control widget. Misspelled words will be detected, and suggestions will be provided.

3. **Menu Options**:
    - **Insert Word**: Add a new word to the dictionary.
    - **Find Word**: Search for a word in the dictionary.
    - **Remove Word**: Delete a word from the dictionary.
    - **Write Dictionary**: Save the current dictionary to a specified file.
    - **Clear Dictionary**: Remove all words from the dictionary.
    - **Dump Dictionary**: Display all words in the dictionary in a new window.
    - **Show Dictionary Size**: Show the number of words in the dictionary.
    - **Load New Dictionary**: Load a different dictionary file.
    - **Exit**: Close the application.

## Code Structure
### myspellGUI.h

Header file containing class definitions for `SpellChecker` and `SpellCheckerFrame`.

### myspellGUI.cpp

The main implementation file containing all the wxWidgets logic for the graphical user interface. It manages the frame, event bindings, and text control widget for spell checking.

### hashtable.h

Header file declaring the template class `HashTable` and its member functions. Provides the interface for the hashtable operations defined in hashtable.hpp.

### hashtable.hpp

Implementation file for the `HashTable` template class. Contains the detailed implementation of all the hashtable operations. Key functions include:

- **`insert`**: Adds a new word to the hashtable.
- **`remove`**: Deletes a word from the hashtable.
- **`contains`**: Checks if a word exists in the hashtable.
- **`load`**: Loads words from a file into the hashtable.
- **`write_to_file`**: Writes the current dictionary to a file.
- **`rehash`**: Resizes the hashtable when it becomes too full.
- **`clear`**: Clears all entries in the hashtable.
- **`dump`**: Outputs the contents of the hashtable for debugging.
- **`wxDump`**: Outputs the contents of the hashtable in a wxWidgets-friendly format.

### Makefile

Defines the build process for the project. Includes targets for compiling the application, cleaning build artifacts, and running the application.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgments

- **wxWidgets**: For providing the graphical user interface framework.
