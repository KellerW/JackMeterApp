#include "MyFrame.hpp"
#include "CommandInvoker.hpp"
#include <wx/msgdlg.h>
#include "AppContextSingleton.hpp"

// Event Table Definition
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(wxID_ANY, MyFrame::OnSetFrequency)   // Example
    // Add similar bindings for other functions like OnSetDuration, OnSetMaxCount, etc.
    EVT_BUTTON(wxID_ANY, MyFrame::OnSetDuration)
    EVT_BUTTON(wxID_ANY, MyFrame::OnSetMaxCount)
    EVT_BUTTON(wxID_ANY, MyFrame::OnSetConnectedOnly)
    EVT_BUTTON(wxID_ANY, MyFrame::OnSetPlainOutput)
    EVT_BUTTON(wxID_ANY, MyFrame::OnSetNoMatching)
    EVT_BUTTON(wxID_ANY, MyFrame::OnSetHelp)
    EVT_BUTTON(wxID_ANY, MyFrame::OnLoadWavFile)
    EVT_BUTTON(wxID_ANY, MyFrame::OnPrintPPM)
    EVT_BUTTON(wxID_ANY, MyFrame::OnPrintFFT)
    EVT_BUTTON(wxID_ANY, MyFrame::OnPrintDT)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size), ctx(AppContextSingleton::get_instance())
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxButton* btnFrequency = new wxButton(panel, wxID_ANY, wxT("Set Frequency"));
    wxButton* btnDuration = new wxButton(panel, wxID_ANY, wxT("Set Duration"));
    wxButton* btnMaxCount = new wxButton(panel, wxID_ANY, wxT("Set Max Count"));
    wxButton* btnConnectedOnly = new wxButton(panel, wxID_ANY, wxT("Connected Only"));
    wxButton* btnPlainOutput = new wxButton(panel, wxID_ANY, wxT("Plain Output"));
    wxButton* btnNoMatching = new wxButton(panel, wxID_ANY, wxT("No Matching"));
    wxButton* btnHelp = new wxButton(panel, wxID_ANY, wxT("Help"));
    wxButton* btnLoadWaveFile = new wxButton(panel, wxID_ANY, wxT("Load WAV File"));
    wxButton* btnPrintPPM = new wxButton(panel, wxID_ANY, wxT("Print PPM"));
    wxButton* btnPrintFFT = new wxButton(panel, wxID_ANY, wxT("Print FFT"));
    wxButton* btnPrintDT = new wxButton(panel, wxID_ANY, wxT("Print DT"));

    sizer->Add(btnFrequency, 0, wxALL, 10);
    sizer->Add(btnDuration, 0, wxALL, 10);
    sizer->Add(btnMaxCount, 0, wxALL, 10);
    sizer->Add(btnConnectedOnly, 0, wxALL, 10);
    sizer->Add(btnPlainOutput, 0, wxALL, 10);
    sizer->Add(btnNoMatching, 0, wxALL, 10);
    sizer->Add(btnHelp, 0, wxALL, 10);
    sizer->Add(btnLoadWaveFile, 0, wxALL, 10);
    sizer->Add(btnPrintPPM, 0, wxALL, 10);
    sizer->Add(btnPrintFFT, 0, wxALL, 10);
    sizer->Add(btnPrintDT, 0, wxALL, 10);

    panel->SetSizer(sizer);

    // Bind button events to functions
    Bind(wxEVT_BUTTON, &MyFrame::OnSetFrequency, this, btnFrequency->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnSetDuration, this, btnDuration->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnSetMaxCount, this, btnMaxCount->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnSetConnectedOnly, this, btnConnectedOnly->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnSetPlainOutput, this, btnPlainOutput->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnSetNoMatching, this, btnNoMatching->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnSetHelp, this, btnHelp->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnLoadWavFile, this, btnLoadWaveFile->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnPrintPPM, this, btnPrintPPM->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnPrintFFT, this, btnPrintFFT->GetId());
    Bind(wxEVT_BUTTON, &MyFrame::OnPrintDT, this, btnPrintDT->GetId());
}

