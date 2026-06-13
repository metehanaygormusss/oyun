#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <cmath>

class NPC {
public:
    sf::Sprite sprite;
    std::string isim = "Ak Sakalli Dede";
    
    int gorevAsamasi = 1;      // Hangi görevdeyiz (1'den 5'e kadar)
    int durum = 0;             // 0: Görev Veriliyor, 1: Görev Devam Ediyor, 2: Görev Bitti (Teslim)
    
    std::vector<std::string> aktifDiyalog; 
    int diyalogSayfasi = 0;    // Ekranda gösterilen mevcut cümle indeksi
    bool secimEkraniMi = false; 
    bool etkilesimVar = false;

    NPC(float x, float y, const sf::Texture& doku) : sprite(doku) {
        sprite.setPosition({x, y});
        sprite.setScale({1.5f, 1.5f});
        diyaloglariYukle();
    }

    // Görev aşamasına ve duruma göre Dedenin söyleyeceği cümle dizilerini hazırlar
    // Görev aşamasına ve duruma göre Dedenin söyleyeceği cümle dizilerini hazırlar
    void diyaloglariYukle() {
        aktifDiyalog.clear();
        diyalogSayfasi = 0;
        secimEkraniMi = false;

        if (gorevAsamasi == 1) {
            if (durum == 0) {
                aktifDiyalog.push_back("Ah... Bu lanet olasi nem de dizlerimi sizlatiyor...\nHa? Kim var orada?");
                aktifDiyalog.push_back("Meto: Benim, Meto. Buralar biraz karismis gibi,\niyi misin ihtiyar?");
                aktifDiyalog.push_back("Ben sadece yasli bir adamim evlat, asil orman iyi degil.\nGeceleri su topraktan mor bir sis tutmeye basladigindan\nberi hayvanlar delirdi.");
                aktifDiyalog.push_back("Uyku haram oldu bana. Eger sirtindaki o yayi sadece sus\ndiye tasimiyorsan, gidip su yakindaki 2 canavari\ntemizle de bu ihtiyar biraz rahat etsin.");
            } else if (durum == 1) {
                aktifDiyalog.push_back("Hala buradasin? Git ve su canavarlari temizle evlat.");
            } else if (durum == 2) {
                aktifDiyalog.push_back("Guzel is cikardin. Al bakalim, bu altinlar senin.");
            }
        }
        else if (gorevAsamasi == 2) {
            if (durum == 0) {
                aktifDiyalog.push_back("Fena degil, fena degil... Genclik iste, nasil da enerjiksin.\nAma ustun les gibi kokuyor evlat, benden bir adim\nuzakta dur.");
                aktifDiyalog.push_back("Meto: Canavarlarin kani yuzunden.\nBu sisin kaynagi nedir?");
                aktifDiyalog.push_back("Mesele de o ya. Havadan degil, sudan geliyor gibi.\nNehrin otesindeki sularin rengi zifte dondu.");
                aktifDiyalog.push_back("Gidip o tas kopruyu gec ve karsi kiyiyi bir kolacan et.\nAma adimlarina dikkat et, o kopruden gecenlerin\ncogu sadece masallarda geri dondu.");
            } else if (durum == 1) {
                aktifDiyalog.push_back("Koprunun otesini merakla bekliyorum. Dikkatli ol.");
            } else if (durum == 2) {
                aktifDiyalog.push_back("Sag salim dondun demek! Al bu cizmeler yollarini\nhizlandirsin.");
            }
        }
        else if (gorevAsamasi == 3) {
            if (durum == 0) {
                aktifDiyalog.push_back("Gozlerim beni yaniltmiyorsa saglam dondun. Yoksa\nhayalet misin? Geriye dogru uc adim at bakayim...\nSaka saka. Ne gordun orada?");
                aktifDiyalog.push_back("Meto: Buyuk bir yarik var, sis dogrudan oradan siziyor.");
                aktifDiyalog.push_back("Dreadmyst... Eskilerin anlattigi o dehset sisi\ngercekten donmus demek. Isimiz zor cocuk.");
                aktifDiyalog.push_back("Sisin dogasini cozmem icin bana bir ornek lazim.\nO yaratiklarin kanina bulanmis 'Yozlasmis Yurek'lerinden\n3 tane bul getir. Ellerin biraz kirlenecek.");
            } else if (durum == 1) {
                aktifDiyalog.push_back("Bana o lanet olasi yureklerden 3 tane lazim.\nEnvanterinde olmadan gelme evlat.");
            } else if (durum == 2) {
                aktifDiyalog.push_back("Oggk... Igrenc kokuyorlar. Sunlari suraya birak.\nMidem kalkti ihtiyar yasimda...");
            }
        }
        else if (gorevAsamasi == 4) {
            if (durum == 0) {
                aktifDiyalog.push_back("Yurekleri inceledim Meto. Durum kotu.\nUzerindeki o incecik kiyafetle sisin merkezine inersen,\nbes dakikaya sen de o canavarlara donusursun.");
                aktifDiyalog.push_back("Eski orman muhafizlarinin geride biraktigi\n'Kadim Orman Zirhi' parcalarini etrafa sacacagim.\nGit onu bul, envanterini (I) acip sag tikla ve kusan.\nSise anca oyle dayanabilirsin.");
            } else if (durum == 1) {
                aktifDiyalog.push_back("Kadim Orman Zirhini kusanmadan bir adim bile atma!\nEnvanterinden (I veya ;) sag tiklayarak kusanabilirsin.");
            } else if (durum == 2) {
                aktifDiyalog.push_back("Vay canina... Bu zirhi en son gordugumde\nsaclarim simsiyahti. Tam bir Korucu oldun.");
            }
        }
        else if (gorevAsamasi == 5) {
            if (durum == 0) {
                aktifDiyalog.push_back("Meto: Hazirim ihtiyar. Butun bunlari\nbaslatan seyi nerede bulacagim?");
                aktifDiyalog.push_back("Sisin kalbinde. Artik ormanda devasa bir golge dolasiyor.\nDreadmyst Komutani... Adimlari etrafi titretiyor\nnamussuzun.");
                aktifDiyalog.push_back("Yayini gergin, zihnini berrak tut. Coklu Atislarini\n(1 Tusu) bosa harcama, manana dikkat et.\nGit ve bu kabusu bitir evlat!");
            } else if (durum == 1) {
                aktifDiyalog.push_back("O devasa Dreadmyst Komutanini oldurmeden ormana\nhuzur gelmeyecek!");
            } else if (durum == 2) {
                aktifDiyalog.push_back("Inanilmaz! Gercekten basardin...\nBu Efsanevi Yay artik senin, Ormanin Kahramani!");
            }
        }
        else {
            aktifDiyalog.push_back("Su an baska isimiz yok evlat. Orman guvende.\n(Dreadmyst Demosu Bitti - Tebrikler!)");
        }
    }

