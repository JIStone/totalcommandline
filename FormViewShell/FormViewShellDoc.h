// FormViewShellDoc.h : CFormViewShellDoc Ŭ������ �������̽�
//


#pragma once


class CFormViewShellDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CFormViewShellDoc();
	DECLARE_DYNCREATE(CFormViewShellDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CFormViewShellDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	// �������� ���
	CString m_IniFilePath;
	// tcl ���ϰ��(���������)
	CString m_tclFilePath;
	CString m_TiltleString;
	// ���嵥���͸� ���� �ε�
	BOOL m_bIsFirstLoad;
};


