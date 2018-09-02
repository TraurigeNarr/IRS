#pragma once
#include "PropertyGridCombo.h"
#include "PropertyGridInPlaceEdit.h"

#include <map>
#include <vector>
#include <string>

// CPropertyGrid

#define WM_PG_ITEMCHANGED WM_USER+486

typedef UINT HSECTION;
typedef UINT HITEM;

class ICustomItem;

class CPropertyGrid : public CWnd
{
  DECLARE_DYNAMIC(CPropertyGrid)

public:
  // display mode
  enum EDisplayMode
  {
    DM_CATEGORIZED = 0,
    DM_ALPHABETICAL,
    DM_NOSORT
  };

  // editing
  enum EEditMode
  {
    EM_CUSTOM = 0,
    EM_INPLACE,
    EM_DROPDOWN,
    EM_MODAL
  };

  enum EItemType
  {
    IT_CUSTOM = 0,
    IT_STRING,
    IT_TEXT,
    IT_INTEGER,
    IT_DOUBLE,
    IT_COMBO,
    IT_BOOLEAN,
    IT_DATE,
    IT_DATETIME,
    IT_FILE,
    IT_FOLDER,
    IT_COLOR,
    IT_FONT
  };

  class CItem
  {
  public:
    HITEM m_id;
    bool m_editable;
    bool m_undefined;
    EItemType m_type;
    std::wstring m_name;

    std::vector<std::wstring> m_options;

    int m_nValue;
    double m_dValue;
    std::wstring m_strValue;
    bool m_bValue;
    COleDateTime m_dtValue;
    COLORREF m_clrValue;
    LOGFONT m_lfValue;
    ICustomItem* m_pCustom;

    bool m_undefined_old;
    int m_nValue_old;
    double m_dValue_old;
    std::wstring m_strValue_old;
    bool m_bValue_old;
    COleDateTime m_dtValue_old;
    COLORREF m_clrValue_old;
    LOGFONT m_lfValue_old;

    CRect m_rcName;
    CRect m_rcValue;

    bool operator==(const HITEM& item) const;
    bool operator==(const std::wstring& name) const;

    void ValidateChanges();
  };

  class CSection
  {
  public:
    HSECTION m_id;
    std::wstring m_title;
    bool m_collapsed;
    std::vector<CItem> m_items;

    CRect m_rcSign;
    CRect m_rcTitle;

    bool operator==(const HSECTION& section) const;
  };

public:
  CPropertyGrid();
  virtual ~CPropertyGrid();

  // customization
  bool GetShadeTitles();
  void SetShadeTitles(bool shade_titles);
  bool GetDrawLines();
  void SetDrawLines(bool draw_lines);
  bool GetDrawGutter();
  void SetDrawGutter(bool draw_gutter);
  bool GetFocusDisabled();
  void SetFocusDisabled(bool focus_disabled);
  bool GetBoldModified();
  void SetBoldModified(bool bold_modified);
  bool GetBoldEditables();
  void SetBoldEditables(bool bold_editables);
  HITEM GetFocusedItem() const { return m_focused_item; }
  HSECTION GetFocusedSection() const { return m_focused_section; }
  
  // item management
  CSection* FindSection(HSECTION hs) const;

  // gutter width
  int GetGutterWidth();
  void SetGutterWidth(int gutter_width);

  // custom colors
  void SetTextColor(COLORREF clrText);
  void SetTitleColor(COLORREF clrText);
  void SetBackColor(COLORREF clrBack);
  void SetShadeColor(COLORREF clrShade);
  void SetFocusColor(COLORREF clrFocus);
  void SetHiliteColor(COLORREF clrHilite);
  void SetEditableColor(COLORREF clrEditable);
  void SetDisabledColor(COLORREF clrDisabled);

  // localization
  void SetTrueFalseStrings(std::wstring strTrue, std::wstring strFalse);
  void SetOkCancelStrings(std::wstring strOk, std::wstring strCancel);
  void SetDateTimeStrings(std::wstring strDate, std::wstring strTime);
  void SetUndefinedString(std::wstring strUndefined);
  void SetEmptyString(std::wstring strEmpty);

  // add a section
  HSECTION AddSection(std::wstring title, bool collapsed = false, HSECTION after = -1);

  // add items
  HITEM AddCustomItem(HSECTION, std::wstring name, ICustomItem* pItem, bool editable = true, HITEM after = -1);
  HITEM AddStringItem(HSECTION section, std::wstring name, std::wstring value, bool editable = true, HITEM after = -1);
  HITEM AddTextItem(HSECTION section, std::wstring name, std::wstring value, bool editable = true, HITEM after = -1);
  HITEM AddIntegerItem(HSECTION section, std::wstring name, int value, std::wstring format = L"", bool editable = true, bool undefined = false, HITEM after = -1);
  HITEM AddDoubleItem(HSECTION section, std::wstring name, double value, std::wstring format = L"", bool editable = true, bool undefined = false, HITEM after = -1);
  HITEM AddComboItem(HSECTION section, std::wstring name, const std::vector<std::wstring>& values, int cur, bool editable = true, bool undefined = false, HITEM after = -1);
  HITEM AddBoolItem(HSECTION section, std::wstring name, bool value, bool editable = true, bool undefined = false, HITEM after = -1);
  /*HITEM AddDateItem(HSECTION section, std::wstring name, COleDateTime value, std::wstring format = "", bool editable = true, bool undefined = false, HITEM after = -1);
  HITEM AddDateTimeItem(HSECTION section, std::wstring name, COleDateTime value, std::wstring format = "", bool editable = true, bool undefined = false, HITEM after = -1);
  HITEM AddFileItem(HSECTION section, std::wstring name, std::wstring value, std::wstring filter = "", bool editable = true, HITEM after = -1);
  HITEM AddFolderItem(HSECTION section, std::wstring name, std::wstring value, std::wstring title = "", bool editable = true, HITEM after = -1);
  HITEM AddColorItem(HSECTION section, std::wstring name, COLORREF value, bool editable = true, bool undefined = false, HITEM after = -1);
  HITEM AddFontItem(HSECTION section, std::wstring name, LOGFONT value, bool editable = true, bool undefined = false, HITEM after = -1);
*/

