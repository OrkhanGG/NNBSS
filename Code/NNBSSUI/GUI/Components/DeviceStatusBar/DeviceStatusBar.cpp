/////////////////////////////////////////////////////////////////////////////
// Name:        DeviceStatusBar.cpp
// Purpose:     Displays Hardware Performance
// Author:      Orkhan Aliyev
// Created:     02-04-2022
// Copyright:   (c) Orkhan Aliyev	
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "DeviceStatusBar.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/control.h"
#include "wx/settings.h"
#include "wx/dc.h"
#include "wx/dcclient.h"
#endif // WX_PRECOMP
#include "wx/dcgraph.h"

#include "DeviceStatusBar.h"

// NNBSS Bridge
#include <NNBSSBridge/MainBridge.h>

void CNNBSSDeviceStatusBar::_SetupInterface()
{
	this->SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);

	// Create widgets
		// CPU
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_CPU_BARNAME)->SetLabel(wxString::Format("CPU Usage(%s):",
		CNNBSSHardwareAddressHandle()->GetCpuInfo()));
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_CPU_BARNAME)->_SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_CPU_BARNAME)->_SetBorderColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_CPU_BARNAME)->_SetIndicatorColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_CPU_BARNAME)->_SetTextColour(*wxWHITE);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_CPU_BARNAME)->_SetTextFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT, false));
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_CPU_BARNAME)->_ShowText(true);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_CPU_BARNAME)->_SetMinMax(0, 100);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_CPU_BARNAME)->_SetCurrent(0);
	// Virtual Memory
	float _totalVM, _availableVM, _reservedVM, _percentageVM;
	CNNBSSHardwareAddressHandle()->GetVirtualMemoryInfo(_totalVM, _availableVM, _reservedVM, _percentageVM);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME)->SetLabel(wxString::Format("Virtual Memory Usage (%i):", (int)_totalVM));
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME)->_SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME)->_SetBorderColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME)->_SetIndicatorColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME)->_SetTextColour(*wxWHITE);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME)->_SetTextFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT, false));
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME)->_ShowText(true);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME)->_SetMinMax(0, 100);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME)->_SetCurrent(0);

	// RAM
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_RAM_BARNAME)->SetLabel(wxString::Format("RAM Usage(%iGB[%f]):",
		(int)floor(CNNBSSHardwareAddressHandle()->GetRamSize() + 0.5), CNNBSSHardwareAddressHandle()->GetRamSize()));
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_RAM_BARNAME)->_SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_RAM_BARNAME)->_SetBorderColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_RAM_BARNAME)->_SetIndicatorColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_RAM_BARNAME)->_SetTextColour(*wxWHITE);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_RAM_BARNAME)->_SetTextFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT, false));
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_RAM_BARNAME)->_ShowText(true);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_RAM_BARNAME)->_SetMinMax(0, 100);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_RAM_BARNAME)->_SetCurrent(0);

	// Memory
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_MEMORY_BARNAME)->SetLabel("Memory:");
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_MEMORY_BARNAME)->_SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_MEMORY_BARNAME)->_SetBorderColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_MEMORY_BARNAME)->_SetIndicatorColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_MEMORY_BARNAME)->_SetTextColour(*wxWHITE);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_MEMORY_BARNAME)->_SetTextFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT, false));
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_MEMORY_BARNAME)->_ShowText(true);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_MEMORY_BARNAME)->_SetMinMax(0, 100);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_MEMORY_BARNAME)->_SetCurrent(0);

	// Disk Space
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_DISKSPACE_BARNAME)->SetLabel("Disk Space:");
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_DISKSPACE_BARNAME)->_SetBackgroundColour(RGB_COLOURDEF_TRIPLE20);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_DISKSPACE_BARNAME)->_SetBorderColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_DISKSPACE_BARNAME)->_SetIndicatorColour(RGB_COLOURDEF_NNBSSBLUEHIGHLIGHT);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_DISKSPACE_BARNAME)->_SetTextColour(*wxWHITE);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_DISKSPACE_BARNAME)->_SetTextFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT, false));
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_DISKSPACE_BARNAME)->_ShowText(true);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_DISKSPACE_BARNAME)->_SetMinMax(0, 100);
	CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_DISKSPACE_BARNAME)->_SetCurrent(0);

	// GPU Name
	wxStaticText* p_GPUName = new wxStaticText(this, wxID_ANY, wxString::Format("GPU:%s",CNNBSSHardwareAddressHandle()->GetGPUName()));
	p_GPUName->SetForegroundColour(RGB_COLOURDEF_WHITE);

	// don't forget to change this whenever adding (a) new item(s)
	int verticalColumns = 3;
	int horizontalColumns = 2;
	wxSizer* p_MainSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* p_ColumnsSizer = new wxGridSizer(verticalColumns,horizontalColumns,5,5);

	// Create Grid Sizers
	// CPU Grid Sizer
	p_ColumnsSizer->Add(CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_CPU_BARNAME), wxSizerFlags().Proportion(1).Expand());
	// Virtual Memory Grid Sizer
	p_ColumnsSizer->Add(CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_VIRTUALMEMORY_BARNAME), wxSizerFlags().Proportion(1).Expand());
	// RAM Grid Sizer
	p_ColumnsSizer->Add(CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_RAM_BARNAME), wxSizerFlags().Proportion(1).Expand());
	// Memory Grid Sizer
	p_ColumnsSizer->Add(CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_MEMORY_BARNAME), wxSizerFlags().Proportion(1).Expand());
	// Disk Space Grid Sizer
	p_ColumnsSizer->Add(CNNBSSStatusProgressBarDeviceStatusAddressHandle(this, NNBSS_STATUSBAR_DISKSPACE_BARNAME), wxSizerFlags().Proportion(1).Expand());
	// GPU Name
	p_ColumnsSizer->Add(p_GPUName);

	p_MainSizer->Add(p_ColumnsSizer, wxSizerFlags().Proportion(1).Expand());
	p_MainSizer->FitInside(this);
	p_MainSizer->Layout();
	SetSizer(p_MainSizer);
	Layout();

	SetMinSize(GetBestSize() + wxSize(10, 0));
	SetSize(GetBestSize()+wxSize(10,0));

	//wxStaticText* p_CPUUsageText = new wxStaticText(this,wxID_ANY, wxString::Format("CPU Usage: 0") );

}

