#include "MyApp.hpp"
#include "MyFrame.hpp"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("JackMeter GUI", wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}
