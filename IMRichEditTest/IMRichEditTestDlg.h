
// IMRichEditTestDlg.h : ͷ�ļ�
//

#pragma once

#include "im_richedit/im_richedit_sdk.h"

// CIMRichEditTestDlg �Ի���
class CIMRichEditTestDlg : public CDialog,
                           public im_richedit::IMRichEditDelegate
{
// ����
public:
	CIMRichEditTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IMRICHEDITTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

  void EraseBackground(HDC dc, const RECT& rect);
  void PostRenderRichObject(ULONG richobject_id, HDC dc, const RECT& rect);
  im_richedit::IMRichEdit* richedit_;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
