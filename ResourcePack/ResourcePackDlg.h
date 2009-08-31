// ResourcePackDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#define M_PACKEXTENSION ".pck"


// CResourcePackDlg 对话框
class CResourcePackDlg : public CDialog
{
// 构造
public:
	CResourcePackDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RESOURCEPACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton packButton;
	CEdit foldernameText;
	TCHAR foldername[M_PATHMAX];
	TCHAR packname[M_PATHMAX];
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedPack();
};
