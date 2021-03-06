#ifndef xrServer_EntitiesH
#define xrServer_EntitiesH

#include "xrMessages.h"
#include "net_utils.h"
#ifdef _EDITOR
	#include "PropertiesListHelper.h"
#endif
#include "xrServer_Entity_Custom.h"
#include "ai_alife_server_objects.h"

// refs
//class xrServerEntity;
//class xrClientData;

// t-defs
struct	SRotation
{
	float  yaw, pitch;
	SRotation() { yaw=pitch=0; }
};

// 
//***** CFormed (Base)
class xrSE_CFormed
{
public:
	enum{
    	cfSphere=0,
        cfBox
    };
	union shape_data
	{
		Fsphere		sphere;
		Fmatrix		box;
	};
	struct shape_def
	{
		u8			type;
		shape_data	data;
	};
    DEFINE_VECTOR					(shape_def,ShapeVec,ShapeIt);
	ShapeVec						shapes;
public:
	void							cform_read			(NET_Packet& P);
	void							cform_write			(NET_Packet& P);
};

class xrSE_Visualed
{
	string64						visual_name;
public:
#ifdef _EDITOR
	AnsiString						play_animation;
	IVisual*						visual;
    void __fastcall					OnChangeVisual		(PropValue* sender);
    void 							PlayAnimation		(LPCSTR name);
#endif
public:
									xrSE_Visualed		(LPCSTR name=0)
    {
    	strcpy						(visual_name,name?name:"");
#ifdef _EDITOR
		play_animation				= "$editor";
		visual						= 0;
        OnChangeVisual				(0);
#endif
    }
	void							visual_read			(NET_Packet& P);
	void							visual_write		(NET_Packet& P);

    void							set_visual			(LPCSTR name);
	LPCSTR							get_visual			() {return visual_name;};
    
#ifdef _EDITOR
    void 							FillProp			(LPCSTR pref, PropItemVec& values);
#endif
};

// Some preprocessor help
#ifdef _EDITOR
#define xrSE_EDITOR_METHODS	virtual void FillProp(LPCSTR pref, PropItemVec& values);
#else
#define xrSE_EDITOR_METHODS	
#endif

//

//
#define xrSE_DECLARE_BEGIN(__A,__B)	class __A : public __B	{ typedef __B inherited; public:
#define xrSE_DECLARE_BEGIN2(__A,__B,__C) class __A : public __B, public __C	{ typedef __B inherited; public:
#define xrSE_DECLARE_BEGIN3(__A,__B,__C,__D) class __A : public __B, public __C, public __D	{ typedef __B inherited; public:

//
#define	xrSE_DECLARE_END \
public:\
virtual void 						UPDATE_Read		(NET_Packet& P); \
virtual void 						UPDATE_Write	(NET_Packet& P); \
virtual void 						STATE_Read		(NET_Packet& P, u16 size); \
virtual void 						STATE_Write		(NET_Packet& P); \
xrSE_EDITOR_METHODS\
};

xrSE_DECLARE_BEGIN2(xrSE_HangingLamp,xrServerEntity,xrSE_Visualed)
#ifdef _EDITOR
	void __fastcall					OnChangeAnim	(PropValue* sender);
#endif
	enum{
    	flPhysic					= (1<<0)
    };
	Flags16							flags;
    float							mass;
	u32								color;
	string32						startup_animation;
	string64						color_animator;
	string64						spot_texture;
	string32						spot_bone;
	float							spot_range;
	float							spot_cone_angle;
    float							spot_brightness;
									xrSE_HangingLamp	(LPCSTR caSection);
    virtual							~xrSE_HangingLamp	();
xrSE_DECLARE_END

xrSE_DECLARE_BEGIN2(xrSE_DeviceTorch,CALifeItem,xrSE_Visualed)
	u32								color;
	string64						animator;
	string64						spot_texture;
	float							spot_range;
	float							spot_cone_angle;
    float							spot_brightness;
									xrSE_DeviceTorch	(LPCSTR caSection);
    virtual							~xrSE_DeviceTorch	();
xrSE_DECLARE_END

