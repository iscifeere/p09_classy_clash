#include "AudioManager.h"

const std::string AudioManager::NAME{"AudioManager"};
std::unordered_map<EventID, Sound> AudioManager::m_EventSounds{};

Sound AudioManager::SfxPlayerHit0{};
Sound AudioManager::SfxPlayerKilled{};
Sound AudioManager::SfxEnemy_Killed_1{};
Sound AudioManager::SfxDash_1{};
Sound AudioManager::SfxPunch_1{};
Sound AudioManager::SfxPunch_2{};
Sound AudioManager::SfxPunch_3{};
Sound AudioManager::SfxPunch_4{};
Sound AudioManager::SfxShoot_1{};
Sound AudioManager::SfxShoot_2{};
Sound AudioManager::SfxSlash_1{};
Sound AudioManager::SfxSlash_2{};
Sound AudioManager::SfxShield_1{};
Sound AudioManager::SfxShield_2{};
Sound AudioManager::SfxShield_3{};
Sound AudioManager::SfxShield_4{};
Sound AudioManager::SfxExplosion_1{};
Sound AudioManager::SfxExplosion_2{};
Sound AudioManager::SfxCharge_Up_1{};
Sound AudioManager::SfxHealth{};
Sound AudioManager::SfxHeart_Pick_Up{};
Sound AudioManager::SfxBlocked{};
Sound AudioManager::SfxCoin_1{};
Sound AudioManager::SfxCoin_2{};
Sound AudioManager::SfxBonus_1{};
Sound AudioManager::SfxBonus_2{};
Sound AudioManager::SfxBonus_3{};
Sound AudioManager::SfxBonus_4{};
Sound AudioManager::SfxBonus_5{};
Sound AudioManager::SfxMenu_Open{};
Sound AudioManager::SfxMenu_Close{};
Sound AudioManager::SfxToggle_Option_1{};
Sound AudioManager::SfxToggle_Option_2{};
Sound AudioManager::SfxToggle_Option_3{};
Sound AudioManager::SfxMisc_1{};
Sound AudioManager::SfxMisc_2{};
Sound AudioManager::SfxMisc_3{};
Sound AudioManager::SfxMisc_4{};
Sound AudioManager::SfxMisc_5{};
Sound AudioManager::SfxMisc_6{};

Music AudioManager::Bgm_Conflict{};

void AudioManager::Init()
{
    InitAudioDevice();

    m_EventSounds[EventID::HeartCollected] = LoadSound("../assets/sfx/Health.wav");    // TODO should store references to sounds, not the actual sound
    m_EventSounds[EventID::CoinCollected] = LoadSound("../assets/sfx/Coin_1.wav");
    m_EventSounds[EventID::GemCollected] = LoadSound("../assets/sfx/Heart_Pick_Up.wav");
    m_EventSounds[EventID::EnemyKilled] = LoadSound("../assets/sfx/Enemy_Killed_1.wav");

    SfxPlayerHit0 = LoadSound("../assets/sfx/Player_Hit_0.wav");
    SfxPlayerKilled = LoadSound("../assets/sfx/Player_Killed.wav");
    SfxEnemy_Killed_1 = LoadSound("../assets/sfx/Enemy_Killed_1.wav");
    SfxDash_1 = LoadSound("../assets/sfx/Dash_1.wav");
    SfxPunch_1 = LoadSound("../assets/sfx/Punch_1.wav");
    SfxPunch_2 = LoadSound("../assets/sfx/Punch_2.wav");
    SfxPunch_3 = LoadSound("../assets/sfx/Punch_3.wav");
    SfxPunch_4 = LoadSound("../assets/sfx/Punch_4.wav");
    SfxShoot_1 = LoadSound("../assets/sfx/Shoot_1.wav");
    SfxShoot_2 = LoadSound("../assets/sfx/Shoot_2.wav");
    SfxSlash_1 = LoadSound("../assets/sfx/Slash_1.wav");
    SfxSlash_2 = LoadSound("../assets/sfx/Slash_2.wav");
    SfxShield_1 = LoadSound("../assets/sfx/Shield_1.wav");
    SfxShield_2 = LoadSound("../assets/sfx/Shield_2.wav");
    SfxShield_3 = LoadSound("../assets/sfx/Shield_3.wav");
    SfxShield_4 = LoadSound("../assets/sfx/Shield_4.wav");
    SfxExplosion_1 = LoadSound("../assets/sfx/Explosion_1.wav");
    SfxExplosion_2 = LoadSound("../assets/sfx/Explosion_2.wav");
    SfxCharge_Up_1 = LoadSound("../assets/sfx/Charge_Up_1.wav");
    SfxHealth = LoadSound("../assets/sfx/Health.wav");
    SfxHeart_Pick_Up = LoadSound("../assets/sfx/Heart_Pick_Up.wav");
    SfxBlocked = LoadSound("../assets/sfx/Blocked.wav");
    SfxCoin_1 = LoadSound("../assets/sfx/Coin_1.wav");
    SfxCoin_2 = LoadSound("../assets/sfx/Coin_2.wav");
    SfxBonus_1 = LoadSound("../assets/sfx/Bonus_1.wav");
    SfxBonus_2 = LoadSound("../assets/sfx/Bonus_2.wav");
    SfxBonus_3 = LoadSound("../assets/sfx/Bonus_3.wav");
    SfxBonus_4 = LoadSound("../assets/sfx/Bonus_4.wav");
    SfxBonus_5 = LoadSound("../assets/sfx/Bonus_5.wav");
    SfxMenu_Open = LoadSound("../assets/sfx/Menu_Open.wav");
    SfxMenu_Close = LoadSound("../assets/sfx/Menu_Close.wav");
    SfxToggle_Option_1 = LoadSound("../assets/sfx/Toggle_Option_1.wav");
    SfxToggle_Option_2 = LoadSound("../assets/sfx/Toggle_Option_2.wav");
    SfxToggle_Option_3 = LoadSound("../assets/sfx/Toggle_Option_3.wav");
    SfxMisc_1 = LoadSound("../assets/sfx/Misc_1.wav");
    SfxMisc_2 = LoadSound("../assets/sfx/Misc_2.wav");
    SfxMisc_3 = LoadSound("../assets/sfx/Misc_3.wav");
    SfxMisc_4 = LoadSound("../assets/sfx/Misc_4.wav");
    SfxMisc_5 = LoadSound("../assets/sfx/Misc_5.wav");
    SfxMisc_6 = LoadSound("../assets/sfx/Misc_6.wav");

    Bgm_Conflict = LoadMusicStream("../assets/music/Conflict.wav");
}

