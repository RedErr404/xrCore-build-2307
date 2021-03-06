#include "stdafx.h"

#include "UIMessageBox.h"
#include "UIMessageBoxEx.h"
#include "../UIDialogHolder.h"

CUIMessageBoxEx::CUIMessageBoxEx(){
	m_pMessageBox = xr_new<CUIMessageBox>();
//	m_pMessageBox->SetAutoDelete(true);
	AttachChild(m_pMessageBox);
}

CUIMessageBoxEx::~CUIMessageBoxEx(){
	xr_delete(m_pMessageBox);
}

void CUIMessageBoxEx::Init(LPCSTR xml_template){
	CUIDialogWnd::Init(0,0,1024,768);
	m_pMessageBox->Init(xml_template);
}

void CUIMessageBoxEx::SendMessage(CUIWindow* pWnd, s16 msg, void* pData /* = NULL */){
	if (pWnd == m_pMessageBox)
	{
		R_ASSERT3(GetMessageTarget(),*m_windowName,"window has no message target");
		GetMessageTarget()->SendMessage(this,msg,pData);
		switch (msg){
			case MESSAGE_BOX_OK_CLICKED:
			case MESSAGE_BOX_YES_CLICKED:
			case MESSAGE_BOX_NO_CLICKED:
			case MESSAGE_BOX_CANCEL_CLICKED:
				GetHolder()->StartStopMenu(this, false);
			default:
				break;
		}
		
	}
	
}