  // contents
  void ResetContents();
  bool RemoveSection(HSECTION hs);
  bool RemoveItem(HITEM item);
  void ValidateChanges();

  // status
  int GetNumSections();
  int GetSectionSize(HSECTION hs);

  // get item value
  bool GetItemValue(HITEM item, std::wstring& strValue) const;
  bool GetItemValue(HITEM item, int& nValue) const;
  bool GetItemValue(HITEM item, double& dValue) const;
  bool GetItemValue(HITEM item, bool& bValue) const;
  bool GetItemValue(HITEM item, COleDateTime& dtValue) const;
  bool GetItemValue(HITEM item, COLORREF& clrValue) const;
  bool GetItemValue(HITEM item, LOGFONT& lfValue) const;
  template <typename TCustom>
  bool GetItemValue(HITEM item, TCustom*& pCustom) const
  {
    // get the item
    CItem* pItem = FindItem(item);
    if (pItem == NULL) return false;

    // check
    if (pItem->m_type == IT_CUSTOM)
    {
      pCustom = static_cast<TCustom*>(pItem->m_pCustom);
      return true;
    }
    return false;
  }

  // set item value
  bool SetItemValue(HITEM item, const std::wstring strValue);
  bool SetItemValue(HITEM item, const int nValue);
  bool SetItemValue(HITEM item, const double nValue);
  bool SetItemValue(HITEM item, const bool bValue);
  bool SetItemValue(HITEM item, const COleDateTime dtValue);
  bool SetItemValue(HITEM item, const COLORREF clrValue);
  bool SetItemValue(HITEM item, const LOGFONT lfValue);
  template <typename TCustom>
  bool SetItemValue(HITEM item, TCustom* pCustom)
  {
    // get the item
    CItem* pItem = FindItem(item);
    if (pItem == NULL) return false;

    // check
    if (pItem->m_type == IT_CUSTOM && typeid(TCustom) == typeid(*pCustom))
    {
      pItem->m_pCustom = static_cast<TCustom*>(pCustom);
      Invalidate();
      return true;
    }
    return false;
  }

  // for custom items
  int GetTextMargin();
  CFont* GetFontNormal();
  CFont* GetFontBold();

  // appearance stuff
  void SetDisplayMode(EDisplayMode display_mode);
  void ExpandAll(bool expand);
  void ExpandSection(HSECTION hs, bool expand);
  bool IsSectionCollapsed(HSECTION hs);

protected:
  friend bool item_alpha_sort(std::vector<CPropertyGrid::CItem>::iterator it1, std::vector<CPropertyGrid::CItem>::iterator it2);

  std::vector<CSection> m_sections;

  HSECTION m_focused_section;
  HITEM m_focused_item;

  EDisplayMode m_display_mode;

  bool m_shade_titles;
  bool m_draw_lines;
  bool m_draw_gutter;
  bool m_focus_disabled;
  bool m_bold_modified;
  bool m_bold_editables;

  int m_gutter_width;
  bool m_resizing_gutter;
  CPoint m_ptLast;

  CFont m_fntNormal;
  CFont m_fntBold;

  int m_line_height;

  CRect m_rect_button;
  CWnd* m_control;
  bool m_button_pushed;
  bool m_button_depressed;
  bool m_value_clicked;
  bool m_custom_tracking;

  HSECTION m_section_id;
  HITEM m_item_id;

  std::wstring m_strTrue;
  std::wstring m_strFalse;
  std::wstring m_strOk;
  std::wstring m_strCancel;
  std::wstring m_strDate;
  std::wstring m_strTime;
  std::wstring m_strUndefined;
  std::wstring m_strEmpty;

  COLORREF m_clrText;
  COLORREF m_clrTitle;
  COLORREF m_clrBack;
  COLORREF m_clrShade;
  COLORREF m_clrFocus;
  COLORREF m_clrHilite;
  COLORREF m_clrEditable;
  COLORREF m_clrDisabled;

protected:
  DECLARE_MESSAGE_MAP()

  // init control
  void InitControl();

  // drawing
  void DrawItem(CDC& dc, int w, int x, int y, std::vector<CItem>::iterator& it);

  CItem* FindItem(HITEM hi) const;
  HITEM AddItem(HSECTION hs, EItemType type, std::wstring name, void* pValue, bool editable, bool undefined, HITEM after);

  // scrolling stuff
  CScrollBar m_scrollbar;
  bool m_scroll_enabled;
  int GetScrollOffset();
  void RecalcLayout();

  // editing
  EEditMode GetEditMode(CItem& item);
  void DeleteEditControl();
  void EditFocusedItem();

  // movement in list
  void MoveForward(HSECTION& focused_section, HITEM& focused_item);

  // keyboard
  void FocusNextItem();
  void FocusPrevItem();

protected:
  virtual void PreSubclassWindow();
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnPaint();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nHitTest, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg LRESULT OnComboSelChanged(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnEditChanged(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnDateChanged(WPARAM wParam, LPARAM lParam);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg UINT OnGetDlgCode();
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