void MyFrame::OnSetFrequency(wxCommandEvent& event)
{
     (void)event;
    wxTextEntryDialog dialog(this, wxT("Enter Frequency"), wxT("Frequency Command"));
    if (dialog.ShowModal() == wxID_OK) {
        auto command = std::make_shared<jackmeter::commands::FrequencyCommand>(ctx->config, dialog.GetValue().ToStdString().c_str());
        invoker.addCommand(command);
        invoker.executeCommands();
    }
}

void MyFrame::OnSetDuration(wxCommandEvent& event)
{
     (void)event;
    wxTextEntryDialog dialog(this, wxT("Enter Duration"), wxT("Duration Command"));
    if (dialog.ShowModal() == wxID_OK) {
        auto command = std::make_shared<jackmeter::commands::DurationCommand>(ctx->config, dialog.GetValue().ToStdString().c_str());
        invoker.addCommand(command);
        invoker.executeCommands();
    }
}

void MyFrame::OnSetMaxCount(wxCommandEvent& event)
{
     (void)event;
    wxTextEntryDialog dialog(this, wxT("Enter Max Count"), wxT("Max Count Command"));
    if (dialog.ShowModal() == wxID_OK) {
        auto command = std::make_shared<jackmeter::commands::MaxCountCommand>(ctx->config, dialog.GetValue().ToStdString().c_str());
        invoker.addCommand(command);
        invoker.executeCommands();
    }
}

void MyFrame::OnSetConnectedOnly(wxCommandEvent& event)
{
     (void)event;
    auto command = std::make_shared<jackmeter::commands::ConnectedOnlyCommand>(ctx->config);
    invoker.addCommand(command);
    invoker.executeCommands();
}

void MyFrame::OnSetPlainOutput(wxCommandEvent& event)
{
     (void)event;
    auto command = std::make_shared<jackmeter::commands::PlainOutputCommand>(ctx->config);
    invoker.addCommand(command);
    invoker.executeCommands();
}

void MyFrame::OnSetNoMatching(wxCommandEvent& event)
{
     (void)event;
    auto command = std::make_shared<jackmeter::commands::NoMatchingCommand>(ctx->config);
    invoker.addCommand(command);
    invoker.executeCommands();
}

void MyFrame::OnSetHelp(wxCommandEvent& event)
{
     (void)event;
    auto command = std::make_shared<jackmeter::commands::HelpCommand>();
    invoker.addCommand(command);
    invoker.executeCommands();
}

void MyFrame::OnLoadWavFile(wxCommandEvent& event)
{
     
    (void)event; 
    wxFileDialog openFileDialog(this, _("Open WAV file"), "", "", "WAV files (*.wav)|*.wav", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    auto command = std::make_shared<jackmeter::commands::LoadWaveFile>(openFileDialog.GetPath().ToStdString().c_str(), ctx->config);
     invoker.addCommand(command);
     invoker.executeCommands();
}

void MyFrame::OnPrintPPM(wxCommandEvent& event)
{
    (void)event; 
    auto command = std::make_shared<jackmeter::commands::PrintPPM>(ctx->config);
    invoker.addCommand(command);
    wxTextEntryDialog dialog(this, wxT("Enter Duration"), wxT("Duration Command"));
    if (dialog.ShowModal() == wxID_OK) 
    {
        auto command = std::make_shared<jackmeter::commands::DurationCommand>(ctx->config, "5");
        invoker.addCommand(command);
    }
    wxFileDialog openFileDialog(this, _("Open WAV file"), "", "", "WAV files (*.wav)|*.wav", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    auto command2 = std::make_shared<jackmeter::commands::LoadWaveFile>(openFileDialog.GetPath().ToStdString().c_str(), ctx->config);
     invoker.addCommand(command2);
    
        invoker.executeCommands();
}

void MyFrame::OnPrintFFT(wxCommandEvent& event)
{
    (void)event; 
    auto command = std::make_shared<jackmeter::commands::PrintFFT>(ctx->config);
    invoker.addCommand(command);
    invoker.executeCommands();
}

void MyFrame::OnPrintDT(wxCommandEvent& event)
{
    
    (void)event; 
    auto command = std::make_shared<jackmeter::commands::PrintDT>(ctx->config);
    invoker.addCommand(command);
    invoker.executeCommands();
}
