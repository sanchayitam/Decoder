// AML DecoderDlg.h : header file
//

#if !defined(AFX_AMLDECODERDLG_H__F3DED0D3_F3AF_48CF_910A_91BF3FBA3E46__INCLUDED_)
#define AFX_AMLDECODERDLG_H__F3DED0D3_F3AF_48CF_910A_91BF3FBA3E46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CAMLDecoderDlg dialog

class CAMLDecoderDlg : public CDialog
{
// Construction
public:
	CAMLDecoderDlg(CWnd* pParent = NULL);	// standard constructor
    //friend void print_text(char* Message);
// Dialog Data
	//{{AFX_DATA(CAMLDecoderDlg)
	enum { IDD = IDD_AMLDECODER_DIALOG };
	CEdit	m_InputEdit;
	CEdit	m_OutputEdit;
	CEdit	m_InputWindow;
	CButton	m_Help;
	CButton	m_Decoder;
	CButton	m_Clear;
	CButton	m_BrowseFile;
	CString FileName;
	CString FileExtension;
	CString	m_OutputFile;
	int		m_Radio;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMLDecoderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAMLDecoderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDecoder();
	afx_msg void OnFile();
	afx_msg void OnClear();
	afx_msg void OnChangeEdit1();
	afx_msg void OnCp();
	afx_msg void OnSmp();
	afx_msg void OnMm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMLDECODERDLG_H__F3DED0D3_F3AF_48CF_910A_91BF3FBA3E46__INCLUDED_)
