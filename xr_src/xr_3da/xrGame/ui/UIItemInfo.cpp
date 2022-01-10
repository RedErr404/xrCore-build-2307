// UIItemInfo.cpp:  окошко, для отображения информации о вещи

#include "stdafx.h"
#include "uiiteminfo.h"
#include "uistatic.h"
#include "UI3dStatic.h"
#include "UIXmlInit.h"

#include "UIListWnd.h"
#include "UIProgressBar.h"
#include "UIScrollView.h"

#include "../string_table.h"
#include "../Inventory_Item.h"
#include "UIInventoryUtilities.h"
#include "../PhysicsShellHolder.h"

//////////////////////////////////////////////////////////////////////////

const char * const 		fieldsCaptionColor		= "%c<255,216,186,140>";

//////////////////////////////////////////////////////////////////////////

CUIItemInfo::CUIItemInfo()
{
	UIItemImageSize.set			(0.0f,0.0f);
	UIItem3dPos.set(0.0f, 0.0f);
	UICondProgresBar			= NULL;
	UIItem3d = NULL;
}

CUIItemInfo::~CUIItemInfo()
{
}

void CUIItemInfo::Init(float x, float y, float width, float height, const char* xml_name)
{
	UIName			= xr_new<CUIStatic>(); AttachChild(UIName);		UIName->SetAutoDelete(true);
	UIWeight		= xr_new<CUIStatic>(); AttachChild(UIWeight);	UIWeight->SetAutoDelete(true);
	UICost			= xr_new<CUIStatic>(); AttachChild(UICost);		UICost->SetAutoDelete(true);
	UICondition		= xr_new<CUIStatic>(); AttachChild(UICondition);UICondition->SetAutoDelete(true);
	UIItemImage		= xr_new<CUIStatic>(); AttachChild(UIItemImage);	UIItemImage->SetAutoDelete(true);
	UIItem3d		= xr_new<CUI3dStatic>(); AttachChild(UIItem3d); UIItem3d->SetAutoDelete(true);

	inherited::Init(x, y, width, height);

	CUIXml uiXml;
	bool xml_result = uiXml.Init(CONFIG_PATH, UI_PATH,xml_name);
	R_ASSERT2(xml_result, "xml file not found");

	CUIXmlInit xml_init;

	xml_init.InitStatic(uiXml, "static", 0, UIName);
	UIName->Enable(false);
	UIName->SetElipsis(CUIStatic::eepEnd, 3);

	xml_init.InitStatic(uiXml, "static", 1, UIWeight);
	UIWeight->Enable(false);

	xml_init.InitStatic(uiXml, "static", 2, UICost);
	UICost->Enable(false);

	xml_init.InitStatic(uiXml, "static", 3, UICondition);
	UICondition->Enable(false);

	if(uiXml.NavigateToNode("condition_progress",0)){
		UICondProgresBar= xr_new<CUIProgressBar>(); AttachChild(UICondProgresBar);UICondProgresBar->SetAutoDelete(true);
		xml_init.InitProgressBar(uiXml, "condition_progress", 0, UICondProgresBar);
		UICondProgresBar->Enable(false);
	}
	UIDesc	= xr_new<CUIScrollView>(); AttachChild(UIDesc); UIDesc->SetAutoDelete(true);
	xml_init.InitScrollView(uiXml, "descr_list", 0, UIDesc);

	if (uiXml.NavigateToNode("image_static", 0))
	{	
		xml_init.InitStatic(uiXml, "image_static", 0, UIItemImage);
		UIItemImage->TextureAvailable(true);
	}
	UIItemImage->TextureOff();
	UIItemImage->ClipperOn();
	UIItemImageSize.set(UIItemImage->GetWidth(),UIItemImage->GetHeight());

	if (uiXml.NavigateToNode("image_static", 0))
	{
		float x = uiXml.ReadAttribFlt("image_static", 0, "x");
		float y = uiXml.ReadAttribFlt("image_static", 0, "y");
		float width = uiXml.ReadAttribFlt("image_static", 0, "width");
		float height = uiXml.ReadAttribFlt("image_static", 0, "height");
		UIItem3d->Init(x, y, width, height);

		UIItem3dPos.set(UIItem3d->GetWndPos());
	}

}

