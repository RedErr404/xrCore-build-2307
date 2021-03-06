#ifndef SH_RT_H
#define SH_RT_H
#pragma once

class	ENGINE_API	CTexture;

class	ENGINE_API	CRT
{
public:
	IDirect3DTexture8*	pSurface;
	IDirect3DSurface8*	pRT;
	CTexture*			pTexture;
	
	DWORD				dwReference;
	DWORD				dwWidth;
	DWORD				dwHeight;

	CRT					(){	ZeroMemory(this,sizeof(CRT));}

	void				Create	(LPCSTR Name, DWORD w, DWORD h);
	void				Destroy	();
	IC BOOL				Valid	()	{ return 0!=pTexture; }
};

#endif // SH_RT_H