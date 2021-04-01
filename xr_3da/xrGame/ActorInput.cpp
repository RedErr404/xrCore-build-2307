#include "stdafx.h"
#include <dinput.h>
#include "Actor.h"
#include "Torch.h"
#include "trade.h"
#include "../CameraBase.h"
#ifdef DEBUG
#include "PHDebug.h"
#endif
#include "hit.h"
#include "PHDestroyable.h"
#include "Car.h"
#include "HudManager.h"
#include "UIGameSP.h"
#include "inventory.h"
#include "level.h"
#include "game_cl_base.h"
#include "xr_level_controller.h"
#include "UsableScriptObject.h"
#include "clsid_game.h"
#include "actorcondition.h"
#include "actor_input_handler.h"

float gCheckHitK = 1.0f;

void CActor::IR_OnKeyboardPress(int cmd)
{
	if (Remote())		return;

	if(GameID()!=GAME_DEATHMATCH){
		if(cmd == kEXT_14)
			gCheckHitK = 2.0f;
		if(cmd == kEXT_13)
			gCheckHitK = 4.0f;
	}
	if (conditions().IsSleeping())	return;
	if (IsTalking())	return;
	if (m_input_external_handler && !m_input_external_handler->authorized(cmd))	return;
	
	switch (cmd)
	{
	case kWPN_FIRE:
		{
			mstate_wishful &=~mcSprint;
			//-----------------------------
			if (OnServer())
			{
				NET_Packet P;
				P.w_begin(M_PLAYER_FIRE); 
				P.w_u16(ID());
				u_EventSend(P);
			}
		}break;
	default:
		{
		}break;
	}

	if (!g_Alive()) return;
/*
	switch(cmd){
		case kFOLLOWER1:
		case kFOLLOWER2:
		case kFOLLOWER3:
		case kFOLLOWER4:
		case kFOLLOWER5:
			SendCmdToFollowers(cmd);	break;
	};
*/
	if(m_holder && kUSE != cmd)
	{
		m_holder->OnKeyboardPress			(cmd);
		if(m_holder->allowWeapon() && inventory().Action(cmd, CMD_START))		return;
		return;
	}else
		if(inventory().Action(cmd, CMD_START))					return;

	switch(cmd){
	case kACCEL:	mstate_wishful |= mcAccel;					break;
	case kL_STRAFE:	mstate_wishful |= mcLStrafe;				break;
	case kR_STRAFE:	mstate_wishful |= mcRStrafe;				break;
	case kL_LOOKOUT:
					mstate_wishful |= mcLLookout;				
//					mstate_wishful &= ~mcAnyMove; 
					break;
	case kR_LOOKOUT:
					mstate_wishful |= mcRLookout;				
//					mstate_wishful &= ~mcAnyMove; 
					break;
	case kFWD:		mstate_wishful |= mcFwd;					break;
	case kBACK:		mstate_wishful |= mcBack;					break;
	case kJUMP:		mstate_wishful |= mcJump;					break;
	case kCROUCH:	mstate_wishful |= mcCrouch;					break;
	case kCROUCH_TOGGLE:	
		{
			if (mstate_wishful & mcCrouch)
				mstate_wishful &=~mcCrouch;
			else
				mstate_wishful |= mcCrouch;					
		}break;
	case kSPRINT_TOGGLE:	
		{
			if (mstate_wishful & mcSprint)
				mstate_wishful &=~mcSprint;
			else
				mstate_wishful |= mcSprint;					
		}break;
	case kCAM_1:	cam_Set			(eacFirstEye);				break;
	case kCAM_2:	cam_Set			(eacLookAt);				break;
	case kCAM_3:	cam_Set			(eacFreeLook);				break;

	case kNIGHT_VISION: {
		PIItem I = CAttachmentOwner::attachedItem(CLSID_DEVICE_TORCH);
			if (I){
				CTorch* torch = smart_cast<CTorch*>(I);
				if (torch) torch->SwitchNightVision();
			}
		}break;
	case kTORCH:{ 
			PIItem I = CAttachmentOwner::attachedItem(CLSID_DEVICE_TORCH);
			if (I){
				CTorch* torch = smart_cast<CTorch*>(I);
				if (torch) torch->Switch();
			}
		}break;
	case kWPN_1:	
	case kWPN_2:	
	case kWPN_3:	
	case kWPN_4:	
	case kWPN_5:	
	case kWPN_6:	
	case kWPN_7:	
	case kWPN_8:	
	case kWPN_9:	
		//Weapons->ActivateWeaponID	(cmd-kWPN_1);			
		break;
	case kBINOCULARS:
		//Weapons->ActivateWeaponID	(Weapons->WeaponCount()-1);
		break;
	case kWPN_RELOAD:
		//Weapons->Reload			();
		break;
	case kUSE:
		ActorUse();
		break;
	case kDROP:
		b_DropActivated			= TRUE;
		f_DropPower				= 0;
		break;
		//-----------------------------------------------------
		/*
	case kHyperJump:
		{
			Fvector pos	= Device.vCameraPosition;
			Fvector dir = Device.vCameraDirection;

			collide::rq_result result;
			BOOL reach_wall = Level().ObjectSpace.RayPick(pos, dir, 100.0f, 
				collide::rqtBoth, result,Level().CurrentControlEntity()) && !result.O;
			////////////////////////////////////
			if (!reach_wall || result.range < 1) break;

			dir.mul(result.range-0.5f);
			Fmatrix	M = Level().CurrentControlEntity()->XFORM();
			M.translate_add(dir);
			Level().CurrentControlEntity()->ForceTransform(M);
		}break;
		*/
	case kHyperKick:
		{
			m_dwStartKickTime = Level().timeServer();
		}break;
		//-----------------------------------------------------
	case kNEXT_SLOT:
		{
			OnNextWeaponSlot();
		}break;
	case kPREV_SLOT:
		{
			OnPrevWeaponSlot();
		}break;
	}
}
void CActor::IR_OnMouseWheel(int direction)
{
	if(inventory().Action( (direction>0)? kWPN_ZOOM_DEC:kWPN_ZOOM_INC , CMD_START)) return;


	if (direction>0)
		OnNextWeaponSlot				();
	else
		OnPrevWeaponSlot				();
}
void CActor::IR_OnKeyboardRelease(int cmd)
{
	if (Remote())		return;
	if(GameID()!=GAME_DEATHMATCH){
		if(cmd == kEXT_14)
			gCheckHitK = 1.0f;
		if(cmd == kEXT_13)
			gCheckHitK = 1.0f;
	}

	if (conditions().IsSleeping())	return;
	if (m_input_external_handler && !m_input_external_handler->authorized(cmd))	return;

	if (g_Alive())	
	{
		if (cmd == kUSE) 
			PickupModeOff();

		if(m_holder)
		{
			m_holder->OnKeyboardRelease(cmd);
			
			if(m_holder->allowWeapon() && inventory().Action(cmd, CMD_STOP))		return;
			return;
		}else
			if(inventory().Action(cmd, CMD_STOP))		return;



		switch(cmd)
		{
		case kACCEL:	mstate_wishful &=~mcAccel;		break;
		case kL_STRAFE:	mstate_wishful &=~mcLStrafe;	break;
		case kR_STRAFE:	mstate_wishful &=~mcRStrafe;	break;
		case kL_LOOKOUT:mstate_wishful &=~mcLLookout;	break;
		case kR_LOOKOUT:mstate_wishful &=~mcRLookout;	break;
		case kFWD:		mstate_wishful &=~mcFwd;		break;
		case kBACK:		mstate_wishful &=~mcBack;		break;
		case kJUMP:		mstate_wishful &=~mcJump;		break;
		case kCROUCH:	mstate_wishful &=~mcCrouch;		break;

		case kHyperKick:
			{
				u32 FullKickTime = Level().timeServer() - m_dwStartKickTime;
				
				collide::rq_result& RQ = HUD().GetCurrentRayQuery();
				CActor* pActor = smart_cast<CActor*>(RQ.O);
				if (!pActor || pActor->g_Alive()) break;

				Fvector original_dir, position_in_bone_space;
				original_dir.set(0, 1, 0);
				position_in_bone_space.set(0, 1, 0);				

				NET_Packet		P;
				CGameObject::u_EventGen	(P,GE_HIT,RQ.O->ID());
				P.w_u16			(ID());
				P.w_u16			(ID());
				P.w_dir			(original_dir);
				P.w_float		(0);
				P.w_s16			((s16)RQ.element);
				P.w_vec3		(position_in_bone_space);
				P.w_float		(float(FullKickTime)*10);
				P.w_u16			(2);
				Level().Send(P);

			}break;
		case kDROP:		if(GAME_PHASE_INPROGRESS == Game().Phase()) g_PerformDrop();				break;
		}
	}
}

