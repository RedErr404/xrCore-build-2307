#include "common.h"

uniform float3x4	m_xform;
uniform float3x4	m_xform_v;
uniform float4 		consts; 	// {1/quant,1/quant,???,???}
uniform float4 		c_scale,c_bias,wind,wave;

p_flat 	main 	(v_tree I)
{
	p_flat 		o;

	// Transform to world coords
	float3 	pos	= mul		(m_xform, I.P);

	// 
	float 	base 	= m_xform._24;			// take base height from matrix
	float 	dp	= calc_cyclic  	(wave.w+dot(pos,(float3)wave));
	float 	H 	= pos.y - base;			// height of vertex (scaled, rotated, etc.)
	float 	frac 	= I.tc.z*consts.x;		// fractional (or rigidity)
	float 	inten 	= H * dp;			// intensity
	float2 	result	= calc_xz_wave	(wind.xz*inten, frac);
	float4 	f_pos 	= float4(pos.x+result.x, pos.y, pos.z+result.y, 1);

	// Final xform
	float3	Pe	= mul		(m_V,  f_pos				);
	float 	hemi 	= I.Nh.w*c_scale.w + c_bias.w;
	o.hpos		= mul		(m_VP, f_pos				);
	o.N 		= mul		((float3x3)m_xform_v, unpack_bx2(I.Nh)	);
	o.tcdh 		= float2	((I.tc * consts).xy			);	//+result;
	o.position	= float4	(Pe, hemi				);

	#ifdef USE_TDETAIL
	o.tcdbump	= o.tcdh*dt_params;					// dt tc
	#endif

	return o;
}
FXVS;
