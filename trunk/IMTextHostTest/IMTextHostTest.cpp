// IMTextHostTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <atlbase.h>
#include <gdiplus.h>
#include "IMTextHostTest.h"
#include "im_richedit/im_richedit_sdk.h"
using namespace im_richedit;
using namespace Gdiplus;

#define MAX_LOADSTRING 100

Bitmap* background_image = NULL;
COLORREF halftone_color;
ULONG_PTR gdiplus_token = 0;

namespace {

COLORREF HalftoneBitmap(Bitmap* bitmap) {
  BitmapData bitmap_data;
  int img_width = background_image->GetWidth();
  int img_height = background_image->GetHeight();
  Rect lock_rect(0, 0, img_width, img_height);
  background_image->LockBits(&lock_rect, ImageLockModeWrite,
                             PixelFormat24bppRGB, &bitmap_data);
  unsigned char* pixel = static_cast<unsigned char*>(bitmap_data.Scan0);
  DWORD sum_h = 0;
  DWORD sum_l = 0;
  DWORD sum_s = 0;
  // 1.计算半色调
  int max_h = (img_width > 100) ? 100 : img_width;
  int max_v = (img_height > 100) ? 100 : img_height;
  for (int i = 0; i < max_v; ++i) {
    int offset = i * bitmap_data.Stride;
    for (int j = 0; j < max_h; ++j) {
      unsigned char* it = pixel + offset + j * 3;
      unsigned char b = *it++;
      unsigned char g = *it++;
      unsigned char r = *it++;
      WORD h, l, s;
      ColorRGBToHLS(RGB(r, g, b), &h, &l, &s);
      sum_h += h;
      sum_l += l;
      sum_s += s;
    }
  }
  COLORREF halftone_color = ColorHLSToRGB(
      static_cast<WORD>(sum_h / (max_h * max_v)),
      static_cast<WORD>(sum_l / (max_h * max_v)),
      static_cast<WORD>(sum_s / (max_h * max_v)));
  unsigned char halftone_color_r = GetRValue(halftone_color);
  unsigned char halftone_color_g = GetGValue(halftone_color);
  unsigned char halftone_color_b = GetBValue(halftone_color);
  // 2.对图片横向渐变融合
  int multiply = (img_width >= 255) ? 255 : img_width;
  for (int i = 0; i < img_height; ++i) {
    int offset = i * bitmap_data.Stride;
    for (int j = img_width - multiply; j < img_width; ++j) {
      unsigned char* it = pixel + offset + j * 3;
      float alpha_src = static_cast<float>(img_width - j) / multiply;
      *it = static_cast<unsigned char>(*it * alpha_src +
                                       halftone_color_b * (1.0f - alpha_src));
      ++it;
      *it = static_cast<unsigned char>(*it * alpha_src +
                                       halftone_color_g * (1.0f - alpha_src));
      ++it;
      *it = static_cast<unsigned char>(*it * alpha_src +
                                       halftone_color_r * (1.0f - alpha_src));
      ++it;
    }
  }
  // 2.对图片纵向渐变融合
  multiply = (img_height >= 255) ? 255 : img_height;
  for (int i = img_height - multiply; i < img_height; ++i) {
    int offset = i * bitmap_data.Stride;
    for (int j = 0; j < img_width; ++j) {
      unsigned char* it = pixel + offset + j * 3;
      float alpha_src = (float)(img_height - i) / 255;
      *it = static_cast<unsigned char>(*it * alpha_src +
                                       halftone_color_b * (1.0f - alpha_src));
      ++it;
      *it = static_cast<unsigned char>(*it * alpha_src +
                                       halftone_color_g * (1.0f - alpha_src));
      ++it;
      *it = static_cast<unsigned char>(*it * alpha_src +
                                       halftone_color_r * (1.0f - alpha_src));
      ++it;
    }
  }
  background_image->UnlockBits(&bitmap_data);
  
  return halftone_color;
}

}  // namespace

