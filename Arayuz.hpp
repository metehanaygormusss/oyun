#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>
#include "Oyuncu.hpp"
#include "Gorev.hpp"
#include "Harita.hpp"
#include "Dusman.hpp" 

struct CanavarIni {
    int id; 
    sf::Sprite sprite; 
    float x, y;
    float genislik = 40.0f; 
    float yukseklik = 40.0f;
    float mevcutCan = 300.0f; 
    float maxCan = 300.0f;
    float spawnSayaci = 0.0f; 
    int aktifDusmanSayisi = 0; 
    bool yokEdilsinMi = false;

    CanavarIni(int _id, float _x, float _y, const sf::Texture& doku) 
        : id(_id), sprite(doku), x(_x), y(_y) {
        sprite.setPosition({x, y});
        sf::FloatRect b = sprite.getLocalBounds();
        sprite.setOrigin({b.size.x / 2.0f, b.size.y / 2.0f});
        sprite.setScale({0.5f, 0.5f});
        sprite.setColor(sf::Color(100, 50, 50)); 
    }
};

class Arayuz {
public:
    sf::RectangleShape oyuncuCanBarOn;
    sf::RectangleShape oyuncuManaBarOn;
    sf::RectangleShape oyuncuExpBarOn;
    
    sf::RectangleShape menuArkaPlan;
    sf::RectangleShape portreKutusu;
    sf::RectangleShape ekipmanCizgisi;

    bool surukleniyorMu = false;
    int kaynakTip = -1; 
    int kaynakIndex = -1;
    Esya suruklenenEsya;

    bool duzenlemeModu = false;
    int suruklenenEleman = -1;
    int suruklenenSlot = -1;
    int suruklenenIslem = -1;
    sf::Vector2f suruklemeOffset = {0.0f, 0.0f};     

    Arayuz() {
        oyuncuCanBarOn.setFillColor(sf::Color(180, 20, 20)); 
        oyuncuManaBarOn.setFillColor(sf::Color(20, 80, 180)); 
        oyuncuExpBarOn.setFillColor(sf::Color(200, 150, 20)); 

        menuArkaPlan.setSize(sf::Vector2f({1000.0f, 600.0f}));
        menuArkaPlan.setFillColor(sf::Color(12, 20, 38, 250)); 
        menuArkaPlan.setOutlineColor(sf::Color(220, 170, 40)); 
        menuArkaPlan.setOutlineThickness(0.0f);
        menuArkaPlan.setPosition({300.0f, 110.0f});

        portreKutusu.setSize(sf::Vector2f({180.0f, 240.0f}));
        portreKutusu.setFillColor(sf::Color(18, 30, 55));
        portreKutusu.setOutlineColor(sf::Color(170, 130, 30));
        portreKutusu.setOutlineThickness(2.0f);
        portreKutusu.setPosition({350.0f, 200.0f});

        ekipmanCizgisi.setSize(sf::Vector2f({940.0f, 2.0f}));
        ekipmanCizgisi.setFillColor(sf::Color(170, 130, 30, 150));
        ekipmanCizgisi.setPosition({330.0f, 680.0f});
    }

    std::vector<std::string> kayitlariListele() {
        std::vector<std::string> dosyalar;
        for (const auto& entry : std::filesystem::directory_iterator(".")) {
            std::string isim = entry.path().filename().string();
            if (isim.find("kayit_") == 0 && isim.find(".txt") != std::string::npos) {
                dosyalar.push_back(isim.substr(0, isim.find(".txt")));
            }
        }
        return dosyalar;
    }

    sf::FloatRect getAksiyonSlotRect(int index) {
        if (index < 0 || index >= 8) return sf::FloatRect({0.0f, 0.0f}, {0.0f, 0.0f});
        return sf::FloatRect({slotX[index], slotY[index]}, {slotSize[index], slotSize[index]});
    }

    sf::FloatRect getSlotResizeHandleRect(int index) {
        if (index < 0 || index >= 8) return sf::FloatRect({0.0f, 0.0f}, {0.0f, 0.0f});
        float handleSize = 10.0f;
        return sf::FloatRect({slotX[index] + slotSize[index] - handleSize, slotY[index] + slotSize[index] - handleSize}, {handleSize, handleSize});
    }

    sf::FloatRect getTabRect() {
        return sf::FloatRect({tabX, tabY}, {tabW, tabH});
    }

    sf::FloatRect getTabResizeHandleRect() {
        float handleSize = 14.0f;
        return sf::FloatRect({tabX + tabW - handleSize, tabY + tabH - handleSize}, {handleSize, handleSize});
    }

    int statButonKontrol(sf::Vector2f farePos) {
        if (sf::FloatRect({850.0f, 195.0f}, {20.0f, 20.0f}).contains(farePos)) return 1; 
        if (sf::FloatRect({680.0f, 300.0f}, {20.0f, 20.0f}).contains(farePos)) return 2; 
        if (sf::FloatRect({680.0f, 340.0f}, {20.0f, 20.0f}).contains(farePos)) return 3; 
        if (sf::FloatRect({850.0f, 300.0f}, {20.0f, 20.0f}).contains(farePos)) return 4; 
        return 0;
    }

    int yetenekButonKontrol(sf::Vector2f farePos) {
        if (sf::FloatRect({900.0f, 295.0f}, {20.0f, 20.0f}).contains(farePos)) return 1;
        if (sf::FloatRect({900.0f, 365.0f}, {20.0f, 20.0f}).contains(farePos)) return 2;
        if (sf::FloatRect({900.0f, 435.0f}, {20.0f, 20.0f}).contains(farePos)) return 3;
        // YENİ: Patlayan ok geliştirme butonu tık kontrolü eklendi
        if (sf::FloatRect({900.0f, 505.0f}, {20.0f, 20.0f}).contains(farePos)) return 4;
        return 0;
    }

    void cizTooltip(sf::RenderWindow& window, sf::Font& font, sf::Vector2f pos, const std::string& baslik, const std::string& detay) {
        sf::Text tBaslik(font); tBaslik.setString(baslik); tBaslik.setCharacterSize(18); tBaslik.setFillColor(sf::Color(255, 215, 0));
        sf::Text tDetay(font); tDetay.setString(detay); tDetay.setCharacterSize(16); tDetay.setFillColor(sf::Color(220, 220, 220));

        float width = std::max(tBaslik.getLocalBounds().size.x, tDetay.getLocalBounds().size.x) + 20.0f;
        float height = tBaslik.getLocalBounds().size.y + tDetay.getLocalBounds().size.y + 35.0f;

        sf::RectangleShape arkaPlan({width, height});
        arkaPlan.setFillColor(sf::Color(10, 15, 25, 245));
        arkaPlan.setOutlineColor(sf::Color(200, 150, 40));
        arkaPlan.setOutlineThickness(2.0f);

        float pX = pos.x + 15.0f;
        float pY = pos.y + 15.0f;
        if (pX + width > 1600.0f) pX = pos.x - width - 10.0f;
        if (pY + height > 900.0f) pY = pos.y - height - 10.0f;

        arkaPlan.setPosition({pX, pY});
        tBaslik.setPosition({pX + 10.0f, pY + 10.0f});
        tDetay.setPosition({pX + 10.0f, pY + 32.0f});

        window.draw(arkaPlan);
        window.draw(tBaslik);
        window.draw(tDetay);
    }

