
// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include <Imagehlp.h>
#pragma comment(lib,"Imagehlp.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMainDlg �Ի���



CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMainDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



typedef struct {
	DWORD ExitStatus;
	DWORD PebBaseAddress;
	DWORD AffinityMask;
	DWORD BasePriority;
	ULONG UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;
typedef LONG(WINAPI *PROCNTQSIP)(HANDLE, UINT, PVOID, ULONG, PULONG);

LPBYTE GetExeEntryPointI(HANDLE procHdl, const char* ansiExeFilePath) {
	LPBYTE entryPoint = NULL;

	PROCNTQSIP NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(GetModuleHandleA("ntdll"), "NtQueryInformationProcess");

	if (!NtQueryInformationProcess)
		return entryPoint;

	LONG status;
	DWORD dwParentPID = (DWORD)-1;
	PROCESS_BASIC_INFORMATION pbi;

	// Retrieve information
	status = NtQueryInformationProcess(procHdl,
		0,
		(PVOID)&pbi,
		sizeof(PROCESS_BASIC_INFORMATION),
		NULL
		);

	DWORD bytesRead = 0;
	HINSTANCE baseAddr = NULL;
	ReadProcessMemory(procHdl,
		(PVOID)(pbi.PebBaseAddress + 8),    // ���8���� ImageBaseAddress �� PEB �ṹ�е�ƫ��
		&baseAddr,
		sizeof(baseAddr),
		&bytesRead);

	PLOADED_IMAGE pImage = ImageLoad(ansiExeFilePath, NULL);    // ImageLoad ֻ֧��ANSI���롣����
	if (pImage == NULL)
		return entryPoint;
	PIMAGE_NT_HEADERS pNTHeader = pImage->FileHeader;
	entryPoint = (LPBYTE)baseAddr + pNTHeader->OptionalHeader.AddressOfEntryPoint;
	ImageUnload(pImage);

	return entryPoint;
}


LPBYTE GetExeEntryPoint(char *filename)
{
	PIMAGE_NT_HEADERS pNTHeader;
	DWORD pEntryPoint;
	PLOADED_IMAGE pImage;
	pImage = ImageLoad(filename, NULL);
	if (pImage == NULL)
		return NULL;
	pNTHeader = pImage->FileHeader;
	pEntryPoint = pNTHeader->OptionalHeader.AddressOfEntryPoint + pNTHeader->OptionalHeader.ImageBase;
	ImageUnload(pImage);
	return (LPBYTE)pEntryPoint;
}







// ���ӳ����DLL���õĽṹ��
#pragma pack (push ,1) // ��֤����Ľṹ�����BYTE���루���룩
typedef struct INJECT_CODE
{
	BYTE      int_PUSHAD;         // pushad        0x60       
	BYTE      int_PUSH;             // push &szDLL     0x68
	DWORD push_Value;           //            &szDLL = "ApiSpy.dll"��path
	BYTE      int_MOVEAX;              //  move eax &LoadLibrary  0xB8
	DWORD eax_Value;             //     &LoadLibrary
	WORD    call_eax;         //     call eax    0xD0FF(FF D0) (LoadLibrary("ApiSpy.dll");
	BYTE      jmp_MOVEAX;             //     move eax &ReplaceOldCode  0xB8       
	DWORD jmp_Value;             //     JMP�Ĳ���
	WORD    jmp_eax;        //     jmp eax   0xE0FF(FF E0) jmp ReplaceOldCode;
	char szDLL[MAX_PATH]; //  "ApiSpy.dll"��FullPath
}INJECT_LOADLIBRARY_CODE, *LPINJECT_CODE;
#pragma pack (pop , 1)

typedef struct
{
	LPBYTE  lpEntryPoint;   // Ŀ����̵���ڵ�ַ
	BYTE      oldcode[sizeof(INJECT_CODE)];        // Ŀ����̵Ĵ��뱣��
}SPY_MEM_SHARE, *LPSPY_MEM_SHARE;




void CMainDlg::OnBnClickedOk()
{
	USES_CONVERSION;
	char szCurrentDir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, szCurrentDir);
	CString csConfigFile;
	csConfigFile = szCurrentDir;
	csConfigFile.Append(_TEXT("\\config.ini"));
	TCHAR tzChrome[MAX_PATH];
	TCHAR tzParam[MAX_PATH];
	GetPrivateProfileString(_TEXT("public"), _TEXT("chrome"), _TEXT(""), tzChrome, MAX_PATH, csConfigFile.GetString());
	GetPrivateProfileString(_TEXT("public"), _TEXT("param"), _TEXT(""), tzParam, MAX_PATH, csConfigFile.GetString());
	CString sRunFile;
	sRunFile.Format(_TEXT("%s"), tzChrome);
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi = {0};
	CString sRunCommandLine;
	sRunCommandLine.Format(_TEXT("%s %s"), sRunFile.GetString(), tzParam);
	BOOL bRet = CreateProcess(NULL, sRunCommandLine.GetBuffer(0), NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	if (bRet){
		LPBYTE  pEntryPoint = GetExeEntryPointI(pi.hProcess,T2A(sRunFile));
		if (pEntryPoint){
			HANDLE hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, sizeof(SPY_MEM_SHARE), TEXT("MyDllMapView"));
			LPSPY_MEM_SHARE lpMap = (LPSPY_MEM_SHARE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

			SIZE_T stRead;
			ReadProcessMemory(pi.hProcess, pEntryPoint, &lpMap->oldcode, sizeof(INJECT_CODE),&stRead);
			lpMap->lpEntryPoint = pEntryPoint;
			INJECT_CODE     newCode;
			strcpy_s(newCode.szDLL, szCurrentDir);
			strcat_s(newCode.szDLL, "\\dll.dll");
			// ׼��Ӳ���루�����룩  
			newCode.int_PUSHAD = 0x60;
			newCode.int_PUSH = 0x68;
			newCode.int_MOVEAX = 0xB8;
			newCode.call_eax = 0xD0FF;
			newCode.jmp_MOVEAX = 0xB8;
			newCode.jmp_eax = 0xE0FF;
			newCode.eax_Value = (DWORD)&LoadLibraryA;
			newCode.push_Value = (DWORD)(pEntryPoint + offsetof(INJECT_CODE, szDLL));
			DWORD dwNewFlg, dwOldFlg;
			dwNewFlg = PAGE_READWRITE;
			VirtualProtectEx(pi.hProcess, (LPVOID)pEntryPoint, sizeof(DWORD), dwNewFlg, &dwOldFlg);
			WriteProcessMemory(pi.hProcess, pEntryPoint, &newCode, sizeof(newCode), NULL);//&dwWrited);  
			VirtualProtectEx(pi.hProcess, (LPVOID)pEntryPoint, sizeof(DWORD), dwOldFlg, &dwNewFlg);
			UnmapViewOfFile(lpMap);
			ResumeThread(pi.hThread);
		}
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}



	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}
