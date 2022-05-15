///////////////////////////////////////////////////////////////////////////////
// Name:        DetectionSamplesList.h
// Purpose:     Displays and let the user add or remove detection models
// Author:      Orkhan Aliyev
// Created:     2022-01-29 5:58AM
// Copyright:   (c) 2022 Orkhan Aliyev
///////////////////////////////////////////////////////////////////////////////

#ifndef DETECTIONSAMPLESLIST_H
#define DETECTIONSAMPLESLIST_H

class CNNBSSControlButton;

// Standart C Lib(s)
#include <vector>

class CNNBSSDetectionSamplesList : public wxPanel
{
public:
    CNNBSSDetectionSamplesList  (wxWindow* parent = nullptr);
    ~CNNBSSDetectionSamplesList ();

    void                        _SetupDesign();

    int                         _GetGridWidth();
    int                         _GetGridHeight();
private:
    void                        _SetGridWidth(int width);
    void                        _SetGridHeight(int height);
    void                        _SetBalancedGridScale(int balancedScale);
    bool                        ChangeGridSize(wxSize size);

    const wxString              _ButtonMainLabel{ _("Add") };
    const wxString              _ButtonNote{ _("Click to add a cascade classifier") };

    const int                   _MinGridWidth = 4;
    const int                   _MinGridHeight = 4;
    int                         _GridWidth = 4;
    int                         _GridHeight = 4;

    bool                        IfExists(const wxString& cascadeName);
    bool                        Add(const wxString& path,const wxString& cascadeName,const int& position);

    void                        UpdateList();
    void                        OnAnyButtonClick(wxMouseEvent& evt);
    enum                        DetectionModelEvents
    {
                                CMEID_LABEL = 205,
                                CMEID_REMOVE,
    };
    void                        OnContextMenu(wxContextMenuEvent& evt);

    void                        OnChangeLabel(const int& RowNumber);
    void                        RemoveDM(const int& RowNumber);
    void                        PropertiesDM(const int& RowNumber);

    // UI controls/widgets
    wxPanel*                    p_DetectionSamplesListBG = nullptr;
    wxBoxSizer*                 p_MainSizer = nullptr;
    wxGridSizer*                p_GridSizer = nullptr;

    struct SListElements
    {
        CNNBSSControlButton*    p_Button{nullptr};

        wxString                DisplayName;
        wxString                Label;
        wxString                FileName;
        wxString                FilePath;
        bool                    Works;
        bool                    IsEmpty{ true };
        int                     Position{0};
    };

    std::vector<SListElements>  m_ListElements;
};

#endif // #ifndef DETECTIONSAMPLESLIST_H