    void cizAnaMenu(sf::RenderWindow& window, sf::Font& font, int altMenu, std::string inputMetin, sf::Texture& arkaplanDoku) {
        sf::Sprite bg(arkaplanDoku);
        sf::Vector2u size = arkaplanDoku.getSize();
        bg.setScale({1600.0f / size.x, 900.0f / size.y});
        window.draw(bg);

        // Şeffaf menü arka plan kutusu KILDIRILDI

        auto metinYaz = [&](const std::string& metin, float x, float y, int boyut, sf::Color renk) {
            sf::Text t(font); t.setString(metin); t.setCharacterSize(boyut);
            t.setFillColor(renk); t.setPosition({x, y}); window.draw(t);
        };

        float bX = 650.0f, bY = 350.0f;
        
        if (altMenu == 0) { 
            std::string butonlar[] = {"Yeni Oyun", "Kayit Yukle", "Ayarlar", "Cikis"};
            for(int i=0; i<4; i++) {
                sf::RectangleShape btn({300.0f, 50.0f}); btn.setPosition({bX, bY + i*80.0f});
                btn.setFillColor(sf::Color(18, 30, 55)); btn.setOutlineColor(sf::Color(170, 130, 30)); btn.setOutlineThickness(2.0f); window.draw(btn);
                
                sf::Text bT(font); bT.setString(butonlar[i]); bT.setCharacterSize(26); bT.setFillColor(sf::Color::White);
                sf::FloatRect textB = bT.getLocalBounds();
                bT.setPosition({bX + 150.0f - textB.size.x/2.0f, bY + i*80.0f + 10.0f}); window.draw(bT);
            }
        } else if (altMenu == 1) { 
            metinYaz("YUKLENECEK KAYDI SEC:", bX - 50.0f, bY - 40.0f, 22, sf::Color::Cyan);
            auto liste = kayitlariListele();
            for(size_t i=0; i<liste.size() && i < 4; i++) {
                sf::RectangleShape btn({300.0f, 50.0f}); btn.setPosition({bX, bY + i*80.0f});
                btn.setFillColor(sf::Color(18, 30, 55)); btn.setOutlineColor(sf::Color(170, 130, 30)); btn.setOutlineThickness(2.0f); window.draw(btn);
                metinYaz(liste[i], bX + 20.0f, bY + 10.0f + i*80.0f, 22, sf::Color::White);
            }
            metinYaz("Geri Don [ESC]", bX, bY + 340.0f, 20, sf::Color(240, 80, 80));
        }
    }

    int anaMenuButonKontrol(sf::Vector2f farePos, int altMenu) {
        float bX = 650.0f, bY = 350.0f;
        if (altMenu == 0) {
            for(int i=0; i<4; i++) if(sf::FloatRect({bX, bY + i*80.0f}, {300.0f, 50.0f}).contains(farePos)) return i+1; 
        } else if (altMenu == 1) {
            auto liste = kayitlariListele();
            for(size_t i=0; i<liste.size() && i < 4; i++) {
                if(sf::FloatRect({bX, bY + i*80.0f}, {300.0f, 50.0f}).contains(farePos)) return 11 + i; 
            }
        }
        return 0;
    }

    int arayuzButonKontrol(sf::Vector2f farePos) {
        sf::FloatRect rectEnvanter({1135.0f, 22.0f}, {90.0f, 97.0f});
        sf::FloatRect rectGorevler({1247.0f, 22.0f}, {90.0f, 97.0f});
        sf::FloatRect rectAyarlar({1360.0f, 22.0f}, {90.0f, 97.0f});
        sf::FloatRect rectMenu({1487.0f, 22.0f}, {75.0f, 97.0f});
        sf::FloatRect rectPortre({15.0f, 15.0f}, {130.0f, 130.0f}); 

        if (rectEnvanter.contains(farePos)) return 1; 
        if (rectGorevler.contains(farePos)) return 2; 
        if (rectAyarlar.contains(farePos))  return 4; 
        if (rectMenu.contains(farePos))     return 3; 
        if (rectPortre.contains(farePos))   return 5; 

        return 0; 
    }

    int ayarlarButonKontrol(sf::Vector2f farePos, int altMenu) {
        float bX = 650.0f, bY = 230.0f;
        if (altMenu == 0) { 
            if(sf::FloatRect({bX, bY}, {300.0f, 40.0f}).contains(farePos)) return 1; 
            if(sf::FloatRect({bX, bY+55.0f}, {300.0f, 40.0f}).contains(farePos)) return 2; 
            if(sf::FloatRect({bX, bY+110.0f}, {300.0f, 40.0f}).contains(farePos)) return 6; 
            if(sf::FloatRect({bX, bY+165.0f}, {300.0f, 40.0f}).contains(farePos)) return 7; 
            if(sf::FloatRect({bX, bY+220.0f}, {300.0f, 40.0f}).contains(farePos)) return 3; 
            if(sf::FloatRect({bX, bY+275.0f}, {300.0f, 40.0f}).contains(farePos)) return 4; 
            if(sf::FloatRect({bX, bY+330.0f}, {300.0f, 40.0f}).contains(farePos)) return 8; 
            if(sf::FloatRect({bX, bY+385.0f}, {300.0f, 40.0f}).contains(farePos)) return 5; 
        } else if (altMenu == 1) { 
            auto liste = kayitlariListele();
            for(size_t i=0; i<liste.size() && i < 6; i++) {
                if(sf::FloatRect({bX, bY + i*50.0f}, {300.0f, 40.0f}).contains(farePos)) return 11 + i;
            }
        }
        return 0;
    }

    void cizDuzenlemeModu(sf::RenderWindow& window, sf::Font& font) {
        sf::RectangleShape bg({1600.0f, 900.0f});
        bg.setFillColor(sf::Color(0, 0, 0, 180)); 
        window.draw(bg);

        sf::CircleShape mmBg(90.0f);
        mmBg.setOrigin({90.0f, 90.0f});
        mmBg.setPosition({miniMapX, miniMapY});
        mmBg.setFillColor(sf::Color(255, 255, 0, 100)); 
        mmBg.setOutlineColor(sf::Color::Yellow);
        mmBg.setOutlineThickness(2.0f);
        window.draw(mmBg);
        
        for (int i = 0; i < 8; i++) {
            sf::FloatRect slotRect = getAksiyonSlotRect(i);
            sf::FloatRect handleRect = getSlotResizeHandleRect(i);
            
            sf::RectangleShape slot(sf::Vector2f({slotRect.size.x, slotRect.size.y}));
            slot.setPosition({slotRect.position.x, slotRect.position.y});
            slot.setFillColor(sf::Color(255, 255, 0, 80));
            slot.setOutlineColor(sf::Color::Yellow);
            slot.setOutlineThickness(2.0f);
            window.draw(slot);
            
            sf::Text slotNum(font);
            slotNum.setString(std::to_string(i + 1));
            slotNum.setCharacterSize(16);
            slotNum.setFillColor(sf::Color::White);
            slotNum.setPosition({slotRect.position.x + slotRect.size.x / 2.0f - 5.0f, slotRect.position.y + slotRect.size.y / 2.0f - 8.0f});
            window.draw(slotNum);
            
            sf::CircleShape handle(5.0f);
            handle.setOrigin({5.0f, 5.0f});
            handle.setPosition({handleRect.position.x + handleRect.size.x / 2.0f, handleRect.position.y + handleRect.size.y / 2.0f});
            handle.setFillColor(sf::Color(255, 100, 100));
            handle.setOutlineColor(sf::Color(255, 255, 255));
            handle.setOutlineThickness(1.0f);
            window.draw(handle);
        }

        {
            sf::FloatRect tabRect = getTabRect();
            sf::FloatRect tabHandle = getTabResizeHandleRect();

            sf::RectangleShape tabGosterge(sf::Vector2f({tabRect.size.x, tabRect.size.y}));
            tabGosterge.setPosition({tabRect.position.x, tabRect.position.y});
            tabGosterge.setFillColor(sf::Color(100, 200, 255, 50));
            tabGosterge.setOutlineColor(sf::Color(100, 200, 255));
            tabGosterge.setOutlineThickness(2.0f);
            window.draw(tabGosterge);

            sf::Text tabEtiket(font);
            tabEtiket.setString("Tab Arkaplan");
            tabEtiket.setCharacterSize(14);
            tabEtiket.setFillColor(sf::Color(100, 200, 255));
            tabEtiket.setPosition({tabRect.position.x + 6.0f, tabRect.position.y + 4.0f});
            window.draw(tabEtiket);

            sf::RectangleShape tabHandleBox({tabHandle.size.x, tabHandle.size.y});
            tabHandleBox.setPosition({tabHandle.position.x, tabHandle.position.y});
            tabHandleBox.setFillColor(sf::Color(100, 200, 255));
            tabHandleBox.setOutlineColor(sf::Color::White);
            tabHandleBox.setOutlineThickness(1.0f);
            window.draw(tabHandleBox);
        }

        sf::Text uyari(font);
        uyari.setString("ARAYUZ DUZENLEME MODU\nSari: Skill slotlari | Mavi: Tab arkaplan\nKose kutucuklari ile boyutlandir.\nCikmak ve kaydetmek icin ESC'ye basin.");
        uyari.setCharacterSize(24);
        uyari.setFillColor(sf::Color::White);
        
        sf::FloatRect bounds = uyari.getLocalBounds();
        uyari.setPosition({800.0f - bounds.size.x / 2.0f, 400.0f});
        window.draw(uyari);
    }

