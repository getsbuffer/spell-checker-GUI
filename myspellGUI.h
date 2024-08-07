#ifndef MYSPELLGUI_H
#define MYSPELLGUI_H

#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/popupwin.h>
#include <vector>
#include "hashtable.h"

class SpellChecker : public wxApp 
{
	public:
		bool OnInit() override;
};

class SpellCheckerFrame : public wxFrame 
{
	public:
		SpellCheckerFrame();
	private:
		void OnExit(wxCommandEvent&);
		void OnAbout(wxCommandEvent&);
		void OnEnter(wxCommandEvent&);
		void OnAdd(wxCommandEvent&);
		void OnFind(wxCommandEvent&);
		void OnEdit(wxCommandEvent&);
		void OnDelete(wxCommandEvent&);
		void OnClear(wxCommandEvent&);
		void OnRemove(wxCommandEvent&);
		void OnNo(wxCommandEvent&);
		void OnYes(wxCommandEvent&);
		std::vector<string> makeCandidates(const std::string&, const cop4530::HashTable<std::string>&);
		std::string removePunctuation(const std::string&);
		std::string makeCapital(const std::string&);
		bool isSpaces(const std::string&);
		cop4530::HashTable<string> dictionary;
};

#endif // SPELLCHECKER_H
