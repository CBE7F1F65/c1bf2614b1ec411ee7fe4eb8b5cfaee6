// CustomDlg.h : 头文件
//

#pragma once


// CCustomDlg 对话框
class CCustomDlg : public CDialog
{
// 构造
public:
	CCustomDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CUSTOM_DIALOG };

	void SetDisplay();
	void LoadIniValue();
	void LoadDefaultValue();

	void getScreenmodeValue();
	void getRenderskipValue();

	bool checkValid(bool reporterror = true);
	bool saveFile();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	bool havejoy;
	union{
		struct{
			int keyUp;
			int keyDown;
			int keyLeft;
			int keyRight;
			int keyFire;
			int keyBomb;
			int keySlow;
			int keyCircle;
			int keyPause;
			int keySkip;
			int keyEnter;
			int keyEscape;
			int keyCapture;
		};
		int keyKey[13];
	};
	union{
		struct{
			int	joyFire;
			int	joyBomb;
			int	joySlow;
			int	joyCircle;
			int	joyPause;
		};
		int		joyKey[5];
	};
	int screenmode;
	int renderskip;
	int circlearound;

	int defaultwindow;
	int windowleftx;
	int windowtopy;
	int windoww;
	int windowh;
	int windowtopmost;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDefaultwindow();
	afx_msg void OnBnClickedTopmost();
	afx_msg void OnBnClickedFullscreen();
	afx_msg void OnBnClickedWindow();
	afx_msg void OnBnClickedReload();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRenderskip0();
	afx_msg void OnBnClickedRenderskip2();
	afx_msg void OnBnClickedRenderskip3();
	afx_msg void OnEnChangeJoyFire();
	afx_msg void OnEnChangeJoyBomb();
	afx_msg void OnEnChangeJoySlow();
	afx_msg void OnEnChangeJoyCircle();
	afx_msg void OnEnChangeJoyPause();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCirclearound();
};
