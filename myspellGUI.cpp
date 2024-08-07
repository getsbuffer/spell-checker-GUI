
#include "myspellGUI.h"
#include <sstream>
#include <vector>
#include <future>

using namespace std;

// Implement spell checker
wxIMPLEMENT_APP(SpellChecker);

bool SpellChecker::OnInit() 
{
	SpellCheckerFrame *frame = new SpellCheckerFrame();
	frame->Show();
	return true;
}

SpellCheckerFrame::SpellCheckerFrame() : wxFrame(nullptr, wxID_ANY, "Spellcheck") 
{
	// Loading hashtable
	bool success = dictionary.load("dictionary.txt");
	if (!success) 
	{
		wxMessageBox("Error: Dictionary could not be found.", "Error", wxOK | wxICON_ERROR);
		Close(true);
		return;
	}
	// Making menu
	wxMenu *menuFile = new wxMenu;
	menuFile->AppendSeparator();
	menuFile->Append(wxID_ADD, "Insert word");
	menuFile->Append(wxID_FIND, "Find word");
	menuFile->Append(wxID_DELETE, "Remove word");
	menuFile->Append(wxID_EDIT, "Write dictionary");
	menuFile->Append(wxID_CLEAR, "Clear dictionary");
	menuFile->Append(wxID_REMOVE, "Dump dictionary");
	menuFile->Append(wxID_NO, "Show dictionary size");
	menuFile->Append(wxID_YES, "Load new dictionary");

	menuFile->Append(wxID_EXIT);

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText("Simple Spellchecker");

	// Event listeners need to bind to these wxMenu objects
	Bind(wxEVT_MENU, &SpellCheckerFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &SpellCheckerFrame::OnAdd, this, wxID_ADD);
	Bind(wxEVT_MENU, &SpellCheckerFrame::OnFind, this, wxID_FIND);
	Bind(wxEVT_MENU, &SpellCheckerFrame::OnDelete, this, wxID_DELETE);
	Bind(wxEVT_MENU, &SpellCheckerFrame::OnEdit, this, wxID_EDIT);
	Bind(wxEVT_MENU, &SpellCheckerFrame::OnClear, this, wxID_CLEAR);
	Bind(wxEVT_MENU, &SpellCheckerFrame::OnRemove, this, wxID_REMOVE);
	Bind(wxEVT_MENU, &SpellCheckerFrame::OnNo, this, wxID_NO);
	Bind(wxEVT_MENU, &SpellCheckerFrame::OnYes, this, wxID_YES);
	Bind(wxEVT_MENU, &SpellCheckerFrame::OnExit, this, wxID_EXIT);

	// Set size
	SetSize(wxSize(800, 600));

	// Text control widget
	wxTextCtrl *textCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);

	// Setting text control as sizer for the frame
	SetSizer(new wxBoxSizer(wxVERTICAL));
	GetSizer()->Add(textCtrl, 1, wxEXPAND);

	// Event handler for when user presses Enter
	textCtrl->Bind(wxEVT_TEXT_ENTER, &SpellCheckerFrame::OnEnter, this);
}

void SpellCheckerFrame::OnEnter(wxCommandEvent& event) 
{
	wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	// Handle the case where the event object is not a wxTextCtrl
	if (!textCtrl)
		return;

	wxString enteredText = textCtrl->GetValue();
	istringstream stream(enteredText.ToStdString());
	string word;
	vector<string> candidates;

	while (stream >> word) 
	{
		/* First, get rid of punctuation and capitalization */
		string currentWord = removePunctuation(word);
		bool wasCapital = false;

		/* Check if it is a capital, make it not a capital if so */
		if (isupper(currentWord[0]))  
		{
			currentWord[0] = tolower(currentWord[0]);
			wasCapital = true;
		}

		if (!dictionary.contains(currentWord) && !isSpaces(currentWord)) 
		{
			// Find suitable candidates
			candidates = makeCandidates(currentWord, dictionary);

			if (!candidates.empty()) 
			{
				// Display menu with candidates
				wxMenu menu;

				for (size_t i = 0; i < candidates.size(); i++) 
					menu.Append(i + 1, wxString(candidates[i]));

				// Add "Add to dictionary" option
				menu.Append(candidates.size() + 1, "Add to dictionary");

				// Show the menu at the text control position
				wxPoint position = textCtrl->ClientToScreen(wxPoint(0, 0));
				int selectedItem = textCtrl->GetPopupMenuSelectionFromUser(menu, position);

				// Process the selected item
				if (selectedItem != wxID_NONE) 
				{
					if (selectedItem == static_cast<int>(candidates.size() + 1)) 
						dictionary.insert(currentWord);
					else 
					{
						// Handle candidate selection logic
						wxString selectedCandidate = candidates[selectedItem - 1];

						// Replace the word in the text control
						wxString newText = textCtrl->GetValue();
						// Restore current word if it had a capital
						if (wasCapital) 
						{
							currentWord[0] = toupper(currentWord[0]);
							selectedCandidate[0] = toupper(selectedCandidate[0]);

						}
						newText.Replace(currentWord, selectedCandidate);
						textCtrl->SetValue(newText);
					}
				}
			} 
			else 
			{
				// No candidates, ask the user to add to the dictionary
				wxString message = wxString::Format("The word '%s' is not in the dictionary and has no candidates. Would you like to add it to the dictionary?", wxString(currentWord));
				wxMessageDialog dialog(this, message, "Add to Dictionary", wxYES_NO | wxICON_QUESTION);

				if (dialog.ShowModal() == wxID_YES) 
					dictionary.insert(currentWord);
			}
		}
	}
}

