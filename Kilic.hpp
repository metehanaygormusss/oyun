#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class KilicSlasher {
public:
    sf::Sprite sprite;
    float sayac = 0.0f;
    int kare = 0;
    bool bitti = false;
    bool hasarVerildi = false; 
    bool patlayanMi = false; 
    
    KilicSlasher(sf::Vector2f baslangic, sf::Vector2f hedef, const sf::Texture& doku) : sprite(doku) {
        int kareGenisligi = doku.getSize().x / 3;
        int kareYuksekligi = doku.getSize().y / 3;
        if(kareGenisligi <= 0) kareGenisligi = 1;
        if(kareYuksekligi <= 0) kareYuksekligi = 1;

        sprite.setTextureRect(sf::IntRect({0, 0}, {kareGenisligi, kareYuksekligi}));
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        
        sf::Vector2f fark = hedef - baslangic;
        
        sf::Vector2f isoBaslangic = { baslangic.x - baslangic.y, (baslangic.x + baslangic.y) / 2.0f };
        sf::Vector2f isoHedef     = { hedef.x - hedef.y, (hedef.x + hedef.y) / 2.0f };
        sf::Vector2f isoFark      = isoHedef - isoBaslangic;

        float aciDerece = std::atan2(isoFark.y, isoFark.x) * 180.0f / 3.14159265f;
        sprite.setRotation(sf::degrees(aciDerece));
        
        float mesafe = std::sqrt(fark.x * fark.x + fark.y * fark.y);
        sf::Vector2f yon = (mesafe > 0.0f) ? (fark / mesafe) : sf::Vector2f(1.0f, 0.0f);
        sf::Vector2f spawnPos = baslangic + yon * 35.0f; 
        
        sprite.setPosition(spawnPos);
        sprite.setScale({1.5f, 1.5f}); 
    }

    void guncelle(float dt, const sf::Texture& doku) {
        sayac += dt;
        if (sayac >= 0.02f) { 
            sayac -= 0.02f;
            kare++;
            if (kare >= 9) { 
                bitti = true;
            } else {
                int col = kare % 3;
                int row = kare / 3;
                int kareGenisligi = doku.getSize().x / 3;
                int kareYuksekligi = doku.getSize().y / 3;
                if(kareGenisligi <= 0) kareGenisligi = 1;
                if(kareYuksekligi <= 0) kareYuksekligi = 1;
                
                sprite.setTextureRect(sf::IntRect({col * kareGenisligi, row * kareYuksekligi}, {kareGenisligi, kareYuksekligi}));
            }
        }
    }
};