#include "main.h"
#include "rc.h"

COLORREF GetColor(HWND parent, COLORREF cur) {
	COLORREF custCols[16] = { 0 };
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof cc);
	cc.lStructSize = sizeof cc;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.hwndOwner = parent;
	cc.lpCustColors = custCols;
	cc.rgbResult = cur;
	if (ChooseColor(&cc))
		cur = cc.rgbResult;
	return cur;
}

int SizeDialog::IDD(){
	return IDD_SIZE; 
}

bool SizeDialog::OnInitDialog(){
	SetInt(IDC_EDIT1, x);
	SetInt(IDC_EDIT2, y);
	return true;
}

bool SizeDialog::OnOK(){
	try {
		x = GetInt(IDC_EDIT1);
		y = GetInt(IDC_EDIT2);
	}
	catch (XCtrl&) {
		return false;
	}
	return true;
}


void MainWindow::OnPaint(HDC hdc){
	HBRUSH brush = CreateSolidBrush(color);
	RECT rc;
	GetClientRect(*this, &rc);
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
	SetWindowExtEx(hdc, x, y, NULL);
	for (int i = 0; i < x; ++i) {
		for (int j = (i & 1); j < y; j+=2) {
			RECT r = { i, j, i + 1, j + 1 };
			FillRect(hdc, &r, brush);
		}

	}
	DeleteObject(brush);
}

void MainWindow::OnCommand(int id){
	SizeDialog dlg;
	dlg.x = x;
	dlg.y = y;
	switch(id){
		case ID_SIZE:
			if (dlg.DoModal(0, *this) == IDOK) {
				if (dlg.x < 0 || dlg.y < 0) {
					MessageBox(*this,"Please enter positive number","Warning",MB_OK | MB_ICONWARNING);
				}
				else {
					x = dlg.x;
					y = dlg.y;
					InvalidateRect(*this, NULL, true);
				}
				
			}
			break;
		case ID_COLOR:
			color = GetColor(*this, color);
			InvalidateRect(*this, NULL, true);
			break;
		case ID_EXIT: 
			DestroyWindow(*this); 
			break;
	}
}

void MainWindow::OnDestroy(){
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	Application app;
	MainWindow wnd;	
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP", 
		(int)LoadMenu(hInstance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.Run();
}