// Physyc Object ////////////////////////////////////////////////////
enum EPOType {
	epotBox,
	epotFixedChain,
    epotFreeChain,
    epotSkeleton
};
xrSE_DECLARE_BEGIN2(xrSE_PhysicObject,CALifeDynamicObject,xrSE_Visualed)
	u32 							type;
	f32 							mass;
    string32 						fixed_bone;
									xrSE_PhysicObject	(LPCSTR caSection);
    virtual 						~xrSE_PhysicObject	();
xrSE_DECLARE_END
/////////////////////////////////////////////////////////////////////

//***** Weapon
xrSE_DECLARE_BEGIN2(xrSE_Weapon,CALifeItem,xrSE_Visualed)
	u32								timestamp;
	u8								flags;
	u8								state;

	u16								a_current;
	u16								a_elapsed;

									xrSE_Weapon		(LPCSTR caSection);

	virtual void					OnEvent			(NET_Packet& P, u16 type, u32 time, u32 sender );

	u8								get_slot		();
	u16								get_ammo_limit	();
	u16								get_ammo_total	();
	u16								get_ammo_elapsed();
	u16								get_ammo_magsize();
xrSE_DECLARE_END

//***** WeaponAmmo
xrSE_DECLARE_BEGIN2(xrSE_WeaponAmmo,CALifeItem,xrSE_Visualed)
	u16								a_elapsed;
	u16								m_boxSize;
							
									xrSE_WeaponAmmo	(LPCSTR caSection);
xrSE_DECLARE_END

//***** Teamed
xrSE_DECLARE_BEGIN(xrSE_Teamed,CALifeDynamicObject)
	u8								s_team;
	u8								s_squad;
	u8								s_group;
	float							fHealth;

									xrSE_Teamed		(LPCSTR caSection);
	virtual u8						g_team			()					{ return s_team;	}
	virtual u8						g_squad			()					{ return s_squad;	}
	virtual u8						g_group			()					{ return s_group;	}
xrSE_DECLARE_END

//***** Dummy
xrSE_DECLARE_BEGIN(xrSE_Dummy,xrServerEntity)
	enum SStyle{
		esAnimated					=	1<<0,	
		esModel						=	1<<1, 
		esParticles					=	1<<2, 
		esSound						=	1<<3,
		esRelativePosition			=	1<<4
	};
	u8								s_style;
	char*							s_Animation;
	char*							s_Model;
	char*							s_Particles;
	char*							s_Sound;
									xrSE_Dummy		(LPCSTR caSection);
    virtual							~xrSE_Dummy		();
xrSE_DECLARE_END

//***** MercuryBall
xrSE_DECLARE_BEGIN2(xrSE_MercuryBall,CALifeItem,xrSE_Visualed)
									xrSE_MercuryBall(LPCSTR caSection);
xrSE_DECLARE_END

//***** Car
xrSE_DECLARE_BEGIN2(xrSE_Car,xrServerEntity,xrSE_Visualed)
									xrSE_Car		(LPCSTR caSection) : xrServerEntity(caSection)
	{
		if (pSettings->section_exist(caSection) && pSettings->line_exist(caSection,"visual"))
    	    set_visual		(pSettings->r_string(caSection,"visual"));
	};
xrSE_DECLARE_END

//***** Crow
xrSE_DECLARE_BEGIN2(xrSE_Crow,xrServerEntity,xrSE_Visualed)
									xrSE_Crow		(LPCSTR caSection) : xrServerEntity(caSection)
	{
		if (pSettings->section_exist(caSection) && pSettings->line_exist(caSection,"visual"))
    	    set_visual		(pSettings->r_string(caSection,"visual"));
	};
xrSE_DECLARE_END

//***** Health
xrSE_DECLARE_BEGIN2(xrSE_Health,CALifeItem,xrSE_Visualed)
	u8								amount;
									xrSE_Health		(LPCSTR caSection) : CALifeItem(caSection), xrServerEntity(caSection)
	{
		if (pSettings->section_exist(caSection) && pSettings->line_exist(caSection,"visual"))
    	    set_visual		(pSettings->r_string(caSection,"visual"));
	};
xrSE_DECLARE_END

//***** Target
xrSE_DECLARE_BEGIN(xrSE_Target,xrServerEntity)
									xrSE_Target		(LPCSTR caSection) : xrServerEntity(caSection)
	{
	};
xrSE_DECLARE_END