vector<string> SpellCheckerFrame::makeCandidates(const string& badWord, const cop4530::HashTable<string>& dictionary) 
{
	char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 
			     'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 
			     's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	vector<string> candidates;
	string temp = badWord;
	for (size_t i = 0; i < badWord.size(); i++) 
	{
		temp = badWord;
		for (int j = 0; j < 26; j++) 
		{
			temp[i] = alphabet[j];
			if (dictionary.contains(temp))
				candidates.push_back(temp);
		}
	}
	// If more than 10 candidates, reduce candidate number to 10
	while (candidates.size() > 10)
		candidates.pop_back();
	return candidates;
}
string SpellCheckerFrame::removePunctuation(const string& word) 
{
	string temp = word;
	int size = word.size() - 1;
	if (word[size] == ',' || word[size] == ';' || word[size] == '!' || word[size] == '.' || word[size] == ':' || word[size] == '?')
		temp = temp.substr(0, size);
	return temp;
}

string SpellCheckerFrame::makeCapital(const string& word) 
{
	string capital = word;
	for (size_t i = 0; i < capital.size(); i++)
		capital[i] = toupper(capital[i]);
	return capital;
}

bool SpellCheckerFrame::isSpaces(const string& word) 
{
	bool result = true;
	for (size_t i = 0; i < word.size(); i++) 
		if (!isspace(word[i]))
			result = false;
	return result;
}

void SpellCheckerFrame::OnAdd(wxCommandEvent& event) 
{
	wxString wxWordToAdd = wxGetTextFromUser("Please enter word to insert: ");
	if (!wxWordToAdd.empty()) 
	{
		string wordToAddStr = string(wxWordToAdd.mb_str());
		dictionary.insert(wordToAddStr);
		wxMessageBox("Word has been inserted into the dictionary.");
	}
}

void SpellCheckerFrame::OnFind(wxCommandEvent& event) 
{
	wxString wxWordToFind = wxGetTextFromUser("Please enter word to search for: ");
	if (!wxWordToFind.empty()) 
	{
		string wordToFindStr = string(wxWordToFind.mb_str());
		bool foundWord = dictionary.contains(wordToFindStr);
		if (foundWord)
			wxMessageBox("Word is contained in the dictionary");
		else
			wxMessageBox("Word not found in the dictionary");
	}

}
void SpellCheckerFrame::OnDelete(wxCommandEvent& event) 
{
	wxString wxWordToRemove = wxGetTextFromUser("Please enter word to remove: ");
	if (!wxWordToRemove.empty()) 
	{
		string wordToRemoveStr = string(wxWordToRemove.mb_str());
		dictionary.remove(wordToRemoveStr);
		wxMessageBox("Word has been removed from the dictionary.");
	}
}
void SpellCheckerFrame::OnEdit(wxCommandEvent& event) 
{
	wxString wxWordOutput = wxGetTextFromUser("Please enter file to write dictionary to: ");
	if (!wxWordOutput.empty()) 
	{
		char wordOutput[1024];
		strncpy(wordOutput, (const char*)wxWordOutput.mb_str(wxConvUTF8), 1023);
		wordOutput[1023] = '\0';
		dictionary.write_to_file(wordOutput);
		wxMessageBox("Dictionary has been written to a file: ");
	}
}
void SpellCheckerFrame::OnRemove(wxCommandEvent& event) 
{
	wxString wxDump(dictionary.wxDump().c_str(), wxConvUTF8);
	wxFrame* dumpFrame = new wxFrame(this, wxID_ANY, "Dictionary dump", wxDefaultPosition);
	wxPanel* panel = new wxPanel(dumpFrame);
	wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, wxDump, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	textCtrl->SetBackgroundColour(*wxWHITE);

	wxFont font(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	textCtrl->SetFont(font);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(textCtrl, 1, wxEXPAND | wxALL, 0);
	panel->SetSizer(sizer);
	dumpFrame->Show(true);
}

void SpellCheckerFrame::OnClear(wxCommandEvent& event) 
{
	wxString message = wxString::Format("Are you sure you would like to clear the dictionary?");
	wxMessageDialog dialog(this, message, "Yes", wxYES_NO | wxICON_QUESTION);
	if (dialog.ShowModal() == wxID_YES) 
		dictionary.clear();
}


void SpellCheckerFrame::OnNo(wxCommandEvent& event) 
{
	size_t size = dictionary.getSize();
	wxString stringSize = wxString::Format(wxT("Size of the dictionary is: %s"), wxString::Format(wxT("%lu"), size));
	wxMessageBox(stringSize, "Dictionary Size");

}

void SpellCheckerFrame::OnYes(wxCommandEvent& event) 
{
	wxString wxWordOutput = wxGetTextFromUser("Please enter dictionary file you'd like to load in");
	if (!wxWordOutput.empty()) 
	{
		char wordOutput[1024];
		strncpy(wordOutput, (const char*)wxWordOutput.mb_str(wxConvUTF8), 1023);
		wordOutput[1023] = '\0';
		bool success = dictionary.load(wordOutput);
		if (success)
			wxMessageBox("Dictionary has been loaded");
		else
			wxMessageBox("Error: Couldn't load dictionary file");

	}
}

void SpellCheckerFrame::OnExit(wxCommandEvent& event) 
{
	Close(true);
}

void SpellCheckerFrame::OnAbout(wxCommandEvent& event) 
{
	wxMessageBox("This is a simple spellchecker program. You are free to type text in the frame below. If there is a spelling error, a red squiggle will appear. If you click on the word, it will then display a menu of possible candidates for the misspelled word. Enjoy!", "About Spellchecker", wxOK | wxICON_INFORMATION);
}