    void cizSabitHUD(sf::RenderWindow& window, const sf::Texture& uiDokusu, sf::Font& font, Oyuncu& oyuncu) {
        int texW = static_cast<int>(uiDokusu.getSize().x);
        int texH = static_cast<int>(uiDokusu.getSize().y);
        float scaleX = (1600.0f * 0.75f) / texW;
        float scaleY = (900.0f * 0.75f) / texH;

        sf::Sprite uiSolUst(uiDokusu);
        uiSolUst.setTextureRect(sf::IntRect({0, 0}, {texW / 2, texH / 2}));
        uiSolUst.setScale({scaleX, scaleY});
        uiSolUst.setPosition({0.0f, 0.0f});
        window.draw(uiSolUst);

        sf::Sprite uiSagUst(uiDokusu);
        uiSagUst.setTextureRect(sf::IntRect({texW / 2, 0}, {texW / 2, texH / 2}));
        uiSagUst.setScale({scaleX, scaleY});
        uiSagUst.setPosition({1600.0f - (texW / 2.0f * scaleX), 0.0f});
        window.draw(uiSagUst);

        sf::Sprite uiAlt(uiDokusu);
        uiAlt.setTextureRect(sf::IntRect({0, texH / 2}, {texW, texH / 2}));
        uiAlt.setScale({scaleX, scaleY});
        uiAlt.setPosition({(1600.0f - (texW * scaleX)) / 2.0f, 900.0f - (texH / 2.0f * scaleY)});
        window.draw(uiAlt);

        float canOran = oyuncu.mevcutCan / oyuncu.maxCan; if (canOran < 0.0f) canOran = 0.0f;
        float manaOran = oyuncu.mevcutMana / oyuncu.maxMana; if (manaOran < 0.0f) manaOran = 0.0f;
        float expOran = oyuncu.exp / oyuncu.maxExp; if (expOran < 0.0f) expOran = 0.0f;
        float maxBarGenislik = 318.0f; 
        
        oyuncuCanBarOn.setSize({maxBarGenislik * canOran, 22.0f}); oyuncuCanBarOn.setPosition({220.0f, 38.0f}); window.draw(oyuncuCanBarOn);
        oyuncuManaBarOn.setSize({maxBarGenislik * manaOran, 22.0f}); oyuncuManaBarOn.setPosition({220.0f, 76.0f}); window.draw(oyuncuManaBarOn);
        oyuncuExpBarOn.setSize({maxBarGenislik * expOran, 16.0f}); oyuncuExpBarOn.setPosition({220.0f, 111.0f}); window.draw(oyuncuExpBarOn);

        sf::Text levelText(font); levelText.setString(std::to_string(oyuncu.seviye)); 
        levelText.setCharacterSize(16); levelText.setFillColor(sf::Color(255, 215, 0)); levelText.setPosition({75.0f, 126.0f}); window.draw(levelText);
    }

    void cizAksiyonBari(sf::RenderWindow& window, sf::Font& font, Oyuncu& oyuncu, const sf::Texture& cokluDoku, const sf::Texture& atilmaDoku, const sf::Texture& ekstraDoku, const sf::Texture& patlamaOkDoku, sf::Vector2f farePos) {
        
        std::string hoverBaslik = "";
        std::string hoverDetay = "";

        for (int i = 0; i < 8; i++) {
            sf::FloatRect rect = getAksiyonSlotRect(i);
            float posX = rect.position.x;
            float posY = rect.position.y;
            float boyut = rect.size.x;

            sf::RectangleShape kutu({boyut, boyut});
            kutu.setPosition({posX, posY});
            kutu.setFillColor(sf::Color(15, 20, 30, 200));
            kutu.setOutlineThickness(0.0f);
            window.draw(kutu);

            sf::Text tusTxt(font);
            tusTxt.setString(std::to_string(i + 1));
            tusTxt.setCharacterSize(14);
            tusTxt.setFillColor(sf::Color(150, 150, 150));
            tusTxt.setPosition({posX + 4.0f, posY + 2.0f});
            window.draw(tusTxt);

            if (oyuncu.hizliErisim[i] == "") continue; 

            float beklemeSuresi = 0.0f;
            const sf::Texture* ikonDoku = nullptr;

            if (oyuncu.hizliErisim[i] == "Coklu Atis") { beklemeSuresi = oyuncu.cokluAtisBekleme; ikonDoku = &cokluDoku; }
            else if (oyuncu.hizliErisim[i] == "Atilma") { beklemeSuresi = oyuncu.atilmaBekleme; ikonDoku = &atilmaDoku; }
            else if (oyuncu.hizliErisim[i] == "Ekstra Atis") { beklemeSuresi = oyuncu.ekstraBekleme; ikonDoku = &ekstraDoku; }
            else if (oyuncu.hizliErisim[i] == "Patlayan Ok") { beklemeSuresi = oyuncu.patlamaBekleme; ikonDoku = &patlamaOkDoku; }

            if (ikonDoku) {
                sf::Sprite ikon(*ikonDoku);
                ikon.setScale({boyut / ikonDoku->getSize().x, boyut / ikonDoku->getSize().y});
                ikon.setPosition({posX, posY}); window.draw(ikon);

                if (beklemeSuresi > 0.0f) {
                    sf::RectangleShape karartma({boyut, boyut}); karartma.setFillColor(sf::Color(0,0,0,180)); karartma.setPosition({posX, posY}); window.draw(karartma);
                    sf::Text sureText(font); char buffer[10]; snprintf(buffer, sizeof(buffer), "%.1f", beklemeSuresi);
                    sureText.setString(buffer); sureText.setCharacterSize(16); sureText.setFillColor(sf::Color(255,215,0));
                    sf::FloatRect bounds = sureText.getLocalBounds();
                    sureText.setPosition({posX + (boyut - bounds.size.x)/2.0f, posY + (boyut - bounds.size.y)/2.0f - 5.0f}); window.draw(sureText);
                }
            } 
            else if (oyuncu.hizliErisim[i] == "Can Potu") {
                int toplamPot = 0; sf::Texture* potDoku = nullptr;
                for(int k=0; k<42; k++) {
                    if (oyuncu.envanter[k].doluMu && oyuncu.envanter[k].isim == "Can Potu") {
                        toplamPot += oyuncu.envanter[k].miktar;
                        potDoku = &oyuncu.envanter[k].doku;
                    }
                }
                if (toplamPot > 0 && potDoku) {
                    sf::Sprite ikon(*potDoku);
                    ikon.setScale({boyut / potDoku->getSize().x, boyut / potDoku->getSize().y});
                    ikon.setPosition({posX, posY}); window.draw(ikon);

                    sf::Text miktar(font); miktar.setString(std::to_string(toplamPot)); miktar.setCharacterSize(14);
                    miktar.setFillColor(sf::Color::White); miktar.setPosition({posX + boyut*0.6f, posY + boyut*0.6f}); window.draw(miktar);
                } else {
                    oyuncu.hizliErisim[i] = ""; 
                }
            }

            if (rect.contains(farePos)) {
                if (oyuncu.hizliErisim[i] == "Coklu Atis") {
                    hoverBaslik = "Coklu Atis"; hoverDetay = "Seviye: " + std::to_string(oyuncu.skillCokluAtisLvl) + "\nBekleme: " + std::to_string((int)std::max(5.0f, 15.0f - oyuncu.skillCokluAtisLvl * 1.5f)) + "sn\nHasar Bonusu: +" + std::to_string(oyuncu.skillCokluAtisLvl * 5) + "\nMana Bedeli: 25\nEtki: Pasif olarak seri atis yapar.";
                } else if (oyuncu.hizliErisim[i] == "Atilma") {
                    hoverBaslik = "Atilma"; hoverDetay = "Seviye: " + std::to_string(oyuncu.skillAtilmaLvl) + "\nBekleme: " + std::to_string((int)std::max(2.0f, 8.0f - oyuncu.skillAtilmaLvl * 1.0f)) + "sn\nMana Bedeli: 15\nEtki: Fare yonune pruzsuzce kayar.";
                } else if (oyuncu.hizliErisim[i] == "Ekstra Atis") {
                    hoverBaslik = "Ekstra Atis"; hoverDetay = "Seviye: " + std::to_string(oyuncu.skillEkstraLvl) + "\nBekleme: " + std::to_string((int)std::max(10.0f, 30.0f - oyuncu.skillEkstraLvl * 2.0f)) + "sn\nMana Bedeli: 30\nEtki: " + std::to_string((int)(10.0f + oyuncu.skillEkstraLvl * 2.0f)) + "sn boyunca cift ok atar.";
                } else if (oyuncu.hizliErisim[i] == "Patlayan Ok") {
                    hoverBaslik = "Patlayan Ok"; hoverDetay = "Seviye: " + std::to_string(oyuncu.skillPatlamaLvl) + "\nBekleme: " + std::to_string((int)std::max(8.0f, 20.0f - oyuncu.skillPatlamaLvl * 2.0f)) + "sn\nMana Bedeli: 40\nEtki: Genis alana yuksek hasar verir.";
                } else if (oyuncu.hizliErisim[i] == "Can Potu") {
                    hoverBaslik = "Can Potu"; hoverDetay = "Etki: Aninda 50 Can yeniler.\n(Istemedigin skilleri Sag Tik ile silebilirsin)";
                }
            }
        }

        if (hoverBaslik != "" && !surukleniyorMu) {
            cizTooltip(window, font, farePos, hoverBaslik, hoverDetay);
        }
    }