//***** Target Assault
xrSE_DECLARE_BEGIN(xrSE_Target_Assault,xrSE_Target)
									xrSE_Target_Assault(LPCSTR caSection) : xrSE_Target(caSection)
	{
	};
xrSE_DECLARE_END

//***** Target CS Base
xrSE_DECLARE_BEGIN(xrSE_Target_CSBase,xrSE_Target)
	float							radius;
	u8								s_team;
	virtual u8						g_team() {return s_team;};
									xrSE_Target_CSBase(LPCSTR caSection);
xrSE_DECLARE_END

//***** Target CS Cask
xrSE_DECLARE_BEGIN(xrSE_Target_CSCask,xrSE_Target)
	string64						s_Model;
									xrSE_Target_CSCask(LPCSTR caSection);
xrSE_DECLARE_END

//***** Target CS
xrSE_DECLARE_BEGIN(xrSE_Target_CS,xrSE_Target)
	string64				s_Model;
									xrSE_Target_CS	(LPCSTR caSection);
xrSE_DECLARE_END

//***** Spectator
xrSE_DECLARE_BEGIN(xrSE_Spectator,xrServerEntity)
									xrSE_Spectator	(LPCSTR caSection) : xrServerEntity(caSection)
	{
	};
	virtual u8						g_team() {return 0;};
xrSE_DECLARE_END

//***** Actor
xrSE_DECLARE_BEGIN3(xrSE_Actor,xrSE_Teamed,CALifeTraderParams,xrSE_Visualed)
	u32								timestamp;
	u8								flags;
	u16								mstate;
	float							model_yaw;
	SRotation						torso;
	Fvector							accel;
	Fvector							velocity;
	float							fArmor;
	u8								weapon;
									xrSE_Actor		(LPCSTR caSection);
xrSE_DECLARE_END

//***** Enemy
xrSE_DECLARE_BEGIN(xrSE_Enemy,xrSE_Teamed)
	u32								timestamp;				// server(game) timestamp
	u8								flags;
	float							o_model;				// model yaw
	SRotation						o_torso;				// torso in world coords
									xrSE_Enemy		(LPCSTR caSection) : xrSE_Teamed(caSection), xrServerEntity(caSection)
	{
	}
xrSE_DECLARE_END

class CALifeMonsterAbstract : public xrSE_Enemy {
public:
	typedef	xrSE_Enemy inherited;
	
	_GRAPH_ID						m_tNextGraphID;
	_GRAPH_ID						m_tPrevGraphID;
	float							m_fGoingSpeed;
	float							m_fCurSpeed;
	float							m_fDistanceFromPoint;
	float							m_fDistanceToPoint;
	TERRAIN_VECTOR					m_tpaTerrain;
	
									CALifeMonsterAbstract(LPCSTR caSection)	: xrSE_Enemy(caSection), xrServerEntity(caSection)
	{
		m_tNextGraphID				= m_tGraphID;
		m_tPrevGraphID				= m_tGraphID;
		m_fCurSpeed					= 0.0f;
		m_fDistanceFromPoint		= 0.0f;
		m_fDistanceToPoint			= 0.0f;
		m_tpaTerrain.clear				();
		LPCSTR							S;
		if (pSettings->line_exist		(caSection,"monster_section")) {
			S							= pSettings->r_string	(pSettings->r_string(caSection,"monster_section"),"terrain");
			m_fGoingSpeed				= pSettings->r_float	(pSettings->r_string(caSection,"monster_section"), "going_speed");
		}
		else {
			S							= pSettings->r_string	(caSection,"terrain");
			m_fGoingSpeed				= pSettings->r_float	(caSection, "going_speed");
		}
		u32								N = _GetItemCount(S);
		R_ASSERT						(((N % (LOCATION_TYPE_COUNT + 2)) == 0) && (N));
		STerrainPlace					tTerrainPlace;
		tTerrainPlace.tMask.resize		(LOCATION_TYPE_COUNT);
		string16						I;
		for (u32 i=0; i<N;) {
			for (u32 j=0; j<LOCATION_TYPE_COUNT; j++, i++)
				tTerrainPlace.tMask[j] = _LOCATION_ID(atoi(_GetItem(S,i,I)));
			tTerrainPlace.dwMinTime		= atoi(_GetItem(S,i++,I))*1000;
			tTerrainPlace.dwMaxTime		= atoi(_GetItem(S,i++,I))*1000;
			m_tpaTerrain.push_back(tTerrainPlace);
		}
	};

