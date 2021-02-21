#pragma onde

#include <wx/wx.h>
#include <wx/rawbmp.h>

#include <moon/wx/bmpviewer.hpp>

//Struct to hold undo operation info.
struct wxPixelEditorUndo
{
    uint16_t x;
    uint16_t y;
    uint8_t r;
    uint8_t g;
    uint8_t b;

    bool operator==(const wxPixelEditorUndo& other)
    {
        return memcmp(this, &other, sizeof(wxPixelEditorUndo)) == 0;
    }
};

//Maximum of 1024 bytes of undo.
static constexpr size_t wxPixelEditorMaxUndo = 1024/sizeof(wxPixelEditorUndo);

//A controls that displays a pixel bitmap and allows drawing on it.
class wxPixelEditor : public wxBitmapView
{
public:
    wxPixelEditor(wxWindow* parent, wxWindowID id)
        : wxBitmapView(parent, id)
        {
            m_UndoList.reserve(wxPixelEditorMaxUndo);

            Bind(wxEVT_LEFT_DOWN, &wxPixelEditor::OnMouseDown, this);
            Bind(wxEVT_RIGHT_DOWN, &wxPixelEditor::OnMouseDown, this);
            Bind(wxEVT_LEFT_UP, &wxPixelEditor::OnMouseUp, this);
            Bind(wxEVT_RIGHT_UP, &wxPixelEditor::OnMouseUp, this);
            Bind(wxEVT_MOTION, &wxPixelEditor::OnMotion, this);
            Bind(wxEVT_KEY_DOWN, &wxPixelEditor::OnKeyDown, this);
        }
private:
    bool m_PixelGrid = false;
    wxColour m_Palette[2] { *wxBLACK, *wxWHITE };
    wxMouseButton m_LastKey = wxMOUSE_BTN_NONE;
    std::vector<wxPixelEditorUndo> m_UndoList;
    wxPoint m_LastPoint { -1, -1 };
    wxColour m_LastColour;
public:
    void SetPixelGrid(bool grid)
    {
        m_PixelGrid = grid;
        Refresh();
    }

    bool GetPixelGrid() const
    {
        return m_PixelGrid;
    }    

    void SetPixel(int color, int x, int y)
    {
        SetPixel(m_Palette[color], x, y);
    }

    void SetPixel(const wxColour& color, int x, int y) 
    {
        wxNativePixelData data(*GetBitmap());
        wxNativePixelData::Iterator it = data.GetPixels();

        it.Offset(data, x, y);

        wxColour old_color;
        old_color.Set(it.Red(), it.Green(), it.Blue());

        it.Red() = color.Red();
        it.Green() = color.Green();
        it.Blue() = color.Blue();

        AddUndo(old_color, x, y);

        Refresh();        
    }

    void SetPixel(int color, wxPoint point)
    {
        point.x += GetVisibleColumnsBegin()*GetScale();
        point.y += GetVisibleRowsBegin()*GetScale();

        point.x /= GetScale();
        point.y /= GetScale();

        if(point.x > GetColumnCount() || point.y > GetRowCount() || point.x < 0 || point.y < 0)
        {
            return;
        }

        if(m_LastPoint == point)
        {
            if(m_LastColour == m_Palette[color])
            {
                //Don't draw several times on the same pixel. This is very useful for motion
                return;
            }
        }

        SetPixel(color, point.x, point.y);

        m_LastPoint = point;
        m_LastColour = m_Palette[color];
    }

    void Undo()
    {
        if(!m_UndoList.size())
            return;

        wxPixelEditorUndo undo = m_UndoList.back();
        m_UndoList.pop_back();

        wxColour color(undo.r, undo.g, undo.b);

        SetPixel(color, undo.x, undo.y);

        //Clear the last operation from undo
        m_UndoList.pop_back();
    }
private:
    void AddUndo(const wxColour& color, int x, int y)
    {
        wxPixelEditorUndo undo = {(uint16_t)x, (uint16_t)y, color.Red(), color.Green(), color.Blue()};

        if(m_UndoList.size() && undo == m_UndoList.back())
        {
            return;
        }

        m_UndoList.push_back(undo);

        if(m_UndoList.size() > wxPixelEditorMaxUndo)
        {
            m_UndoList.erase(m_UndoList.begin());
        }
    }
    void OnDraw(wxDC& dc) override
    {
        wxBitmapView::OnDraw(dc);

        if(!m_PixelGrid)
            return;

        dc.SetUserScale(1, 1);

        int row_start = GetVisibleRowsBegin();
        int col_start = GetVisibleColumnsBegin();

        int row_end = GetVisibleRowsEnd();
        int col_end = GetVisibleColumnsEnd();

        int scale = GetScale();

        dc.SetPen(*wxBLACK_PEN);

        for(int y = row_start+1; y < row_end; ++y)
        {
            dc.DrawLine(col_start*scale, y*scale, col_end*scale, y*scale);
        }

        for(int x = col_start+1; x < col_end; ++x)
        {
            dc.DrawLine(x*scale, row_start*scale, x*scale, row_end*scale);
        }
    }

    void OnMouseDown(wxMouseEvent& event)
    {
        int color = event.GetButton() == wxMouseButton::wxMOUSE_BTN_RIGHT;
        m_LastKey = (wxMouseButton)event.GetButton();
        wxPoint point = event.GetPosition();

        SetPixel(color, point);

        event.Skip();
    }

    void OnMouseUp(wxMouseEvent& event)
    {
        if(m_LastKey == event.GetButton())
        {
            m_LastKey = wxMOUSE_BTN_NONE;
        }
    }

    void OnMotion(wxMouseEvent& event)
    {
        if(m_LastKey == wxMouseButton::wxMOUSE_BTN_NONE)
        {
            event.Skip();
            return;
        }

        int color = m_LastKey == wxMouseButton::wxMOUSE_BTN_RIGHT;
        wxPoint point = event.GetPosition();        

        SetPixel(color, point);
    }

    void OnKeyDown(wxKeyEvent& event)
    {
        wxChar key = event.GetUnicodeKey();

        if(key == L'Z')
        {
            if(event.ControlDown())
            {
                Undo();
            }            
        }        
    }
};