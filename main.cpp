#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <vector>
#include <iostream>
#include <algorithm> 
#include <array>
#include <cmath>
#include <cstdlib> 
#include <cstdint>
#include <fstream> 
#include <cstdio> 

#include "Sabitler.hpp"
#include "Harita.hpp"
#include "ZeminDokusu.hpp"
#include "Mermi.hpp"
#include "Kilic.hpp" 
#include "Dusman.hpp"
#include "Oyuncu.hpp"
#include "Arayuz.hpp"
#include "NPC.hpp"
#include "Gorev.hpp"
#include "Yardimci.hpp" 

int main() {
    ayarYukle(); 
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({1600, 900}), "Dreadmyst Online - A New Online RPG", sf::Style::Default);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    sf::Clock clock;

    sf::View oyunKamerasi; oyunKamerasi.setSize({800.0f, 450.0f}); 

    int oyunDurumu = 0; 
    int anaMenuAltSecenek = 0; 

    bool menuAcikMi = false, envanterAcikMi = false, gorevAcikMi = false, haritaAcikMi = false, ayarlarAcikMi = false; 
    int ayarlarAltMenu = 0, oyunSesSeviyesi = 100, karakterSekmesi = 0;
    sf::Listener::setGlobalVolume(oyunSesSeviyesi);
    
    std::string klavyeMetin = "";

    bool cTusuBasiliMi = false, iTusuBasiliMi = false, qTusuBasiliMi = false, mTusuBasiliMi = false; 
    bool solTikOnceki = false, sagTikOnceki = false, spaceOnceki = false, escOnceki = false, birOnceki = false;
    bool klavyeBirOnceki[8] = {false, false, false, false, false, false, false, false}; 

    sf::Font font; if (!font.openFromFile("assets/pixel.ttf")) std::cout << "Hata: assets/pixel.ttf yok!\n";

    sf::Texture mermiDokusu, dusmanDokusu, cimenDokusu, duvarDokusu, dedeDokusu, kopruDokusu, uiDokusu, inDokusu, potDokusu;
    sf::Texture atilmaDokusu, ekstraAtisDokusu;
    sf::Texture grassDoku[12];
    sf::Texture menuArkaDoku;
    sf::Texture patlamaDokusu, patlayanOkDokusu;
    sf::Texture altinDokusu, atDokusu, tabArkaDoku;
    
    sf::Texture okcuDusmanDokusu;
    sf::Texture kumDokusu; 
    sf::Texture slashDokusu; // YENİ: Kılıç Slash efekti dokusu

    std::vector<sf::Texture> suDokulari(40);
    for (int i = 0; i < 40; ++i) {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "assets/water_128px_frames/%04d.png", i);
        if (!suDokulari[i].loadFromFile(buffer)) {}
    }

    std::vector<sf::Texture> agacDokulari(5);
    for (int i = 0; i < 16; ++i) {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "assets/agac_sprite/Pine_01_Body_000_00%02d.png", i + 1);
        if (!agacDokulari[i].loadFromFile(buffer)) {}
    }

    if (!menuArkaDoku.loadFromFile("assets/menu_arkaplan.jpg")) std::cout << "Hata: assets/menu_arkaplan.jpg yuklenemedi!\n";
    if (!tabArkaDoku.loadFromFile("assets/tab_background.png")) std::cout << "Hata: assets/tab_background.png yok!\n";
    if (!atilmaDokusu.loadFromFile("assets/atilma.png")) std::cout << "Hata: assets/atilma.png yok!\n";
    if (!ekstraAtisDokusu.loadFromFile("assets/ekstra_atis.png")) std::cout << "Hata: assets/ekstra_atis.png yok!\n";
    if (!dusmanDokusu.loadFromFile("assets/dusman.png")) std::cout << "Hata: assets/dusman.png yok!\n";
    
    if (!altinDokusu.loadFromFile("assets/gold.png")) std::cout << "Hata: assets/gold.png yok!\n";
    if (!atDokusu.loadFromFile("assets/at.png")) std::cout << "Hata: assets/at.png yok!\n";

    if (!patlayanOkDokusu.loadFromFile("assets/patlayanok.png")) std::cout << "Hata: assets/patlayanok.png yok!\n";
    if (!patlamaDokusu.loadFromFile("assets/patlama_spritesheet.png")) std::cout << "Hata: assets/patlama_spritesheet.png yok!\n";
    if (!okcuDusmanDokusu.loadFromFile("assets/okcudusman.png")) std::cout << "Hata: assets/okcudusman.png yok!\n";
    if (!kumDokusu.loadFromFile("assets/kum.png")) std::cout << "Hata: assets/kum.png yok!\n";
    if (!slashDokusu.loadFromFile("assets/pixel_art_sword_slash_sprites.png")) std::cout << "Hata: assets/pixel_art_sword_slash_sprites.png yok!\n";

    const char* grassDosyalar[12] = {
        "assets/grass_colorMap.png", "assets/grass1_colorMap.png", "assets/grass2_colorMap.png", "assets/grass3_colorMap.png",
        "assets/grass4_colorMap.png", "assets/grass5_colorMap.png", "assets/grass6_colorMap.png", "assets/grass7_colorMap.png",
        "assets/grass8_colorMap.png", "assets/jungle_grass1_colorMap.png", "assets/jungle_grass2_colorMap.png", "assets/mulch_colorMap.png"
    };
    for (int gi = 0; gi < 12; ++gi) {
        if (!grassDoku[gi].loadFromFile(grassDosyalar[gi])) std::cout << "Hata: " << grassDosyalar[gi] << " yok!\n";
        grassDoku[gi].setRepeated(true);
    }

    if (!mermiDokusu.loadFromFile("assets/mermi.png")) std::cout << "Hata: mermi.png yok!\n";
    if (!cimenDokusu.loadFromFile("assets/5025.jpg")) std::cout << "Hata: 5025.jpg yok!\n";
    if (!duvarDokusu.loadFromFile("assets/duvar.png")) std::cout << "Hata: duvar.png yok!\n";
    if (!dedeDokusu.loadFromFile("assets/ak_sakalli_dede.png")) std::cout << "Hata: ak_sakalli_dede.png yok!\n";
    if (!kopruDokusu.loadFromFile("assets/kopru.png")) std::cout << "Hata: kopru.png yok!\n";
    if (!uiDokusu.loadFromFile("assets/ui.png")) std::cout << "Hata: ui.png yuklenemedi!\n";
    if (!inDokusu.loadFromFile("assets/canavarini.png")) std::cout << "Hata: canavarini.png yuklenemedi!\n";
    if (!potDokusu.loadFromFile("assets/canpotu.png")) std::cout << "Hata: canpotu.png yuklenemedi!\n";

    AnimasyonSheet idleSheet, walkSheet, attackSheet, deathSheet;
    idleSheet.yukle("assets/oyuncu_aminasyon/Idle_8dir_sheet.png", IDLE_KARE_SAYISI, IDLE_KARE_SURESI);
    walkSheet.yukle("assets/oyuncu_aminasyon/Walking_8dir_sheet.png", WALK_KARE_SAYISI, WALK_KARE_SURESI);
    attackSheet.yukle("assets/oyuncu_aminasyon/Hook Punch_8dir_sheet.png", ATTACK_KARE_SAYISI, ATTACK_KARE_SURESI);
    deathSheet.yukle("assets/oyuncu_aminasyon/Standing Death Backward 01_8dir_sheet.png", DEATH_KARE_SAYISI, DEATH_KARE_SURESI);

    sf::Image fogImg;
    fogImg.resize({2500, 2500}, sf::Color(8, 12, 16, 250)); 
    for(unsigned int y = 0; y < 2500; y++) {
        for(unsigned int x = 0; x < 2500; x++) {
            float dx = static_cast<float>(x) - 1250.0f;
            float dy = static_cast<float>(y) - 1250.0f;
            float dist = std::sqrt(dx * dx + dy * dy);
            float radius = 120.0f;   
            float yumusama = 280.0f; 
            
            if(dist < radius) {
                fogImg.setPixel({x, y}, sf::Color(8, 12, 16, 0));
            } else if (dist < radius + yumusama) {
                float oran = (dist - radius) / yumusama;
                fogImg.setPixel({x, y}, sf::Color(8, 12, 16, static_cast<std::uint8_t>(250.0f * oran)));
            }
        }
    }
    sf::Texture fogTex;
    (void)fogTex.loadFromImage(fogImg); 
    sf::Sprite fogSprite(fogTex);
    fogSprite.setOrigin({1250.0f, 1250.0f});

    sf::Sprite cimenSprite(cimenDokusu), kopruSprite(kopruDokusu), kumSprite(kumDokusu); 
    
    sf::Sprite suSprite(suDokulari[0]);
    suSprite.setColor(sf::Color(255, 255, 255, 200)); 

    sf::Sprite grassSprite[12] = {
        sf::Sprite(grassDoku[0]),  sf::Sprite(grassDoku[1]), sf::Sprite(grassDoku[2]),  sf::Sprite(grassDoku[3]),
        sf::Sprite(grassDoku[4]),  sf::Sprite(grassDoku[5]), sf::Sprite(grassDoku[6]),  sf::Sprite(grassDoku[7]),
        sf::Sprite(grassDoku[8]),  sf::Sprite(grassDoku[9]), sf::Sprite(grassDoku[10]), sf::Sprite(grassDoku[11])
    };

    Oyuncu oyuncu(120.0f, 120.0f, idleSheet.texture); 
    oyuncu.sheetleriAyarla(idleSheet, walkSheet, attackSheet, deathSheet);
    
    Binek at(oyuncu.sprite.getPosition().x, oyuncu.sprite.getPosition().y, atDokusu);

    Arayuz ui; 
    ui.menuArkaPlan.setTexture(&tabArkaDoku);
    ui.menuArkaPlan.setFillColor(sf::Color(255, 255, 255, 255));
    ui.menuArkaPlan.setSize(sf::Vector2f({tabW, tabH}));
    ui.menuArkaPlan.setPosition({tabX, tabY});

    NPC dede(220.0f, 120.0f, dedeDokusu); GorevYoneticisi gorevYoneticisi; 

    std::vector<Mermi> mermiler; 
    std::vector<Mermi> dusmanMermileri; 
    std::vector<KilicSlasher> slashEfektleri; // YENİ: Kılıç kesme efektleri listesi
    std::vector<PatlamaEfekti> patlamalar; 
    std::vector<Dusman> dusmanlar; 
    std::vector<HasarMetni> hasarMetinleri; 
    std::vector<YerdekiEsya> yerdekiEsyalar; std::vector<Bildirim> sistemMesajlari; std::vector<CanavarIni> inler;

    int nextDusmanId = 1, nextInId = 1, seciliDusmanId = -1;
    inler.emplace_back(nextInId++, 800.0f, 400.0f, inDokusu); 
    inler.emplace_back(nextInId++, 1000.0f, 1200.0f, inDokusu);

    inler[0].spawnSayaci = 10.0f;
    inler[1].spawnSayaci = 10.0f;

    dusmanlar.emplace_back(nextDusmanId++, 600.0f, 300.0f, okcuDusmanDokusu, -1, 1);
    dusmanlar.emplace_back(nextDusmanId++, 1200.0f, 200.0f, dusmanDokusu, -1, 0);

    std::vector<sf::Sprite> statikDuvarDeposu, statikAgacDeposu; 
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (harita[y][x] == 1) {
                sf::Sprite dSprite(duvarDokusu); dSprite.setScale({tileSize / duvarDokusu.getSize().x, tileSize / duvarDokusu.getSize().y});
                dSprite.setPosition({x * tileSize, y * tileSize}); statikDuvarDeposu.push_back(dSprite);
            } else if (harita[y][x] == 2) {
                sf::Sprite aSprite(agacDokulari[0]); 
                float tw = agacDokulari[0].getSize().x;
                float th = agacDokulari[0].getSize().y;
                if (tw > 0.0f && th > 0.0f) { 
                    float agacScaleX = (tileSize * 2.8f) / tw;
                    float agacScaleY = (tileSize * 2.8f) / th;
                    aSprite.setScale({agacScaleX, agacScaleY});
                }
                aSprite.setPosition({x * tileSize, y * tileSize}); statikAgacDeposu.push_back(aSprite);
            }
        }
    }

    auto oyunuKaydet = [&](std::string isim) {
        std::ofstream out(isim + ".txt");
        out << oyuncu.sprite.getPosition().x << " " << oyuncu.sprite.getPosition().y << "\n";
        out << oyuncu.mevcutCan << " " << oyuncu.maxCan << " " << oyuncu.mevcutMana << " " << oyuncu.maxMana << "\n";
        out << oyuncu.exp << " " << oyuncu.maxExp << " " << oyuncu.seviye << " " << oyuncu.altin << "\n";
        out << oyuncu.statPuanlari << " " << oyuncu.statSaldiriGucu << " " << oyuncu.statZirh << " " << oyuncu.hareketHizi << "\n";
        out << oyuncu.skillCokluAtisLvl << " " << oyuncu.skillAtilmaLvl << " " << oyuncu.skillEkstraLvl << " " << oyuncu.skillPatlamaLvl << "\n"; 
        out << dede.gorevAsamasi << " " << dede.durum << " " << dede.diyalogSayfasi << "\n";
        out << gununSaati << "\n"; 
        
        for(int i=0; i<42; i++) out << (oyuncu.envanter[i].doluMu ? sifreleIsim(oyuncu.envanter[i].isim) : "BOS") << " " << oyuncu.envanter[i].miktar << "\n";
        for(int i=0; i<6; i++) out << (oyuncu.ekipmanlar[i].doluMu ? sifreleIsim(oyuncu.ekipmanlar[i].isim) : "BOS") << " " << oyuncu.ekipmanlar[i].miktar << "\n";
        for(int i=0; i<8; i++) out << sifreleIsim(oyuncu.hizliErisim[i]) << "\n";
        
        out << gorevYoneticisi.aktifGorevler.size() << "\n";
        for(auto& g : gorevYoneticisi.aktifGorevler) out << g.id << " " << g.tamamlandi << " " << g.anlikSayi << " " << g.odulAlindi << "\n";
        out.close();
        sistemMesajlari.push_back({"Oyun Kaydedildi: " + isim, 4.0f});
    };

    auto oyunuYukle = [&](std::string isim) -> bool {
        std::ifstream in(isim + ".txt");
        if(!in.is_open()) { sistemMesajlari.push_back({"Kayit bulunamadi: " + isim, 4.0f}); return false; }
        
        float px, py; in >> px >> py; oyuncu.sprite.setPosition({px, py});
        in >> oyuncu.mevcutCan >> oyuncu.maxCan >> oyuncu.mevcutMana >> oyuncu.maxMana >> oyuncu.exp >> oyuncu.maxExp >> oyuncu.seviye >> oyuncu.altin;
        in >> oyuncu.statPuanlari >> oyuncu.statSaldiriGucu >> oyuncu.statZirh >> oyuncu.hareketHizi;
        in >> oyuncu.skillCokluAtisLvl >> oyuncu.skillAtilmaLvl >> oyuncu.skillEkstraLvl >> oyuncu.skillPatlamaLvl; 
        in >> dede.gorevAsamasi >> dede.durum >> dede.diyalogSayfasi; dede.diyaloglariYukle();
        if(in >> gununSaati) {} else { gununSaati = 12; }
        
        std::string hamIsim; int miktar; 
        for(int i=0; i<42; i++) {
            in >> hamIsim >> miktar; std::string gercek = cozIsim(hamIsim);
            if(gercek == "") oyuncu.envanter[i] = {"", "", false};
            else for(auto& e : oyuncu.tumOyunEsyalari) if(e.isim == gercek) { oyuncu.envanter[i] = e; oyuncu.envanter[i].miktar = miktar; }
        }
        for(int i=0; i<6; i++) {
            in >> hamIsim >> miktar; std::string gercek = cozIsim(hamIsim);
            if(gercek == "") oyuncu.ekipmanlar[i] = {"", "", false};
            else for(auto& e : oyuncu.tumOyunEsyalari) if(e.isim == gercek) { oyuncu.ekipmanlar[i] = e; oyuncu.ekipmanlar[i].miktar = miktar; }
        }
        for(int i=0; i<8; i++) {
            in >> hamIsim; oyuncu.hizliErisim[i] = cozIsim(hamIsim);
        }
        
        size_t gSize; in >> gSize; gorevYoneticisi.aktifGorevler.clear();
        for(size_t i=0; i<gSize; i++) {
            int id, anlik; bool tam, odul; in >> id >> tam >> anlik >> odul; std::string baslik, aciklama; int hedef = 1;
            if(id == 1) { baslik = "Ormanin Huzursuzlugu"; aciklama = "Yakindaki 2 canavari temizle.\n\nOdul: 500 Altin, 150 EXP"; hedef = 2; }
            else if(id == 2) { baslik = "Suya Karisan Zehir"; aciklama = "Kopruyu gec ve nehrin karsisina ulas.\n\nOdul: Deri Cizme, 200 EXP"; hedef = 1; }
            else if(id == 3) { baslik = "Lanetli Ganimet"; aciklama = "Envanterine 3 adet Yozlasmis Yurek topla.\n\nOdul: 1000 Altin, 300 EXP"; hedef = 3; }
            else if(id == 4) { baslik = "Korucunun Mirasi"; aciklama = "Haritada beliren Kadim Orman Zirhini kusan.\n\nOdul: Golge Yuzugu, 500 EXP"; hedef = 1; }
            else if(id == 5) { baslik = "Sisin Efendisi"; aciklama = "Ormanin derinligindeki Boss'u kes!\n\nOdul: Efsanevi Yay, 5000 Altin"; hedef = 1; }
            gorevYoneticisi.aktifGorevler.push_back({id, baslik, aciklama, tam, hedef, anlik, odul});
        }
        in.close(); sistemMesajlari.push_back({"Oyun Yuklendi: " + isim, 4.0f}); 
        ayarlarAcikMi = false; ayarlarAltMenu = 0; anaMenuAltSecenek = 0;
        return true;
    };

    float atesGecikmesi = 0.0f, hasarGecikmesi = 0.0f, skillOkGecikmesi = 0.0f;
    int skillKalanOk = 0, q1Oldurulen = 0;
    bool bossSpawlandi = false;
    const float ATES_HIZI = 0.4f; 

    float fpsSayacSuresi = 0.0f;
    int fpsGosterge = 0;
    int anlikKareSayisi = 0;

    while (window.isOpen()) {
        float deltaZaman = clock.restart().asSeconds();
        
        anlikKareSayisi++;
        fpsSayacSuresi += deltaZaman;
        if(fpsSayacSuresi >= 1.0f) {
            fpsGosterge = anlikKareSayisi;
            anlikKareSayisi = 0;
            fpsSayacSuresi = 0.0f;
        }
        
        static float gercekZamanSayaci = 0.0f;
        gercekZamanSayaci += deltaZaman;
        if (gercekZamanSayaci >= 10.0f) {
            gercekZamanSayaci = 0.0f;
            gununSaati = (gununSaati + 1) % 24;
        }
        
        static float suAnimSayaci = 0.0f;
        static int suAnimKaresi = 0;
        suAnimSayaci += deltaZaman;
        if (suAnimSayaci >= 0.08f) { 
            suAnimSayaci -= 0.08f;
            suAnimKaresi = (suAnimKaresi + 1) % 40;
            suSprite.setTexture(suDokulari[suAnimKaresi], true);
        }

        sf::Vector2f oyuncuMerkezCull = oyuncu.sprite.getPosition() + sf::Vector2f(oyuncu.genislik / 2.0f, oyuncu.yukseklik / 2.0f);
        static float agacAnimSayaci = 0.0f;
        static int agacAnimKaresi = 0;
        agacAnimSayaci += deltaZaman;
        if (agacAnimSayaci >= 0.6f) { 
            agacAnimSayaci -= 0.6f;
            agacAnimKaresi = (agacAnimKaresi + 1) % 5;
            for(auto& aSprite : statikAgacDeposu) {
                if(std::abs(aSprite.getPosition().x - oyuncuMerkezCull.x) < 1500.0f && 
                   std::abs(aSprite.getPosition().y - oyuncuMerkezCull.y) < 1500.0f) {
                    aSprite.setTexture(agacDokulari[agacAnimKaresi], true);
                }
            }
        }
        
        bool yaziYaziliyorMu = (oyunDurumu == 0 && anaMenuAltSecenek == 1) || (oyunDurumu == 1 && ayarlarAcikMi && (ayarlarAltMenu == 1 || ayarlarAltMenu == 2));

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            
            if (const auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                if (yaziYaziliyorMu) {
                    uint32_t c = textEvent->unicode;
                    if (c == 8 && klavyeMetin.size() > 0) klavyeMetin.pop_back(); 
                    else if (c >= 32 && c <= 126 && c != 95 && klavyeMetin.size() < 15) klavyeMetin += static_cast<char>(c); 
                }
            }
        }

        window.clear(); 

        bool solTikAnlik = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        bool solTikBasildi = solTikAnlik && !solTikOnceki;
        bool solTikBirakildi = !solTikAnlik && solTikOnceki; solTikOnceki = solTikAnlik;

        bool sagTikAnlik = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
        bool sagTikBasildi = sagTikAnlik && !sagTikOnceki; sagTikOnceki = sagTikAnlik;

        bool spaceBasildi = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
        bool escBasildi = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
        bool enterBasildi = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);

        sf::Vector2f farePencerePosUI = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());
        sf::Vector2f fareDunyaIso = window.mapPixelToCoords(sf::Mouse::getPosition(window), oyunKamerasi);
        sf::Vector2f fareDunyaCart = tersIzoYap(fareDunyaIso.x, fareDunyaIso.y);
        
        if (solTikBirakildi && ui.surukleniyorMu) {
            if (!envanterAcikMi && !menuAcikMi) {
                ui.surukleniyorMu = false;
            }
        }

        if (enterBasildi && yaziYaziliyorMu) {
            if (oyunDurumu == 0 && anaMenuAltSecenek == 1 && klavyeMetin != "") {
                if(oyunuYukle(klavyeMetin)) oyunDurumu = 1;
                klavyeMetin = "";
            }
            else if (oyunDurumu == 1 && ayarlarAcikMi && klavyeMetin != "") {
                if (ayarlarAltMenu == 1) oyunuYukle(klavyeMetin);
                else if (ayarlarAltMenu == 2) oyunuKaydet(klavyeMetin);
                klavyeMetin = "";
            }
        }

        if (oyunDurumu == 0) {
            if (solTikBasildi) {
                int secim = ui.anaMenuButonKontrol(farePencerePosUI, anaMenuAltSecenek);
                if (anaMenuAltSecenek == 0) {
                    if (secim == 1) oyunDurumu = 1; 
                    else if (secim == 2) { anaMenuAltSecenek = 1; klavyeMetin = ""; }
                    else if (secim == 3) { ayarlarAcikMi = true; oyunDurumu = 1; } 
                    else if (secim == 4) window.close();
                } else if (anaMenuAltSecenek == 1) {
                    if (secim >= 11 && secim <= 14) {
                        auto liste = ui.kayitlariListele();
                        if (secim - 11 < liste.size()) {
                            if(oyunuYukle(liste[secim - 11])) oyunDurumu = 1;
                        }
                    }
                }
            }
            if (escBasildi && !escOnceki && anaMenuAltSecenek != 0) {
                anaMenuAltSecenek = 0; klavyeMetin = "";
            } escOnceki = escBasildi;

            ui.cizAnaMenu(window, font, anaMenuAltSecenek, klavyeMetin, menuArkaDoku); 
            
            float bildirimY = 820.0f;
            for (int i = sistemMesajlari.size() - 1; i >= 0; i--) {
                sf::Text bText(font); bText.setString(sistemMesajlari[i].metin); bText.setCharacterSize(22); bText.setFillColor(sf::Color(255, 215, 0));
                bText.setOutlineColor(sf::Color(0, 0, 0)); bText.setOutlineThickness(2.0f);
                sf::Color c = bText.getFillColor(); sf::Color oc = bText.getOutlineColor();
                c.a = static_cast<std::uint8_t>(255.0f * std::max(0.0f, std::min(1.0f, sistemMesajlari[i].omur))); oc.a = c.a;
                bText.setFillColor(c); bText.setOutlineColor(oc); bText.setPosition({30.0f, bildirimY}); window.draw(bText);
                bildirimY -= 30.0f; sistemMesajlari[i].omur -= deltaZaman;
                if(sistemMesajlari[i].omur <= 0.0f) sistemMesajlari.erase(sistemMesajlari.begin() + i);
            }
            window.display(); continue; 
        }

        if (escBasildi && !escOnceki) {
            if (ui.duzenlemeModu) { 
                ui.duzenlemeModu = false; ui.suruklenenEleman = -1; ui.suruklenenSlot = -1; ui.suruklenenIslem = -1; ayarKaydet();
                sistemMesajlari.push_back({"Arayuz Basariyla Kaydedildi", 4.0f});
            }
            else if (ayarlarAcikMi && ayarlarAltMenu != 0) { ayarlarAltMenu = 0; klavyeMetin = ""; }
            else if (seciliDusmanId != -1) seciliDusmanId = -1;
            else if (ui.surukleniyorMu) ui.surukleniyorMu = false;
            else { menuAcikMi = false; envanterAcikMi = false; gorevAcikMi = false; haritaAcikMi = false; ayarlarAcikMi = false; ayarlarAltMenu = 0;
            if (dede.etkilesimVar) dede.etkilesimVar = false; }
        } escOnceki = escBasildi;

        if (!yaziYaziliyorMu) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) { if (!cTusuBasiliMi) { menuAcikMi = !menuAcikMi; cTusuBasiliMi = true; } } else cTusuBasiliMi = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) { if (!iTusuBasiliMi) { envanterAcikMi = !envanterAcikMi; iTusuBasiliMi = true; } } else iTusuBasiliMi = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) { if (!qTusuBasiliMi) { gorevAcikMi = !gorevAcikMi; qTusuBasiliMi = true; } } else qTusuBasiliMi = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) { if (!mTusuBasiliMi) { haritaAcikMi = !haritaAcikMi; mTusuBasiliMi = true; } } else mTusuBasiliMi = false;
        } else {
            cTusuBasiliMi = false; iTusuBasiliMi = false; qTusuBasiliMi = false; mTusuBasiliMi = false;
        }

        int toplamSaldiri = 20 + oyuncu.statSaldiriGucu; 
        int toplamZirh = 10 + oyuncu.statZirh;    
        for (int i = 0; i < 6; i++) { if (oyuncu.ekipmanlar[i].doluMu) { toplamSaldiri += oyuncu.ekipmanlar[i].saldiriGucu; toplamZirh += oyuncu.ekipmanlar[i].zirh; } }

        sf::Vector2f oyuncuMerkez = oyuncu.sprite.getPosition() + sf::Vector2f(oyuncu.genislik / 2.0f, oyuncu.yukseklik / 2.0f);
        sf::Vector2f oyuncuIsoPos = izoYap(oyuncuMerkez.x, oyuncuMerkez.y);

        bool fareEsyaUzerinde = false; int hoverEsyaIndeks = -1;
        for (int i = 0; i < yerdekiEsyalar.size(); i++) {
            yerdekiEsyalar[i].sprite.setTexture(yerdekiEsyalar[i].esya.doku); 
            yerdekiEsyalar[i].sprite.setPosition(izoYap(yerdekiEsyalar[i].gercekPos.x, yerdekiEsyalar[i].gercekPos.y));
            if (yerdekiEsyalar[i].sprite.getGlobalBounds().contains(fareDunyaIso)) { fareEsyaUzerinde = true; hoverEsyaIndeks = i; break; }
        }

        int hoverDusmanId = -1;
        for (const auto& dusman : dusmanlar) {
            sf::Vector2f dMerkez = dusman.sprite.getPosition() + sf::Vector2f(dusman.genislik / 2.0f, dusman.yukseklik / 2.0f);
            sf::Vector2f isoPos = izoYap(dMerkez.x, dMerkez.y);
            if (fareDunyaIso.x >= isoPos.x - 30.0f && fareDunyaIso.x <= isoPos.x + 30.0f && fareDunyaIso.y >= isoPos.y - 60.0f && fareDunyaIso.y <= isoPos.y + 10.0f) { hoverDusmanId = dusman.id; break; }
        }

        if (ui.duzenlemeModu) {
            if (solTikBasildi) {
                for (int i = 0; i < 8; i++) {
                    if (ui.getSlotResizeHandleRect(i).contains(farePencerePosUI)) {
                        ui.suruklenenSlot = i; ui.suruklenenIslem = 1; break;
                    }
                }
                if (ui.suruklenenSlot == -1) {
                    for (int i = 0; i < 8; i++) {
                        if (ui.getAksiyonSlotRect(i).contains(farePencerePosUI)) {
                            ui.suruklenenSlot = i; ui.suruklenenIslem = 0; break;
                        }
                    }
                }
                if (ui.suruklenenSlot == -1 && ui.suruklenenEleman == -1) {
                    if (ui.getTabResizeHandleRect().contains(farePencerePosUI)) {
                        ui.suruklenenEleman = 3;
                    }
                }
                if (ui.suruklenenSlot == -1 && ui.suruklenenEleman == -1) {
                    if (ui.getTabRect().contains(farePencerePosUI)) {
                        ui.suruklenenEleman = 4;
                        ui.suruklemeOffset = {farePencerePosUI.x - tabX, farePencerePosUI.y - tabY};
                    }
                }
                sf::FloatRect mmRect({miniMapX - 90.0f, miniMapY - 90.0f}, {180.0f, 180.0f});
                if (ui.suruklenenSlot == -1 && ui.suruklenenEleman == -1 && mmRect.contains(farePencerePosUI)) {
                    ui.suruklenenEleman = 2;
                }
            }
            if (solTikAnlik) {
                if (ui.suruklenenSlot != -1) {
                    if (ui.suruklenenIslem == 0) {
                        slotX[ui.suruklenenSlot] = farePencerePosUI.x - slotSize[ui.suruklenenSlot] / 2.0f;
                        slotY[ui.suruklenenSlot] = farePencerePosUI.y - slotSize[ui.suruklenenSlot] / 2.0f;
                    } else if (ui.suruklenenIslem == 1) {
                        float yeniX = farePencerePosUI.x; float yeniY = farePencerePosUI.y;
                        float minBoyut = 20.0f;
                        float yeniBoyut = std::max(minBoyut, std::max(yeniX - slotX[ui.suruklenenSlot], yeniY - slotY[ui.suruklenenSlot]));
                        slotSize[ui.suruklenenSlot] = yeniBoyut;
                    }
                } else if (ui.suruklenenEleman == 2) {
                    miniMapX = farePencerePosUI.x; miniMapY = farePencerePosUI.y;
                } else if (ui.suruklenenEleman == 3) {
                    tabW = std::max(100.0f, farePencerePosUI.x - tabX);
                    tabH = std::max(60.0f, farePencerePosUI.y - tabY);
                    ui.menuArkaPlan.setSize(sf::Vector2f({tabW, tabH}));
                } else if (ui.suruklenenEleman == 4) {
                    tabX = farePencerePosUI.x - ui.suruklemeOffset.x;
                    tabY = farePencerePosUI.y - ui.suruklemeOffset.y;
                    ui.menuArkaPlan.setPosition({tabX, tabY});
                }
            }
            if (solTikBirakildi) {
                ui.suruklenenSlot = -1; ui.suruklenenEleman = -1; ui.suruklenenIslem = -1;
            }
        }
        else {
            if (solTikBasildi && menuAcikMi && karakterSekmesi == 1 && oyuncu.yetenekPuanlari > 0 && !yaziYaziliyorMu) {
                int yetSecim = ui.yetenekButonKontrol(farePencerePosUI);
                if (yetSecim == 1) { oyuncu.skillCokluAtisLvl++; oyuncu.yetenekPuanlari--; }
                else if (yetSecim == 2) { oyuncu.skillAtilmaLvl++; oyuncu.yetenekPuanlari--; }
                else if (yetSecim == 3) { oyuncu.skillEkstraLvl++; oyuncu.yetenekPuanlari--; }
                else if (yetSecim == 4) { oyuncu.skillPatlamaLvl++; oyuncu.yetenekPuanlari--; }
            }

            if (solTikBasildi && menuAcikMi && karakterSekmesi == 0 && oyuncu.statPuanlari > 0 && !yaziYaziliyorMu) {
                int statSecim = ui.statButonKontrol(farePencerePosUI);
                if (statSecim == 1) { oyuncu.maxCan += 20.0f; oyuncu.mevcutCan += 20.0f; oyuncu.statPuanlari--; }
                else if (statSecim == 2) { oyuncu.statSaldiriGucu += 3; oyuncu.statPuanlari--; }
                else if (statSecim == 3) { oyuncu.statZirh += 2; oyuncu.statPuanlari--; }
                else if (statSecim == 4) { oyuncu.hareketHizi += 5.0f; oyuncu.statPuanlari--; }
            }
            
            if (sagTikBasildi && !ui.surukleniyorMu && !yaziYaziliyorMu) {
                for (int i = 0; i < 8; i++) {
                    if (ui.getAksiyonSlotRect(i).contains(farePencerePosUI)) {
                        oyuncu.hizliErisim[i] = "";
                        break;
                    }
                }
            }

            if (solTikBirakildi && ui.surukleniyorMu && ui.kaynakTip == 3) {
                for(int i=0; i<8; i++) {
                    if (ui.getAksiyonSlotRect(i).contains(farePencerePosUI)) {
                        oyuncu.hizliErisim[i] = ui.suruklenenEsya.isim; break;
                    }
                }
                ui.surukleniyorMu = false;
            }

            if (solTikBasildi && !yaziYaziliyorMu) {
                int tiklananButon = ui.arayuzButonKontrol(farePencerePosUI);
                if (tiklananButon == 1) { envanterAcikMi = !envanterAcikMi; menuAcikMi = false; gorevAcikMi = false; haritaAcikMi = false; ayarlarAcikMi = false; } 
                else if (tiklananButon == 2) { gorevAcikMi = !gorevAcikMi; menuAcikMi = false; envanterAcikMi = false; haritaAcikMi = false; ayarlarAcikMi = false; } 
                else if (tiklananButon == 3 || tiklananButon == 5) { menuAcikMi = !menuAcikMi; envanterAcikMi = false; gorevAcikMi = false; haritaAcikMi = false; ayarlarAcikMi = false; } 
                else if (tiklananButon == 4) { ayarlarAcikMi = !ayarlarAcikMi; menuAcikMi = false; envanterAcikMi = false; gorevAcikMi = false; haritaAcikMi = false; ayarlarAltMenu = 0; }
                else if (ayarlarAcikMi) {
                    int secim = ui.ayarlarButonKontrol(farePencerePosUI, ayarlarAltMenu);
                    if (ayarlarAltMenu == 0) {
                        if (secim == 1) ayarlarAcikMi = false; else if (secim == 2) { ayarlarAltMenu = 2; klavyeMetin = ""; } else if (secim == 6) { ayarlarAltMenu = 1; klavyeMetin = ""; } 
                        else if (secim == 7) { ayarlarAcikMi = false; ui.duzenlemeModu = true; sistemMesajlari.push_back({"Arayuz Duzenleme Modu Aktif", 4.0f}); }
                        else if (secim == 3) sistemMesajlari.push_back({"Cozunurluk degistirilemez (Sabit)", 3.0f});
                        else if (secim == 4) { oyunSesSeviyesi -= 25; if(oyunSesSeviyesi < 0) oyunSesSeviyesi = 100; sf::Listener::setGlobalVolume(oyunSesSeviyesi); }
                        else if (secim == 8) { 
                            gununSaati = (gununSaati + 6) % 24; 
                            sistemMesajlari.push_back({"Saat degistirildi: " + std::to_string(gununSaati) + ":00", 3.0f});
                        }
                        else if (secim == 5) window.close();
                    } else if (ayarlarAltMenu == 1) {
                        auto liste = ui.kayitlariListele();
                        if (secim >= 11 && secim <= 14 && (secim - 11) < liste.size()) {
                            if(oyunuYukle(liste[secim - 11])) oyunDurumu = 1;
                        }
                    }
                }
                else if (gorevAcikMi) { gorevYoneticisi.tiklamaKontrol(farePencerePosUI.x, farePencerePosUI.y); }
                else if (!menuAcikMi && !envanterAcikMi && !gorevAcikMi && !haritaAcikMi && !ayarlarAcikMi && !dede.etkilesimVar && !ui.surukleniyorMu) {
                    if (hoverDusmanId != -1) { seciliDusmanId = hoverDusmanId; }
                    else if (fareEsyaUzerinde && hoverEsyaIndeks != -1) {
                        sf::Vector2f fark = yerdekiEsyalar[hoverEsyaIndeks].gercekPos - oyuncuMerkez;
                        if (std::sqrt(fark.x*fark.x + fark.y*fark.y) < 60.0f) {
                            if (yerdekiEsyalar[hoverEsyaIndeks].esya.isim == "Altin") {
                                oyuncu.altin += yerdekiEsyalar[hoverEsyaIndeks].esya.miktar;
                                sistemMesajlari.push_back({"+" + std::to_string(yerdekiEsyalar[hoverEsyaIndeks].esya.miktar) + " Altin", 4.0f});
                                yerdekiEsyalar.erase(yerdekiEsyalar.begin() + hoverEsyaIndeks);
                            } else {
                                bool eklendi = false;
                                if (yerdekiEsyalar[hoverEsyaIndeks].esya.birikebilirMi) {
                                    for(int k=0; k<42; k++) {
                                        if (oyuncu.envanter[k].doluMu && oyuncu.envanter[k].isim == yerdekiEsyalar[hoverEsyaIndeks].esya.isim) {
                                            oyuncu.envanter[k].miktar++; eklendi = true; break;
                                        }
                                    }
                                }
                                if (!eklendi) {
                                    for(int k=0; k<42; k++) {
                                        if(!oyuncu.envanter[k].doluMu) {
                                            oyuncu.envanter[k] = yerdekiEsyalar[hoverEsyaIndeks].esya; eklendi = true; break;
                                        }
                                    }
                                }
                                if(eklendi) {
                                    sistemMesajlari.push_back({"Alindi: " + yerdekiEsyalar[hoverEsyaIndeks].esya.isim, 4.0f});
                                    yerdekiEsyalar.erase(yerdekiEsyalar.begin() + hoverEsyaIndeks);
                                } else sistemMesajlari.push_back({"Envanter Dolu!", 3.0f});
                            }
                        } else sistemMesajlari.push_back({"Esya almak icin cok uzak!", 3.0f});
                    } else seciliDusmanId = -1;
                }
            }
        }

        bool dedeYakinda = dede.yakindaMi(oyuncu.sprite.getPosition());
        if (dedeYakinda && !yaziYaziliyorMu) { if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) && !dede.etkilesimVar) { dede.etkilesimVar = true; dede.diyaloglariYukle(); } } 
        else { dede.etkilesimVar = false; }

        if (dede.etkilesimVar && !menuAcikMi && !envanterAcikMi && !yaziYaziliyorMu) {
            if (spaceBasildi && !spaceOnceki && !dede.secimEkraniMi && !(dede.durum == 2 && dede.diyalogSayfasi == dede.aktifDiyalog.size()-1)) dede.sonrakiDiyalog();
            bool dedeKlavye = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad1);
            if (dedeKlavye && !birOnceki) {
                if (dede.secimEkraniMi) {
                    dede.durum = 1; dede.secimEkraniMi = false; dede.diyaloglariYukle();
                    if (dede.gorevAsamasi == 1) { gorevYoneticisi.gorevEkle(1, "Ormanin Huzursuzlugu", "Yakindaki 2 canavari temizle.\n\nOdul: 500 Altin, 150 EXP", 2); sistemMesajlari.push_back({"Gorev Basladi: Ormanin Huzursuzlugu", 5.0f}); } 
                    else if (dede.gorevAsamasi == 2) { gorevYoneticisi.gorevEkle(2, "Suya Karisan Zehir", "Kopruyu gec ve nehrin karsisina ulas.\n\nOdul: Deri Cizme, 200 EXP", 1); sistemMesajlari.push_back({"Gorev Basladi: Suya Karisan Zehir", 5.0f}); } 
                    else if (dede.gorevAsamasi == 3) { gorevYoneticisi.gorevEkle(3, "Lanetli Ganimet", "Envanterine 3 adet Yozlasmis Yurek topla.\n\nOdul: 1000 Altin, 300 EXP", 3); sistemMesajlari.push_back({"Gorev Basladi: Lanetli Ganimet", 5.0f}); } 
                    else if (dede.gorevAsamasi == 4) { gorevYoneticisi.gorevEkle(4, "Korucunun Mirasi", "Haritada beliren Kadim Orman Zirhini kusan.\n\nOdul: Golge Yuzugu, 500 EXP", 1); sistemMesajlari.push_back({"Gorev Basladi: Korucunun Mirasi", 5.0f});
                        Esya zirh; for(auto& e : oyuncu.tumOyunEsyalari) if(e.isim == "Kadim Orman Zirhi") zirh = e;
                        yerdekiEsyalar.push_back(YerdekiEsya(zirh, oyuncuMerkez.x + 100.0f, oyuncuMerkez.y + 100.0f));
                    } 
                    else if (dede.gorevAsamasi == 5) { gorevYoneticisi.gorevEkle(5, "Sisin Efendisi", "Ormanin derinligindeki Boss'u kes!\n\nOdul: Efsanevi Yay, 5000 Altin", 1); sistemMesajlari.push_back({"Gorev Basladi: Sisin Efendisi", 5.0f}); }
                } 
                else if (dede.durum == 2 && dede.diyalogSayfasi == dede.aktifDiyalog.size()-1) {
                    if (dede.gorevAsamasi == 1) { oyuncu.altin += 500; oyuncu.expKazan(150); }
                    if (dede.gorevAsamasi == 2) { Esya cizme; for(auto& e : oyuncu.tumOyunEsyalari) if (e.isim == "Deri Cizme") cizme = e; oyuncu.envanter[0] = cizme; oyuncu.expKazan(200); sistemMesajlari.push_back({"Alindi: Deri Cizme", 4.0f}); }
                    if (dede.gorevAsamasi == 3) {
                        int silinen = 0; for(int i=0; i<42; i++) { if(oyuncu.envanter[i].isim == "Yozlasmis Yurek" && silinen < 3) { oyuncu.envanter[i].miktar--; if(oyuncu.envanter[i].miktar <= 0) oyuncu.envanter[i] = {"", "", false}; silinen++; } }
                        oyuncu.altin += 1000; oyuncu.expKazan(300);
                    }
                    if (dede.gorevAsamasi == 4) { Esya yuzuk; for(auto& e : oyuncu.tumOyunEsyalari) if (e.isim == "Golge Yuzugu") yuzuk = e; oyuncu.envanter[1] = yuzuk; oyuncu.expKazan(500); sistemMesajlari.push_back({"Alindi: Golge Yuzugu", 4.0f}); }
                    if (dede.gorevAsamasi == 5) { Esya efsanevi; for(auto& e : oyuncu.tumOyunEsyalari) if (e.isim == "Efsanevi Yay") efsanevi = e; oyuncu.envanter[2] = efsanevi; oyuncu.altin += 5000; sistemMesajlari.push_back({"Alindi: Efsanevi Yay", 4.0f}); }
                    for(auto& g : gorevYoneticisi.aktifGorevler) if (g.id == dede.gorevAsamasi) g.odulAlindi = true;
                    dede.gorevAsamasi++; dede.durum = 0; dede.diyaloglariYukle(); dede.etkilesimVar = false; 
                }
            }
            birOnceki = dedeKlavye;
        }
        spaceOnceki = spaceBasildi; 

        if (!menuAcikMi && !envanterAcikMi && !gorevAcikMi && !haritaAcikMi && !ayarlarAcikMi && !dede.etkilesimVar && !ui.duzenlemeModu) {
            
            if (atesGecikmesi > 0.0f) atesGecikmesi -= deltaZaman;
            if (hasarGecikmesi > 0.0f) hasarGecikmesi -= deltaZaman;

            oyuncu.hareketVeAnimasyonGuncelle(deltaZaman);
            oyuncu.degerleriYenile(deltaZaman); 
            at.takipEt(oyuncuMerkez, deltaZaman); 

            auto patlamaYarat = [&](sf::Vector2f merkezPos) {
                patlamalar.push_back(PatlamaEfekti(merkezPos, patlamaDokusu));
                float alanHasari = 100.0f + (oyuncu.skillPatlamaLvl * 50.0f);
                
                for (size_t dIdx = 0; dIdx < dusmanlar.size(); dIdx++) {
                    sf::Vector2f aoeMerkez = dusmanlar[dIdx].sprite.getPosition() + sf::Vector2f(dusmanlar[dIdx].genislik/2.0f, dusmanlar[dIdx].yukseklik/2.0f);
                    float farkX = merkezPos.x - aoeMerkez.x;
                    float farkY = merkezPos.y - aoeMerkez.y;
                    if (std::sqrt(farkX*farkX + farkY*farkY) < 150.0f) {
                        dusmanlar[dIdx].mevcutCan -= alanHasari;
                        dusmanlar[dIdx].aktifMi = true;
                        sf::Vector2f dIso = izoYap(aoeMerkez.x, aoeMerkez.y);
                        hasarMetinleri.emplace_back(font, alanHasari, sf::Vector2f(dIso.x, dIso.y - 60.0f), sf::Color(255, 100, 0));
                    }
                }
                for (size_t inIdx = 0; inIdx < inler.size(); inIdx++) {
                    if (inler[inIdx].yokEdilsinMi) continue;
                    sf::Vector2f aoeMerkez = inler[inIdx].sprite.getPosition() + sf::Vector2f(inler[inIdx].genislik/2.0f, inler[inIdx].yukseklik/2.0f);
                    float farkX = merkezPos.x - aoeMerkez.x;
                    float farkY = merkezPos.y - aoeMerkez.y;
                    if (std::sqrt(farkX*farkX + farkY*farkY) < 150.0f) {
                        inler[inIdx].mevcutCan -= alanHasari;
                        sf::Vector2f inIso = izoYap(aoeMerkez.x, aoeMerkez.y);
                        hasarMetinleri.emplace_back(font, alanHasari, sf::Vector2f(inIso.x, inIso.y - 60.0f), sf::Color(255, 100, 0));
                    }
                }
            };

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && hoverDusmanId == -1 && !yaziYaziliyorMu) {
                if (atesGecikmesi <= 0.0f && oyuncu.ekipmanlar[2].doluMu) {
                    oyuncu.atesEtmeTetikle(); 
                    
                    // YAY KONTROLÜ
                    bool yayMi = (oyuncu.ekipmanlar[2].isim.find("Yay") != std::string::npos || oyuncu.ekipmanlar[2].isim.find("yay") != std::string::npos);

                    if (yayMi) {
                        if (oyuncu.ekstraSuresi > 0.0f) {
                            sf::Vector2f fark = fareDunyaCart - oyuncuMerkez;
                            float angle = std::atan2(fark.y, fark.x);
                            float a1 = angle - 0.15f; 
                            float a2 = angle + 0.15f; 
                            sf::Vector2f h1 = oyuncuMerkez + sf::Vector2f(std::cos(a1), std::sin(a1)) * 100.0f;
                            sf::Vector2f h2 = oyuncuMerkez + sf::Vector2f(std::cos(a2), std::sin(a2)) * 100.0f;
                            mermiler.push_back(Mermi(oyuncuMerkez, h1, mermiDokusu));
                            mermiler.push_back(Mermi(oyuncuMerkez, h2, mermiDokusu));
                        } else {
                            mermiler.push_back(Mermi(oyuncuMerkez, fareDunyaCart, mermiDokusu));
                        }
                    } else {
                        // KILIÇ KONTROLÜ
                        slashEfektleri.push_back(KilicSlasher(oyuncuMerkez, fareDunyaCart, slashDokusu));
                    }
                    atesGecikmesi = ATES_HIZI;
                }
            }

            sf::Keyboard::Key sT[8] = {sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num2, sf::Keyboard::Key::Num3, sf::Keyboard::Key::Num4, sf::Keyboard::Key::Num5, sf::Keyboard::Key::Num6, sf::Keyboard::Key::Num7, sf::Keyboard::Key::Num8};
            sf::Keyboard::Key nT[8] = {sf::Keyboard::Key::Numpad1, sf::Keyboard::Key::Numpad2, sf::Keyboard::Key::Numpad3, sf::Keyboard::Key::Numpad4, sf::Keyboard::Key::Numpad5, sf::Keyboard::Key::Numpad6, sf::Keyboard::Key::Numpad7, sf::Keyboard::Key::Numpad8};
            
            for (int k = 0; k < 8; k++) {
                bool tusBasildi = sf::Keyboard::isKeyPressed(sT[k]) || sf::Keyboard::isKeyPressed(nT[k]);
                
                if (tusBasildi && !klavyeBirOnceki[k] && !yaziYaziliyorMu) {
                    if (oyuncu.hizliErisim[k] == "Coklu Atis" && oyuncu.cokluAtisBekleme <= 0.0f && oyuncu.mevcutMana >= 25.0f && oyuncu.skillCokluAtisLvl > 0) {
                        oyuncu.mevcutMana -= 25.0f; 
                        skillKalanOk = 2 + oyuncu.skillCokluAtisLvl; 
                        skillOkGecikmesi = 0.0f; 
                        oyuncu.cokluAtisBekleme = std::max(5.0f, 15.0f - oyuncu.skillCokluAtisLvl * 1.5f); 
                    }
                    else if (oyuncu.hizliErisim[k] == "Atilma" && oyuncu.atilmaBekleme <= 0.0f && oyuncu.mevcutMana >= 15.0f && oyuncu.skillAtilmaLvl > 0) {
                        sf::Vector2f dir = fareDunyaCart - oyuncuMerkez;
                        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
                        if(len > 0) {
                            dir /= len;
                            oyuncu.atilmaYonu = dir;
                            oyuncu.atilmaKalanSure = 0.15f; 
                            hasarMetinleri.emplace_back(font, "ATILMA!", sf::Vector2f(oyuncuIsoPos.x, oyuncuIsoPos.y - 80.0f), sf::Color::Yellow);
                            oyuncu.atilmaBekleme = std::max(2.0f, 8.0f - oyuncu.skillAtilmaLvl * 1.0f);
                            oyuncu.mevcutMana -= 15.0f;
                        }
                    }
                    else if (oyuncu.hizliErisim[k] == "Ekstra Atis" && oyuncu.ekstraBekleme <= 0.0f && oyuncu.mevcutMana >= 30.0f && oyuncu.skillEkstraLvl > 0) {
                        oyuncu.ekstraSuresi = 10.0f + (oyuncu.skillEkstraLvl * 2.0f); 
                        oyuncu.ekstraBekleme = std::max(10.0f, 30.0f - oyuncu.skillEkstraLvl * 2.0f); 
                        oyuncu.mevcutMana -= 30.0f;
                        hasarMetinleri.emplace_back(font, "EKSTRA ATIS AKTIF!", sf::Vector2f(oyuncuIsoPos.x, oyuncuIsoPos.y - 80.0f), sf::Color::Cyan);
                    }
                    else if (oyuncu.hizliErisim[k] == "Patlayan Ok" && oyuncu.patlamaBekleme <= 0.0f && oyuncu.mevcutMana >= 40.0f && oyuncu.skillPatlamaLvl > 0) {
                        oyuncu.mevcutMana -= 40.0f;
                        oyuncu.patlamaBekleme = std::max(8.0f, 20.0f - oyuncu.skillPatlamaLvl * 2.0f);
                        oyuncu.atesEtmeTetikle();
                        
                        bool yayMi = (oyuncu.ekipmanlar[2].isim.find("Yay") != std::string::npos || oyuncu.ekipmanlar[2].isim.find("yay") != std::string::npos);

                        if (yayMi) {
                            if (oyuncu.ekstraSuresi > 0.0f) {
                                sf::Vector2f fark = fareDunyaCart - oyuncuMerkez;
                                float angle = std::atan2(fark.y, fark.x);
                                sf::Vector2f h1 = oyuncuMerkez + sf::Vector2f(std::cos(angle - 0.15f), std::sin(angle - 0.15f)) * 100.0f;
                                sf::Vector2f h2 = oyuncuMerkez + sf::Vector2f(std::cos(angle + 0.15f), std::sin(angle + 0.15f)) * 100.0f;
                                
                                Mermi m1(oyuncuMerkez, h1, mermiDokusu, true); m1.sprite.setColor(sf::Color(255, 50, 50));
                                Mermi m2(oyuncuMerkez, h2, mermiDokusu, true); m2.sprite.setColor(sf::Color(255, 50, 50));
                                mermiler.push_back(m1); mermiler.push_back(m2);
                            } else {
                                Mermi m1(oyuncuMerkez, fareDunyaCart, mermiDokusu, true); m1.sprite.setColor(sf::Color(255, 50, 50));
                                mermiler.push_back(m1);
                            }
                            hasarMetinleri.emplace_back(font, "PATLAYAN OK!", sf::Vector2f(oyuncuIsoPos.x, oyuncuIsoPos.y - 80.0f), sf::Color(255, 100, 0));
                        } else {
                            KilicSlasher ks(oyuncuMerkez, fareDunyaCart, slashDokusu);
                            ks.patlayanMi = true; // Kılıç ile de patlayan atak vuracak!
                            ks.sprite.setColor(sf::Color(255, 100, 100)); // Rengini kırmızı yap
                            slashEfektleri.push_back(ks);
                            hasarMetinleri.emplace_back(font, "PATLAYAN DARBE!", sf::Vector2f(oyuncuIsoPos.x, oyuncuIsoPos.y - 80.0f), sf::Color(255, 100, 0));
                        }
                    }
                    else if (oyuncu.hizliErisim[k] == "Can Potu") {
                        for (int e = 0; e < 42; e++) {
                            if (oyuncu.envanter[e].doluMu && oyuncu.envanter[e].isim == "Can Potu") {
                                oyuncu.envanter[e].miktar--; 
                                oyuncu.mevcutCan += 50.0f; if (oyuncu.mevcutCan > oyuncu.maxCan) oyuncu.mevcutCan = oyuncu.maxCan;
                                sf::Vector2f oIso = izoYap(oyuncuMerkez.x, oyuncuMerkez.y);
                                hasarMetinleri.emplace_back(font, "+50 CAN", sf::Vector2f(oIso.x, oIso.y - 80.0f), sf::Color::Green);
                                
                                if (oyuncu.envanter[e].miktar <= 0) oyuncu.envanter[e] = {"", "", false};
                                
                                bool halaVar = false;
                                for (int j = 0; j < 42; j++) if (oyuncu.envanter[j].doluMu && oyuncu.envanter[j].isim == "Can Potu") halaVar = true;
                                if (!halaVar) oyuncu.hizliErisim[k] = ""; 
                                break;
                            }
                        }
                    }
                }
                klavyeBirOnceki[k] = tusBasildi;
            }

            if (skillKalanOk > 0) {
                skillOkGecikmesi -= deltaZaman;
                if (skillOkGecikmesi <= 0.0f) {
                    oyuncu.atesEtmeTetikle(); 
                    bool yayMi = (oyuncu.ekipmanlar[2].isim.find("Yay") != std::string::npos || oyuncu.ekipmanlar[2].isim.find("yay") != std::string::npos);
                    if (yayMi) {
                        if (oyuncu.ekstraSuresi > 0.0f) {
                            sf::Vector2f fark = fareDunyaCart - oyuncuMerkez;
                            float angle = std::atan2(fark.y, fark.x);
                            sf::Vector2f h1 = oyuncuMerkez + sf::Vector2f(std::cos(angle - 0.15f), std::sin(angle - 0.15f)) * 100.0f;
                            sf::Vector2f h2 = oyuncuMerkez + sf::Vector2f(std::cos(angle + 0.15f), std::sin(angle + 0.15f)) * 100.0f;
                            mermiler.push_back(Mermi(oyuncuMerkez, h1, mermiDokusu));
                            mermiler.push_back(Mermi(oyuncuMerkez, h2, mermiDokusu));
                        } else {
                            mermiler.push_back(Mermi(oyuncuMerkez, fareDunyaCart, mermiDokusu));
                        }
                    } else {
                        slashEfektleri.push_back(KilicSlasher(oyuncuMerkez, fareDunyaCart, slashDokusu));
                    }
                    skillKalanOk--; skillOkGecikmesi = 0.15f; 
                }
            }

            for (auto& dusman : dusmanlar) {
                dusman.takipEt(oyuncuMerkez, deltaZaman);
                
                if (dusman.aktifMi && dusman.tip == 1 && dusman.atesGecikmesi <= 0.0f) {
                    sf::Vector2f dMerkez = dusman.sprite.getPosition() + sf::Vector2f(dusman.genislik / 2.0f, dusman.yukseklik / 2.0f);
                    
                    sf::Vector2f fark = oyuncuMerkez - dMerkez;
                    float mesafe = std::sqrt(fark.x * fark.x + fark.y * fark.y);
                    
                    if (mesafe > 0.0f && mesafe < dusman.gorisMenzili) {
                        Mermi yeniOk(dMerkez, oyuncuMerkez, mermiDokusu);
                        yeniOk.hiz = 400.0f; 
                        yeniOk.sprite.setColor(sf::Color(180, 50, 50)); 
                        dusmanMermileri.push_back(yeniOk);
                        
                        dusman.atesGecikmesi = 1.5f; 
                    }
                }
            }
            
            for (size_t i = 0; i < dusmanMermileri.size(); i++) {
                dusmanMermileri[i].guncelle(deltaZaman); 
                sf::Vector2f mPos = dusmanMermileri[i].sprite.getPosition();
                
                int mGridX = static_cast<int>(mPos.x / tileSize), mGridY = static_cast<int>(mPos.y / tileSize);
                if (mGridX >= 0 && mGridX < MAP_WIDTH && mGridY >= 0 && mGridY < MAP_HEIGHT) {
                    int tile = harita[mGridY][mGridX]; 
                    if (tile == 1 || tile == 2) dusmanMermileri[i].yokEdilsinMi = true; 
                } else {
                    dusmanMermileri[i].yokEdilsinMi = true;
                }

                if (dusmanMermileri[i].yokEdilsinMi) { dusmanMermileri.erase(dusmanMermileri.begin() + i); i--; continue; }

                sf::Vector2f fark = mPos - oyuncuMerkez;
                float mesafe = std::sqrt(fark.x * fark.x + fark.y * fark.y);
                if (mesafe < 20.0f) { 
                    dusmanMermileri[i].yokEdilsinMi = true; 
                    
                    float hamHasar = 25.0f; 
                    float netHasar = std::max(1.0f, hamHasar - (toplamZirh * 0.15f)); 
                    oyuncu.mevcutCan -= netHasar; 
                    
                    sf::Vector2f oIso = izoYap(oyuncuMerkez.x, oyuncuMerkez.y);
                    hasarMetinleri.emplace_back(font, netHasar, sf::Vector2f(oIso.x, oIso.y - 80.0f), sf::Color(255, 50, 50));
                }

                if (dusmanMermileri[i].yokEdilsinMi) { dusmanMermileri.erase(dusmanMermileri.begin() + i); i--; }
            }

            if (dede.gorevAsamasi == 2 && dede.durum == 1) {
                if (oyuncuMerkez.x > tileSize * 12 && oyuncuMerkez.y > tileSize * 12) {
                    dede.durum = 2; gorevYoneticisi.ilerlemeGuncelle(2, 1); 
                    sistemMesajlari.push_back({"Gorev Tamamlandi: Suya Karisan Zehir", 5.0f});
                }
            }
            if (dede.gorevAsamasi == 3 && dede.durum == 1) {
                int yurekSayisi = 0; for(int k=0; k<42; k++) if(oyuncu.envanter[k].isim == "Yozlasmis Yurek") yurekSayisi += oyuncu.envanter[k].miktar;
                for (auto& g : gorevYoneticisi.aktifGorevler) {
                    if (g.id == 3) {
                        g.anlikSayi = yurekSayisi;
                        if (g.anlikSayi >= g.hedefSayi && !g.tamamlandi) { g.tamamlandi = true; dede.durum = 2; sistemMesajlari.push_back({"Gorev Tamamlandi: Lanetli Ganimet", 5.0f}); }
                    }
                }
            }
            if (dede.gorevAsamasi == 4 && dede.durum == 1) {
                if (oyuncu.ekipmanlar[3].isim == "Kadim Orman Zirhi") {
                    dede.durum = 2; gorevYoneticisi.ilerlemeGuncelle(4, 1); sistemMesajlari.push_back({"Gorev Tamamlandi: Korucunun Mirasi", 5.0f});
                }
            }
            if (dede.gorevAsamasi == 5 && dede.durum == 1 && !bossSpawlandi) {
                Dusman boss(nextDusmanId++, 1200.0f, 1200.0f, dusmanDokusu, -1, 2); 
                boss.mevcutCan = 1000.0f; boss.maxCan = 1000.0f; boss.genislik = 80.0f; boss.yukseklik = 80.0f;
                boss.sprite.setScale({2.5f, 2.5f}); boss.sprite.setColor(sf::Color(120, 30, 180));
                dusmanlar.push_back(boss); bossSpawlandi = true;
                hasarMetinleri.emplace_back(font, "SİSİN EFENDİSİ UYANDI!", sf::Vector2f(oyuncuIsoPos.x, oyuncuIsoPos.y - 120.0f), sf::Color::Red);
            }

            for (auto& in : inler) {
                if (in.yokEdilsinMi) continue;
                if (in.aktifDusmanSayisi < 3) {
                    in.spawnSayaci += deltaZaman;
                    if (in.spawnSayaci >= 10.0f) {
                        in.spawnSayaci = 0.0f; in.aktifDusmanSayisi++;
                        
                        int secilenTip = (std::rand() % 100 < 50) ? 0 : 1;
                        const sf::Texture& secilenDoku = (secilenTip == 1) ? okcuDusmanDokusu : dusmanDokusu;
                        
                        dusmanlar.emplace_back(nextDusmanId++, in.x + 20.0f, in.y + 20.0f, secilenDoku, in.id, secilenTip);
                    }
                }
            }

            for (int i = 0; i < hasarMetinleri.size(); i++) {
                hasarMetinleri[i].guncelle(deltaZaman);
                if (hasarMetinleri[i].omur <= 0.0f) { hasarMetinleri.erase(hasarMetinleri.begin() + i); i--; }
            }

            for (auto& dusman : dusmanlar) {
                sf::Vector2f dMerkez = dusman.sprite.getPosition() + sf::Vector2f(dusman.genislik / 2.0f, dusman.yukseklik / 2.0f);
                sf::Vector2f fark = dMerkez - oyuncuMerkez; float mesafe = std::sqrt(fark.x * fark.x + fark.y * fark.y);
                float carpicmaSiniri = (dusman.genislik > 50.0f) ? 50.0f : 25.0f;

                if (dusman.aktifMi && mesafe < carpicmaSiniri) { 
                    if (hasarGecikmesi <= 0.0f) { 
                        float hamHasar = (dusman.genislik > 50.0f) ? 40.0f : 15.0f; 
                        float netHasar = std::max(1.0f, hamHasar - (toplamZirh * 0.15f)); 
                        oyuncu.mevcutCan -= netHasar; hasarGecikmesi = 0.5f; 
                        sf::Vector2f oIso = izoYap(oyuncuMerkez.x, oyuncuMerkez.y);
                        hasarMetinleri.emplace_back(font, netHasar, sf::Vector2f(oIso.x, oIso.y - 80.0f), sf::Color(255, 50, 50));
                    }
                }
            }

            for (size_t i = 0; i < mermiler.size(); i++) {
                mermiler[i].guncelle(deltaZaman); sf::Vector2f mPos = mermiler[i].sprite.getPosition();
                int mGridX = static_cast<int>(mPos.x / tileSize), mGridY = static_cast<int>(mPos.y / tileSize);
                if (mGridX >= 0 && mGridX < MAP_WIDTH && mGridY >= 0 && mGridY < MAP_HEIGHT) {
                    int tile = harita[mGridY][mGridX]; 
                    if (tile == 1 || tile == 2) {
                        mermiler[i].yokEdilsinMi = true;
                        if (mermiler[i].patlayanMi) patlamaYarat(mPos);
                    }
                } else mermiler[i].yokEdilsinMi = true;

                if (mermiler[i].yokEdilsinMi) { mermiler.erase(mermiler.begin() + i); i--; continue; }

                bool carpistiMi = false;
                for (size_t j = 0; j < dusmanlar.size(); j++) {
                    sf::Vector2f dMerkez = dusmanlar[j].sprite.getPosition() + sf::Vector2f(dusmanlar[j].genislik / 2.0f, dusmanlar[j].yukseklik / 2.0f);
                    sf::Vector2f fark = mPos - dMerkez; float mesafe = std::sqrt(fark.x * fark.x + fark.y * fark.y);
                    if (mesafe < ((dusmanlar[j].genislik > 50.0f) ? 40.0f : 20.0f)) { 
                        carpistiMi = true;
                        if (mermiler[i].patlayanMi) {
                            patlamaYarat(mPos); 
                        } else {
                            float gercekHasar = static_cast<float>(toplamSaldiri + (oyuncu.skillCokluAtisLvl * 5)); 
                            dusmanlar[j].mevcutCan -= gercekHasar; dusmanlar[j].aktifMi = true;
                            sf::Vector2f dMerkezIso = izoYap(dMerkez.x, dMerkez.y);
                            hasarMetinleri.emplace_back(font, gercekHasar, sf::Vector2f(dMerkezIso.x, dMerkezIso.y - 60.0f), sf::Color(255, 215, 0));
                        }
                        break;
                    }
                }

                if (!carpistiMi) {
                    for (size_t j = 0; j < inler.size(); j++) {
                        if (inler[j].yokEdilsinMi) continue;
                        sf::Vector2f inMerkez = inler[j].sprite.getPosition() + sf::Vector2f(inler[j].genislik / 2.0f, inler[j].yukseklik / 2.0f);
                        sf::Vector2f fark = mPos - inMerkez;
                        if (std::sqrt(fark.x * fark.x + fark.y * fark.y) < 40.0f) { 
                            carpistiMi = true;
                            if (mermiler[i].patlayanMi) {
                                patlamaYarat(mPos); 
                            } else {
                                inler[j].mevcutCan -= static_cast<float>(toplamSaldiri);
                                sf::Vector2f inMerkezIso = izoYap(inMerkez.x, inMerkez.y);
                                hasarMetinleri.emplace_back(font, toplamSaldiri, sf::Vector2f(inMerkezIso.x, inMerkezIso.y - 60.0f), sf::Color(255, 100, 100));
                            }
                            break;
                        }
                    }
                }

                if (carpistiMi) mermiler[i].yokEdilsinMi = true;
                if (mermiler[i].yokEdilsinMi) { mermiler.erase(mermiler.begin() + i); i--; }
            }

            // YENİ: KILIÇ SLASH ÇARPIŞMALARI VE HASAR HESAPLAMASI
            for (size_t i = 0; i < slashEfektleri.size(); i++) {
                slashEfektleri[i].guncelle(deltaZaman, slashDokusu);
                
                if (!slashEfektleri[i].hasarVerildi) {
                    float gercekHasar = static_cast<float>(toplamSaldiri + (oyuncu.skillCokluAtisLvl * 5)); 
                    
                    if (slashEfektleri[i].patlayanMi) {
                        patlamaYarat(slashEfektleri[i].sprite.getPosition());
                    }

                    for (size_t j = 0; j < dusmanlar.size(); j++) {
                        sf::Vector2f dMerkez = dusmanlar[j].sprite.getPosition() + sf::Vector2f(dusmanlar[j].genislik / 2.0f, dusmanlar[j].yukseklik / 2.0f);
                        sf::Vector2f fark = slashEfektleri[i].sprite.getPosition() - dMerkez; 
                        float mesafe = std::sqrt(fark.x * fark.x + fark.y * fark.y);
                        
                        if (mesafe < ((dusmanlar[j].genislik > 50.0f) ? 80.0f : 60.0f)) { 
                            dusmanlar[j].mevcutCan -= gercekHasar; 
                            dusmanlar[j].aktifMi = true;
                            sf::Vector2f dMerkezIso = izoYap(dMerkez.x, dMerkez.y);
                            hasarMetinleri.emplace_back(font, gercekHasar, sf::Vector2f(dMerkezIso.x, dMerkezIso.y - 60.0f), sf::Color(255, 215, 0));
                        }
                    }
                    for (size_t j = 0; j < inler.size(); j++) {
                        if (inler[j].yokEdilsinMi) continue;
                        sf::Vector2f inMerkez = inler[j].sprite.getPosition() + sf::Vector2f(inler[j].genislik / 2.0f, inler[j].yukseklik / 2.0f);
                        sf::Vector2f fark = slashEfektleri[i].sprite.getPosition() - inMerkez;
                        if (std::sqrt(fark.x * fark.x + fark.y * fark.y) < 60.0f) { 
                            inler[j].mevcutCan -= gercekHasar;
                            sf::Vector2f inMerkezIso = izoYap(inMerkez.x, inMerkez.y);
                            hasarMetinleri.emplace_back(font, gercekHasar, sf::Vector2f(inMerkezIso.x, inMerkezIso.y - 60.0f), sf::Color(255, 100, 100));
                        }
                    }
                    slashEfektleri[i].hasarVerildi = true;
                }

                if (slashEfektleri[i].bitti) { 
                    slashEfektleri.erase(slashEfektleri.begin() + i); 
                    i--; 
                }
            }
            
            for (size_t i = 0; i < patlamalar.size(); i++) {
                patlamalar[i].guncelle(deltaZaman, patlamaDokusu);
                if (patlamalar[i].bitti) { patlamalar.erase(patlamalar.begin() + i); i--; }
            }
        }

        for (size_t j = 0; j < inler.size(); j++) {
            if (!inler[j].yokEdilsinMi && inler[j].mevcutCan <= 0.0f) {
                inler[j].yokEdilsinMi = true; sf::Vector2f isoPos = izoYap(inler[j].x, inler[j].y);
                hasarMetinleri.emplace_back(font, "IN YOK EDILDI!", sf::Vector2f(isoPos.x, isoPos.y - 60.0f), sf::Color(255, 215, 0));
            }
        }

        for (size_t j = 0; j < dusmanlar.size(); j++) {
            if (dusmanlar[j].mevcutCan <= 0.0f) dusmanlar[j].yokEdilsinMi = true; 
            if (dusmanlar[j].yokEdilsinMi) { 
                sf::Vector2f gercekPos = dusmanlar[j].sprite.getPosition();
                sf::Vector2f isoPos = izoYap(gercekPos.x + dusmanlar[j].genislik / 2.0f, gercekPos.y + dusmanlar[j].yukseklik / 2.0f);
                
                bool bossMuydu = (dusmanlar[j].tip == 2);
                float kazanilanExp = bossMuydu ? 500.0f : 35.0f;
                hasarMetinleri.emplace_back(font, "+" + std::to_string((int)kazanilanExp) + " EXP", sf::Vector2f(isoPos.x, isoPos.y - 40.0f), sf::Color(255, 215, 0));
                oyuncu.expKazan(kazanilanExp); 

                Esya altinEsya = {"Altin", "Para", true, altinDokusu, 0, 0, 0, 0, 20 + (std::rand() % 31), true};
                yerdekiEsyalar.push_back(YerdekiEsya(altinEsya, gercekPos.x - 20.0f, gercekPos.y)); 

                if (!bossMuydu && std::rand() % 100 < 25) {
                    Esya pot = {"Can Potu", "Iksir", true, potDokusu, 50, 0, 0, 0, 1, true};
                    for(auto& e : oyuncu.tumOyunEsyalari) if(e.isim == "Can Potu") { pot = e; pot.miktar = 1; break; }
                    yerdekiEsyalar.push_back(YerdekiEsya(pot, gercekPos.x + 20.0f, gercekPos.y)); 
                }

                if (dede.gorevAsamasi == 1 && dede.durum == 1) {
                    q1Oldurulen++; gorevYoneticisi.ilerlemeGuncelle(1, 1); 
                    if (q1Oldurulen >= 2) { dede.durum = 2; sistemMesajlari.push_back({"Gorev Tamamlandi: Ormanin Huzursuzlugu", 5.0f}); }
                }

                if (dede.gorevAsamasi == 3 && dede.durum == 1 && !bossMuydu) {
                    Esya yurek; for(auto& e : oyuncu.tumOyunEsyalari) if(e.isim == "Yozlasmis Yurek") yurek = e;
                    yurek.miktar = 1; yurek.birikebilirMi = true; 
                    yerdekiEsyalar.push_back(YerdekiEsya(yurek, gercekPos.x, gercekPos.y));
                }

                if (dede.gorevAsamasi == 5 && dede.durum == 1 && bossMuydu) {
                    dede.durum = 2; gorevYoneticisi.ilerlemeGuncelle(5, 1); sistemMesajlari.push_back({"Gorev Tamamlandi: Sisin Efendisi", 6.0f});
                }

                if (dusmanlar[j].ureticiId != -1) for(auto& in : inler) if (in.id == dusmanlar[j].ureticiId) { in.aktifDusmanSayisi--; break; }
                if (seciliDusmanId == dusmanlar[j].id) seciliDusmanId = -1;
                dusmanlar.erase(dusmanlar.begin() + j); j--; 
            }
        }

        oyunKamerasi.setCenter(oyuncuIsoPos); window.clear(sf::Color::Black); window.setView(oyunKamerasi);

        int pGridX = static_cast<int>(oyuncuMerkez.x / tileSize);
        int pGridY = static_cast<int>(oyuncuMerkez.y / tileSize);
        int gorusMesafesi = 35; 
        
        int basY = std::max(0, pGridY - gorusMesafesi);
        int bitY = std::min(MAP_HEIGHT - 1, pGridY + gorusMesafesi);
        int basX = std::max(0, pGridX - gorusMesafesi);
        int bitX = std::min(MAP_WIDTH - 1, pGridX + gorusMesafesi);

        for (int y = basY; y <= bitY; ++y) {
            for (int x = basX; x <= bitX; ++x) {
                sf::Vector2f isoPos = izoYap(x * tileSize, y * tileSize); int tile = harita[y][x];
                sf::Sprite* aktifSprite = nullptr;
                if (tile == 3) {
                    aktifSprite = &suSprite;
                } else if (tile == 4) {
                    aktifSprite = &kopruSprite;
                } else if (tile == 8) { 
                    aktifSprite = &kumSprite;
                } else {
                    uint8_t dokuIdx = zemindokusu[y][x];
                    aktifSprite = &grassSprite[dokuIdx];
                }

                if (aktifSprite) {
                    sf::FloatRect bounds = aktifSprite->getLocalBounds();
                    float w = bounds.size.x; 
                    float h = bounds.size.y;
                    
                    if (w > 0.0f && h > 0.0f) { 
                        sf::Transform t; t.translate({isoPos.x, isoPos.y + (tileSize / 2.0f)}); t.scale({1.0f, 0.5f}); t.rotate(sf::degrees(45.0f)); 
                        float olcek = (tileSize * 1.41421356f) / w; t.scale({olcek, olcek}); t.translate({-w / 2.0f, -h / 2.0f}); 
                        sf::RenderStates states; states.transform = t; window.draw(*aktifSprite, states);
                    }
                }
            }
        }

        if (dede.gorevAsamasi == 2 && dede.durum == 1) {
            sf::ConvexShape sariAlan(4); float targetX = 14 * tileSize, targetY = 14 * tileSize;
            sariAlan.setPoint(0, izoYap(targetX, targetY)); sariAlan.setPoint(1, izoYap(targetX + 160.0f, targetY));
            sariAlan.setPoint(2, izoYap(targetX + 160.0f, targetY + 160.0f)); sariAlan.setPoint(3, izoYap(targetX, targetY + 160.0f));
            sariAlan.setFillColor(sf::Color(255, 255, 0, 100)); window.draw(sariAlan);
        }

        std::vector<CizilebilirVarlik> renderListesi;
        renderListesi.push_back({&oyuncu.sprite, oyuncu.sprite.getPosition().x + oyuncu.sprite.getPosition().y, false, oyuncu.genislik, oyuncu.yukseklik});
        renderListesi.push_back({&at.sprite, at.sprite.getPosition().x + at.sprite.getPosition().y, false, 40.0f, 40.0f}); 
        renderListesi.push_back({&dede.sprite, dede.sprite.getPosition().x + dede.sprite.getPosition().y, false, tileSize, tileSize});
        for (auto& dusman : dusmanlar) renderListesi.push_back({&dusman.sprite, dusman.sprite.getPosition().x + dusman.sprite.getPosition().y, false, dusman.genislik, dusman.yukseklik});
        for (auto& in : inler) if(!in.yokEdilsinMi) renderListesi.push_back({&in.sprite, in.x + in.y, false, in.genislik, in.yukseklik});

        for (auto& dSprite : statikDuvarDeposu) {
            if(std::abs(dSprite.getPosition().x - oyuncuMerkez.x) < 1500.0f && std::abs(dSprite.getPosition().y - oyuncuMerkez.y) < 1500.0f) {
                renderListesi.push_back({&dSprite, dSprite.getPosition().x + dSprite.getPosition().y, true, tileSize, tileSize});
            }
        }
        for (auto& aSprite : statikAgacDeposu) {
            if(std::abs(aSprite.getPosition().x - oyuncuMerkez.x) < 1500.0f && std::abs(aSprite.getPosition().y - oyuncuMerkez.y) < 1500.0f) {
                renderListesi.push_back({&aSprite, aSprite.getPosition().x + aSprite.getPosition().y, false, tileSize, tileSize});
            }
        }

        std::sort(renderListesi.begin(), renderListesi.end(), [](const CizilebilirVarlik& a, const CizilebilirVarlik& b) { return a.zIndeks < b.zIndeks; });

        for (const auto& varlik : renderListesi) {
            sf::Sprite cizimSprite = *varlik.spritePtr; sf::Vector2f gercekPos = varlik.spritePtr->getPosition();
            sf::Vector2f merkezIso = izoYap(gercekPos.x + varlik.genislik / 2.0f, gercekPos.y + varlik.yukseklik / 2.0f);
            sf::FloatRect bounds = cizimSprite.getLocalBounds();
            cizimSprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y * 0.85f}); cizimSprite.setPosition(merkezIso); window.draw(cizimSprite);
        }

        for (auto& ye : yerdekiEsyalar) {
            ye.sprite.setTexture(ye.esya.doku); sf::Vector2f isoPos = izoYap(ye.gercekPos.x, ye.gercekPos.y);
            ye.sprite.setPosition(isoPos); window.draw(ye.sprite);
        }

        for (auto& mermi : mermiler) {
            sf::Sprite cMermi = mermi.sprite; sf::FloatRect bounds = cMermi.getLocalBounds(); cMermi.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            sf::Vector2f merkezIso = izoYap(cMermi.getPosition().x, cMermi.getPosition().y); cMermi.setPosition({merkezIso.x, merkezIso.y - 25.0f}); window.draw(cMermi);
        }

        for (auto& mermi : dusmanMermileri) {
            sf::Sprite cMermi = mermi.sprite; sf::FloatRect bounds = cMermi.getLocalBounds(); cMermi.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            sf::Vector2f merkezIso = izoYap(cMermi.getPosition().x, cMermi.getPosition().y); cMermi.setPosition({merkezIso.x, merkezIso.y - 25.0f}); window.draw(cMermi);
        }
        
        // YENİ: Kılıç kesme animasyonlarının çizimi
        for (auto& slash : slashEfektleri) {
            sf::Sprite cSlash = slash.sprite; 
            sf::Vector2f merkezIso = izoYap(cSlash.getPosition().x, cSlash.getPosition().y); 
            cSlash.setPosition({merkezIso.x, merkezIso.y - 10.0f}); 
            window.draw(cSlash);
        }

        for (auto& patlama : patlamalar) {
            sf::Vector2f gercekPos = patlama.sprite.getPosition();
            sf::Vector2f merkezIso = izoYap(gercekPos.x, gercekPos.y);
            
            sf::Sprite geciciSprite = patlama.sprite;
            geciciSprite.setPosition({merkezIso.x, merkezIso.y - 20.0f}); 
            window.draw(geciciSprite);
        }

        for (auto& dusman : dusmanlar) {
            if (dusman.id == seciliDusmanId) {
                sf::Vector2f gercekPos = dusman.sprite.getPosition(); sf::Vector2f merkezIso = izoYap(gercekPos.x + dusman.genislik / 2.0f, gercekPos.y + dusman.yukseklik / 2.0f);
                sf::FloatRect bounds = dusman.sprite.getLocalBounds(); dusman.cizCanBariIzometrik(window, merkezIso, bounds.size.y * 0.9f);
            }
        }

        for (auto& hm : hasarMetinleri) window.draw(hm.text);

        if (hoverDusmanId != -1) {
            for (const auto& d : dusmanlar) {
                if (d.id == hoverDusmanId) {
                    sf::Text dusmanIsim(font); 
                    if (d.tip == 1) dusmanIsim.setString("Okcu Canavari");
                    else if (d.tip == 2) dusmanIsim.setString("Sisin Efendisi");
                    else dusmanIsim.setString("Orman Canavari");
                    
                    dusmanIsim.setCharacterSize(16); dusmanIsim.setFillColor(sf::Color::Red);
                    dusmanIsim.setOutlineColor(sf::Color::Black); dusmanIsim.setOutlineThickness(2.0f);
                    sf::Vector2f dMerkez = d.sprite.getPosition() + sf::Vector2f(d.genislik / 2.0f, d.yukseklik / 2.0f);
                    sf::Vector2f isoPos = izoYap(dMerkez.x, dMerkez.y); dusmanIsim.setPosition({isoPos.x - 40.0f, isoPos.y - 85.0f}); window.draw(dusmanIsim); break;
                }
            }
        }
        
        if (hoverEsyaIndeks != -1 && hoverEsyaIndeks < yerdekiEsyalar.size()) {
            sf::Text esyaIsim(font); esyaIsim.setString(yerdekiEsyalar[hoverEsyaIndeks].esya.isim);
            esyaIsim.setCharacterSize(16); esyaIsim.setFillColor(sf::Color::White); esyaIsim.setOutlineColor(sf::Color::Black); esyaIsim.setOutlineThickness(2.0f);
            sf::Vector2f isoPos = izoYap(yerdekiEsyalar[hoverEsyaIndeks].gercekPos.x, yerdekiEsyalar[hoverEsyaIndeks].gercekPos.y);
            esyaIsim.setPosition({isoPos.x - 30.0f, isoPos.y - 45.0f}); window.draw(esyaIsim);
        }

        if (dede.sprite.getGlobalBounds().contains(fareDunyaCart)) {
            sf::Text npcIsim(font); npcIsim.setString(dede.isim); npcIsim.setCharacterSize(18); npcIsim.setFillColor(sf::Color::Green);
            npcIsim.setOutlineColor(sf::Color::Black); npcIsim.setOutlineThickness(2.0f); sf::Vector2f isoPos = izoYap(dede.sprite.getPosition().x, dede.sprite.getPosition().y);
            npcIsim.setPosition({isoPos.x + 12.0f, isoPos.y - 20.0f}); window.draw(npcIsim);
        }

        fogSprite.setPosition(oyuncuIsoPos);
        window.draw(fogSprite);

        window.setView(window.getDefaultView());
        
        int alpha = 0;
        if (gununSaati >= 18) alpha = (gununSaati - 18) * 35; 
        else if (gununSaati <= 6) alpha = 210 - (gununSaati * 35); 
        
        if (alpha < 0) alpha = 0;
        if (alpha > 210) alpha = 210; 

        if (alpha > 0) {
            sf::RectangleShape geceKatmani({1600.0f, 900.0f});
            geceKatmani.setFillColor(sf::Color(15, 15, 35, alpha));
            window.draw(geceKatmani);
        }

        ui.cizSabitHUD(window, uiDokusu, font, oyuncu);
        
        if (!ui.duzenlemeModu && !menuAcikMi && !envanterAcikMi && !ayarlarAcikMi && !haritaAcikMi) {
            char debugBilgi[128];
            snprintf(debugBilgi, sizeof(debugBilgi), "FPS: %d\nHarita: %dx%d\nKoor: X:%d Y:%d", 
                     fpsGosterge, MAP_WIDTH, MAP_HEIGHT, 
                     static_cast<int>(oyuncuMerkez.x / tileSize), 
                     static_cast<int>(oyuncuMerkez.y / tileSize));
            
            sf::Text txtDebug(font);
            txtDebug.setString(debugBilgi);
            txtDebug.setCharacterSize(16);
            txtDebug.setFillColor(sf::Color(200, 255, 200));
            txtDebug.setOutlineColor(sf::Color::Black);
            txtDebug.setOutlineThickness(2.0f);
            
            sf::FloatRect textBounds = txtDebug.getLocalBounds();
            txtDebug.setPosition({1580.0f - textBounds.size.x, 20.0f});
            window.draw(txtDebug);
        }
        
        if (!haritaAcikMi && !ayarlarAcikMi) {
            ui.cizMiniMap(window, oyuncuMerkez, dusmanlar, inler, dede.sprite.getPosition());
            ui.cizAksiyonBari(window, font, oyuncu, mermiDokusu, atilmaDokusu, ekstraAtisDokusu, patlayanOkDokusu, farePencerePosUI);
        }
        
        if (ui.duzenlemeModu) {
            ui.cizDuzenlemeModu(window, font);
        }
        else if (menuAcikMi) ui.cizKarakterEkrani(window, font, oyuncu, idleSheet.texture, karakterSekmesi, solTikBasildi, solTikBirakildi, farePencerePosUI, mermiDokusu, atilmaDokusu, ekstraAtisDokusu, patlayanOkDokusu);
        else if (envanterAcikMi) ui.cizEnvanterEkrani(window, font, oyuncu, idleSheet.texture, farePencerePosUI, solTikBasildi, solTikBirakildi, sagTikBasildi);
        else if (gorevAcikMi) ui.cizGorevEkrani(window, font, gorevYoneticisi);
        else if (haritaAcikMi) ui.cizHaritaEkrani(window, font, oyuncu.sprite.getPosition());
        else if (ayarlarAcikMi) ui.cizAyarlarEkrani(window, font, ayarlarAltMenu, oyunSesSeviyesi, klavyeMetin);
        
        if (ui.surukleniyorMu && ui.kaynakTip != 3) {
            sf::Sprite ikon(ui.suruklenenEsya.doku);
            float sc = std::min((45.0f * 0.8f) / ikon.getLocalBounds().size.x, (45.0f * 0.8f) / ikon.getLocalBounds().size.y);
            ikon.setScale({sc, sc}); ikon.setOrigin({ikon.getLocalBounds().size.x / 2.0f, ikon.getLocalBounds().size.y / 2.0f});
            ikon.setPosition(farePencerePosUI); window.draw(ikon);
        }
        
        float bildirimY = 820.0f;
        for (int i = sistemMesajlari.size() - 1; i >= 0; i--) {
            sf::Text bText(font); bText.setString(sistemMesajlari[i].metin); bText.setCharacterSize(22); bText.setFillColor(sf::Color(255, 215, 0));
            bText.setOutlineColor(sf::Color(0, 0, 0)); bText.setOutlineThickness(2.0f);
            sf::Color c = bText.getFillColor(); sf::Color oc = bText.getOutlineColor();
            c.a = static_cast<std::uint8_t>(255.0f * std::max(0.0f, std::min(1.0f, sistemMesajlari[i].omur))); oc.a = c.a;
            bText.setFillColor(c); bText.setOutlineColor(oc); bText.setPosition({30.0f, bildirimY}); window.draw(bText);
            
            bildirimY -= 30.0f; sistemMesajlari[i].omur -= deltaZaman;
            if(sistemMesajlari[i].omur <= 0.0f) sistemMesajlari.erase(sistemMesajlari.begin() + i);
        }
        
        if (dede.etkilesimVar && !menuAcikMi && !envanterAcikMi && !ayarlarAcikMi) { window.setView(window.getDefaultView()); dede.diyalogGoster(window, font); }
        window.display();
    }

    return 0;
}