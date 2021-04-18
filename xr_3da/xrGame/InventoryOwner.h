/////////////////////////////////////////////////////
// ��� ����������, ������� ���������
// InventoryOwner.h
//////////////////////////////////////////////////////

#pragma once
#include "InfoPortionDefs.h"
#include "pda_space.h"
#include "attachment_owner.h"
#include "script_space_forward.h"
#include "character_info.h"
#include "inventory_space.h"

class CSE_Abstract;
class CInventory;
class CInventoryItem;
class CTrade;
class CPda;
class CGameObject;
class CEntityAlive;
class CCustomZone;
class CInfoPortionWrapper;
class NET_Packet;

class CCharacterInfo;
class CSpecificCharacter;


class CInventoryOwner : public CAttachmentOwner {							
public:
					CInventoryOwner				();
	virtual			~CInventoryOwner			();

public:
	virtual CInventoryOwner*	cast_inventory_owner	()						{return this;}
public:

	//////////////////////////////////////////////////////////////////////////
	// ����� �������

	virtual DLL_Pure	*_construct				();
	virtual BOOL		net_Spawn				(CSE_Abstract* DC);
	virtual void		net_Destroy				();
			void		Init					();
	virtual void		Load					(LPCSTR section);
	virtual void		reinit					();
	virtual void		reload					(LPCSTR section);
	virtual void		OnEvent					(NET_Packet& P, u16 type);

	//serialization
	virtual void	save						(NET_Packet &output_packet);
	virtual void	load						(IReader &input_packet);

	
	//����������
	virtual void	UpdateInventoryOwner		(u32 deltaT);
	virtual bool	CanPutInSlot				(PIItem item, u32 slot){return true;};
	// ��������
	u32					m_dwMoney;


	CPda* GetPDA		() const;
	bool IsActivePDA	() const;

	//������� ����� �������
	//������ ��������� ���������� ������� PDA ���������.
	//������ ���� �������������� � ������������ �������
	virtual void ReceivePdaMessage(u16 who, EPdaMsg msg, INFO_ID info_id);
	//�������� ��������� ������� ��������� PDA
	virtual void SendPdaMessage(u16 who, EPdaMsg msg, INFO_ID info_id);

	// ���������
	CInventory	*m_inventory;			
	
	////////////////////////////////////
	//�������� � ������� � ����������

	virtual bool	AllowItemToTrade 	(CInventoryItem const * item, EItemPlace place) const {return true;};
	virtual void	OnFollowerCmd		(int cmd)		{};//redefine for CAI_Stalkker
	//������������� ������� ��������
	CTrade* GetTrade();

	//��� ��������� ���������
	virtual bool OfferTalk		(CInventoryOwner* talk_partner);
	virtual void StartTalk		(CInventoryOwner* talk_partner, bool start_trade = true);
	virtual void StopTalk		();
	virtual bool IsTalking		();
	
	virtual void EnableTalk		()		{m_bAllowTalk = true;}
	virtual void DisableTalk	()		{m_bAllowTalk = false;}
	virtual bool IsTalkEnabled	()		{ return m_bAllowTalk;}
	
	void EnableTrade			()		{m_bAllowTrade = true;}
	void DisableTrade			()		{m_bAllowTrade = false;}
	bool IsTradeEnabled			()		{ return m_bAllowTrade;}

	CInventoryOwner* GetTalkPartner()	{return m_pTalkPartner;}
	virtual void	 NewPdaContact		(CInventoryOwner*);
	virtual void	 LostPdaContact		(CInventoryOwner*);

	//������� ��� 
	virtual LPCSTR	Name        () const;
	u32					get_money		() const				{return m_money;}
	void				set_money		(u32 amount, bool bSendEvent);
protected:
	u32					m_money;
	// ��������
	CTrade*				m_pTrade;
	bool				m_bTalking; 
	CInventoryOwner*	m_pTalkPartner;

	bool				m_bAllowTalk;
	bool				m_bAllowTrade;


