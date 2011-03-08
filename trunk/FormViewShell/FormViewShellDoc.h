// FormViewShellDoc.h : CFormViewShellDoc 클래스의 인터페이스
//


#pragma once


class CFormViewShellDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CFormViewShellDoc();
	DECLARE_DYNCREATE(CFormViewShellDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현입니다.
public:
	virtual ~CFormViewShellDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	// 설정파일 경로
	CString m_IniFilePath;
	// tcl 파일경로(상대경로위해)
	CString m_tclFilePath;
	CString m_TiltleString;
	// 저장데이터를 최초 로드
	BOOL m_bIsFirstLoad;
};


