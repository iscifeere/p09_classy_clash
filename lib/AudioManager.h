#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <unordered_map>
#include "raylib.h"
#include "EventManager.h"

class AudioManager
{
public:
    AudioManager() = delete;

    static void Init();
    static void Shutdown();
    static void PlayEventSound(EventID event);

    static const std::string NAME;

    static Sound SfxPlayerHit0;
    static Sound SfxPlayerKilled;
    static Sound SfxEnemy_Killed_1;
    static Sound SfxDash_1;
    static Sound SfxPunch_1;
    static Sound SfxPunch_2;
    static Sound SfxPunch_3;
    static Sound SfxPunch_4;
    static Sound SfxShoot_1;
    static Sound SfxShoot_2;
    static Sound SfxSlash_1;
    static Sound SfxSlash_2;
    static Sound SfxShield_1;
    static Sound SfxShield_2;
    static Sound SfxShield_3;
    static Sound SfxShield_4;
    static Sound SfxExplosion_1;
    static Sound SfxExplosion_2;
    static Sound SfxCharge_Up_1;
    static Sound SfxHealth;
    static Sound SfxHeart_Pick_Up;
    static Sound SfxBlocked;
    static Sound SfxCoin_1;
    static Sound SfxCoin_2;
    static Sound SfxBonus_1;
    static Sound SfxBonus_2;
    static Sound SfxBonus_3;
    static Sound SfxBonus_4;
    static Sound SfxBonus_5;
    static Sound SfxMenu_Open;
    static Sound SfxMenu_Close;
    static Sound SfxToggle_Option_1;
    static Sound SfxToggle_Option_2;
    static Sound SfxToggle_Option_3;
    static Sound SfxMisc_1;
    static Sound SfxMisc_2;
    static Sound SfxMisc_3;
    static Sound SfxMisc_4;
    static Sound SfxMisc_5;
    static Sound SfxMisc_6;

    static Music Bgm_Conflict;

private:
    static std::unordered_map<EventID, Sound> m_EventSounds;

};

#endif