void CUIItemInfo::InitItem(CInventoryItem* pInvItem)
{
	Msg("Init item");
	if(pInvItem){
		Msg("Init item pInvItem");
		CStringTable stbl;

		string256 str;
		sprintf(str, "%s", pInvItem->Name());
		UIName->SetText(str);

		sprintf(str, "%s%s: %%c<default>%3.2f", fieldsCaptionColor, *stbl.translate("weight"), pInvItem->Weight());
		UIWeight->SetText(str);

		sprintf(str, "%s%s: %%c<default>%d", fieldsCaptionColor, *stbl.translate("base cost"), pInvItem->Cost());
		UICost->SetText(str);
		
		float cond = pInvItem->GetCondition();
		sprintf(str, "%s%s: %%c<default>%3.2f", fieldsCaptionColor, *stbl.translate("condition"), cond);
		UICondition->SetText					(str);
		if(UICondProgresBar){
			UICondProgresBar->Show				(true);
			UICondProgresBar->SetProgressPos	( s16(iFloor(cond*100.0f+1.0f-EPS)) );
		}

		UIDesc->Clear();
		CUIStatic* pItem = xr_new<CUIStatic>();
		pItem->SetWidth(UIDesc->GetDesiredChildWidth());
		pItem->SetText(*pInvItem->ItemDescription());
		pItem->AdjustHeightToText();
		UIDesc->AddWindow(pItem);

		// Загружаем картинку
		UIItemImage->SetShader(InventoryUtilities::GetEquipmentIconsShader());

		int iGridWidth		= pInvItem->GetGridWidth();
		int iGridHeight		= pInvItem->GetGridHeight();
		int iXPos			= pInvItem->GetXPos();
		int iYPos			= pInvItem->GetYPos();

		UIItemImage->GetUIStaticItem().SetOriginalRect(	float(iXPos*INV_GRID_WIDTH), float(iYPos*INV_GRID_HEIGHT),
														float(iGridWidth*INV_GRID_WIDTH),	float(iGridHeight*INV_GRID_HEIGHT));
		UIItemImage->TextureOn	();
		UIItemImage->ClipperOn	();
		UIItemImage->SetStretchTexture(true);
		Frect rect				= {0.0f, 0.0f, float(iGridWidth*INV_GRID_WIDTH),	float(iGridHeight*INV_GRID_HEIGHT)};
		UIItemImage->GetUIStaticItem().SetRect(rect);
		UIItemImage->SetWidth(_min(rect.width(), UIItemImageSize.x));
		UIItemImage->SetHeight(_min(rect.height(), UIItemImageSize.y));

		Msg("Init item UIItem3d init");
		UIItem3d->SetWndPos(UIItem3dPos);
		UIItem3d->SetWidth(_min(rect.width(), UIItemImageSize.x));
		UIItem3d->SetHeight(_min(rect.height(), UIItemImageSize.y));

		CGameObject* GO = smart_cast<CGameObject*>(pInvItem);
		UIItem3d->SetGameObject(GO);
		UIItem3d->Show(true);
		Msg("Init item UIItem3d init finish");

	}else{
		UIName->SetText			(NULL);
		UIWeight->SetText		(NULL);
		UICost->SetText			(NULL);
		UICondition->SetText	(NULL);
		UIDesc->Clear			();
		UIItemImage->TextureOff	();
		UIItem3d->SetGameObject(NULL);
		UIItem3d->Show(false);
		if(UICondProgresBar)
			UICondProgresBar->Show	(false);
	}
}

//////////////////////////////////////////////////////////////////////////

void CUIItemInfo::Draw()
{
//	y_rotate_angle += ROTATION_SPEED*(float)Device.dwTimeDelta/1000.f;
//   	if(y_rotate_angle>2*PI) y_rotate_angle = 0;
//	UI3dStatic.SetRotate(0,y_rotate_angle,0);
	
	UIItem3d->SetRotate(0.f, 0.5f*Device.fTimeGlobal, 0.f);
	inherited::Draw();
}

//////////////////////////////////////////////////////////////////////////

void CUIItemInfo::AlignRight(CUIStatic* Item, float offset)
{
	if (Item->GetFont())
	{
		float	s	= Item->GetFont()->SizeOf(Item->GetText());
				s	-= Item->GetFont()->SizeOf(fieldsCaptionColor);
				s	-= Item->GetFont()->SizeOf("%c<default>");
		Item->SetWndPos(GetWndRect().right - s + offset, Item->GetWndRect().top);
	}
}
