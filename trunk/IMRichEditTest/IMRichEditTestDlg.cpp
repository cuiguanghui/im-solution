
// IMRichEditTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IMRichEditTest.h"
#include "IMRichEditTestDlg.h"

#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIMRichEditTestDlg 对话框

CIMRichEditTestDlg::CIMRichEditTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIMRichEditTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  richedit_ = NULL;
}

void CIMRichEditTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIMRichEditTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BUTTON_INSERT, &CIMRichEditTestDlg::OnBnClickedButtonInsert)
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_BUTTON_CREATE, &CIMRichEditTestDlg::OnBnClickedButtonCreate)
  ON_BN_CLICKED(IDC_BUTTON_DELETE, &CIMRichEditTestDlg::OnBnClickedButtonDelete)
  ON_BN_CLICKED(IDC_BUTTON_PARAINDENT, &CIMRichEditTestDlg::OnBnClickedButtonParaindent)
  ON_BN_CLICKED(IDC_BUTTON_LINENEW, &CIMRichEditTestDlg::OnBnClickedButtonLinenew)
  ON_BN_CLICKED(IDC_BUTTON_CURRENT_FONTSIZE, &CIMRichEditTestDlg::OnBnClickedButtonCurrentFontsize)
  ON_BN_CLICKED(IDC_BUTTON_CURRENT_FONTNAME, &CIMRichEditTestDlg::OnBnClickedButtonCurrentFontname)
  ON_BN_CLICKED(IDC_BUTTON_CURRENT_FONTBOLD, &CIMRichEditTestDlg::OnBnClickedButtonCurrentFontbold)
  ON_BN_CLICKED(IDC_BUTTON_CURRENT_FONTITALIC, &CIMRichEditTestDlg::OnBnClickedButtonCurrentFontitalic)
  ON_BN_CLICKED(IDC_BUTTON_CURRENT_TEXTCOLOR, &CIMRichEditTestDlg::OnBnClickedButtonCurrentTextcolor)
  ON_BN_CLICKED(IDC_BUTTON_CURRENT_SAVESTATE, &CIMRichEditTestDlg::OnBnClickedButtonCurrentSavestate)
  ON_BN_CLICKED(IDC_BUTTON_CURRENT_RESTORESTATE, &CIMRichEditTestDlg::OnBnClickedButtonCurrentRestorestate)
  ON_BN_CLICKED(IDC_BUTTON_APPENDTEXT, &CIMRichEditTestDlg::OnBnClickedButtonAppendtext)
  ON_BN_CLICKED(IDC_BUTTON_SCROLLTOEND, &CIMRichEditTestDlg::OnBnClickedButtonScrolltoend)
  ON_BN_CLICKED(IDC_BUTTON_APPENDLINK, &CIMRichEditTestDlg::OnBnClickedButtonAppendlink)
  ON_BN_CLICKED(IDC_BUTTON_COPY, &CIMRichEditTestDlg::OnBnClickedButtonCopy)
  ON_BN_CLICKED(IDC_BUTTON_CUT, &CIMRichEditTestDlg::OnBnClickedButtonCut)
  ON_BN_CLICKED(IDC_BUTTON_PASTE, &CIMRichEditTestDlg::OnBnClickedButtonPaste)
END_MESSAGE_MAP()


// CIMRichEditTestDlg 消息处理程序

BOOL CIMRichEditTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

  CWnd* edit = GetDlgItem(IDC_RICHEDIT21);
  edit->ModifyStyle(0, WS_CLIPSIBLINGS);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIMRichEditTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIMRichEditTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIMRichEditTestDlg::EraseBackground(HDC dc, const RECT& rect)
{
  COLORREF old_color = SetBkColor(dc, GetSysColor(COLOR_WINDOW));
  if(old_color != CLR_INVALID) {
    ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
    SetBkColor(dc, old_color);
  }
}

void CIMRichEditTestDlg::PostRenderRichObject(ULONG richobject_id,
                                              HDC dc, const RECT& rect) {
}

void CIMRichEditTestDlg::OnBnClickedButtonInsert()
{
  static LPCWSTR pictures[] = {
    L"{[W]SJ`1FUG%F3J0GE}I0JW.gif",
    L"6ZD$%HBUZ[H~@Y(R{46]9`1.gif",
    L"09O%LI_1[_I5QD8}PJDQWMV.gif",
    L"CWEE]V{CGAH7WS%Q8@32M@F.bmp",
    L"DY{GNN~H`71ATL}YSD2}0OK.gif",
    L"GRBZ2]TSQ2KV4{0(@E)Z3`7.gif",
    L"X1`E@`R9LLX~6VHSWO`NXY0.gif",
    L"Z~2OKACDD{L1UUZ0)BR01QJ.gif"
  };
  if (richedit_) {
    WCHAR path[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, path, MAX_PATH);
    PathRemoveFileSpecW(path);
    PathRemoveFileSpecW(path);
    PathAppendW(path, L"faces");
    PathAppendW(path, pictures[rand() % (ARRAYSIZE(pictures))]);

    ULONG id = richedit_->InsertRichObject(
        im_richedit::IMRichObjectSystemPicture);
    richedit_->SetRichObjectPicture(id, path);
  }
}

