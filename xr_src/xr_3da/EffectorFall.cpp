// EffectorFall.cpp: implementation of the CEffectorFall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectorFall.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define FALL_SPEED 3.5f
#define FALL_MAXDIST 0.75f

CEffectorFall::CEffectorFall(float power, float life_time) : CCameraEffector(eCEFall, 100000.f, TRUE)
{
	fPower	= (power>1)?1:((power<0)?0:power*power);
	fPhase	= 0;
}

CEffectorFall::~CEffectorFall()
{
}

BOOL CEffectorFall::Process(Fvector &p, Fvector &d, Fvector &n, float& fFov, float& fFar, float& fAspect)
{
	//string512 buf;
	//strconcat(buf, buf, "CEffectorFall::Process"); //
	
	fPhase+=FALL_SPEED*Device.fTimeDelta;
	if (fPhase < 1)	{ 
		p.y -= FALL_MAXDIST*fPower*_sin(M_PI*fPhase + M_PI); 
	}
	else
	{ 
		fLifeTime = -1; 
	}

	Msg("CEffectorFall::Process Phase=%f LifeTime=%f Power=%f", fPhase, fLifeTime, fPower);
	return TRUE;
}
