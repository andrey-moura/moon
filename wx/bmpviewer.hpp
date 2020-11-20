#pragma once

#include <wx/wx.h>
#include <wx/vscroll.h>
#include <wx/dcbuffer.h>

class wxBitmapView : public wxHVScrolledWindow
{
public:	
	wxBitmapView(wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr)
		: wxHVScrolledWindow(parent, id, pos, size, style, name)
	{
		SetDoubleBuffered(true);
		SetBackgroundStyle(wxBackgroundStyle::wxBG_STYLE_PAINT);

		Bind(wxEVT_PAINT, &wxBitmapView::OnPaintEvent, this);
	}

	wxBitmapView(wxWindow* parent,
		wxWindowID id,
		wxBitmap* bitmap,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr)
		: wxHVScrolledWindow(parent, id, pos, size, style, name), m_pBitmap(bitmap)
	{
		SetDoubleBuffered(true);
		SetBackgroundStyle(wxBackgroundStyle::wxBG_STYLE_PAINT);

		Bind(wxEVT_PAINT, &wxBitmapView::OnPaintEvent, this);
	}

	virtual wxCoord OnGetRowHeight(size_t row) const
	{
		return m_Scale;
	}
	virtual wxCoord OnGetColumnWidth(size_t col) const
	{
		return m_Scale;
	}
private:
	wxBitmap* m_pBitmap = nullptr;
	size_t m_Scale = 1;
public:
	void SetScale(size_t scale)
	{
		ScrollToRowColumn(0, 0);
		m_Scale = scale;
		wxVarHScrollHelper::RefreshAll();
		wxVarVScrollHelper::RefreshAll();	
		Refresh();
	}
	void SetBitmap(wxBitmap* bitmap)
	{
		m_pBitmap = bitmap;

		if(!m_pBitmap) return;
		if(!m_pBitmap->IsOk()) return;

		SetRowColumnCount(bitmap->GetHeight(), bitmap->GetWidth());
		Refresh();
	}
private:
	void OnDraw(wxDC& dc)
	{
		dc.Clear();

		if (!m_pBitmap) return;

		if (!m_pBitmap->IsOk()) return;

		dc.SetUserScale(m_Scale, m_Scale);
		dc.DrawBitmap(*m_pBitmap, wxPoint(0, 0));
	}
	void OnPaintEvent(wxPaintEvent& event)
	{
		wxBufferedPaintDC dc(this);
		OnDraw(dc);
		PrepareDC(dc);

		event.Skip();
	}
};