	virtual void					STATE_Write	(NET_Packet &tNetPacket);
	virtual void					STATE_Read	(NET_Packet &tNetPacket, u16 size);
	virtual void					UPDATE_Write(NET_Packet &tNetPacket);
	virtual void					UPDATE_Read	(NET_Packet &tNetPacket);
};

class CALifeDynamicAnomalousZone : public CALifeMonsterAbstract, public CALifeZone {
public:
									CALifeDynamicAnomalousZone(LPCSTR caSection) : CALifeMonsterAbstract(caSection), CALifeZone(caSection), xrServerEntity(caSection)
	{
	};
	virtual void					STATE_Write	(NET_Packet &tNetPacket);
	virtual void					STATE_Read	(NET_Packet &tNetPacket, u16 size);
	virtual void					UPDATE_Write(NET_Packet &tNetPacket);
	virtual void					UPDATE_Read	(NET_Packet &tNetPacket);
};

class CALifeMonster : public CALifeMonsterAbstract {
public:
									CALifeMonster(LPCSTR caSection) : CALifeMonsterAbstract(caSection), xrServerEntity(caSection)
	{
	};
	virtual void					STATE_Write	(NET_Packet &tNetPacket);
	virtual void					STATE_Read	(NET_Packet &tNetPacket, u16 size);
	virtual void					UPDATE_Write(NET_Packet &tNetPacket);
	virtual void					UPDATE_Read	(NET_Packet &tNetPacket);
};

class CALifeHumanAbstract : public CALifeMonsterAbstract, public CALifeTraderAbstract {
public:
	DWORD_VECTOR					m_tpaVertices;
	BOOL_VECTOR						m_baVisitedVertices;
	PERSONAL_TASK_P_VECTOR			m_tpTasks;
	ETaskState						m_tTaskState;
	u32								m_dwCurNode;
	u32								m_dwCurTaskLocation;
	u32								m_dwCurTask;
	float							m_fSearchSpeed;

									CALifeHumanAbstract(LPCSTR caSection) : CALifeMonsterAbstract(caSection), CALifeTraderAbstract(caSection), xrServerEntity(caSection)
	{
		m_tpaVertices.clear			();
		m_baVisitedVertices.clear	();
		m_tpTasks.clear				();
		m_dwCurTask					= u32(-1);
		m_tTaskState				= eTaskStateNoTask;
		m_dwCurNode					= u32(-1);
		m_dwCurTaskLocation			= u32(-1);
		m_fSearchSpeed				= pSettings->r_float(caSection, "search_speed");
	};

	virtual							~CALifeHumanAbstract()
	{
		free_vector					(m_tpTasks);
	};
	
	virtual void					STATE_Write	(NET_Packet &tNetPacket);
	virtual void					STATE_Read	(NET_Packet &tNetPacket, u16 size);
	virtual void					UPDATE_Write(NET_Packet &tNetPacket);
	virtual void					UPDATE_Read	(NET_Packet &tNetPacket);

};

class CALifeHuman : public CALifeHumanAbstract {
public:
									CALifeHuman	(LPCSTR caSection) : CALifeHumanAbstract(caSection), xrServerEntity(caSection)
	{
	};
	virtual void					STATE_Write	(NET_Packet &tNetPacket);
	virtual void					STATE_Read	(NET_Packet &tNetPacket, u16 size);
	virtual void					UPDATE_Write(NET_Packet &tNetPacket);
	virtual void					UPDATE_Read	(NET_Packet &tNetPacket);
};

xrSE_DECLARE_BEGIN2(xrSE_Rat,CALifeMonsterAbstract,xrSE_Visualed)
	// Personal characteristics:
	float							fEyeFov;
	float							fEyeRange;
	float							fMinSpeed;
	float							fMaxSpeed;
	float							fAttackSpeed;
	float							fMaxPursuitRadius;
	float							fMaxHomeRadius;
	// morale
	float							fMoraleSuccessAttackQuant;
	float							fMoraleDeathQuant;
	float							fMoraleFearQuant;
	float							fMoraleRestoreQuant;
	u16								u16MoraleRestoreTimeInterval;
	float							fMoraleMinValue;
	float							fMoraleMaxValue;
	float							fMoraleNormalValue;
	// attack
	float							fHitPower;
	u16								u16HitInterval;
	float							fAttackDistance;
	float							fAttackAngle;
	float							fAttackSuccessProbability;

									xrSE_Rat	(LPCSTR caSection);				// constructor for variable initialization
