#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <cstdint> 
#include "Sabitler.hpp"
#include "Oyuncu.hpp" 

inline sf::Vector2f izoYap(float x, float y) { return sf::Vector2f(x - y, (x + y) / 2.0f); }
inline sf::Vector2f tersIzoYap(float isoX, float isoY) { return sf::Vector2f((isoX + 2.0f * isoY) / 2.0f, (2.0f * isoY - isoX) / 2.0f); }

struct CizilebilirVarlik { sf::Sprite* spritePtr; float zIndeks; bool duvarMi; float genislik; float yukseklik; };
struct Bildirim { std::string metin; float omur; };

struct HasarMetni {
    sf::Text text; float omur; float maksOmur; sf::Vector2f pos;
    HasarMetni(const sf::Font& font, float miktar, sf::Vector2f baslangic, sf::Color renk) : text(font) {
        text.setString(std::to_string(static_cast<int>(miktar))); text.setCharacterSize(24); text.setFillColor(renk);
        text.setOutlineColor(sf::Color::Black); text.setOutlineThickness(2.0f); pos = baslangic; text.setPosition(pos);
        omur = 1.0f; maksOmur = 1.0f;
    }
    HasarMetni(const sf::Font& font, std::string ozelMetin, sf::Vector2f baslangic, sf::Color renk) : text(font) {
        text.setString(ozelMetin); text.setCharacterSize(24); text.setFillColor(renk);
        text.setOutlineColor(sf::Color::Black); text.setOutlineThickness(2.0f); pos = baslangic; text.setPosition(pos);
        omur = 2.0f; maksOmur = 2.0f;
    }
    void guncelle(float dt) {
        omur -= dt; pos.y -= 50.0f * dt; text.setPosition(pos);
        sf::Color c = text.getFillColor(); sf::Color oc = text.getOutlineColor();
        c.a = static_cast<std::uint8_t>(255.0f * std::max(0.0f, (omur / maksOmur))); oc.a = c.a;
        text.setFillColor(c); text.setOutlineColor(oc);
    }
};

struct YerdekiEsya {
    sf::Sprite sprite; Esya esya; sf::Vector2f gercekPos;
    YerdekiEsya(Esya e, float x, float y) : sprite(e.doku), esya(e), gercekPos(x, y) {
        sf::FloatRect bounds = sprite.getLocalBounds(); sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        float maxB = std::max(bounds.size.x, bounds.size.y); float olcek = (maxB > 0.0f) ? (30.0f / maxB) : 1.0f; 
        sprite.setScale({olcek, olcek});
    }
};

struct Binek {
    sf::Sprite sprite;
    float hareketHizi = 150.0f; 
    
    Binek(float x, float y, const sf::Texture& doku) : sprite(doku) {
        sprite.setPosition({x, y});
        sf::FloatRect b = sprite.getLocalBounds();
        sprite.setOrigin({b.size.x / 2.0f, b.size.y * 0.85f});
        
        float olcek = (40.0f * 1.5f) / std::max(b.size.x, b.size.y);
        sprite.setScale({olcek, olcek});
    }

    void takipEt(sf::Vector2f hedefPos, float deltaZaman) {
        sf::Vector2f mevcutPos = sprite.getPosition();
        sf::Vector2f fark = hedefPos - mevcutPos;
        float mesafe = std::sqrt(fark.x * fark.x + fark.y * fark.y);
        
        if (mesafe > 90.0f) { 
            sf::Vector2f yon = fark / mesafe;
            mevcutPos += yon * hareketHizi * deltaZaman;
            sprite.setPosition(mevcutPos);
            
            if (yon.x > 0.05f) sprite.setScale({std::abs(sprite.getScale().x), sprite.getScale().y}); 
            else if (yon.x < -0.05f) sprite.setScale({-std::abs(sprite.getScale().x), sprite.getScale().y});
        }
    }
};

// YENİ: 7 Karelik Patlama Animasyonu Sistemi
struct PatlamaEfekti {
    sf::Sprite sprite;
    float sayac = 0.0f;
    int kare = 0;
    bool bitti = false;
    
    PatlamaEfekti(sf::Vector2f pos, const sf::Texture& doku) : sprite(doku) {
        int kareGenisligi = static_cast<int>(doku.getSize().x) / 7;
        sprite.setTextureRect(sf::IntRect({0, 0}, {kareGenisligi, static_cast<int>(doku.getSize().y)})); 
        sf::FloatRect b = sprite.getLocalBounds();
        sprite.setOrigin({b.size.x / 2.0f, b.size.y / 2.0f}); 
        sprite.setPosition(pos);
        sprite.setScale({1.5f, 1.5f}); // Patlama görselini büyütüyoruz
    }
    
    void guncelle(float dt, const sf::Texture& doku) {
        sayac += dt;
        if (sayac >= 0.04f) { // Patlama hızı (0.04 saniyede bir kare)
            sayac = 0.0f;
            kare++;
            if (kare >= 7) bitti = true;
            else {
                int kareGenisligi = static_cast<int>(doku.getSize().x) / 7;
                sprite.setTextureRect(sf::IntRect({kare * kareGenisligi, 0}, {kareGenisligi, static_cast<int>(doku.getSize().y)})); 
            }
        }
    }
};

inline std::string sifreleIsim(std::string str) {
    if (str == "") return "BOS"; std::replace(str.begin(), str.end(), ' ', '_'); return str;
}
inline std::string cozIsim(std::string str) {
    if (str == "BOS") return ""; std::replace(str.begin(), str.end(), '_', ' '); return str;
}

inline void ayarKaydet() {
    std::ofstream out("ayarlar.txt");
    out << miniMapX << " " << miniMapY << "\n";
    for (int i = 0; i < 8; i++) {
        out << slotX[i] << " " << slotY[i] << " " << slotSize[i] << "\n";
    }
    out << tabX << " " << tabY << " " << tabW << " " << tabH << "\n"; 
    out.close();
}

inline void ayarYukle() {
    std::ifstream in("ayarlar.txt");
    if(in.is_open()) {
        in >> miniMapX >> miniMapY;
        for (int i = 0; i < 8; i++) {
            in >> slotX[i] >> slotY[i] >> slotSize[i];
        }
        if (in >> tabX >> tabY >> tabW >> tabH) {} 
        in.close();
    }
}