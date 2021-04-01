#ifndef SoundRender_SourceH
#define SoundRender_SourceH
#pragma once

#include "soundrender_cache.h"

// refs
struct OggVorbis_File;

class XRSOUND_EDITOR_API 	CSoundRender_Source	: public CSound_source
{
public:
	shared_str				fname;
	IReader*				wave;					// real source
	cache_cat				CAT;
	BOOL					_3D;
	u32						dwTimeTotal;			// �����
	u32						dwBytesTotal;
//	u32						dwBytesPerSec;
	u32						dwBytesPerMS;

	float					m_fBaseVolume;
	float					m_fMinDist;
	float					m_fMaxDist;
	float					m_fMaxAIDist;
	u32						m_uGameType;
private:
	void 					i_decompress_fr			(OggVorbis_File* ovf, char* dest, u32 size);    
	void 					i_decompress_hr			(OggVorbis_File* ovf, char* dest, u32 size);
	void					LoadWave 				(LPCSTR name, BOOL b3D);
public:
	CSoundRender_Source		();
	~CSoundRender_Source	();

	void					load					(LPCSTR name,	BOOL b3D);
    void					unload					();
	void					decompress				(u32	line);

	virtual	u32				length_ms				()	{return dwTimeTotal;	}
	virtual u32				game_type				()	{return m_uGameType;	}
	virtual LPCSTR			file_name				()	{return *fname;	}
	virtual float			base_volume				()	{return m_fBaseVolume; }
};
#endif