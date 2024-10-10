#pragma once
#include <wx/wx.h>
#include "CommandInvoker.hpp"  // Your existing invoker
#include "AppContext.hpp"      // Your existing app context
#include <memory>
class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    // Event handler functions
    void OnSetFrequency(wxCommandEvent& event);
    void OnSetDuration(wxCommandEvent& event);
    void OnSetMaxCount(wxCommandEvent& event);
    void OnSetConnectedOnly(wxCommandEvent& event);
    void OnSetPlainOutput(wxCommandEvent& event);
    void OnSetNoMatching(wxCommandEvent& event);
    void OnSetHelp(wxCommandEvent& event);
    void OnLoadWavFile(wxCommandEvent& event);
    void OnPrintPPM(wxCommandEvent& event);
    void OnPrintFFT(wxCommandEvent& event);
    void OnPrintDT(wxCommandEvent& event);

    std::shared_ptr<AppContext> ctx;  // Context shared across commands
    jackmeter::invoker::CommandInvoker invoker;  // Add this member

    wxDECLARE_EVENT_TABLE();
};