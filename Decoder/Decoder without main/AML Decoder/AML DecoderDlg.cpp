// AML DecoderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AML Decoder.h"
#include "AML DecoderDlg.h"
#include "Decoder.h"
#include<direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CBrush Brush(RGB(0,128,255));
CBitmap BrowseFile_bmp,Clear_bmp,Decoder_bmp,Help_bmp;
char *fName = 0 ,*DirName = 0;
char *CurrDir ;
CString fileName;
char bufr[80];
int fileFlag;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAMLDecoderDlg dialog

CAMLDecoderDlg::CAMLDecoderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAMLDecoderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAMLDecoderDlg)
	m_OutputFile = _T("");
	m_Radio = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAMLDecoderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAMLDecoderDlg)
	DDX_Control(pDX, IDC_EDIT1, m_InputEdit);
	DDX_Control(pDX, IDC_EDIT2, m_OutputEdit);
	DDX_Control(pDX, ID_HELP, m_Help);
	DDX_Control(pDX, IDC_Decoder, m_Decoder);
	DDX_Control(pDX, IDC_Clear, m_Clear);
	DDX_Control(pDX, IDC_File, m_BrowseFile);
	DDX_Radio(pDX, IDC_Cp, m_Radio);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAMLDecoderDlg, CDialog)
	//{{AFX_MSG_MAP(CAMLDecoderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Decoder, OnDecoder)
	ON_BN_CLICKED(IDC_File, OnFile)
	ON_BN_CLICKED(IDC_Clear, OnClear)
	ON_BN_CLICKED(IDC_Cp, OnCp)
	ON_BN_CLICKED(IDC_Smp, OnSmp)
	ON_BN_CLICKED(IDC_Mm, OnMm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAMLDecoderDlg message handlers

BOOL CAMLDecoderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

		

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	Clear_bmp.LoadBitmap(IDB_BITMAP1);
	m_Clear.SetBitmap((HBITMAP)Clear_bmp.GetSafeHandle());
	BrowseFile_bmp.LoadBitmap(IDB_BITMAP2);
	m_BrowseFile.SetBitmap((HBITMAP)BrowseFile_bmp.GetSafeHandle());
	Decoder_bmp.LoadBitmap(IDB_BITMAP3);
	m_Decoder.SetBitmap((HBITMAP)Decoder_bmp.GetSafeHandle());
	Help_bmp.LoadBitmap(IDB_BITMAP4);
	m_Help.SetBitmap((HBITMAP)Help_bmp.GetSafeHandle());
	
	
	// TODO: Add extra initialization here
	char buffer[80];
	memset(buffer,'/0',sizeof(buffer));
//	m_Inputwindow.GetDlgItem(IDC_LIST1);
//	m_Outputwindow.GetDlgItem(IDC_LIST2);
	m_OutputEdit.GetDlgItem(IDC_EDIT2);
	m_InputEdit.GetDlgItem(IDC_EDIT1); 

 	DirName = (char*) _getcwd(buffer,sizeof(buffer));
	CurrDir = (char*) malloc(strlen(DirName) + 1);
	strcpy(CurrDir,DirName);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAMLDecoderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CAMLDecoderDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAMLDecoderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;
//		this->GetClientRect(&rect); // This place can be used for some color modification for the main dialog box.
//		dc.FillRect(&rect,&Brush);
		
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAMLDecoderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAMLDecoderDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CAMLDecoderDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	free(CurrDir);

	CDialog::OnCancel();
}

void CAMLDecoderDlg::OnDecoder() 
{
	// TODO: Add your control notification handler code here
	FILE *fd;
	CFile hDecodeFile;
	CFileException e;
	char buffer[64000];
    char bufferDupe[64000];  
	char ViewOut[80];
    char* Output_FilePath = 0;
	int j=0,i=0,k,old,latest,diff;
	memset(bufr,'\0',sizeof(bufr));
	memset(buffer,'\0',sizeof(buffer));
	//AfxMessageBox("Test 4");
	if((m_Radio==0)||(m_Radio==1))
	{
		strcpy(bufr,"ELAN");
	}
	else
	{
		strcpy(bufr,"CSLM");
	}
//	AfxMessageBox(m_InputEdit.GetLineCount())
	
	

   // AfxMessageBox("Test 1");
//	m_Outputwindow.ResetContent();
//	m_OutputEdit.RedrawWindow(NULL,NULL,RDW_ERASE); 
	//AfxMessageBox(fName);
	;

	if(Decode(fName,bufr,CurrDir)){
		free(fName);
		return;
	}
	//AfxMessageBox("Test 2");
	free(fName);

    char OutputFile[80];
	strcpy(OutputFile,CurrDir);
	strcat(OutputFile,"\\DecodedMessages.txt");

	if(!hDecodeFile.Open(OutputFile,CFile::modeRead,&e))
		exit(1);
	hDecodeFile.Read(buffer,64000); 
//	hDecodeFile.
/*	j=strlen(buffer);
	k=0;
	for(i=0;i<j;i++){
		if(buffer[i]=='\n'){
			latest=j;
			diff=latest-old;
			for(k=0;k<diff;k++){
				ViewOut[k]=buffer[old++];
			}
			AfxMessageBox(ViewOut,0,0);
			old=latest;
		}
	}*/
	m_OutputEdit.SetWindowText(buffer);
	hDecodeFile.Close(); 
/*	if((fd=fopen("C:\\DecodedMessages.txt","r"))== NULL)
	{
		AfxMessageBox("MDF File is not generated for viewing..........");
	}
	else
	{
		  //AfxMessageBox("Test One");
		  memset(ViewOut,'\0',sizeof(ViewOut));
		  while(!feof(fd))
		  {
		    if(fgets(ViewOut,80,fd)!=NULL)
			{
				i=strlen(ViewOut);
			    ViewOut[i]='\0';
				for(j=0;j<i;j++)
				{
					if((ViewOut[j]==9)||(ViewOut[j]==10))
					{
						ViewOut[j]=' ';
					}
				}
//			CWnd *ptrList = GetDlgItem(IDC_LIST1);
//			ptrList->SetWindowText(ViewOut); 
			m_Outputwindow.AddString(ViewOut);
			

			}
		  }
	}
	fclose(fd);*/
	UpdateData(false); 
}

	