void AudioManager::Shutdown()
{
    UnloadSound(m_EventSounds[EventID::HeartCollected]);
    UnloadSound(m_EventSounds[EventID::CoinCollected]);
    UnloadSound(m_EventSounds[EventID::GemCollected]);
    UnloadSound(m_EventSounds[EventID::EnemyKilled]);

    UnloadSound(SfxPlayerHit0);
    UnloadSound(SfxPlayerKilled);
    UnloadSound(SfxEnemy_Killed_1);
    UnloadSound(SfxDash_1);
    UnloadSound(SfxPunch_1);
    UnloadSound(SfxPunch_2);
    UnloadSound(SfxPunch_3);
    UnloadSound(SfxPunch_4);
    UnloadSound(SfxShoot_1);
    UnloadSound(SfxShoot_2);
    UnloadSound(SfxSlash_1);
    UnloadSound(SfxSlash_2);
    UnloadSound(SfxShield_1);
    UnloadSound(SfxShield_2);
    UnloadSound(SfxShield_3);
    UnloadSound(SfxShield_4);
    UnloadSound(SfxExplosion_1);
    UnloadSound(SfxExplosion_2);
    UnloadSound(SfxCharge_Up_1);
    UnloadSound(SfxHealth);
    UnloadSound(SfxHeart_Pick_Up);
    UnloadSound(SfxBlocked);
    UnloadSound(SfxCoin_1);
    UnloadSound(SfxCoin_2);
    UnloadSound(SfxBonus_1);
    UnloadSound(SfxBonus_2);
    UnloadSound(SfxBonus_3);
    UnloadSound(SfxBonus_4);
    UnloadSound(SfxBonus_5);
    UnloadSound(SfxMenu_Open);
    UnloadSound(SfxMenu_Close);
    UnloadSound(SfxToggle_Option_1);
    UnloadSound(SfxToggle_Option_2);
    UnloadSound(SfxToggle_Option_3);
    UnloadSound(SfxMisc_1);
    UnloadSound(SfxMisc_2);
    UnloadSound(SfxMisc_3);
    UnloadSound(SfxMisc_4);
    UnloadSound(SfxMisc_5);
    UnloadSound(SfxMisc_6);

    UnloadMusicStream(Bgm_Conflict);

    CloseAudioDevice();
}

void AudioManager::PlayEventSound(EventID event)
{
    PlaySound(m_EventSounds[event]);
}