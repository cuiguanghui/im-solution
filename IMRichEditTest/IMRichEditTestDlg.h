
// IMRichEditTestDlg.h : 头文件
//

#pragma once

#include "im_richedit/im_richedit_sdk.h"

// CIMRichEditTestDlg 对话框
class CIMRichEditTestDlg : public CDialog,
                           public im_richedit::IMRichEditDelegate
{
// 构造
public:
	CIMRichEditTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IMRICHEDITTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

  void EraseBackground(HDC dc, const RECT& rect);
  void PostRenderRichObject(ULONG richobject_id, HDC dc, const RECT& rect);
  im_richedit::IMRichEdit* richedit_;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButtonInsert();
  afx_msg void OnDestroy();
  afx_msg void OnBnClickedButtonCreate();
  afx_msg void OnBnClickedButtonDelete();
  afx_msg void OnBnClickedButtonParaindent();
  afx_msg void OnBnClickedButtonLinenew();
  afx_msg void OnBnClickedButtonCurrentFontsize();
  afx_msg void OnBnClickedButtonCurrentFontname();
  afx_msg void OnBnClickedButtonCurrentFontbold();
  afx_msg void OnBnClickedButtonCurrentFontitalic();
  afx_msg void OnBnClickedButtonCurrentTextcolor();
  afx_msg void OnBnClickedButtonCurrentSavestate();
  afx_msg void OnBnClickedButtonCurrentRestorestate();
  afx_msg void OnBnClickedButtonAppendtext();
  afx_msg void OnBnClickedButtonScrolltoend();
  afx_msg void OnBnClickedButtonAppendlink();
  afx_msg void OnBnClickedButtonCopy();
  afx_msg void OnBnClickedButtonCut();
  afx_msg void OnBnClickedButtonPaste();
};
