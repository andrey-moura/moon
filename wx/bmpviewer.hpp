#pragma once

#include <wx/wx.h>
#include <wx/dcbuffer.h>

class wxBitmapView : public wxWindow
{
public:	
	wxBitmapView(wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr)
		: wxWindow(parent, id, pos, size, style, name)
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
		: wxWindow(parent, id, pos, size, style, name), m_pBitmap(bitmap)
	{
		SetDoubleBuffered(true);
		SetBackgroundStyle(wxBackgroundStyle::wxBG_STYLE_PAINT);

		Bind(wxEVT_PAINT, &wxBitmapView::OnPaintEvent, this);
	}
private:
	wxBitmap* m_pBitmap = nullptr;
	size_t m_Scale = 1;
public:
	void SetScale(size_t scale)
	{
		m_Scale = scale;
		Refresh();
	}
	void SetBitmap(wxBitmap* bitmap)
	{
		m_pBitmap = bitmap;
		Refresh();
	}
private:
	void Draw(wxDC& dc)
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
		Draw(dc);

		event.Skip();
	}
};