    void cizMiniMap(sf::RenderWindow& window, sf::Vector2f oyuncuPos, std::vector<Dusman>& dusmanlar, std::vector<CanavarIni>& inler, sf::Vector2f dedePos) {
        float mmX = miniMapX; 
        float mmY = miniMapY;
        float mmRadius = 90.0f;
        float dunyaOlcek = 0.05f; 

        sf::CircleShape arkaPlan(mmRadius);
        arkaPlan.setOrigin({mmRadius, mmRadius});
        arkaPlan.setPosition({mmX, mmY});
        arkaPlan.setFillColor(sf::Color(10, 15, 20, 240)); 
        arkaPlan.setOutlineColor(sf::Color(170, 130, 30));
        arkaPlan.setOutlineThickness(3.0f);
        window.draw(arkaPlan);

        sf::VertexArray minimapTiles(sf::PrimitiveType::Triangles);
        float tileMmSize = tileSize * dunyaOlcek; 

        int pGridX = static_cast<int>(oyuncuPos.x / tileSize);
        int pGridY = static_cast<int>(oyuncuPos.y / tileSize);
        int gorusTile = static_cast<int>((mmRadius / dunyaOlcek) / tileSize) + 1;

        int basY = std::max(0, pGridY - gorusTile);
        int bitY = std::min(MAP_HEIGHT - 1, pGridY + gorusTile);
        int basX = std::max(0, pGridX - gorusTile);
        int bitX = std::min(MAP_WIDTH - 1, pGridX + gorusTile);

        for (int y = basY; y <= bitY; ++y) {
            for (int x = basX; x <= bitX; ++x) {
                int tileTip = harita[y][x];
                
                sf::Color tileRenk;
                if (tileTip == 0) tileRenk = sf::Color(34, 100, 34, 180); 
                else if (tileTip == 1) tileRenk = sf::Color(120, 120, 120, 200); 
                else if (tileTip == 2) tileRenk = sf::Color(0, 80, 0, 200);   
                else if (tileTip == 3) tileRenk = sf::Color(30, 100, 200, 200); 
                else if (tileTip == 4) tileRenk = sf::Color(139, 69, 19, 200); 
                else if (tileTip == 8) tileRenk = sf::Color(244, 164, 96, 200); 
                else continue;

                float gercekX = x * tileSize;
                float gercekY = y * tileSize;

                float dx = gercekX - oyuncuPos.x;
                float dy = gercekY - oyuncuPos.y;
                float mesafe = std::sqrt(dx*dx + dy*dy) * dunyaOlcek;

                if (mesafe < mmRadius - tileMmSize) {
                    float cX = mmX + dx * dunyaOlcek;
                    float cY = mmY + dy * dunyaOlcek;

                    sf::Vector2f p1{cX, cY};
                    sf::Vector2f p2{cX + tileMmSize, cY};
                    sf::Vector2f p3{cX + tileMmSize, cY + tileMmSize};
                    sf::Vector2f p4{cX, cY + tileMmSize};

                    minimapTiles.append(sf::Vertex{p1, tileRenk});
                    minimapTiles.append(sf::Vertex{p2, tileRenk});
                    minimapTiles.append(sf::Vertex{p3, tileRenk});
                    minimapTiles.append(sf::Vertex{p3, tileRenk});
                    minimapTiles.append(sf::Vertex{p4, tileRenk});
                    minimapTiles.append(sf::Vertex{p1, tileRenk});
                }
            }
        }
        window.draw(minimapTiles);

        auto noktaCiz = [&](sf::Vector2f gercekHedef, sf::Color renk, float yariCap = 3.0f) {
            float dx = gercekHedef.x - oyuncuPos.x;
            float dy = gercekHedef.y - oyuncuPos.y;
            float mesafe = std::sqrt(dx*dx + dy*dy) * dunyaOlcek;
            if (mesafe < mmRadius - yariCap) { 
                sf::CircleShape nokta(yariCap);
                nokta.setOrigin({yariCap, yariCap});
                nokta.setFillColor(renk);
                nokta.setPosition({mmX + dx * dunyaOlcek, mmY + dy * dunyaOlcek});
                window.draw(nokta);
            }
        };

        noktaCiz(dedePos, sf::Color::Green, 4.0f);
        for(auto& in : inler) if(!in.yokEdilsinMi) noktaCiz({in.x, in.y}, sf::Color::Magenta, 5.0f);
        for(auto& d : dusmanlar) {
            sf::Vector2f dM = d.sprite.getPosition() + sf::Vector2f(d.genislik/2.0f, d.yukseklik/2.0f);
            noktaCiz(dM, sf::Color::Red, 2.5f);
        }
        
        sf::CircleShape oNokta(4.0f); oNokta.setOrigin({4.0f, 4.0f});
        oNokta.setFillColor(sf::Color(100, 200, 255)); oNokta.setPosition({mmX, mmY}); window.draw(oNokta);
    }

