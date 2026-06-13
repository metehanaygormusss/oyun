#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Harita.hpp"

class Dusman {
public:
    int id;           // YENİ: Düşmanın benzersiz kimliği
    int ureticiId;    // YENİ: Hangi inden çıktığını takip eder

    sf::Sprite sprite;
    float hareketHizi;
    bool yokEdilsinMi = false;
    float genislik = 28.0f; 
    float yukseklik = 28.0f;
    float maxCan = 100.0f;
    float mevcutCan = 100.0f;
    float gorisMenzili = 240.0f; 
    bool aktifMi = false;        
    sf::RectangleShape canBarArka;
    sf::RectangleShape canBarOn;

    Dusman(int _id, float x, float y, const sf::Texture& dusmanDokusu, int _ureticiId = -1) 
        : id(_id), ureticiId(_ureticiId), sprite(dusmanDokusu) {
        
        sprite.setPosition({x, y});
        hareketHizi = 80.0f;
        canBarArka.setSize({tileSize, 5.0f});
        canBarArka.setFillColor(sf::Color(50, 0, 0));
        canBarOn.setSize({tileSize, 5.0f});
        canBarOn.setFillColor(sf::Color::Red);
    }

    void takipEt(sf::Vector2f oyuncuPozisyonu, float deltaZaman) {
        sf::Vector2f mevcutPos = sprite.getPosition();
        sf::Vector2f merkezPos = mevcutPos + sf::Vector2f(genislik / 2.0f, yukseklik / 2.0f);
        sf::Vector2f hamYon = oyuncuPozisyonu - merkezPos;
        
        float mesafe = std::sqrt(hamYon.x * hamYon.x + hamYon.y * hamYon.y);

        if (mesafe <= gorisMenzili) aktifMi = true;
        else if (mesafe > gorisMenzili * 1.5f) aktifMi = false;

        if (aktifMi && mesafe > 0.0f) {
            sf::Vector2f yon = hamYon / mesafe;
            sf::Vector2f yeniPozisyon = mevcutPos + (yon * hareketHizi * deltaZaman);

            sprite.setPosition({yeniPozisyon.x, mevcutPos.y});
            if (haritaCarpismaKontrolu(yeniPozisyon.x, mevcutPos.y, genislik, yukseklik)) sprite.setPosition({mevcutPos.x, mevcutPos.y});

            mevcutPos = sprite.getPosition();
            sprite.setPosition({mevcutPos.x, yeniPozisyon.y});
            if (haritaCarpismaKontrolu(mevcutPos.x, yeniPozisyon.y, genislik, yukseklik)) sprite.setPosition({mevcutPos.x, mevcutPos.y});

            if (yon.x > 0.0f) sprite.setScale({1.0f, 1.0f}); 
            else if (yon.x < 0.0f) sprite.setScale({-1.0f, 1.0f}); 
        }
    }

    void cizCanBariIzometrik(sf::RenderWindow& window, sf::Vector2f isoMerkez, float kafaHizasi) {
        float oran = mevcutCan / maxCan;
        if (oran < 0.0f) oran = 0.0f;
        canBarOn.setSize({tileSize * oran, 5.0f});
        
        sf::Vector2f barPos = {isoMerkez.x - tileSize / 2.0f, isoMerkez.y - kafaHizasi - 15.0f};
        
        canBarArka.setPosition(barPos);
        canBarOn.setPosition(barPos);
        
        window.draw(canBarArka);
        window.draw(canBarOn);
    }
};