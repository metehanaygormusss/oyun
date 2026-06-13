#pragma once
#include <string>
#include <vector>

struct Gorev {
    int id;
    std::string baslik;
    std::string aciklama;
    bool tamamlandi;
    int hedefSayi;
    int anlikSayi;
    bool odulAlindi;
};

class GorevYoneticisi {
public:
    std::vector<Gorev> aktifGorevler;
    int seciliGorevIndeks = 0;

    void gorevEkle(int id, const std::string& baslik, const std::string& aciklama, int hedefSayi) {
        for (const auto& g : aktifGorevler) {
            if (g.id == id) return; 
        }
        aktifGorevler.push_back({id, baslik, aciklama, false, hedefSayi, 0, false});
    }

    void ilerlemeGuncelle(int id, int miktar) {
        for (auto& g : aktifGorevler) {
            if (g.id == id && !g.tamamlandi) {
                g.anlikSayi += miktar;
                if (g.anlikSayi >= g.hedefSayi) {
                    g.anlikSayi = g.hedefSayi;
                    g.tamamlandi = true;
                }
            }
        }
    }

    void tiklamaKontrol(float fareX, float fareY) {
        if (fareX >= 320.0f && fareX <= 540.0f) {
            if (fareY >= 260.0f) { // ÇÖZÜM: Y ekseni koordinatı 340'dan 260'a düzeltildi
                int indeks = static_cast<int>((fareY - 260.0f) / 40.0f);
                if (indeks >= 0 && indeks < static_cast<int>(aktifGorevler.size())) {
                    seciliGorevIndeks = indeks;
                }
            }
        }
    }
};