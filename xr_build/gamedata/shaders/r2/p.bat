@fxc 			/nologo /Tps_2_0 /Emain /Zpr /Fctest\p20_%1 %1
@fxc 			/nologo /Tps_2_b /Emain /Zpr /Fctest\p2b_%1 %1
@fxc /DUSE_SHADER3=1 	/nologo /Tps_3_0 /Emain /Zpr /Fctest\p30_%1 %1
@C:\tools\NVSpNV40\nvshaderperf -v 1 -o test\p2B_%1.log -a NV40 test\p2B_%1
@C:\tools\NVSpNV40\nvshaderperf -v 1 -o test\p30_%1.log -a NV40 test\p30_%1
@C:\tools\NVSpG70\nvshaderperf -a NV40 test\p2B_%1 >> test\p2B_%1.log
@C:\tools\NVSpG70\nvshaderperf -a NV40 test\p30_%1 >> test\p30_%1.log 
@C:\tools\NVSpG70\nvshaderperf -a G70 test\p2B_%1  >> test\p2B_%1.log
@C:\tools\NVSpG70\nvshaderperf -a G70 test\p30_%1  >> test\p30_%1.log 