CNNBSSStatusProgressBar::CNNBSSStatusProgressBar(wxWindow* parent) :
	wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
	, _boolShowText(false)
{
	Bind(wxEVT_PAINT, &CNNBSSStatusProgressBar::paintEvent, this);
}

void CNNBSSStatusProgressBar::paintEvent(wxPaintEvent& WXUNUSED(evt))
{
	wxBufferedPaintDC pDc(this);
	wxGCDC dc(pDc);
	wxGraphicsRenderer* const renderer = wxGraphicsRenderer::GetDefaultRenderer();
	wxGraphicsContext* context = renderer->CreateContext(pDc);
	dc.SetGraphicsContext(context);
	dc.GetGraphicsContext()->SetBrush(GetBackgroundColour());
	dc.GetGraphicsContext()->SetPen(*wxTRANSPARENT_PEN);
	
	dc.SetPen(*wxTRANSPARENT_PEN);// not to draw additional borders with usual dc

	// fill background
	dc.GetGraphicsContext()->DrawRectangle(0, 0, GetSize().x, GetSize().y);

	auto background_brush = wxBrush(GetBackgroundColour(), wxBRUSHSTYLE_SOLID);
	dc.SetBackground(background_brush);
	dc.Clear();

	wxSize fitSize(GetSize());

	if (fitSize.GetWidth() < 1 || fitSize.GetHeight() < 1)
		return;


	wxRect wallRectangle(0, 0, fitSize.GetWidth(), fitSize.GetHeight());

	// draw background
	dc.SetBrush(wxBrush(_backgroundColour));
	dc.DrawRectangle(wallRectangle);

	if (_current > _max)
	{
		_current = _max;
	}
	else if (_current < _min)
	{
		_current = _min;
	}

	double _rangeBased = ((_current - _min) * 100.0) / (_max - _min);

	wxRect current(0, 0, fitSize.GetWidth() * (int)_rangeBased / 100, fitSize.GetHeight());

	// draw indicator
	wxColour _indicatorColourDependent;
	if (_rangeBased < 50)
	{
		_indicatorColourDependent = _indicatorColour;
	}
	else if (_rangeBased >= 50 && _rangeBased < 80)
	{
		_indicatorColourDependent = RGB_COLOURDEF_LIKEORANGE;
	}
	else if (_rangeBased >= 80)
	{
		_indicatorColourDependent = RGB_COLOURDEF_RED;
	}
	dc.SetBrush(wxBrush(_indicatorColourDependent));
	if(_rangeBased >= 1)
	dc.GetGraphicsContext()->DrawRoundedRectangle(current.x,current.y,current.width,current.height,5);

	// draw border
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(wxPen(_borderColour, 1));
	dc.GetGraphicsContext()->DrawRoundedRectangle(0, 0, wallRectangle.width-1, wallRectangle.height-1,5);

	if (_boolShowText)
	{
		// draw text
		dc.SetFont(_textFont);
		dc.SetTextForeground(_textColour);
		dc.DrawLabel(wxString::Format("%s%i%%", _label, (int)_rangeBased), wallRectangle, wxALIGN_CENTER);
	}
}

void CNNBSSStatusProgressBar::paintNow()
{
	Refresh();
}