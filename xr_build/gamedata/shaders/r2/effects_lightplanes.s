function normal		(shader, t_base, t_second, t_detail)
	shader:begin	("base_lplanes","base_lplanes")
			: fog		(false)
			: zb 		(true,false)
			: blend		(true,blend.zero,blend.one)
			: aref 		(false,0)
			: sorting	(0, false)
	shader:sampler	("s_base")      :texture	(t_base)
end