xrSE_DECLARE_END

xrSE_DECLARE_BEGIN2(xrSE_Zombie,CALifeMonsterAbstract,xrSE_Visualed)
	// Personal characteristics:
	float							fEyeFov;
	float							fEyeRange;
	float							fMinSpeed;
	float							fMaxSpeed;
	float							fAttackSpeed;
	float							fMaxPursuitRadius;
	float							fMaxHomeRadius;
	// attack
	float							fHitPower;
	u16								u16HitInterval;
	float							fAttackDistance;
	float							fAttackAngle;

									xrSE_Zombie	(LPCSTR caSection);				// constructor for variable initialization
xrSE_DECLARE_END

xrSE_DECLARE_BEGIN2(xrSE_Dog,CALifeMonsterAbstract,xrSE_Visualed)
	// Personal characteristics:
	float							fEyeFov;
	float							fEyeRange;
	float							fMinSpeed;
	float							fMaxSpeed;
	float							fAttackSpeed;
	float							fMaxPursuitRadius;
	float							fMaxHomeRadius;
	// morale
	float							fMoraleSuccessAttackQuant;
	float							fMoraleDeathQuant;
	float							fMoraleFearQuant;
	float							fMoraleRestoreQuant;
	u16								u16MoraleRestoreTimeInterval;
	float							fMoraleMinValue;
	float							fMoraleMaxValue;
	float							fMoraleNormalValue;
	// attack
	float							fHitPower;
	u16								u16HitInterval;
	float							fAttackDistance;
	float							fAttackAngle;
	float							fAttackSuccessProbability;

									xrSE_Dog	(LPCSTR caSection);				// constructor for variable initialization
xrSE_DECLARE_END

xrSE_DECLARE_BEGIN3(xrSE_Human,CALifeMonsterAbstract,CALifeTraderParams,xrSE_Visualed)
									xrSE_Human	(LPCSTR caSection);
xrSE_DECLARE_END

xrSE_DECLARE_BEGIN(xrSE_Idol,xrSE_Human)
	string256						m_caAnimations;
	u32								m_dwAniPlayType;
									xrSE_Idol	(LPCSTR caSection);
xrSE_DECLARE_END

//***** Zone
//xrSE_DECLARE_BEGIN(xrSE_Zone,CALifeDynamicObject)
class xrSE_Zone : public CALifeDynamicObject, public xrSE_CFormed { typedef CALifeDynamicObject inherited; public:
	f32								m_maxPower;
	f32								m_attn;
	u32								m_period;
									xrSE_Zone	(LPCSTR caSection);
xrSE_DECLARE_END

//***** Detector
xrSE_DECLARE_BEGIN(xrSE_Detector,xrServerEntity)
									xrSE_Detector(LPCSTR caSection) : xrServerEntity(caSection)
	{
	};
xrSE_DECLARE_END

xrSE_DECLARE_BEGIN(xrGraphPoint,xrServerEntity)
public:
	string32						m_caConnectionPointName;
	u32								m_tLevelID;
	u8								m_tLocations[LOCATION_TYPE_COUNT];
									xrGraphPoint(LPCSTR caSection);
xrSE_DECLARE_END

xrSE_DECLARE_BEGIN(xrSE_TempObject,xrServerEntity)
public:
									xrSE_TempObject(LPCSTR caSection);
xrSE_DECLARE_END


xrSE_DECLARE_BEGIN2(xrSE_Biting,CALifeMonsterAbstract,xrSE_Visualed)
									xrSE_Biting	(LPCSTR caSection);				// constructor for variable initialization
xrSE_DECLARE_END


// 
#undef xrSE_EDITOR_METHODS
#undef xrSE_DECLARE_BEGIN
#undef xrSE_DECLARE_END

//
xrServerEntity*	F_entity_Create		(LPCSTR caSection);

template <class T> void	F_entity_Destroy	(T*& P)
{
	xr_delete(P);
};

#endif