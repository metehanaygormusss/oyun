#pragma once

// Eski Düz Harita Sabitleri (Çarpışma ve Hitbox motoru burayı kullanmaya devam ediyor)
inline constexpr float tileSize = 40.0f;
inline constexpr int MAP_WIDTH = 160;
inline constexpr int MAP_HEIGHT = 120;

// Yeni İzometrik (Görsel) Sabitler
inline constexpr float tileW = 64.0f; 
inline constexpr float tileH = 32.0f;

// Sprite Sheet Animasyon Sabitleri
inline constexpr int SHEET_YON_SAYISI   = 8;
inline constexpr int IDLE_KARE_SAYISI   = 60;
inline constexpr int WALK_KARE_SAYISI   = 32;
inline constexpr int ATTACK_KARE_SAYISI = 48;
inline constexpr int DEATH_KARE_SAYISI  = 48;

inline constexpr float IDLE_KARE_SURESI   = 0.06f;
inline constexpr float WALK_KARE_SURESI   = 0.05f;
inline constexpr float ATTACK_KARE_SURESI = 0.04f;
inline constexpr float DEATH_KARE_SURESI  = 0.06f;

// YENİ: Arayüz (UI) Pozisyon Değişkenleri (Düzenlenebilir)
inline float miniMapX = 1450.0f;
inline float miniMapY = 150.0f;

// Skill Slot Pozisyon ve Boyut Değişkenleri (Her slot bağımsız)
inline float slotX[8] = {422.0f, 472.0f, 522.0f, 572.0f, 622.0f, 672.0f, 722.0f, 772.0f};
inline float slotY[8] = {818.0f, 818.0f, 818.0f, 818.0f, 818.0f, 818.0f, 818.0f, 818.0f};
inline float slotSize[8] = {42.0f, 42.0f, 42.0f, 42.0f, 42.0f, 42.0f, 42.0f, 42.0f};

inline int gununSaati = 12;

// YENİ: Tab Arka Plan Boyut ve Konum Değişkenleri (Arayüz Düzenleme Modu ile Değiştirilebilir)
inline float tabX = 345.0f;
inline float tabY = 195.0f;
inline float tabW = 910.0f;
inline float tabH = 510.0f;