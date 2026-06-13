#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Harita.hpp"
#include "Sabitler.hpp"

struct Esya {
    std::string isim;
    std::string tur;
    bool doluMu;
    sf::Texture doku; 
    
    int ekstraCan;
    int saldiriGucu;
    int zirh;
    int ekstraMana;

    int miktar = 1;
    bool birikebilirMi = false;
};

struct AnimasyonSheet {
    sf::Texture texture;
    int kareSayisi    = 1;
    float kareSuresi  = 0.1f;
    int kareGenislik  = 0;
    int kareYukseklik = 0;

    bool yukle(const std::string& dosya, int _sutunSayisi, float _kareSuresi) {
        kareSuresi = _kareSuresi;
        if (!texture.loadFromFile(dosya)) return false;

        kareSayisi    = _sutunSayisi; 
        if (kareSayisi < 1) kareSayisi = 1; 

        kareGenislik  = static_cast<int>(texture.getSize().x) / kareSayisi;
        kareYukseklik = static_cast<int>(texture.getSize().y) / SHEET_YON_SAYISI; 
        
        return true;
    }
};

class Oyuncu {
private:
    const AnimasyonSheet* sIdle   = nullptr;
    const AnimasyonSheet* sWalk   = nullptr;
    const AnimasyonSheet* sAttack = nullptr;
    const AnimasyonSheet* sDeath  = nullptr;

    float animZaman  = 0.0f;
    int   mevcutKare = 0;

    void spriteKareAyarla(const AnimasyonSheet* sheet) {
        if (!sheet) return;

        int kw = sheet->kareGenislik;
        int kh = sheet->kareYukseklik;
        int maxK = sheet->kareSayisi;

        if (mevcutKare >= maxK) mevcutKare = 0;

        int satirNo = static_cast<int>(mevcutYon);

        sf::IntRect kareRect({mevcutKare * kw, satirNo * kh}, {kw, kh});
        sprite.setTexture(sheet->texture, false);
        sprite.setTextureRect(kareRect);

        float olcek = (tileSize * 2.8f) / static_cast<float>(kw);
        sprite.setScale({olcek, olcek});
    }

    void animasyonIlerle(float deltaZaman, const AnimasyonSheet* sheet) {
        if (!sheet) return;
        animZaman += deltaZaman;
        while (animZaman >= sheet->kareSuresi) {
            animZaman -= sheet->kareSuresi;
            mevcutKare++;
        }
        mevcutKare = mevcutKare % sheet->kareSayisi;
    }

public:
    sf::Sprite sprite;
    float hareketHizi = 200.0f;
    float genislik    = 28.0f;
    float yukseklik   = 28.0f;
    float maxCan      = 100.0f;
    float mevcutCan   = 100.0f;
    int   altin       = 1250;

    int seviye = 1;
    float exp = 0.0f;
    float maxExp = 100.0f;
    float mevcutMana = 50.0f;
    float maxMana = 50.0f;
    
    int yetenekPuanlari = 0; 
    int statPuanlari = 0;
    int statSaldiriGucu = 0;
    int statZirh = 0;

    int skillCokluAtisLvl = 1;
    int skillAtilmaLvl = 0;
    int skillEkstraLvl = 0;
    int skillPatlamaLvl = 0; // YENİ: Patlayan Ok Yetenek Seviyesi

    float cokluAtisBekleme = 0.0f;
    float atilmaBekleme = 0.0f;
    float ekstraBekleme = 0.0f;
    float ekstraSuresi = 0.0f;
    float patlamaBekleme = 0.0f; // YENİ: Patlayan Ok Bekleme Süresi
    
    float atilmaKalanSure = 0.0f;
    sf::Vector2f atilmaYonu = {0.0f, 0.0f};
    
    float canYenilenmeHizi = 1.0f;  
    float manaYenilenmeHizi = 3.0f; 

    std::string hizliErisim[8] = {"Coklu Atis", "", "", "", "", "", "", ""};

    std::vector<Esya> tumOyunEsyalari; 
    Esya ekipmanlar[6];
    Esya envanter[42];

    enum Yon { K = 0, KB = 1, B = 2, GB = 3, G = 4, GD = 5, D = 6, KD = 7 };
    Yon mevcutYon = G;

    enum Durum { IDLE, WALK, ATTACK, DEATH };
    Durum mevcutDurum = IDLE;