    void sonrakiDiyalog() {
        if (diyalogSayfasi < aktifDiyalog.size() - 1) {
            diyalogSayfasi++;
        } else {
            // Son sayfaya geldiysek ve yeni görev veriliyorsa seçim ekranı açılır
            if (durum == 0) {
                secimEkraniMi = true;
            } else {
                // ÇÖZÜM: Eğer görev devam ediyorsa (durum == 1), son sayfada sohbeti tamamen kapat!
                etkilesimVar = false;
            }
        }
    }

    bool yakindaMi(sf::Vector2f oyuncuPos) {
        float dx = sprite.getPosition().x - oyuncuPos.x;
        float dy = sprite.getPosition().y - oyuncuPos.y;
        return std::sqrt(dx*dx + dy*dy) < 80.0f; 
    }

    void ciz(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    void diyalogGoster(sf::RenderWindow& window, sf::Font& font) {
        sf::RectangleShape arkaPlan({1000.0f, 200.0f});
        arkaPlan.setFillColor(sf::Color(20, 20, 25, 235)); 
        arkaPlan.setOutlineColor(sf::Color(170, 130, 30)); 
        arkaPlan.setOutlineThickness(4.0f);
        arkaPlan.setPosition({300.0f, 680.0f}); 
        window.draw(arkaPlan);

        // İsim etiketi (Eğer Meto konuşuyorsa rengi değişir)
        sf::Text isimText(font);
        if (aktifDiyalog[diyalogSayfasi].substr(0, 4) == "Meto") {
            isimText.setString("Meto:");
            isimText.setFillColor(sf::Color(100, 200, 255));
        } else {
            isimText.setString(isim + ":");
            isimText.setFillColor(sf::Color(240, 200, 40));
        }
        isimText.setCharacterSize(26);
        isimText.setPosition({330.0f, 695.0f}); 
        window.draw(isimText);

        // Metnin kendisi
        sf::Text diyalogText(font);
        std::string gosterilecekMetin = aktifDiyalog[diyalogSayfasi];
        
        // Eğer isim etiketi yazdırıldıysa, metnin içinden ismi temizle
        if (gosterilecekMetin.substr(0, 4) == "Meto") {
            gosterilecekMetin = gosterilecekMetin.substr(6);
        }
        
        diyalogText.setString(gosterilecekMetin);
        diyalogText.setCharacterSize(20);
        diyalogText.setFillColor(sf::Color(220, 220, 220)); 
        diyalogText.setPosition({330.0f, 740.0f}); 
        window.draw(diyalogText);

        // Devam veya Seçim Yönergesi
        sf::Text yoneriText(font);
        if (secimEkraniMi) {
            yoneriText.setString("[1] Gorevi Kabul Et");
            yoneriText.setFillColor(sf::Color::Green);
        } else if (durum == 2 && diyalogSayfasi == aktifDiyalog.size() - 1) {
            yoneriText.setString("[1] Odulu Al");
            yoneriText.setFillColor(sf::Color::Yellow);
        } else if (diyalogSayfasi < aktifDiyalog.size() - 1) {
            yoneriText.setString("[SPACE] Devam...");
            yoneriText.setFillColor(sf::Color(150, 150, 150));
        }
        yoneriText.setCharacterSize(18);
        yoneriText.setPosition({1100.0f, 840.0f});
        window.draw(yoneriText);
    }
};