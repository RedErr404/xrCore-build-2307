#include "common.h"

p_flat 	main	( v_static I )
{
	// Eye-space pos/normal
	p_flat 		O;
	float4	Pp 	= mul		(m_WVP,	I.P		);
	O.hpos 		= Pp;
	O.N 		= mul		((float3x3)m_WV,	unpack_bx2(I.Nh));
	float3	Pe	= mul		(m_WV,  I.P		);

	O.tcdh		= unpack_tc_base	(I.tc,I.T.w,I.B.w);	// copy tc
	O.position	= float4	(Pe, 		I.Nh.w	);

#ifdef 	USE_TDETAIL
	O.tcdbump	= O.tcdh * dt_params;		// dt tc
#endif

#ifdef USE_LM_HEMI
	O.lmh 		= unpack_tc_lmap	(I.lmh);
#endif

	return	O;
}
FXVS;