    float saldiriSuresiSayaci         = 0.0f;
    const float TOPLAM_SALDIRI_SURESI = 0.30f; 

    Oyuncu(float x, float y, const sf::Texture& baslangicDokusu) : sprite(baslangicDokusu) {
        sprite.setPosition({x, y});

        for (int i = 0; i < 6; ++i) ekipmanlar[i] = {"", "", false};
        for (int i = 0; i < 42; ++i) envanter[i] = {"", "", false};

        esyaVeritabaniniYukle(); 
    }

    void esyaVeritabaniniYukle() {
        std::ifstream dosya("esya_veritabani.txt");
        if (!dosya.is_open()) return;

        std::string satir;
        int envIndeks = 0;

        while (std::getline(dosya, satir)) {
            std::stringstream ss(satir);
            std::string isim, tur, resim, can, saldiri, zirh, mana;
            
            std::getline(ss, isim, ';');
            std::getline(ss, tur, ';');
            std::getline(ss, resim, ';');
            std::getline(ss, can, ';');
            std::getline(ss, saldiri, ';');
            std::getline(ss, zirh, ';');
            std::getline(ss, mana, ';');

            Esya yeniEsya;
            yeniEsya.isim = isim;
            yeniEsya.tur = tur;
            yeniEsya.doluMu = true;
            yeniEsya.ekstraCan = std::stoi(can);
            yeniEsya.saldiriGucu = std::stoi(saldiri);
            yeniEsya.zirh = std::stoi(zirh);
            yeniEsya.ekstraMana = std::stoi(mana);
            (void)yeniEsya.doku.loadFromFile(resim); 
            
            if (isim == "Can Potu" || isim == "Yozlasmis Yurek") {
                yeniEsya.birikebilirMi = true;
            }

            tumOyunEsyalari.push_back(yeniEsya);

            if (envIndeks < 42 && tur != "Iksir" && tur != "Gorev Esyasi") {
                envanter[envIndeks] = yeniEsya;
                envIndeks++;
            }
        }
    }

    void expKazan(float miktar) {
        exp += miktar;
        while (exp >= maxExp) {
            exp -= maxExp;
            seviye++;
            yetenekPuanlari++; 
            statPuanlari++; 
            maxExp *= 1.5f;   
            maxCan += 20.0f;  
            maxMana += 10.0f; 
            
            mevcutCan = maxCan;   
            mevcutMana = maxMana; 
        }
    }

    void degerleriYenile(float deltaZaman) {
        if (mevcutCan > 0.0f && mevcutCan < maxCan) {
            mevcutCan += canYenilenmeHizi * deltaZaman;
            if (mevcutCan > maxCan) mevcutCan = maxCan;
        }
        if (mevcutMana < maxMana) {
            mevcutMana += manaYenilenmeHizi * deltaZaman;
            if (mevcutMana > maxMana) mevcutMana = maxMana;
        }

        if (cokluAtisBekleme > 0.0f) cokluAtisBekleme -= deltaZaman;
        if (atilmaBekleme > 0.0f) atilmaBekleme -= deltaZaman;
        if (ekstraBekleme > 0.0f) ekstraBekleme -= deltaZaman;
        if (ekstraSuresi > 0.0f) ekstraSuresi -= deltaZaman;
        if (patlamaBekleme > 0.0f) patlamaBekleme -= deltaZaman; // YENİ
    }

    void sheetleriAyarla(const AnimasyonSheet& _idle, const AnimasyonSheet& _walk, const AnimasyonSheet& _attack, const AnimasyonSheet& _death) {
        sIdle   = &_idle;
        sWalk   = &_walk;
        sAttack = &_attack;
        sDeath  = &_death;
        spriteKareAyarla(sIdle);
    }

    void atesEtmeTetikle() {
        if (mevcutDurum != ATTACK) {
            mevcutDurum = ATTACK; 
            saldiriSuresiSayaci = TOPLAM_SALDIRI_SURESI; 
            mevcutKare = 0; 
            animZaman = 0.0f;
        }
    }