void CActor::IR_OnKeyboardHold(int cmd)
{
	if (Remote() || !g_Alive())		return;
	if (conditions().IsSleeping())				return;
	if (m_input_external_handler && !m_input_external_handler->authorized(cmd))	return;
	if (IsTalking())				return;

	if(m_holder)
	{
		m_holder->OnKeyboardHold(cmd);
		return;
	}

	float LookFactor = GetLookFactor();
	switch(cmd)
	{
	case kUP:
	case kDOWN: 
		cam_Active()->Move(cmd, 0, LookFactor); break;
	case kCAM_ZOOM_IN: 
	case kCAM_ZOOM_OUT: 
		cam_Active()->Move(cmd); break;
	case kLEFT:
	case kRIGHT:
		if (eacFreeLook!=cam_active) cam_Active()->Move(cmd, 0, LookFactor); break;
	}
}

void CActor::IR_OnMouseMove(int dx, int dy)
{
	if (Remote())		return;
	if (conditions().IsSleeping())	return;

	if(m_holder) 
	{
		m_holder->OnMouseMove(dx,dy);
		return;
	}

	float LookFactor = GetLookFactor();

	CCameraBase* C	= cameras	[cam_active];
	float scale		= (C->f_fov/DEFAULT_FOV)*psMouseSens * psMouseSensScale/50.f  / LookFactor;
	if (dx){
		float d = float(dx)*scale;
		cam_Active()->Move((d<0)?kLEFT:kRIGHT, _abs(d));
	}
	if (dy){
		float d = ((psMouseInvert.test(1))?-1:1)*float(dy)*scale*3.f/4.f;
		cam_Active()->Move((d>0)?kUP:kDOWN, _abs(d));
	}
}

