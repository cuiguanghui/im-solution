
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

// RichEditʹ��ע��:
//   1.����CFE_LINK����������AutoURLDetect�ᵼ��RichEdit������ǰWord�Ƿ�Ϊ����.
//     ���������������, ��������CFE_LINK�����ո��Ա��Word���ֳ���.
//     (���鲻Ҫ����AutoURLDetect, ������������Ľӿڲ���.)
//   2.��Ҫ��ITextServices����EM_SCROLLCARET��Ϣ, ��������ES_NOHIDESEL���, ����
//     ����EM_HIDESELECTION��Ϣ�ı�����(�Զ��������ײ�����).
//   3.����RichEdit��صı�̾���ʹ������Ľӿڲ���, �������δ֪��Ϊ����!!!

namespace im_richedit {

enum IMRichObjectType {
  IMRichObjectCustomPicture,  // �Զ���ͼƬ
  IMRichObjectSystemPicture,  // ϵͳͼƬ
  IMRichObjectFancyCharacter, // �Ų��ַ�
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
  virtual BSTR GetCharFace() const = 0;  // ע��, ���ص�BSTR��Ҫ�ͷ�!!!
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
  // picture_filepath��������СΪMAX_PATH.
  virtual bool GetRichObjectPicture(ULONG richobject_id,
                                     wchar_t* picture_filepath) const = 0;
  virtual bool SetRichObjectPicture(ULONG richobject_id,
                                     const wchar_t* picture_filepath) = 0;
  // Tag����:
  //   IMRichObjectCustomPicture:  �Զ���
  //   IMRichObjectSystemPicture:  ϵͳ���
  //   IMRichObjectFancyCharacter: �ַ�ֵ
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