    void hareketVeAnimasyonGuncelle(float deltaZaman) {
        if (!sIdle || !sWalk || !sAttack) return;

        if (atilmaKalanSure > 0.0f) {
            mevcutDurum = WALK;
            atilmaKalanSure -= deltaZaman;
            float hizliKaymaHizi = 1000.0f + (skillAtilmaLvl * 200.0f); 

            sf::Vector2f mevcutPozisyon = sprite.getPosition();
            sf::Vector2f yeniPozisyon = mevcutPozisyon + (atilmaYonu * hizliKaymaHizi * deltaZaman);

            sprite.setPosition({yeniPozisyon.x, mevcutPozisyon.y});
            if (haritaCarpismaKontrolu(yeniPozisyon.x, mevcutPozisyon.y, genislik, yukseklik)) {
                sprite.setPosition({mevcutPozisyon.x, mevcutPozisyon.y});
                atilmaKalanSure = 0.0f;
            }

            mevcutPozisyon = sprite.getPosition();
            sprite.setPosition({mevcutPozisyon.x, yeniPozisyon.y});
            if (haritaCarpismaKontrolu(mevcutPozisyon.x, yeniPozisyon.y, genislik, yukseklik)) {
                sprite.setPosition({mevcutPozisyon.x, mevcutPozisyon.y});
                atilmaKalanSure = 0.0f; 
            }

            animasyonIlerle(deltaZaman, sWalk); 
            spriteKareAyarla(sWalk);
            return; 
        }

        if (mevcutDurum == ATTACK) {
            saldiriSuresiSayaci -= deltaZaman;
            animasyonIlerle(deltaZaman, sAttack); 
            spriteKareAyarla(sAttack);
            if (saldiriSuresiSayaci <= 0.0f) { 
                mevcutDurum = IDLE; 
                mevcutKare = 0; 
                animZaman = 0.0f; 
            }
            return;
        }

        sf::Vector2f hareket({0.0f, 0.0f});
        sf::Vector2f klavyeGiris({0.0f, 0.0f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    klavyeGiris.y -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  klavyeGiris.y += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  klavyeGiris.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) klavyeGiris.x += 1.0f;

        if (klavyeGiris.x != 0.0f || klavyeGiris.y != 0.0f) {
            if (mevcutDurum != WALK) {
                mevcutDurum = WALK;
                mevcutKare = 0; 
                animZaman = 0.0f;
            }

            if (klavyeGiris.x == 0 && klavyeGiris.y < 0)      mevcutYon = K;
            else if (klavyeGiris.x == 0 && klavyeGiris.y > 0) mevcutYon = G;
            else if (klavyeGiris.x > 0 && klavyeGiris.y == 0) mevcutYon = D;
            else if (klavyeGiris.x < 0 && klavyeGiris.y == 0) mevcutYon = B;
            else if (klavyeGiris.x > 0 && klavyeGiris.y > 0)  mevcutYon = KD;
            else if (klavyeGiris.x > 0 && klavyeGiris.y < 0)  mevcutYon = GD;
            else if (klavyeGiris.x < 0 && klavyeGiris.y > 0)  mevcutYon = KB;
            else if (klavyeGiris.x < 0 && klavyeGiris.y < 0)  mevcutYon = GB;

            hareket.x = klavyeGiris.y + klavyeGiris.x;
            hareket.y = klavyeGiris.y - klavyeGiris.x;
            float uzunluk = std::sqrt(hareket.x * hareket.x + hareket.y * hareket.y);
            hareket /= uzunluk;
        } else {
            mevcutDurum = IDLE;
        }

        if (mevcutDurum == WALK) {
            sf::Vector2f mevcutPozisyon = sprite.getPosition();
            sf::Vector2f yeniPozisyon   = mevcutPozisyon + (hareket * hareketHizi * deltaZaman);

            sprite.setPosition({yeniPozisyon.x, mevcutPozisyon.y});
            if (haritaCarpismaKontrolu(yeniPozisyon.x, mevcutPozisyon.y, genislik, yukseklik)) sprite.setPosition({mevcutPozisyon.x, mevcutPozisyon.y});

            mevcutPozisyon = sprite.getPosition();
            sprite.setPosition({mevcutPozisyon.x, yeniPozisyon.y});
            if (haritaCarpismaKontrolu(mevcutPozisyon.x, yeniPozisyon.y, genislik, yukseklik)) sprite.setPosition({mevcutPozisyon.x, mevcutPozisyon.y});

            animasyonIlerle(deltaZaman, sWalk); 
            spriteKareAyarla(sWalk);
        } else {
            animasyonIlerle(deltaZaman, sIdle); 
            spriteKareAyarla(sIdle);
        }
    }
};