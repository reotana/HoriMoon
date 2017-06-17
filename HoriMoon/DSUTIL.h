#ifdef __cplusplus
extern "C" {
#endif

	typedef struct
	{
		BYTE *pbWaveData;
		DWORD cbWaveSize;
		int iAlloc;
		int iCurrent;
		IDirectSoundBuffer* Buffers[10];

	} SNDOBJ, *HSNDOBJ;

	static const char c_szWAV[] = "WAV";


#define _HSNDOBJ_DEFINED


	IDirectSoundBuffer *DSLoadSoundBuffer(IDirectSound *pDS, LPCTSTR lpName);

	IDirectSoundBuffer *DSLoad3DSoundBuffer(IDirectSound *pDS, LPCTSTR lpName);

	BOOL DSReloadSoundBuffer(IDirectSoundBuffer *pDSB, LPCTSTR lpName);

	BOOL DSGetWaveResource(HMODULE hModule, LPCTSTR lpName,
		WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pdwWaveSize);

	BOOL DSGetWaveFile(HMODULE hModule, LPCTSTR lpName, WAVEFORMATEX **ppWaveHeader,
		BYTE **ppbWaveData, DWORD *pdwWaveSize, void **ppvBase);

#ifndef _HSNDOBJ_DEFINED
#define _HSNDOBJ_DEFINED
	DECLARE_HANDLE32(HSNDOBJ);
#endif

	HSNDOBJ SndObjCreate(IDirectSound *pDS, LPCTSTR lpName, int iConcurrent);

	void SndObjDestroy(SNDOBJ *hSO);

	BOOL SndObjPlay(SNDOBJ *hSO, DWORD dwPlayFlags);

	BOOL SndObjStop(SNDOBJ *hSO);

	IDirectSoundBuffer *SndObjGetFreeBuffer(SNDOBJ *hSO);

	BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD dwWaveSize);

	BOOL DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pdwWaveSize);


#ifdef __cplusplus
}
#endif