void CAMLDecoderDlg::OnFile() 
{
	// TODO: Add your control notification handler code here
	FILE *fp;
	CFile inputFile;
	CFileException e;
	char buffer1[64000];
	char *View, ch =0;
	CString Viewer="" ,File_Path;
	int i= 0, j=0, Line_Count = 0 ,nEnd = 0;
    char buffer[80];

	View=(char *)calloc(1100,1);
    
    memset(bufr,'\0',sizeof(bufr));
	memset(buffer1,'\0',sizeof(buffer1));
	

	static char BASED_CODE szFilter[] = "All Files (*.*)|*.*|Text Files(*.txt)|*.txt|";
	Sign_File:CFileDialog	m_ldFile(TRUE,NULL,NULL,NULL,szFilter);
	m_ldFile.m_ofn.lpstrInitialDir="c:\\";
	if(m_ldFile.DoModal()==IDOK)
	{
		File_Path=m_ldFile.GetPathName();
		
		//FileName=m_ldFile.GetFileName();
		//FileExtension=m_ldFile.GetFileExt();
//	    m_Inputwindow.ResetContent();
		if(!inputFile.Open(File_Path,CFile::modeRead,&e))
			exit(1);
		inputFile.Read(buffer1,inputFile.GetLength());//  .GetLength(); .Read(buffer,64000); 
		m_InputEdit.SetWindowText(buffer1);
		inputFile.Close(); 
	
/*		if((fp=fopen(File_Path,"r"))==NULL)
		{
			AfxMessageBox("MDF File is not generated for viewing..........");
		}
		else
		{
			while(fgets(View,180,fp)!=NULL)
				{
				i=strlen(View);
				View[i]='\0';
					for(j=0;j<i;j++)
					{
						if((View[j]==9)||(View[j]==10))
						{
							View[j]=' ';
						}
					}
				Viewer=View; 
				m_Inputwindow.AddString(View);
				memset(View,'\0',sizeof(View));
				}
		}
		free(View);
		UpdateData(FALSE);*/
	}


	fName = (char*)malloc(strlen(File_Path) + 1);
	strcpy(fName,File_Path);
	//	free(fName);
}

void CAMLDecoderDlg::OnClear() 
{
	// TODO: Add your control notification handler code here
//	m_Inputwindow.Default();  
	//int Count = m_Inputwindow.GetCount();
	int Index = 0;

/*	int Count = m_Inputwindow.DeleteString(0);
	
	for(Index = 0; Index <Count ; Index++)
		m_Inputwindow.DeleteString(Index);
	
	Index = 0;
    Count = m_Outputwindow.GetCount();
	for(Index = 0; Index <Count ; Index++)
		m_Outputwindow.DeleteString(Index); */
//	m_Inputwindow.ResetContent();
//	m_Outputwindow.ResetContent();
	m_InputEdit.SetSel(0,64000,false);
	m_InputEdit.Clear(); 
	m_OutputEdit.SetSel(0,64000,false);
	m_OutputEdit.Clear(); 
	//m_OutputFile.Empty(); 
//	UpdateData(false);
}

void CAMLDecoderDlg::OnCp() 
{
	// TODO: Add your control notification handler code here

	UpdateData(true);
    m_Radio=0;
}

void CAMLDecoderDlg::OnSmp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_Radio=1;
	
}

void CAMLDecoderDlg::OnMm() 
{
	// TODO: Add your control notification handler code here

	UpdateData(true);
	m_Radio=2;
	

}

