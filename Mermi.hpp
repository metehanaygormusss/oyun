#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Mermi {
public:
    sf::Sprite sprite;
    sf::Vector2f yon;
    float hiz = 600.0f;
    bool yokEdilsinMi = false;
    bool patlayanMi = false; // YENİ EKLENDİ

    // HATA ÇÖZÜMÜ: : sprite(doku) başlatma listesi eklendi ve _patlayanMi parametresi koyuldu
    Mermi(sf::Vector2f baslangic, sf::Vector2f hedef, const sf::Texture& doku, bool _patlayanMi = false) : sprite(doku), patlayanMi(_patlayanMi) {
        sprite.setPosition(baslangic);

        // 1. Kartezyen Yön Hesaplama
        sf::Vector2f fark = hedef - baslangic;
        float mesafe = std::sqrt(fark.x * fark.x + fark.y * fark.y);
        if (mesafe > 0.0f) {
            yon = fark / mesafe;
        } else {
            yon = sf::Vector2f(1.0f, 0.0f);
        }

        // 2. İzometrik Açı Hesaplama
        sf::Vector2f isoBaslangic = { baslangic.x - baslangic.y, (baslangic.x + baslangic.y) / 2.0f };
        sf::Vector2f isoHedef     = { hedef.x - hedef.y, (hedef.x + hedef.y) / 2.0f };
        sf::Vector2f isoFark      = isoHedef - isoBaslangic;

        float aciDerece = std::atan2(isoFark.y, isoFark.x) * 180.0f / 3.14159265f;

        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        
        sprite.setRotation(sf::degrees(aciDerece)); 
    }

    void guncelle(float deltaZaman) {
        sf::Vector2f yeniPozisyon = sprite.getPosition() + (yon * hiz * deltaZaman);
        sprite.setPosition(yeniPozisyon);
    }
};