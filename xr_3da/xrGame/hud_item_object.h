////////////////////////////////////////////////////////////////////////////
//	Module 		: hud_item_object.h
//	Created 	: 24.03.2003
//  Modified 	: 27.12.2004
//	Author		: Yuri Dobronravin
//	Description : HUD item
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "inventory_item_object.h"
#include "huditem.h"

class CHudItemObject : 
		public CInventoryItemObject,
		public CHudItem
{
protected: //���� ������ ���� ������� �� ������
						CHudItemObject		();
	virtual				~CHudItemObject		();

public:
	virtual	DLL_Pure	*_construct			();

public:
	virtual CHudItem	*cast_hud_item		()	{return this;}

public:
	virtual void		Load				(LPCSTR section);
	virtual bool		Action				(s32 cmd, u32 flags);
	virtual void		SwitchState			(u32 S);
	virtual void		OnStateSwitch		(u32 S);
	virtual void		OnEvent				(NET_Packet& P, u16 type);
	virtual void		OnH_A_Chield		();
	virtual void		OnH_B_Chield		();
	virtual void		OnH_B_Independent	();
	virtual void		OnH_A_Independent	();
	virtual	BOOL		net_Spawn			(CSE_Abstract* DC);
	virtual void		net_Destroy			();
	virtual bool		Activate			();
	virtual void		Deactivate			();
	virtual void		OnAnimationEnd		();
	virtual void		UpdateCL			();
	virtual void		renderable_Render	();
	virtual void		UpdateHudPosition	();
	virtual void		UpdateHudInertion	(Fmatrix& hud_trans, float actor_yaw, float actor_pitch);
	virtual void		UpdateHudAdditonal	(Fmatrix&);
	virtual void		on_renderable_Render();
};
