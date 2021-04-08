#pragma once

#include "CameraEffector.h"

class CObjectAnimator;
class CEffectorController;

typedef fastdelegate::FastDelegate0<float>		GET_KOEFF_FUNC;

void AddEffector		(int type, const shared_str& sect_name);
void AddEffector		(int type, const shared_str& sect_name, float factor);
void AddEffector		(int type, const shared_str& sect_name, GET_KOEFF_FUNC);
void AddEffector		(int type, const shared_str& sect_name, CEffectorController*);
void RemoveEffector		(int type);

class CEffectorController
{
protected:
	CEffectorCam*				m_ce;
	CEffectorPP*				m_pe;
public:
								CEffectorController():m_ce(NULL),m_pe(NULL)	{}
				void			SetPP		(CEffectorPP* p)				{m_pe=p;}
				void			SetCam		(CEffectorCam* p)				{m_ce=p;}
	virtual		BOOL			Valid		()								{return m_ce||m_pe;};
	virtual	float xr_stdcall	GetFactor	()								=0;
};

class CAnimatorCamEffector :public CEffectorCam
{
	typedef				CEffectorCam			inherited;
	bool				m_bCyclic;
protected:
	virtual bool		Cyclic					() const		{return m_bCyclic;}
	CObjectAnimator*							m_objectAnimator;
public:
						CAnimatorCamEffector	();
	virtual				~CAnimatorCamEffector	();
			void		Start					(LPCSTR fn);
	virtual	BOOL		Process					(Fvector &p, Fvector &d, Fvector &n, float& fFov, float& fFar, float& fAspect);
			void		SetCyclic				(bool b)				{m_bCyclic=b;}
	virtual	BOOL		Valid					();
};



class CAnimatorCamLerpEffector :public CAnimatorCamEffector
{
public:

protected:
	typedef				CAnimatorCamEffector		inherited;
	GET_KOEFF_FUNC									m_func;
public:
			void		SetFactorFunc				(GET_KOEFF_FUNC f)	{m_func=f;}
	virtual	BOOL		Process						(Fvector &p, Fvector &d, Fvector &n, float& fFov, float& fFar, float& fAspect);
};

class CAnimatorCamLerpEffectorConst :public CAnimatorCamLerpEffector
{
protected:
	float				m_factor;
public:
						CAnimatorCamLerpEffectorConst	();
	void				SetFactor						(float v)		{m_factor=v; clamp(m_factor,0.0f,1.0f);}
	float	xr_stdcall	GetFactor						()				{return m_factor;}
};

class CCameraEffectorControlled :public CAnimatorCamLerpEffector
{
	CEffectorController*		m_controller;
public:
						CCameraEffectorControlled		(CEffectorController* c);
	virtual				~CCameraEffectorControlled		();
	virtual BOOL		Valid							();
};

class SndShockEffector:public CEffectorController
{
	typedef CEffectorController inherited;
public:
	int							m_snd_length;	//ms
	int							m_cur_length;	//ms
	float						m_stored_volume;
	float						m_end_time;
	float						m_life_time;
public:
								SndShockEffector	();
								~SndShockEffector	();
	void						Start				(int snd_length, float power);
	void						Update				();

	virtual		BOOL			Valid				();
	virtual	float xr_stdcall	GetFactor			();
};


//////////////////////////////////////////////////////////////////////////
class CControllerPsyHitCamEffector :public CEffectorCam {
	typedef CEffectorCam inherited;
	
	float				m_time_total;
	float				m_time_current;
	Fvector				m_dangle_target;
	Fvector				m_dangle_current;
	Fvector				m_position_source;
	Fvector				m_direction;
	float				m_distance;

public:
						CControllerPsyHitCamEffector	(ECamEffectorType type, const Fvector &src_pos, const Fvector &target_pos, float time);
	virtual	BOOL		Process							(Fvector &p, Fvector &d, Fvector &n, float& fFov, float& fFar, float& fAspect);
};
//////////////////////////////////////////////////////////////////////////

class CActorCondition;
class CActorAlcoholCamEffector :public CAnimatorCamLerpEffector
{
	typedef  CAnimatorCamLerpEffector	inherited;
protected:
	virtual bool		Cyclic						() const		{return true;};
public:
						CActorAlcoholCamEffector	(CActorCondition* c);
};

class CFireHitCamEffector :public CAnimatorCamLerpEffector
{
	typedef  CAnimatorCamLerpEffector	inherited;
	float				m_power;
protected:
	virtual bool		Cyclic						() const		{return false;};
public:
						CFireHitCamEffector			(ECamEffectorType type,float power);
	float	xr_stdcall	GetPower					()				{return m_power;}
};