bool CActor::use_Holder				(CHolderCustom* holder)
{

	if(m_holder){
		CGameObject* holderGO			= smart_cast<CGameObject*>(m_holder);
		
		if(smart_cast<CCar*>(holderGO))
			return use_Vehicle(0);

		if (holderGO->CLS_ID==CLSID_OBJECT_W_MOUNTED ||
			holderGO->CLS_ID==CLSID_OBJECT_W_STATMGUN)
			return use_MountedWeapon(0);

	}else{
		bool b = false;
		CGameObject* holderGO			= smart_cast<CGameObject*>(holder);
		if(smart_cast<CCar*>(holder))
			b = use_Vehicle(holder);

		if (holderGO->CLS_ID==CLSID_OBJECT_W_MOUNTED ||
			holderGO->CLS_ID==CLSID_OBJECT_W_STATMGUN)
			b = use_MountedWeapon(holder);
		
		if(b){//used succesfully
			// switch off torch...
			PIItem I = CAttachmentOwner::attachedItem(CLSID_DEVICE_TORCH);
			if (I){
				CTorch* torch = smart_cast<CTorch*>(I);
				if (torch) torch->Switch(false);
			}
		}
		return b;
	}
	return false;
}

void CActor::ActorUse()
{
	mstate_real = 0;
	PickupModeOn();

		
	if (m_holder)
	{
		CGameObject*	GO			= smart_cast<CGameObject*>(m_holder);
		NET_Packet		P;
		CGameObject::u_EventGen		(P, GEG_PLAYER_DETACH_HOLDER, ID());
		P.w_u32						(GO->ID());
		CGameObject::u_EventSend	(P);
		return;
	}
				
	if(m_PhysicMovementControl->PHCapture())
		m_PhysicMovementControl->PHReleaseObject();

	

	if(m_pUsableObject)m_pUsableObject->use(this);
	

	if(!m_pUsableObject||m_pUsableObject->nonscript_usable())
	{
		if(m_pPersonWeLookingAt)
		{
			CEntityAlive* pEntityAliveWeLookingAt = 
				smart_cast<CEntityAlive*>(m_pPersonWeLookingAt);

			VERIFY(pEntityAliveWeLookingAt);

			if(pEntityAliveWeLookingAt->g_Alive())
			{
				TryToTalk();
			}
			//����� �����
			else  if(!Level().IR_GetKeyState(DIK_LSHIFT))
			{
				//������ ���� ��������� � ������ single
				CUIGameSP* pGameSP = smart_cast<CUIGameSP*>(HUD().GetUI()->UIGame());
				if(pGameSP)pGameSP->StartCarBody(&inventory(), this,
					&m_pPersonWeLookingAt->inventory(),
					smart_cast<CGameObject*>(m_pPersonWeLookingAt));
			}
		}
		else if(m_pVehicleWeLookingAt && smart_cast<CCar*>(m_pVehicleWeLookingAt) && Level().IR_GetKeyState(DIK_LSHIFT))
		{
			//������ ���� ��������� � ������ single
			CUIGameSP* pGameSP = smart_cast<CUIGameSP*>(HUD().GetUI()->UIGame());
			if(pGameSP)pGameSP->StartCarBody(&inventory(), this,
				m_pVehicleWeLookingAt->GetInventory(),
				smart_cast<CGameObject*>(m_pVehicleWeLookingAt));

		}
		collide::rq_result& RQ = HUD().GetCurrentRayQuery();
		CPhysicsShellHolder* object = smart_cast<CPhysicsShellHolder*>(RQ.O);
		u16 element = BI_NONE;
		if(object) 
			element = (u16)RQ.element;

		if(Level().IR_GetKeyState(DIK_LSHIFT))
		{
	
			if(!m_PhysicMovementControl->PHCapture())
			{
				m_PhysicMovementControl->PHCaptureObject(object,element);
			
			}

		}
		else
		{
			if (object && smart_cast<CHolderCustom*>(object))
			{
					NET_Packet		P;
					CGameObject::u_EventGen		(P, GEG_PLAYER_ATTACH_HOLDER, ID());
					P.w_u32						(object->ID());
					CGameObject::u_EventSend	(P);
					return;
			}

		}
	}


}
BOOL CActor::HUDview				( )const 
{ 
	return IsFocused()&&(cam_active==eacFirstEye)&&
		((!m_holder) || (m_holder && m_holder->allowWeapon() && m_holder->HUDView() ) ); 
}

