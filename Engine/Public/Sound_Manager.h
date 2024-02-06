#pragma once
#include "Base.h"

BEGIN(Engine)

using namespace FMOD;

class CSound_Manager final :
    public CBase
{
    DECLARE_SINGLETON(CSound_Manager)

private:
    CSound_Manager();
    ~CSound_Manager();

public:
    HRESULT Initialize();
    void Tick(_float fTimeDelta);
    void Free() override;

public:
    void PlaySound(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
    void PlayBGM(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
    void StopBGM(CHANNELID eID);
    void StopSound(CHANNELID eID);
    void StopAll();
    void SetChannelVolume(CHANNELID eID, float fVolume);
    _bool SetBGM(void* pArg);
    _bool IsPlaying(CHANNELID eID);
    _bool PlayBGM_Event(void* pArg);
    _bool StopBGM_Event(void* pArg);
    _bool Set_BGM_Mute(void* pArg);

private:
    void LoadSoundFile();
    void LoadSoundFile_Loop();

private:

    // ���� ���ҽ� ������ ���� ��ü 
    map<TCHAR*, Sound*> m_mapSound;

    // FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
    Channel* m_pChannelArr[MAXCHANNEL];

    // ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
    FMOD::System* m_pSystem = { nullptr };

  
    list<FADEINFO> m_FadeInfo;
   
    _float m_fFadeTime;

    _float m_fBGMVolumes[8] = { -1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f };

    _bool m_bPlayerDead;

    _bool m_bFirst;

    _bool m_bFirstPlace;

    _bool m_bSecondPlace;

};

END