    void cizHaritaEkrani(sf::RenderWindow& window, sf::Font& font, sf::Vector2f oyuncuPos) {
        window.draw(menuArkaPlan); 
        sf::Text title(font);
        title.setString("DUNYA HARITASI");
        title.setCharacterSize(24);
        title.setFillColor(sf::Color(240, 200, 40));
        
        float centerX = menuArkaPlan.getPosition().x + menuArkaPlan.getSize().x * 0.5f;
        float centerY = menuArkaPlan.getPosition().y + menuArkaPlan.getSize().y * 0.5f;
        
        float titleY = menuArkaPlan.getPosition().y + 24.0f;
        sf::FloatRect bounds = title.getLocalBounds();
        title.setPosition({centerX - (bounds.size.x / 2.0f), titleY});
        window.draw(title);

        float miniIsoW = 5.0f; 
        float miniIsoH = 2.5f; 

        float mapStartX = centerX - (MAP_WIDTH - MAP_HEIGHT) * (miniIsoW / 4.0f);
        float mapStartY = centerY - (MAP_WIDTH + MAP_HEIGHT) * (miniIsoH / 4.0f) + 20.0f; 

        sf::ConvexShape mapBg(4);
        mapBg.setPoint(0, sf::Vector2f(0, 0)); 
        mapBg.setPoint(1, sf::Vector2f(MAP_WIDTH * (miniIsoW / 2.0f), MAP_WIDTH * (miniIsoH / 2.0f))); 
        mapBg.setPoint(2, sf::Vector2f((MAP_WIDTH - MAP_HEIGHT) * (miniIsoW / 2.0f), (MAP_WIDTH + MAP_HEIGHT) * (miniIsoH / 2.0f))); 
        mapBg.setPoint(3, sf::Vector2f(-MAP_HEIGHT * (miniIsoW / 2.0f), MAP_HEIGHT * (miniIsoH / 2.0f))); 
        
        mapBg.setFillColor(sf::Color(34, 50, 34)); 
        mapBg.setOutlineColor(sf::Color(170, 130, 30)); 
        mapBg.setOutlineThickness(3.0f);
        mapBg.setPosition({mapStartX, mapStartY});
        window.draw(mapBg);

        sf::ConvexShape tileShape(4);
        tileShape.setPoint(0, sf::Vector2f(0, 0));
        tileShape.setPoint(1, sf::Vector2f(miniIsoW / 2.0f, miniIsoH / 2.0f));
        tileShape.setPoint(2, sf::Vector2f(0, miniIsoH));
        tileShape.setPoint(3, sf::Vector2f(-miniIsoW / 2.0f, miniIsoH / 2.0f));

        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int t = harita[y][x];
                if (t == 0) continue; 

                tileShape.setPosition({mapStartX + (x - y) * (miniIsoW / 2.0f), mapStartY + (x + y) * (miniIsoH / 2.0f)});

                if (t == 1) tileShape.setFillColor(sf::Color(100, 100, 100)); 
                else if (t == 2) tileShape.setFillColor(sf::Color(20, 100, 20)); 
                else if (t == 3) tileShape.setFillColor(sf::Color(30, 100, 200)); 
                else if (t == 4) tileShape.setFillColor(sf::Color(139, 69, 19)); 
                else if (t == 8) tileShape.setFillColor(sf::Color(244, 164, 96)); 

                window.draw(tileShape);
            }
        }

        float pGridX = oyuncuPos.x / tileSize;
        float pGridY = oyuncuPos.y / tileSize;
        
        float pMapX = mapStartX + (pGridX - pGridY) * (miniIsoW / 2.0f);
        float pMapY = mapStartY + (pGridX + pGridY) * (miniIsoH / 2.0f);

        sf::CircleShape pMarker(5.0f); 
        pMarker.setFillColor(sf::Color::Green);
        pMarker.setOutlineColor(sf::Color(255, 255, 255)); 
        pMarker.setOutlineThickness(2.0f);
        pMarker.setOrigin({5.0f, 5.0f}); 
        pMarker.setPosition({pMapX, pMapY});
        window.draw(pMarker);
    }

    // YENİ: patlamaOkDoku parametresi buraya tekrar eklendi.
    void cizKarakterEkrani(sf::RenderWindow& window, sf::Font& font, Oyuncu& oyuncu, const sf::Texture& durDoku, int& karakterSekmesi, bool solTikBasildi, bool solTikBirakildi, sf::Vector2f farePos, const sf::Texture& cokluDoku, const sf::Texture& atilmaDoku, const sf::Texture& ekstraDoku, const sf::Texture& patlamaOkDoku) {
        
        if (solTikBirakildi && surukleniyorMu && kaynakTip == 3) {
            for(int i=0; i<8; i++) {
                if (getAksiyonSlotRect(i).contains(farePos)) {
                    oyuncu.hizliErisim[i] = suruklenenEsya.isim; 
                    break;
                }
            }
            surukleniyorMu = false;
        }

        window.draw(menuArkaPlan);

        auto metinYaz = [&](const std::string& metin, float x, float y, unsigned int boyut, sf::Color renk) {
            sf::Text t(font); t.setString(metin); t.setCharacterSize(boyut);
            t.setFillColor(renk); t.setPosition({x, y}); window.draw(t);
        };

        sf::FloatRect statBtn({570.0f, 150.0f}, {150.0f, 40.0f});
        sf::FloatRect skillBtn({740.0f, 150.0f}, {150.0f, 40.0f});
        
        if (solTikBasildi && statBtn.contains(farePos)) karakterSekmesi = 0;
        if (solTikBasildi && skillBtn.contains(farePos)) karakterSekmesi = 1;

        metinYaz("Istatistikler", 580.0f, 155.0f, 22, (karakterSekmesi==0)?sf::Color::Yellow:sf::Color::White);
        metinYaz("Yetenek Agaci", 750.0f, 155.0f, 22, (karakterSekmesi==1)?sf::Color::Yellow:sf::Color::White);

        window.draw(portreKutusu);
        sf::Sprite avatar(durDoku);
        avatar.setTextureRect(sf::IntRect({0, 0}, {64, 64})); 
        float scale = std::min((180.0f * 0.9f) / 64.0f, (240.0f * 0.9f) / 64.0f);
        avatar.setScale({scale, scale});
        avatar.setPosition({340.0f + (180.0f - 64.0f * scale)/2.0f, 200.0f + (240.0f - 64.0f * scale)/2.0f});
        window.draw(avatar);
        metinYaz("Meto", 340.0f, 460.0f, 26, sf::Color::White);
        metinYaz("Korucu", 340.0f, 495.0f, 20, sf::Color(150, 150, 150));

        std::string artiHoverBaslik = "";
        std::string artiHoverDetay = "";
        
        auto cizArtiBtn = [&](float x, float y, std::string baslik, std::string detay) {
            sf::RectangleShape btn({20.0f, 20.0f}); btn.setPosition({x, y});
            btn.setFillColor(sf::Color(50, 150, 50)); btn.setOutlineColor(sf::Color::White); btn.setOutlineThickness(1.0f); window.draw(btn);
            metinYaz("+", x + 4.0f, y - 4.0f, 20, sf::Color::White);
            if (sf::FloatRect({x, y}, {20.0f, 20.0f}).contains(farePos)) {
                artiHoverBaslik = baslik; artiHoverDetay = detay;
            }
        };

        if (karakterSekmesi == 0) { 
            int toplamCan = 0, toplamSaldiri = 20 + oyuncu.statSaldiriGucu, toplamZirh = 10 + oyuncu.statZirh;
            for(int i=0; i<6; i++) { if(oyuncu.ekipmanlar[i].doluMu) { toplamSaldiri += oyuncu.ekipmanlar[i].saldiriGucu; toplamZirh += oyuncu.ekipmanlar[i].zirh; } }

            metinYaz("Seviye: " + std::to_string(oyuncu.seviye), 570.0f, 200.0f, 26, sf::Color(240, 200, 40));
            metinYaz("Can: " + std::to_string((int)oyuncu.mevcutCan) + "/" + std::to_string((int)oyuncu.maxCan), 740.0f, 200.0f, 24, sf::Color::Red);
            metinYaz("Mana: " + std::to_string((int)oyuncu.mevcutMana) + "/" + std::to_string((int)oyuncu.maxMana), 960.0f, 200.0f, 24, sf::Color::Cyan);
            
            metinYaz("--- ISTATISTIKLER ---", 570.0f, 260.0f, 24, sf::Color(40, 180, 240));
            metinYaz("GUC: " + std::to_string(20 + oyuncu.statSaldiriGucu), 570.0f, 300.0f, 22, sf::Color::White);
            metinYaz("SAV: " + std::to_string(10 + oyuncu.statZirh), 570.0f, 340.0f, 22, sf::Color::White);
            metinYaz("HIZ: " + std::to_string((int)oyuncu.hareketHizi), 740.0f, 300.0f, 22, sf::Color::White);
            metinYaz("BIL: 10", 740.0f, 340.0f, 22, sf::Color::White); 

            metinYaz("--- SALDIRI ---", 570.0f, 420.0f, 24, sf::Color(240, 80, 80));
            metinYaz("Fiziksel Hasar: " + std::to_string(toplamSaldiri), 570.0f, 460.0f, 20, sf::Color(220, 220, 220));
            metinYaz("Kritik Sans: %5", 570.0f, 500.0f, 20, sf::Color(220, 220, 220));

            metinYaz("--- SAVUNMA ---", 860.0f, 420.0f, 24, sf::Color(80, 240, 120));
            metinYaz("Fiziksel Savunma: " + std::to_string(toplamZirh), 860.0f, 460.0f, 20, sf::Color(220, 220, 220));
            metinYaz("Bloklama: %10", 860.0f, 500.0f, 20, sf::Color(220, 220, 220));
            
            if (oyuncu.statPuanlari > 0) {
                metinYaz("Stat Puani: " + std::to_string(oyuncu.statPuanlari), 570.0f, 380.0f, 20, sf::Color::Green);
                cizArtiBtn(850.0f, 195.0f, "Can Gelistirmesi", "+20 Maksimum Can"); 
                cizArtiBtn(680.0f, 300.0f, "Guc Gelistirmesi", "+3 Fiziksel Hasar"); 
                cizArtiBtn(680.0f, 340.0f, "Savunma Gelistirmesi", "+2 Fiziksel Savunma"); 
                cizArtiBtn(850.0f, 300.0f, "Ceviklik Gelistirmesi", "+5 Hareket Hizi"); 
            }
        } 
        else if (karakterSekmesi == 1) { 
            metinYaz("Bosta Yetenek Puani: " + std::to_string(oyuncu.yetenekPuanlari), 570.0f, 220.0f, 24, sf::Color::Green);
            metinYaz("Surukleyerek Kisayol Barina tasiyabilir veya Sag Tik ile silebilirsin.", 570.0f, 250.0f, 16, sf::Color(150, 150, 150));

            std::string hoverBaslik = ""; std::string hoverDetay = "";

            auto cizSkill = [&](float sX, float sY, const std::string& isim, const sf::Texture& doku, int level, const std::string& detay) {
                sf::RectangleShape skillBox({50.0f, 50.0f});
                skillBox.setFillColor(sf::Color(50, 50, 80));
                skillBox.setOutlineColor(level > 0 ? sf::Color(200, 150, 40) : sf::Color(100, 100, 100));
                skillBox.setOutlineThickness(2.0f);
                skillBox.setPosition({sX, sY}); window.draw(skillBox);

                sf::Sprite ikon(doku);
                ikon.setScale({46.0f / doku.getSize().x, 46.0f / doku.getSize().y});
                ikon.setPosition({sX + 2.0f, sY + 2.0f});
                if(level == 0) ikon.setColor(sf::Color(100, 100, 100, 150));
                window.draw(ikon);

                metinYaz(isim + " (Lvl " + std::to_string(level) + ")", sX + 65.0f, sY + 15.0f, 20, level > 0 ? sf::Color::Yellow : sf::Color(150, 150, 150));

                if (sf::FloatRect({sX, sY}, {50.0f, 50.0f}).contains(farePos)) {
                    hoverBaslik = isim; hoverDetay = detay;
                    if (solTikBasildi && !surukleniyorMu && level > 0) {
                        surukleniyorMu = true; kaynakTip = 3; 
                        suruklenenEsya.isim = isim; suruklenenEsya.tur = "Yetenek"; suruklenenEsya.doku = doku;
                    }
                }
            };

            cizSkill(570.0f, 290.0f, "Coklu Atis", cokluDoku, oyuncu.skillCokluAtisLvl, "Seviye: " + std::to_string(oyuncu.skillCokluAtisLvl) + "\nBekleme: " + std::to_string((int)std::max(5.0f, 15.0f - oyuncu.skillCokluAtisLvl * 1.5f)) + "sn\nHasar Bonusu: +" + std::to_string(oyuncu.skillCokluAtisLvl * 5) + "\nMana Bedeli: 25\n\nPasif olarak seri atis yapar.");
            cizSkill(570.0f, 360.0f, "Atilma", atilmaDoku, oyuncu.skillAtilmaLvl, "Seviye: " + std::to_string(oyuncu.skillAtilmaLvl) + "\nBekleme: " + std::to_string((int)std::max(2.0f, 8.0f - oyuncu.skillAtilmaLvl * 1.0f)) + "sn\nMana Bedeli: 15\n\nFare yonune pruzsuzce kayar.");
            cizSkill(570.0f, 430.0f, "Ekstra Atis", ekstraDoku, oyuncu.skillEkstraLvl, "Seviye: " + std::to_string(oyuncu.skillEkstraLvl) + "\nBekleme: " + std::to_string((int)std::max(10.0f, 30.0f - oyuncu.skillEkstraLvl * 2.0f)) + "sn\nSure: " + std::to_string((int)(10.0f + oyuncu.skillEkstraLvl * 2.0f)) + "sn\nMana Bedeli: 30\n\nBelirli sure cift ok atar.");
            
            // YENİ: Patlayan Ok arayüz çizimi
            cizSkill(570.0f, 500.0f, "Patlayan Ok", patlamaOkDoku, oyuncu.skillPatlamaLvl, "Seviye: " + std::to_string(oyuncu.skillPatlamaLvl) + "\nBekleme: " + std::to_string((int)std::max(8.0f, 20.0f - oyuncu.skillPatlamaLvl * 2.0f)) + "sn\nMana Bedeli: 40\n\nDusmanlara carptiginda alan hasari vurur.");

            if (oyuncu.yetenekPuanlari > 0) {
                cizArtiBtn(900.0f, 295.0f, "Coklu Atis Gelistirmesi", "Sonraki Seviye:\nHasar: +5\nBekleme Suresi: -1.5sn");
                cizArtiBtn(900.0f, 365.0f, "Atilma Gelistirmesi", "Sonraki Seviye:\nKayma Mesafesi Artar\nBekleme Suresi: -1.0sn");
                cizArtiBtn(900.0f, 435.0f, "Ekstra Atis Gelistirmesi", "Sonraki Seviye:\nEtki Suresi: +2.0sn\nBekleme Suresi: -2.0sn");
                // YENİ: Patlayan Ok "+" artırma butonu çizimi
                cizArtiBtn(900.0f, 505.0f, "Patlayan Ok Gelistirmesi", "Sonraki Seviye:\nAlan Hasari: +50\nBekleme Suresi: -2.0sn");
            }

            if (hoverBaslik != "" && !surukleniyorMu) {
                cizTooltip(window, font, farePos, hoverBaslik, hoverDetay);
            }
        }
        
        if (artiHoverBaslik != "") {
            cizTooltip(window, font, farePos, artiHoverBaslik, artiHoverDetay);
        }
    }

    void cizEnvanterEkrani(sf::RenderWindow& window, sf::Font& font, Oyuncu& oyuncu, const sf::Texture& durDoku, sf::Vector2f farePos, bool solTikBasildi, bool solTikBirakildi, bool sagTikBasildi) {
        window.draw(menuArkaPlan);

        sf::Text title(font); title.setString(""); title.setCharacterSize(36); title.setFillColor(sf::Color(240, 200, 40));
        float centerX = menuArkaPlan.getPosition().x + menuArkaPlan.getSize().x * 0.5f; float titleY = menuArkaPlan.getPosition().y + 24.0f;
        sf::FloatRect bounds = title.getLocalBounds(); title.setPosition({centerX - (bounds.size.x / 2.0f), titleY}); window.draw(title);

        sf::RectangleShape dikeyCizgi({2.0f, 440.0f}); dikeyCizgi.setFillColor(sf::Color(170, 130, 30, 150));
        dikeyCizgi.setPosition({menuArkaPlan.getPosition().x + 400.0f, menuArkaPlan.getPosition().y + 80.0f}); window.draw(dikeyCizgi);

        const std::string EKIPMAN_TURLERI[6] = { "Kask Yuvasi", "Aksesuar Yuvasi", "Silah Yuvasi", "Zirh Yuvasi", "Ikinci El Yuvasi", "Cizme Yuvasi" };

        struct YuvaBBox { sf::FloatRect rect; int tip; int index; Esya* esya; };
        std::vector<YuvaBBox> tumYuvalar;

        float eqX = menuArkaPlan.getPosition().x + 200.0f, eqY = menuArkaPlan.getPosition().y + 300.0f, s = 70.0f, g = 10.0f; 
        tumYuvalar.push_back({sf::FloatRect({eqX - s/2.0f, eqY - s - g}, {s, s}), 1, 0, &oyuncu.ekipmanlar[0]});
        tumYuvalar.push_back({sf::FloatRect({eqX + s/2.0f + g, eqY - s - g}, {s, s}), 1, 1, &oyuncu.ekipmanlar[1]});
        tumYuvalar.push_back({sf::FloatRect({eqX - s*1.5f - g, eqY}, {s, s}), 1, 2, &oyuncu.ekipmanlar[2]});
        tumYuvalar.push_back({sf::FloatRect({eqX - s/2.0f, eqY}, {s, s}), 1, 3, &oyuncu.ekipmanlar[3]});
        tumYuvalar.push_back({sf::FloatRect({eqX + s/2.0f + g, eqY}, {s, s}), 1, 4, &oyuncu.ekipmanlar[4]});
        tumYuvalar.push_back({sf::FloatRect({eqX - s/2.0f, eqY + s + g}, {s, s}), 1, 5, &oyuncu.ekipmanlar[5]});

        const float padding = 28.0f;
        const float left = menuArkaPlan.getPosition().x + 440.0f, right = menuArkaPlan.getPosition().x + menuArkaPlan.getSize().x - padding;
        const float top = menuArkaPlan.getPosition().y + 100.0f, bottom = menuArkaPlan.getPosition().y + menuArkaPlan.getSize().y - padding;

        const int sutunSayisi = 7, satirSayisi = 6;
        float kutuBoyutu = std::min((right - left - 5*g)/6, (bottom - top - 6*g)/7);

        int envIndex = 0;
        for (int r = 0; r < satirSayisi; ++r) {
            for (int c = 0; c < sutunSayisi; ++c) {
                float x = left + c * (kutuBoyutu + g); float y = top + r * (kutuBoyutu + g);
                tumYuvalar.push_back({sf::FloatRect({x, y}, {kutuBoyutu, kutuBoyutu}), 0, envIndex, &oyuncu.envanter[envIndex]});
                envIndex++;
            }
        }

        bool fareYuvaUzerinde = false; YuvaBBox* hoverYuva = nullptr;
        for (auto& y : tumYuvalar) { if (y.rect.contains(farePos)) { fareYuvaUzerinde = true; hoverYuva = &y; break; } }

        if (sagTikBasildi && hoverYuva && hoverYuva->esya->doluMu && !surukleniyorMu) {
            if (hoverYuva->tip == 0 && hoverYuva->esya->tur != "Iksir" && hoverYuva->esya->tur != "Gorev Esyasi") { 
                for (int i = 0; i < 6; i++) { if (EKIPMAN_TURLERI[i] == hoverYuva->esya->tur) { std::swap(*hoverYuva->esya, oyuncu.ekipmanlar[i]); break; } }
            } else if (hoverYuva->tip == 1) { 
                for (int i = 0; i < 42; i++) { if (!oyuncu.envanter[i].doluMu) { std::swap(*hoverYuva->esya, oyuncu.envanter[i]); break; } }
            }
        }

        if (solTikBasildi && hoverYuva && hoverYuva->esya->doluMu && !surukleniyorMu) {
            surukleniyorMu = true; kaynakTip = hoverYuva->tip; kaynakIndex = hoverYuva->index;
            suruklenenEsya = *hoverYuva->esya; hoverYuva->esya->doluMu = false; 
        }

        if (solTikBirakildi && surukleniyorMu) {
            bool aksiyonaMiGitti = false;
            for(int i=0; i<8; i++) {
                if (getAksiyonSlotRect(i).contains(farePos)) {
                    aksiyonaMiGitti = true;
                    oyuncu.hizliErisim[i] = suruklenenEsya.isim;
                    break;
                }
            }

            if (aksiyonaMiGitti) {
                if (kaynakTip == 0) oyuncu.envanter[kaynakIndex] = suruklenenEsya;
                else if (kaynakTip == 1) oyuncu.ekipmanlar[kaynakIndex] = suruklenenEsya;
                surukleniyorMu = false;
            } 
            else if (kaynakTip != 3) {
                surukleniyorMu = false;
                if (hoverYuva) {
                    if (kaynakTip == hoverYuva->tip && kaynakIndex == hoverYuva->index) { *hoverYuva->esya = suruklenenEsya; } 
                    else if (hoverYuva->tip == 1 && EKIPMAN_TURLERI[hoverYuva->index] != suruklenenEsya.tur) {
                        if (kaynakTip == 0) oyuncu.envanter[kaynakIndex] = suruklenenEsya; else oyuncu.ekipmanlar[kaynakIndex] = suruklenenEsya;
                    } 
                    else {
                        Esya gecici = *hoverYuva->esya; *hoverYuva->esya = suruklenenEsya;
                        if (kaynakTip == 0) oyuncu.envanter[kaynakIndex] = gecici; else oyuncu.ekipmanlar[kaynakIndex] = gecici;
                    }
                } else {
                    if (kaynakTip == 0) oyuncu.envanter[kaynakIndex] = suruklenenEsya; else oyuncu.ekipmanlar[kaynakIndex] = suruklenenEsya;
                }
            }
        }

        for (auto& y : tumYuvalar) {
            sf::RectangleShape kutu(y.rect.size); kutu.setPosition(y.rect.position);
            kutu.setFillColor(sf::Color(18, 30, 55)); kutu.setOutlineColor(sf::Color(170, 130, 30)); kutu.setOutlineThickness(2.0f); window.draw(kutu);

            if (y.esya->doluMu) {
                sf::Sprite ikon(y.esya->doku);
                float sc = std::min((y.rect.size.x * 0.8f) / ikon.getLocalBounds().size.x, (y.rect.size.y * 0.8f) / ikon.getLocalBounds().size.y);
                ikon.setScale({sc, sc}); ikon.setOrigin({ikon.getLocalBounds().size.x / 2.0f, ikon.getLocalBounds().size.y / 2.0f});
                ikon.setPosition({y.rect.position.x + y.rect.size.x / 2.0f, y.rect.position.y + y.rect.size.y / 2.0f}); window.draw(ikon);
                
                if (y.esya->miktar > 1) {
                    sf::Text mikTxt(font); mikTxt.setString(std::to_string(y.esya->miktar));
                    mikTxt.setCharacterSize(14); mikTxt.setFillColor(sf::Color::White);
                    mikTxt.setPosition({y.rect.position.x + 30.0f, y.rect.position.y + 35.0f});
                    window.draw(mikTxt);
                }
            }
        }

        sf::Text goldText(font); goldText.setString("Altin: " + std::to_string(oyuncu.altin)); goldText.setCharacterSize(26);
        goldText.setFillColor(sf::Color(255, 215, 0)); goldText.setPosition({left, bottom - 80.0f}); window.draw(goldText);

        if (hoverYuva && hoverYuva->esya->doluMu && !surukleniyorMu) { 
            std::string gosterilecekMetin = "";
            if(hoverYuva->esya->saldiriGucu > 0) gosterilecekMetin += "Saldiri: +" + std::to_string(hoverYuva->esya->saldiriGucu) + "\n";
            if(hoverYuva->esya->zirh > 0)        gosterilecekMetin += "Zirh: +" + std::to_string(hoverYuva->esya->zirh) + "\n";
            if(hoverYuva->esya->ekstraCan > 0)   gosterilecekMetin += "Can: +" + std::to_string(hoverYuva->esya->ekstraCan) + "\n";
            
            cizTooltip(window, font, farePos, hoverYuva->esya->isim, gosterilecekMetin);
        }
    }

    void cizGorevEkrani(sf::RenderWindow& window, sf::Font& font, GorevYoneticisi& gy) {
        window.draw(menuArkaPlan);

        auto metinYaz = [&](const std::string& metin, float x, float y, unsigned int boyut, sf::Color renk) {
            sf::Text t(font); t.setString(metin); t.setCharacterSize(boyut); t.setFillColor(renk); t.setPosition({x, y}); window.draw(t);
        };

        sf::RectangleShape dikeyCizgi({2.0f, 500.0f}); dikeyCizgi.setFillColor(sf::Color(170, 130, 30, 150));
        dikeyCizgi.setPosition({550.0f, 200.0f}); window.draw(dikeyCizgi);

        metinYaz("--- GOREV GUNLUGU ---", 570.0f, 180.0f, 28, sf::Color(40, 180, 240));
        metinYaz("Aktif Gorevler:", 320.0f, 220.0f, 22, sf::Color(200, 200, 200));
        
        for (size_t i = 0; i < gy.aktifGorevler.size(); i++) {
            sf::Color renk = (static_cast<int>(i) == gy.seciliGorevIndeks) ? sf::Color(255, 215, 0) : sf::Color::White;
            std::string baslik = gy.aktifGorevler[i].baslik;
            if (static_cast<int>(i) == gy.seciliGorevIndeks) baslik = "> " + baslik;
            metinYaz(baslik, 320.0f, 260.0f + (i * 40.0f), 20, renk);
        }

        if (!gy.aktifGorevler.empty() && gy.seciliGorevIndeks >= 0 && gy.seciliGorevIndeks < static_cast<int>(gy.aktifGorevler.size())) {
            const auto& seciliGorev = gy.aktifGorevler[gy.seciliGorevIndeks];
            metinYaz(seciliGorev.baslik, 580.0f, 240.0f, 26, sf::Color(255, 215, 0));
            metinYaz(seciliGorev.aciklama, 580.0f, 300.0f, 20, sf::Color::White);
            std::string durum;
            if (seciliGorev.tamamlandi) durum = seciliGorev.odulAlindi ? "Durum: Odul Alindi" : "Durum: Dedeye Don!";
            else durum = "Ilerleme: " + std::to_string(seciliGorev.anlikSayi) + " / " + std::to_string(seciliGorev.hedefSayi);
            metinYaz(durum, 580.0f, 550.0f, 22, seciliGorev.tamamlandi ? sf::Color::Green : sf::Color(240, 80, 80));
        } else {
            metinYaz("Su an aktif gorev bulunmuyor.", 580.0f, 240.0f, 22, sf::Color(150, 150, 150));
        }
    }

    void cizAyarlarEkrani(sf::RenderWindow& window, sf::Font& font, int altMenu, int sesSeviyesi, std::string inputMetin) {
        window.draw(menuArkaPlan);
        auto metinYaz = [&](const std::string& metin, float x, float y, int boyut, sf::Color renk) {
            sf::Text t(font); t.setString(metin); t.setCharacterSize(boyut); t.setFillColor(renk); t.setPosition({x, y}); window.draw(t);
        };
        metinYaz("AYARLAR", 720.0f, 170.0f, 36, sf::Color(240, 200, 40));

        float bX = 650.0f, bY = 230.0f;
        if (altMenu == 0) { 
            for(int i=0; i<8; i++) { 
                sf::RectangleShape btn({300.0f, 40.0f}); btn.setPosition({bX, bY + i*55.0f});
                btn.setFillColor(sf::Color(18, 30, 55)); btn.setOutlineColor(sf::Color(170, 130, 30)); btn.setOutlineThickness(2.0f); window.draw(btn);
            }
            metinYaz("Oyuna Devam Et", bX + 60.0f, bY + 5.0f, 22, sf::Color::White);
            metinYaz("Oyunu Kaydet", bX + 70.0f, bY + 60.0f, 22, sf::Color::White);
            metinYaz("Kayit Yukle", bX + 80.0f, bY + 115.0f, 22, sf::Color::White);
            metinYaz("Arayuz Duzenle", bX + 70.0f, bY + 170.0f, 22, sf::Color::Yellow);
            metinYaz("Cozunurluk: 1600x900", bX + 25.0f, bY + 225.0f, 22, sf::Color::White);
            metinYaz("Ses Seviyesi: %" + std::to_string(sesSeviyesi), bX + 40.0f, bY + 280.0f, 22, sf::Color::White);
            metinYaz("Saat: " + std::to_string(gununSaati) + ":00 (Degistir)", bX + 15.0f, bY + 335.0f, 20, sf::Color::Cyan);
            metinYaz("Oyunu Kapat", bX + 80.0f, bY + 390.0f, 22, sf::Color(240, 80, 80));
        } else if (altMenu == 1) { 
            metinYaz("YUKLENECEK KAYDI SEC:", bX - 50.0f, bY - 40.0f, 22, sf::Color::Cyan);
            auto liste = kayitlariListele();
            for(size_t i=0; i<liste.size() && i < 6; i++) {
                sf::RectangleShape btn({300.0f, 40.0f}); btn.setPosition({bX, bY + i*50.0f});
                btn.setFillColor(sf::Color(18, 30, 55)); btn.setOutlineColor(sf::Color(170, 130, 30)); btn.setOutlineThickness(2.0f); window.draw(btn);
                metinYaz(liste[i], bX + 20.0f, bY + 5.0f + i*50.0f, 20, sf::Color::White);
            }
            metinYaz("Geri Don [ESC]", bX, bY + 320.0f, 20, sf::Color(240, 80, 80));
        } else if (altMenu == 2) { 
            metinYaz("Kayit Icin Isim Girin:", bX + 30.0f, bY - 40.0f, 22, sf::Color::Cyan);
            sf::RectangleShape box({300.0f, 40.0f}); box.setPosition({bX, bY});
            box.setFillColor(sf::Color(10, 15, 25)); box.setOutlineColor(sf::Color::White); box.setOutlineThickness(2.0f); window.draw(box);
            metinYaz(inputMetin + "_", bX + 10.0f, bY + 5.0f, 22, sf::Color::Yellow);
            metinYaz("Onaylamak icin ENTER'a bas.", bX, bY + 60.0f, 20, sf::Color(150, 150, 150));
            metinYaz("Geri Don [ESC]", bX, bY + 110.0f, 20, sf::Color(240, 80, 80));
        }
    }
};