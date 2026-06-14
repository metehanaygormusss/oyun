#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include "Harita.hpp"

class Dusman {
public:
    int id;           
    int ureticiId;    

    sf::Sprite sprite;
    float hareketHizi;
    bool yokEdilsinMi = false;
    float genislik; 
    float yukseklik;
    float maxCan = 100.0f;
    float mevcutCan = 100.0f;
    float gorisMenzili = 240.0f; 
    bool aktifMi = false;        
    sf::RectangleShape canBarArka;
    sf::RectangleShape canBarOn;

    int tip = 0; // 0: Yakın dövüş, 1: Okçu, 2: Boss
    float atesGecikmesi = 0.0f; 

    Dusman(int _id, float x, float y, const sf::Texture& dusmanDokusu, int _ureticiId = -1, int _tip = 0) 
        : id(_id), ureticiId(_ureticiId), sprite(dusmanDokusu), tip(_tip) {
        
        sprite.setPosition({x, y});
        hareketHizi = (tip == 1) ? 50.0f : 80.0f; 
        
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y * 0.85f});
        
        float hedefBoyut = (tip == 2) ? 80.0f : 40.0f; 
        float maxBoyut = std::max(bounds.size.x, bounds.size.y);
        
        // SIFIRA BÖLME KORUMASI: Görsel yüklenemediyse (0 boyutluysa) çökmeyi engeller
        if (maxBoyut > 0.0f) {
            float olcek = hedefBoyut / maxBoyut;
            sprite.setScale({olcek, olcek});
        }
        
        genislik = hedefBoyut;
        yukseklik = hedefBoyut;

        if (tip == 2) { 
            hareketHizi = 90.0f; 
        }

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

        if (atesGecikmesi > 0.0f) atesGecikmesi -= deltaZaman;

        if (aktifMi && mesafe > 0.0f) {
            sf::Vector2f yon = hamYon / mesafe;
            
            bool hareketEtsinMi = true;
            if (tip == 1) {
                if (mesafe < 120.0f) yon = -yon; 
                else if (mesafe > 120.0f && mesafe < 200.0f) hareketEtsinMi = false; 
            }

            if (hareketEtsinMi) {
                sf::Vector2f yeniPozisyon = mevcutPos + (yon * hareketHizi * deltaZaman);

                sprite.setPosition({yeniPozisyon.x, mevcutPos.y});
                if (haritaCarpismaKontrolu(yeniPozisyon.x, mevcutPos.y, genislik, yukseklik)) sprite.setPosition({mevcutPos.x, mevcutPos.y});

                mevcutPos = sprite.getPosition();
                sprite.setPosition({mevcutPos.x, yeniPozisyon.y});
                if (haritaCarpismaKontrolu(mevcutPos.x, yeniPozisyon.y, genislik, yukseklik)) sprite.setPosition({mevcutPos.x, mevcutPos.y});
            }

            if (hamYon.x > 0.0f) sprite.setScale({std::abs(sprite.getScale().x), sprite.getScale().y}); 
            else if (hamYon.x < 0.0f) sprite.setScale({-std::abs(sprite.getScale().x), sprite.getScale().y}); 
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