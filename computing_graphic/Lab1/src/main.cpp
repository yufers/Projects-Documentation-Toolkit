#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/dc.h>
#include <wx/dcbuffer.h> // wxBufferedPaintDC

#include "Frame.h"
#include "Painter.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title)
        : wxFrame(nullptr, wxID_ANY, title,
                  wxDefaultPosition, wxSize(800, 600),
                  wxDEFAULT_FRAME_STYLE | wxRESIZE_BORDER),
          m_timer(this, wxID_ANY),
          m_pixelSize(8) // “big pixel” size
    {
        CreateStatusBar(3);
        SetStatusText("Ready", 0);

        m_timer.Start(30); // ~30 Hz

        // Add a panel to the main frame
        wxPanel *m_panel = new wxPanel(this, wxID_ANY);

        Bind(wxEVT_PAINT, &MainFrame::OnPaint, this);
        Bind(wxEVT_SIZE, &MainFrame::OnSize, this);
        Bind(wxEVT_MOTION, &MainFrame::OnMouseMove, this);
        Bind(wxEVT_LEFT_DOWN, &MainFrame::OnLeftDown, this);
        Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);
        Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);

        // Move the key down event binding to the panel
        m_panel->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyDown, this);

        SetFocus();
    }

private:
    wxTimer m_timer;
    int m_pixelSize; // “big pixel” size

    void OnPaint(wxPaintEvent &)
    {
        wxBufferedPaintDC dc(this);
        wxSize client = GetClientSize();

        /* ---- Compute pixel grid ---- */
        int width = client.x / m_pixelSize;
        int height = client.y / m_pixelSize;

        /* If the window is too small – nothing to draw */
        if (width <= 0 || height <= 0)
            return;

        /* ---- Render into a wxImage (no alpha needed) ---- */
        wxImage img(width * m_pixelSize,
                    height * m_pixelSize,
                    false); // no alpha channel

        Frame frame(width, height);
        Painter painter;
        painter.Draw(frame);

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const COLOR &col = frame.GetPixel(x, y);

                /* Fill the corresponding block of pixels in the image */
                for (int dy = 0; dy < m_pixelSize; ++dy)
                {
                    for (int dx = 0; dx < m_pixelSize; ++dx)
                    {
                        int px = x * m_pixelSize + dx;
                        int py = y * m_pixelSize + dy;

                        img.SetRGB(px, py, col.RED, col.GREEN, col.BLUE);
                        /* No SetAlpha() – we requested no alpha channel */
                    }
                }
            }
        }

        /* ---- Convert to bitmap and blit ---- */
        wxBitmap bitmap(img);
        dc.DrawBitmap(bitmap, 0, 0);
    }

    void OnSize(wxSizeEvent &) { Refresh(); }

    void OnMouseMove(wxMouseEvent &evt)
    {
        if (evt.Moving())
        {
            SetStatusText(wxString::Format("X = %d, Y = %d", evt.GetX(), evt.GetY()), 0);
            SetStatusText(wxString::Format("BX = %d, BY = %d",
                                           evt.GetX() / m_pixelSize, evt.GetY() / m_pixelSize),
                          1);
            SetStatusText(wxString::Format("Scale (F1/F2): %d", m_pixelSize), 2);
        }
    }

    void OnLeftDown(wxMouseEvent &evt)
    {
        global_clicked_pixel.X = evt.GetX() / m_pixelSize;
        global_clicked_pixel.Y = evt.GetY() / m_pixelSize;
        Refresh();
    }

    void OnKeyDown(wxKeyEvent &evt)
    {
        int key = evt.GetKeyCode();
        if (key == WXK_F2 && m_pixelSize > 1)
            --m_pixelSize;
        else if (key == WXK_F3 && m_pixelSize < 64)
            ++m_pixelSize;
        else if (key == WXK_F1)
        {
            wxString unicodeMessage = wxString::FromUTF8("Работу выполнил студент группы ПВ-233 Мовчан А.Ю.");
            wxString unicodeTitle = wxString::FromUTF8("О программе");
            wxMessageBox(unicodeMessage, unicodeTitle, wxICON_INFORMATION);
            return;
        }
        SetStatusText(wxString::Format("Scale (F1/F2): %d", m_pixelSize), 2);
        Refresh();
    }

    void OnTimer(wxTimerEvent &)
    {
        global_angle += 0.05f;
        Refresh();
    }

    void OnClose(wxCloseEvent &)
    {
        m_timer.Stop();
        Destroy();
    }
};

class MyPanel : public wxPanel
{
public:
    MyPanel(wxWindow *parent);

private:
};

class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        if (!wxApp::OnInit())
            return false;
        MainFrame *frame = new MainFrame("Graphics Window – wxWidgets");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
