
#ifndef IM_RICHEDIT_IM_RICHEDIT_SDK_H_
#define IM_RICHEDIT_IM_RICHEDIT_SDK_H_

#include <windows.h>
#include <imm.h>
#include <richedit.h>
#include <richole.h>
#include <textserv.h>

#if defined(IM_RICHEDIT_IMPLEMENTATION)
#define IM_RICHEDIT_EXPORT __declspec(dllexport)
#else
#define IM_RICHEDIT_EXPORT __declspec(dllimport)
#endif  // defined(IM_RICHEDIT_IMPLEMENTATION)

// RichEdit使用注意:
//   1.设置CFE_LINK后立即调用AutoURLDetect会导致RichEdit解析当前Word是否为链接.
//     如果想避免这种情况, 必须在这CFE_LINK后插入空格以便把Word区分出来.
//     (建议不要设置AutoURLDetect, 尽可能用下面的接口操作.)
//   2.想要对ITextServices发送EM_SCROLLCARET消息, 必须设置ES_NOHIDESEL风格, 或者
//     发送EM_HIDESELECTION消息改变设置(自动滚动到底部功能).
//   3.所有RichEdit相关的编程尽量使用下面的接口操纵, 否则会有未知行为发生!!!

namespace im_richedit {

enum IMRichObjectType {
  IMRichObjectCustomPicture,  // 自定义图片
  IMRichObjectSystemPicture,  // 系统图片
  IMRichObjectFancyCharacter, // 炫彩字符
};

const ULONG kInvalidRichObjectId = -1;

class IMRichEditDelegate {
 public:
  virtual void EraseBackground(HDC dc, const RECT& rect) = 0;
  virtual void PostRenderRichObject(ULONG richobject_id,
                                    HDC dc, const RECT& rect) = 0;
};

class IMRichEdit {
 public:
  virtual void DeleteThis() = 0;
  virtual int  GetCharSize() const = 0;
  virtual void SetCharSize(int size) = 0;
  virtual BSTR GetCharFace() const = 0;  // 注意, 返回的BSTR需要释放!!!
  virtual void SetCharFace(const wchar_t* face_name) = 0;
  virtual bool GetCharBold() const = 0;
  virtual void SetCharBold(bool bold) = 0;
  virtual bool GetCharItalic() const = 0;
  virtual void SetCharItalic(bool italic) = 0;
  virtual COLORREF GetCharColor() const = 0;
  virtual void SetCharColor(COLORREF color) = 0;
  virtual int  GetSelectionCharSize() const = 0;
  virtual void SetSelectionCharSize(int size) = 0;
  virtual BSTR GetSelectionCharFace() const = 0;
  virtual void SetSelectionCharFace(const wchar_t* face_name) = 0;
  virtual bool GetSelectionCharBold() const = 0;
  virtual void SetSelectionCharBold(bool bold) = 0;
  virtual bool GetSelectionCharItalic() const = 0;
  virtual void SetSelectionCharItalic(bool italic) = 0;
  virtual COLORREF GetSelectionCharColor() const = 0;
  virtual void SetSelectionCharColor(COLORREF color) = 0;
  virtual int  SaveSelectionCharFormat() = 0;
  virtual bool RestoreSelectionCharFormat(int save_state) = 0;
  virtual void SelectAll() = 0;
  virtual void Cut() = 0;
  virtual void Copy() = 0;
  virtual void Paste() = 0;
  virtual void ResetContent() = 0;
  virtual void SetCaretToEnd() = 0;
  virtual void ScrollToCaret() = 0;
  virtual void InsertText(const wchar_t* text) = 0;
  virtual bool InsertLink(const wchar_t* text) = 0;
  virtual void InsertBreak() = 0;
  virtual ULONG InsertRichObject(IMRichObjectType type) = 0;
  virtual ULONG GetRichObjectId(IOleObject* ole_object) const = 0;
  virtual bool  GetRichObjectType(ULONG richobject_id,
                                  IMRichObjectType* type) const = 0;
  // picture_filepath缓冲区大小为MAX_PATH.
  virtual bool GetRichObjectPicture(ULONG richobject_id,
                                     wchar_t* picture_filepath) const = 0;
  virtual bool SetRichObjectPicture(ULONG richobject_id,
                                     const wchar_t* picture_filepath) = 0;
  // Tag含义:
  //   IMRichObjectCustomPicture:  自定义
  //   IMRichObjectSystemPicture:  系统编号
  //   IMRichObjectFancyCharacter: 字符值
  virtual bool GetRichObjectTag(ULONG richobject_id, int* tag) const = 0;
  virtual bool SetRichObjectTag(ULONG richobject_id, int tag) = 0;
  virtual bool GetRichObjectFrameCount(ULONG richobject_id,
                                       UINT* frame_count) const = 0;
  virtual bool GetRichObjectCurremtFrame(ULONG richobject_id,
                                         UINT* current_frame) const = 0;
};


class IIMTexHostDelegate {
 public:
  virtual HWND TxGetHostWindow() = 0;
  virtual BOOL TxRegisterDragDrop(LPDROPTARGET pDropTarget) = 0;
  virtual void TxRevokeDragDrop() = 0;
  virtual BOOL TxEnableScrollBar(UINT sb_flags, UINT arrows) = 0;
  virtual BOOL TxShowScrollBar(int bar, BOOL show) = 0;
  virtual int  TxSetScrollInfo(int bar, LPCSCROLLINFO lpsi, BOOL redraw) = 0;
  virtual BOOL TxSetScrollRange(int bar, int min_pos, int max_pos,
                                BOOL redraw) = 0;
  virtual int  TxSetScrollPos(int bar, int pos, BOOL redraw) = 0;
  virtual void TxInvalidateRect(LPCRECT rect, BOOL erase) = 0;
  virtual void TxSetCapture() = 0;
  virtual void TxReleaseCapture() = 0;
  virtual void TxSetFocus() = 0;
  virtual HRESULT OnTxNotify(DWORD iNotify, void* pv) = 0;
};

class IIMTexHost : public IUnknown {
 public:
  virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam,
                                LRESULT* plresult) = 0;
};

}  // namespace im_richedit

extern "C" {

IM_RICHEDIT_EXPORT HDC IMGetThreadMemDC();

IM_RICHEDIT_EXPORT im_richedit::IMRichEdit* CreateIMRichEdit(
    IRichEditOle* richedit_ole, im_richedit::IMRichEditDelegate* delegate);

IM_RICHEDIT_EXPORT im_richedit::IIMTexHost* CreateIMTexHost(
    DWORD style, im_richedit::IIMTexHostDelegate* delegate);

}  // extern "C"

#endif  // IM_RICHEDIT_IM_RICHEDIT_SDK_H_