	//////////////////////////////////////////////////////////////////////////
	// �������� ����������
public:
	//�������� ������� ����� ������ ����������
	virtual bool OnReceiveInfo	(INFO_ID info_id) const;
	//������ ����������
	virtual void OnDisableInfo	(INFO_ID info_id) const;
	//��������/������� ���������� ����� ������
	virtual void TransferInfo	(INFO_ID info_id, bool add_info) const;
	//���� �� ���������� � ���������
	virtual bool				HasInfo		(INFO_ID info_id) const;
	virtual bool				GetInfo		(INFO_ID info_id, INFO_DATA&) const;

	#ifdef DEBUG
	void CInventoryOwner::DumpInfo() const;
	#endif

	CInfoPortionWrapper			*m_known_info_registry;

	//////////////////////////////////////////////////////////////////////////
	// ��������� 
public:
	const CInventory &inventory() const {VERIFY (m_inventory); return(*m_inventory);}
	CInventory		 &inventory()		{VERIFY (m_inventory); return(*m_inventory);}

	//���������� ������� ������� �������� (� ��������) � ������ ��������
	virtual float GetWeaponAccuracy			() const;
	//������������ ���������� ���
	virtual float MaxCarryWeight			() const;


	//////////////////////////////////////////////////////////////////////////
	//������� �������������� ���������
public:
	virtual CCharacterInfo&				CharacterInfo		() const {VERIFY(m_pCharacterInfo); return *m_pCharacterInfo;}
	virtual const CSpecificCharacter&	SpecificCharacter	() const;
	bool								InfinitiveMoney		()	{return CharacterInfo().m_SpecificCharacter.MoneyDef().inf_money;}

	//��������� ����������� �� ���������� � ��������� ������
	virtual void			SetCommunity	(CHARACTER_COMMUNITY_INDEX);
	virtual void			SetRank			(CHARACTER_RANK_VALUE);
	virtual void			ChangeRank		(CHARACTER_RANK_VALUE);
	virtual void			SetReputation	(CHARACTER_REPUTATION_VALUE);
	virtual void			ChangeReputation(CHARACTER_REPUTATION_VALUE);

	//��� ������ � relation system
	u16								object_id	() const;
	CHARACTER_COMMUNITY_INDEX		Community	() const;
	CHARACTER_RANK_VALUE			Rank		() const;
	CHARACTER_REPUTATION_VALUE		Reputation	() const;

protected:
	CCharacterInfo*			m_pCharacterInfo;
	xr_string				m_game_name;

public:
	virtual void			renderable_Render		();
	virtual void			OnItemTake				(CInventoryItem *inventory_item);
	
	virtual void			OnItemBelt				(CInventoryItem *inventory_item, EItemPlace previous_place);
	virtual void			OnItemRuck				(CInventoryItem *inventory_item, EItemPlace previous_place);
	virtual void			OnItemSlot				(CInventoryItem *inventory_item, EItemPlace previous_place);
	
	virtual void			OnItemDrop				(CInventoryItem *inventory_item);

	virtual void			OnItemDropUpdate		();
	virtual bool			use_bolts				() const {return(true);}
	virtual	void			spawn_supplies			();

	CInventoryItem* CInventoryOwner::GetCurrentOutfit() const;

	//////////////////////////////////////////////////////////////////////////
	// ����� �� ���������
	//////////////////////////////////////////////////////////////////////////
protected:
	shared_str					m_item_to_spawn;
	u32							m_ammo_in_box_to_spawn;

public:
	IC		const shared_str	&item_to_spawn			() const {return m_item_to_spawn;}
	IC		const u32			&ammo_in_box_to_spawn	() const {return m_ammo_in_box_to_spawn;}

public:
	virtual	void				on_weapon_shot_start	(CWeapon *weapon);
	virtual	void				on_weapon_shot_stop		(CWeapon *weapon);
	virtual	void				on_weapon_hide			(CWeapon *weapon);
};

#include "inventory_owner_inline.h"