void CIMRichEditTestDlg::OnDestroy()
{
  if (richedit_) {
    richedit_->DeleteThis();
    richedit_ = NULL;
  }

  CDialog::OnDestroy();
}

void CIMRichEditTestDlg::OnBnClickedButtonCreate()
{
  if (!richedit_) {
    CWnd* edit = GetDlgItem(IDC_RICHEDIT21);
    CComPtr<IRichEditOle> richedit_ole;
    ::SendMessage(edit->GetSafeHwnd(), EM_GETOLEINTERFACE, 0,
                  reinterpret_cast<LPARAM>(&richedit_ole));
    richedit_ = CreateIMRichEdit(richedit_ole, this);
    ATLASSERT(richedit_);
  }
}

void CIMRichEditTestDlg::OnBnClickedButtonDelete()
{
  if (richedit_) {
    richedit_->DeleteThis();
    richedit_ = NULL;
  }
}

void CIMRichEditTestDlg::OnBnClickedButtonParaindent()
{
  //if (richedit_)
  //  richedit_->SetParaStartIndent(20);
}

void CIMRichEditTestDlg::OnBnClickedButtonLinenew()
{
  if (richedit_)
    richedit_->InsertBreak();
}

void CIMRichEditTestDlg::OnBnClickedButtonCurrentFontsize()
{
  if (richedit_)
    richedit_->SetSelectionCharSize(20);
}

void CIMRichEditTestDlg::OnBnClickedButtonCurrentFontname()
{
  if (richedit_)
    richedit_->SetSelectionCharFace(L"微软雅黑");
}

void CIMRichEditTestDlg::OnBnClickedButtonCurrentFontbold()
{
  if (richedit_)
    richedit_->SetSelectionCharBold(!richedit_->GetSelectionCharBold());
}

void CIMRichEditTestDlg::OnBnClickedButtonCurrentFontitalic()
{
  if (richedit_)
    richedit_->SetSelectionCharItalic(!richedit_->GetSelectionCharItalic());
}

void CIMRichEditTestDlg::OnBnClickedButtonCurrentTextcolor()
{
  if (richedit_)
    richedit_->SetSelectionCharColor(RGB(0, 255, 0));
}

void CIMRichEditTestDlg::OnBnClickedButtonCurrentSavestate()
{
  if (richedit_)
    richedit_->SaveSelectionCharFormat();
}

void CIMRichEditTestDlg::OnBnClickedButtonCurrentRestorestate()
{
  if (richedit_)
    richedit_->RestoreSelectionCharFormat(-1);
}

void CIMRichEditTestDlg::OnBnClickedButtonAppendtext()
{
  static LPCWSTR kContactList[] = {
    L"北京-富星星(5798XXXX)  12:57:15",
    L"北京-富蓝色(7058XXXX)  12:56:06"
  };
  static const COLORREF kColorList[] = {
    RGB(65, 180, 120),
    RGB(0, 110, 255)
  };

  if (richedit_) {
    static int line_count = 0;
    ++line_count;

    richedit_->SaveSelectionCharFormat();
    richedit_->SetCaretToEnd();
    richedit_->SetSelectionCharColor(kColorList[line_count % 2]);
    richedit_->InsertText(kContactList[line_count % 2]);
    richedit_->SetCaretToEnd();
    richedit_->RestoreSelectionCharFormat(-1);
    richedit_->InsertBreak();

    //richedit_->SetParaStartIndent(20);
    OnBnClickedButtonInsert();
    richedit_->InsertBreak();
    //richedit_->SetParaStartIndent(0);
    richedit_->SetCaretToEnd();
    richedit_->ScrollToCaret();
  }
}

void CIMRichEditTestDlg::OnBnClickedButtonScrolltoend()
{
  if (richedit_) {
    richedit_->SetCaretToEnd();
    richedit_->ScrollToCaret();
  }
}

void CIMRichEditTestDlg::OnBnClickedButtonAppendlink()
{
  if (richedit_) {
    richedit_->InsertText(L"(");
    richedit_->InsertLink(L"MSN");
    richedit_->InsertText(L")");
  }
}

void CIMRichEditTestDlg::OnBnClickedButtonCopy()
{
  if (richedit_)
    richedit_->Copy();
}

void CIMRichEditTestDlg::OnBnClickedButtonCut()
{
  if (richedit_)
    richedit_->Cut();
}

void CIMRichEditTestDlg::OnBnClickedButtonPaste()
{
  if (richedit_)
    richedit_->Paste();
}