class TextHostExtDelegate : public IMRichEditDelegate,
                            public IIMTexHostDelegate {
 public:
  TextHostExtDelegate() : host_window_(NULL) {}
  ~TextHostExtDelegate() {}

  void set_host_window(HWND host_window) { host_window_ = host_window; }

  void EraseBackground(HDC dc, const RECT& rect) {
    COLORREF old_color = SetBkColor(dc, halftone_color);
    if(old_color != CLR_INVALID) {
      ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
      SetBkColor(dc, old_color);
    }
    {
      Graphics g(dc);
      g.DrawImage(background_image, rect.left, rect.top, rect.left, rect.top,
                  rect.right - rect.left, rect.bottom - rect.top, UnitPixel);
    }
  }

  void PostRenderRichObject(ULONG richobject_id, HDC dc, const RECT& rect) {
  }

  HWND TxGetHostWindow() { return host_window_; }

  BOOL TxRegisterDragDrop(LPDROPTARGET pDropTarget) {
    return RegisterDragDrop(host_window_, pDropTarget);
  }

  void TxRevokeDragDrop() { RevokeDragDrop(host_window_); }

  BOOL TxEnableScrollBar(UINT sb_flags, UINT arrows) {
    return EnableScrollBar(host_window_, sb_flags, arrows);
  }

  BOOL TxShowScrollBar(int bar, BOOL show) {
    return ShowScrollBar(host_window_, bar, show);
  }

  int  TxSetScrollInfo(int bar, LPCSCROLLINFO lpsi, BOOL redraw) {
    return SetScrollInfo(host_window_, bar, lpsi, redraw);
  }

  BOOL TxSetScrollRange(int bar, int min_pos, int max_pos, BOOL redraw) {
    return SetScrollRange(host_window_, bar, min_pos, max_pos, redraw);
  }

  int  TxSetScrollPos(int bar, int pos, BOOL redraw) {
    return SetScrollPos(host_window_, bar, pos, redraw);
  }

  void TxInvalidateRect(LPCRECT rect, BOOL erase) {
    InvalidateRect(host_window_, rect, erase);
  }
  
  void TxSetCapture() { SetCapture(host_window_); }
  
  void TxReleaseCapture() { ReleaseCapture(); }
  
  void TxSetFocus() { SetFocus(host_window_); }

  LRESULT OnTxNotify(DWORD iNotify, void* pv) { return S_OK; }

 private:
  HWND host_window_;
};

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
IIMTexHost* text_host = NULL;
TextHostExtDelegate text_host_delegate;
IMRichEdit* richedit = NULL;

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

  ::OleInitialize(NULL);

  GdiplusStartupInput gdiplus_startup_input;
  ATLVERIFY(GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, NULL) == Ok);
  WCHAR path[MAX_PATH] = { 0 };
  GetModuleFileNameW(NULL, path, MAX_PATH);
  PathRemoveFileSpecW(path);
  PathRemoveFileSpecW(path);
  PathAppendW(path, L"faces");
  PathAppendW(path, L"im_background.jpg");
  background_image = Bitmap::FromFile(path);
  ATLASSERT(background_image);
  halftone_color = HalftoneBitmap(background_image);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMTEXTHOSTTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IMTEXTHOSTTEST));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

  delete background_image;
  GdiplusShutdown(gdiplus_token);

  ::OleUninitialize();

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IMTEXTHOSTTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_IMTEXTHOSTTEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void InsertPicture() {
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
  if (richedit) {
    WCHAR path[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, path, MAX_PATH);
    PathRemoveFileSpecW(path);
    PathRemoveFileSpecW(path);
    PathAppendW(path, L"faces");
    PathAppendW(path, pictures[rand() % (ARRAYSIZE(pictures))]);

    ULONG id = richedit->InsertRichObject(
        im_richedit::IMRichObjectSystemPicture);
    richedit->SetRichObjectPicture(id, path);
  }
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
	  case WM_COMMAND:
		  wmId    = LOWORD(wParam);
		  wmEvent = HIWORD(wParam);
		  // Parse the menu selections:
		  switch (wmId) {
		    case IDM_ABOUT:
			    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			    break;
		    case IDM_EXIT:
			    DestroyWindow(hWnd);
			    break;
        case ID_FILE_INSERTPICTURE:
          InsertPicture();
          break;
        case ID_FILE_APPENDTEXT: {
          static LPCWSTR kContactList[] = {
            L"北京-富星星(5798XXXX)  12:57:15",
            L"北京-富蓝色(7058XXXX)  12:56:06"
          };
          static const COLORREF kColorList[] = {
            RGB(65, 180, 120),
            RGB(0, 110, 255)
          };

          if (richedit) {
            static int line_count = 0;
            ++line_count;

            richedit->SaveSelectionCharFormat();
            richedit->SetCaretToEnd();
            richedit->SetSelectionCharColor(kColorList[line_count % 2]);
            richedit->InsertText(kContactList[line_count % 2]);
            richedit->SetCaretToEnd();
            richedit->RestoreSelectionCharFormat(-1);
            richedit->InsertBreak();

            //richedit->SetParaStartIndent(20);
            InsertPicture();
            richedit->InsertBreak();
            //richedit->SetParaStartIndent(0);
            richedit->SetCaretToEnd();
            richedit->ScrollToCaret();
          }
          break;
        }
        case ID_FILE_SETTEXTCOLOR: {
          COLORREF clr = RGB(0, 255, 255);
          richedit->SetCharColor(clr);
          clr = richedit->GetCharColor();
          break;
        }
		    default:
			    return DefWindowProc(hWnd, message, wParam, lParam);
		  }
		  break;
    case WM_PAINT: {
		  hdc = BeginPaint(hWnd, &ps);
      
      HDC mem_dc = IMGetThreadMemDC();
      COLORREF old_color = SetBkColor(mem_dc, halftone_color);
      if(old_color != CLR_INVALID) {
        ExtTextOut(mem_dc, 0, 0, ETO_OPAQUE, &ps.rcPaint, NULL, 0, NULL);
        SetBkColor(mem_dc, old_color);
      }
      {
        Graphics g(mem_dc);
        g.DrawImage(background_image, ps.rcPaint.left, ps.rcPaint.top,
                    ps.rcPaint.left, ps.rcPaint.top,
                    ps.rcPaint.right - ps.rcPaint.left,
                    ps.rcPaint.bottom - ps.rcPaint.top, UnitPixel);
      }
      if (text_host) {
        text_host->TxSendMessage(WM_PAINT, reinterpret_cast<WPARAM>(mem_dc),
                                 0, NULL);
      }

      BitBlt(ps.hdc, ps.rcPaint.left, ps.rcPaint.top,
             ps.rcPaint.right - ps.rcPaint.left,
             ps.rcPaint.bottom - ps.rcPaint.top,
             mem_dc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
		  EndPaint(hWnd, &ps);
		  break;
    }
    case WM_CREATE: {
      text_host_delegate.set_host_window(hWnd);
      RECT client_rect;
      GetClientRect(hWnd, &client_rect);
      text_host = CreateIMTexHost(WS_VSCROLL | ES_MULTILINE | ES_NOHIDESEL,
                                  &text_host_delegate);

      CComPtr<IRichEditOle> richedit_ole;
      text_host->TxSendMessage(
          EM_GETOLEINTERFACE, 0, reinterpret_cast<LPARAM>(&richedit_ole), NULL);
      richedit = CreateIMRichEdit(richedit_ole, &text_host_delegate);
      break;
    }
	  case WM_DESTROY:
      if (richedit) {
        richedit->DeleteThis();
        richedit = NULL;
      }
      if (text_host) {
        text_host->TxSendMessage(WM_DESTROY, 0, 0, NULL);
        text_host->Release();
        text_host = NULL;
      }
		  PostQuitMessage(0);
		  break;
    case WM_ERASEBKGND:
      return 1;
    case WM_KILLFOCUS:
    case WM_LBUTTONDBLCLK:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEACTIVATE:
    case WM_MOUSELEAVE:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL:
    case WM_RBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_SETCURSOR:
    case WM_SETFOCUS:
    case WM_SIZE:
    case WM_TIMER:
    case WM_VSCROLL: {
      if (text_host) {
        LRESULT lr;
        text_host->TxSendMessage(message, wParam, lParam, &lr);
        return lr;
      }
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
    case WM_CHAR:
    case WM_IME_CHAR:
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP: {
      HRESULT hr;
      if (text_host)
        hr = text_host->TxSendMessage(message, wParam, lParam, NULL);
      if (hr == S_MSG_KEY_IGNORED)
        return DefWindowProc(hWnd, message, wParam, lParam);
      break;
    }
    case WM_IME_COMPOSITION:
    case WM_IME_COMPOSITIONFULL:
    case WM_IME_ENDCOMPOSITION:
    case WM_IME_NOTIFY:
    case WM_IME_STARTCOMPOSITION:
    case WM_INPUTLANGCHANGEREQUEST: {
      HRESULT hr = S_FALSE;
      if (text_host)
        hr = text_host->TxSendMessage(message, wParam, lParam, NULL);
      if (hr != S_OK)
        return DefWindowProc(hWnd, message, wParam, lParam);
      break;
    }
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