//void CActor::IR_OnMousePress(int btn)
static	u32 SlotsToCheck [] = {
		KNIFE_SLOT		,		// 0
		PISTOL_SLOT		,		// 1
		RIFLE_SLOT		,		// 2
		GRENADE_SLOT	,		// 3
		APPARATUS_SLOT	,		// 4
};

void	CActor::OnNextWeaponSlot()
{
	u32 ActiveSlot = inventory().GetActiveSlot();
	if (ActiveSlot == NO_ACTIVE_SLOT) ActiveSlot = inventory().GetPrevActiveSlot();
	if (ActiveSlot == NO_ACTIVE_SLOT) return;
	
	u32 NumSlotsToCheck = sizeof(SlotsToCheck)/sizeof(u32);	
	for (u32 CurSlot=0; CurSlot<NumSlotsToCheck; CurSlot++)
	{
		if (SlotsToCheck[CurSlot] == ActiveSlot) break;
	};
	if (CurSlot >= NumSlotsToCheck) return;
	for (u32 i=CurSlot+1; i<NumSlotsToCheck; i++)
	{
		if (inventory().ItemFromSlot(SlotsToCheck[i]))
		{
			IR_OnKeyboardPress(kWPN_1+(i-KNIFE_SLOT));
			return;
		}
	}
};

void	CActor::OnPrevWeaponSlot()
{
	u32 ActiveSlot = inventory().GetActiveSlot();
	if (ActiveSlot == NO_ACTIVE_SLOT) ActiveSlot = inventory().GetPrevActiveSlot();
	if (ActiveSlot == NO_ACTIVE_SLOT) return;

	u32 NumSlotsToCheck = sizeof(SlotsToCheck)/sizeof(u32);	
	for (u32 CurSlot=0; CurSlot<NumSlotsToCheck; CurSlot++)
	{
		if (SlotsToCheck[CurSlot] == ActiveSlot) break;
	};
	if (CurSlot >= NumSlotsToCheck) return;
	for (s32 i=s32(CurSlot-1); i>=0; i--)
	{
		if (inventory().ItemFromSlot(SlotsToCheck[i]))
		{
			IR_OnKeyboardPress(kWPN_1+(i-KNIFE_SLOT));
			return;
		}
	}
};

float	CActor::GetLookFactor()
{
	if (m_input_external_handler) 
		return m_input_external_handler->mouse_scale_factor();

	
	float factor = 1.f;
	//  [7/19/2005]
	PIItem pItem = (inventory().GetActiveSlot() != NO_ACTIVE_SLOT ? 
		inventory().ItemFromSlot(inventory().GetActiveSlot()) : NULL);
	if (pItem)
	{
		factor *= pItem->GetControlInertionFactor();
	}
	//  [7/19/2005]
	VERIFY(!fis_zero(factor));
	return factor;
}

void CActor::set_input_external_handler(CActorInputHandler *handler) 
{
	m_input_external_handler = handler;
	if (handler) 
		mstate_wishful = 0;
}



