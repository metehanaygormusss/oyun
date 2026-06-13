import tkinter as tk
from tkinter import messagebox
import random
import re
import os

# Harita Sabitleri
COLS = 160
ROWS = 120
CELL_SIZE = 8 

# Palet: Zemin Türleri ve Varlıklar
PALET = {
    0: {"isim": "Cimen (Varsayilan)", "renk": "#7CFC00"},
    1: {"isim": "Duvar", "renk": "#808080"},
    2: {"isim": "Agac", "renk": "#006400"},
    3: {"isim": "Nehir", "renk": "#1E90FF"},
    4: {"isim": "Kopru", "renk": "#8B4513"},
    5: {"isim": "IN (Spawner)", "renk": "#FF0000"},
    7: {"isim": "Dusman (Anlik)", "renk": "#FF8C00"}, 
    6: {"isim": "NPC (Dede)", "renk": "#FFD700"},
    -1: {"isim": "SILGI (Cimen Yapar)", "renk": "#FFFFFF"} 
}

class HaritaYaratici(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Metoland - Otomatik Harita ve Obje Yaratici")
        self.geometry("1400x850")
        
        self.harita_verisi = [[0 for _ in range(COLS)] for _ in range(ROWS)]
        self.inler = []
        self.npcler = []
        self.dusmanlar = [] 
        
        self.secili_firca = tk.IntVar(value=1) 
        self.firca_boyutu = tk.IntVar(value=1) 

        # Canlı Sayaç Değişkenleri
        self.in_sayisi_var = tk.StringVar(value="IN (Spawner): 0")
        self.dusman_sayisi_var = tk.StringVar(value="Dusman: 0")
        self.npc_sayisi_var = tk.StringVar(value="Dede: 0 / 1")
        
        self.arayuz_olustur()
        
    def arayuz_olustur(self):
        sol_panel = tk.Frame(self, width=280, bg="#2b2b2b")
        sol_panel.pack(side=tk.LEFT, fill=tk.Y)
        
        tk.Label(sol_panel, text="FIRCA BOYUTU", fg="yellow", bg="#2b2b2b", font=("Arial", 12, "bold")).pack(pady=(10, 5))
        boyut_frame = tk.Frame(sol_panel, bg="#2b2b2b")
        boyut_frame.pack(pady=5)
        
        tk.Radiobutton(boyut_frame, text="1x1", variable=self.firca_boyutu, value=1, bg="#444", fg="white", selectcolor="#000", indicatoron=0, width=5).pack(side=tk.LEFT, padx=2)
        tk.Radiobutton(boyut_frame, text="3x3", variable=self.firca_boyutu, value=3, bg="#444", fg="white", selectcolor="#000", indicatoron=0, width=5).pack(side=tk.LEFT, padx=2)
        tk.Radiobutton(boyut_frame, text="5x5", variable=self.firca_boyutu, value=5, bg="#444", fg="white", selectcolor="#000", indicatoron=0, width=5).pack(side=tk.LEFT, padx=2)

        tk.Label(sol_panel, text="FIRCA PALETI", fg="white", bg="#2b2b2b", font=("Arial", 12, "bold")).pack(pady=(20, 10))
        
        for k, v in PALET.items():
            btn = tk.Radiobutton(sol_panel, text=v["isim"], variable=self.secili_firca, value=k, 
                                 bg=v["renk"], fg="black", selectcolor="#FFFFFF", indicatoron=0, 
                                 width=22, height=2, font=("Arial", 10, "bold"))
            if k == -1: btn.config(fg="red", bg="#E0E0E0") 
            btn.pack(pady=3, padx=10)

        # Bilgi Paneli (Canlı Sayaçlar)
        bilgi_frame = tk.Frame(sol_panel, bg="#1a1a1a", bd=2, relief=tk.SUNKEN)
        bilgi_frame.pack(pady=15, padx=10, fill=tk.X)
        tk.Label(bilgi_frame, textvariable=self.in_sayisi_var, fg="#FF0000", bg="#1a1a1a", font=("Arial", 11, "bold")).pack(anchor=tk.W, padx=5, pady=2)
        tk.Label(bilgi_frame, textvariable=self.dusman_sayisi_var, fg="#FF8C00", bg="#1a1a1a", font=("Arial", 11, "bold")).pack(anchor=tk.W, padx=5, pady=2)
        tk.Label(bilgi_frame, textvariable=self.npc_sayisi_var, fg="#FFD700", bg="#1a1a1a", font=("Arial", 11, "bold")).pack(anchor=tk.W, padx=5, pady=2)
            
        tk.Button(sol_panel, text="🎲 RASTGELE URET (SEED)", command=self.rastgele_uret, bg="#8A2BE2", fg="white", font=("Arial", 11, "bold"), height=2).pack(pady=(10, 5), fill=tk.X, padx=10)
        tk.Button(sol_panel, text="Temizle (Hepsi Cimen)", command=self.temizle, bg="#cc0000", fg="white").pack(pady=5, fill=tk.X, padx=10)
        tk.Button(sol_panel, text="🚀 OYUNA OTOMATIK ISLE", command=self.disa_aktar, bg="#00cc00", fg="white", font=("Arial", 12, "bold"), height=3).pack(pady=(15, 10), fill=tk.X, padx=10)
        
        sag_panel = tk.Frame(self)
        sag_panel.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        
        vbar = tk.Scrollbar(sag_panel, orient=tk.VERTICAL)
        hbar = tk.Scrollbar(sag_panel, orient=tk.HORIZONTAL)
        vbar.pack(side=tk.RIGHT, fill=tk.Y)
        hbar.pack(side=tk.BOTTOM, fill=tk.X)
        
        self.canvas = tk.Canvas(sag_panel, bg="black", width=1200, height=800, 
                                scrollregion=(0, 0, COLS * CELL_SIZE, ROWS * CELL_SIZE),
                                xscrollcommand=hbar.set, yscrollcommand=vbar.set)
        self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        hbar.config(command=self.canvas.xview)
        vbar.config(command=self.canvas.yview)
        
        self.canvas.bind("<B1-Motion>", self.boya)
        self.canvas.bind("<Button-1>", self.boya)
        
        self.ekrani_ciz()

    def sayaclari_guncelle(self):
        self.in_sayisi_var.set(f"IN (Spawner): {len(self.inler)}")
        self.dusman_sayisi_var.set(f"Dusman: {len(self.dusmanlar)}")
        self.npc_sayisi_var.set(f"Dede: {len(self.npcler)} / 1")

    def ekrani_ciz(self):
        self.canvas.delete("all")
        for y in range(ROWS):
            for x in range(COLS):
                renk = PALET[self.harita_verisi[y][x]]["renk"]
                x0, y0 = x * CELL_SIZE, y * CELL_SIZE
                self.canvas.create_rectangle(x0, y0, x0 + CELL_SIZE, y0 + CELL_SIZE, fill=renk, outline="#444", tags=f"hucre_{x}_{y}")
                
        for i_x, i_y in self.inler:
            x0, y0 = i_x * CELL_SIZE, i_y * CELL_SIZE
            self.canvas.create_oval(x0, y0, x0 + CELL_SIZE, y0 + CELL_SIZE, fill=PALET[5]["renk"], tags="obje")
        for d_x, d_y in self.dusmanlar:
            x0, y0 = d_x * CELL_SIZE, d_y * CELL_SIZE
            self.canvas.create_oval(x0, y0, x0 + CELL_SIZE, y0 + CELL_SIZE, fill=PALET[7]["renk"], tags="obje")
        for n_x, n_y in self.npcler:
            x0, y0 = n_x * CELL_SIZE, n_y * CELL_SIZE
            self.canvas.create_oval(x0, y0, x0 + CELL_SIZE, y0 + CELL_SIZE, fill=PALET[6]["renk"], tags="obje")
            
        self.sayaclari_guncelle()

    def boya(self, event):
        merkez_x = int(self.canvas.canvasx(event.x) // CELL_SIZE)
        merkez_y = int(self.canvas.canvasy(event.y) // CELL_SIZE)
        firca = self.secili_firca.get()
        boyut = self.firca_boyutu.get()
        offset = boyut // 2
        
        for dy in range(-offset, offset + 1):
            for dx in range(-offset, offset + 1):
                x = merkez_x + dx
                y = merkez_y + dy
                
                if 0 <= x < COLS and 0 <= y < ROWS:
                    if firca == -1: 
                        self.harita_verisi[y][x] = 0 
                        self.canvas.itemconfig(f"hucre_{x}_{y}", fill=PALET[0]["renk"])
                        if (x, y) in self.inler: self.inler.remove((x, y)); self.ekrani_ciz()
                        if (x, y) in self.dusmanlar: self.dusmanlar.remove((x, y)); self.ekrani_ciz()
                        if (x, y) in self.npcler: self.npcler.remove((x, y)); self.ekrani_ciz()
                            
                    elif firca <= 4: 
                        self.harita_verisi[y][x] = firca
                        self.canvas.itemconfig(f"hucre_{x}_{y}", fill=PALET[firca]["renk"])
                        
                    elif firca == 5: 
                        if dx == 0 and dy == 0 and (x, y) not in self.inler:
                            self.inler.append((x, y)); self.ekrani_ciz()
                    elif firca == 7: 
                        if dx == 0 and dy == 0 and (x, y) not in self.dusmanlar:
                            self.dusmanlar.append((x, y)); self.ekrani_ciz()
                    elif firca == 6: 
                        if dx == 0 and dy == 0:
                            # LIMIT: Sadece 1 Dede olabilir, eski Dede'yi silip yeni yere koyar.
                            self.npcler = [(x, y)]
                            self.ekrani_ciz()

    def temizle(self):
        self.harita_verisi = [[0 for _ in range(COLS)] for _ in range(ROWS)]
        self.inler = []
        self.dusmanlar = []
        self.npcler = []
        self.ekrani_ciz()

    def rastgele_uret(self):
        self.temizle()
        
        for _ in range(random.randint(20, 40)):
            cx, cy = random.randint(5, COLS-5), random.randint(5, ROWS-5)
            for _ in range(random.randint(20, 150)):
                x, y = cx + random.randint(-8, 8), cy + random.randint(-8, 8)
                if 0 <= x < COLS and 0 <= y < ROWS: self.harita_verisi[y][x] = 2 
        
        for _ in range(random.randint(1, 3)):
            x, y = random.randint(0, COLS-1), 0
            while y < ROWS:
                for dx in range(-2, 3):
                    nx = x + dx
                    if 0 <= nx < COLS: self.harita_verisi[y][nx] = 3
                y += 1
                x += random.choice([-1, 0, 1])
                
        for _ in range(random.randint(10, 25)):
            x, y = random.randint(10, COLS-10), random.randint(10, ROWS-10)
            for dx in range(random.randint(3, 8)):
                if 0 <= x+dx < COLS and self.harita_verisi[y][x+dx] == 0: self.harita_verisi[y][x+dx] = 1

        for _ in range(random.randint(3, 6)):
            while True:
                ix, iy = random.randint(10, COLS-10), random.randint(10, ROWS-10)
                if self.harita_verisi[iy][ix] == 0:
                    self.inler.append((ix, iy))
                    break
                    
        for _ in range(random.randint(10, 20)):
            while True:
                dx, dy = random.randint(10, COLS-10), random.randint(10, ROWS-10)
                if self.harita_verisi[dy][dx] == 0:
                    self.dusmanlar.append((dx, dy))
                    break

        while True:
            nx, ny = random.randint(10, 40), random.randint(10, 40)
            if self.harita_verisi[ny][nx] == 0:
                self.npcler = [(nx, ny)]
                break
                
        self.ekrani_ciz()

    def disa_aktar(self):
        try:
            # Harita.hpp Dosyasını Oluşturur ve EKSİK OLAN C++ ÇARPIŞMA FONKSİYONUNU İÇİNE YERLEŞTİRİR
            with open("Harita.hpp", "w", encoding="utf-8") as f:
                f.write("#pragma once\n#include \"Sabitler.hpp\"\n\n")
                f.write("inline int harita[MAP_HEIGHT][MAP_WIDTH] = {\n")
                for y in range(ROWS):
                    satir_str = ",".join(map(str, self.harita_verisi[y]))
                    f.write(f"    {{{satir_str}}}")
                    if y < ROWS - 1: f.write(",\n")
                    else: f.write("\n")
                f.write("};\n\n")
                
                # C++ Motoru İçin Hata Veren Çarpışma Fonksiyonunu Otomatik Ekler
                f.write("inline bool haritaCarpismaKontrolu(float x, float y, float genislik, float yukseklik) {\n")
                f.write("    int solGrid = static_cast<int>(x / tileSize);\n")
                f.write("    int sagGrid = static_cast<int>((x + genislik) / tileSize);\n")
                f.write("    int ustGrid = static_cast<int>(y / tileSize);\n")
                f.write("    int altGrid = static_cast<int>((y + yukseklik) / tileSize);\n\n")
                f.write("    if (solGrid < 0 || sagGrid >= MAP_WIDTH || ustGrid < 0 || altGrid >= MAP_HEIGHT) return true;\n\n")
                f.write("    for (int gy = ustGrid; gy <= altGrid; ++gy) {\n")
                f.write("        for (int gx = solGrid; gx <= sagGrid; ++gx) {\n")
                f.write("            int tile = harita[gy][gx];\n")
                f.write("            if (tile == 1 || tile == 2 || tile == 3) { return true; }\n")
                f.write("        }\n")
                f.write("    }\n")
                f.write("    return false;\n")
                f.write("}\n")
                
            if os.path.exists("main.cpp"):
                with open("main.cpp", "r", encoding="utf-8") as f:
                    code = f.read()
                
                if self.npcler:
                    nx, ny = self.npcler[0]
                    npc_x, npc_y = nx * 40.0, ny * 40.0
                    code = re.sub(r'NPC dede\([0-9.]+f,\s*[0-9.]+f,\s*dedeDokusu\);', f'NPC dede({npc_x}f, {npc_y}f, dedeDokusu);', code)
                
                obj_kodu = ""
                for ix, iy in self.inler:
                    obj_kodu += f"    inler.emplace_back(nextInId++, {ix * 40.0}f, {iy * 40.0}f, inDokusu);\n"
                for i in range(len(self.inler)):
                    obj_kodu += f"    inler[{i}].spawnSayaci = 10.0f;\n"
                obj_kodu += "\n"
                for dx, dy in self.dusmanlar:
                    obj_kodu += f"    dusmanlar.emplace_back(nextDusmanId++, {dx * 40.0}f, {dy * 40.0}f, dusmanDokusu);\n"
                
                pattern = r'(int nextDusmanId = 1, nextInId = 1, seciliDusmanId = -1;\s*).*?(std::vector<sf::Sprite> statikDuvarDeposu)'
                yeni_blok = r'\1\n' + obj_kodu + r'\n    \2'
                
                yeni_code = re.sub(pattern, yeni_blok, code, flags=re.DOTALL)
                
                if yeni_code != code:
                    with open("main.cpp", "w", encoding="utf-8") as f:
                        f.write(yeni_code)
                    messagebox.showinfo("Basarili!", "Harita 'Harita.hpp' icine, Dusman/In/NPC konumlari ise dogrudan 'main.cpp' icine islendi!\n\nHicbir kopyala-yapistira gerek yok, direkt C++ derleyip oyuna girebilirsin.")
                else:
                    messagebox.showwarning("Uyari", "Harita degistirildi fakat main.cpp icindeki dusman satirlari bulunamadi.")
            else:
                messagebox.showerror("Hata", "main.cpp dosyasi bulunamadi! Lutfen bu Python dosyasini oyunun klasorunde calistirin.")
                
        except Exception as e:
            messagebox.showerror("Hata", f"Kodu islerken hata olustu: {e}")

if __name__ == "__main__":
    app = HaritaYaratici()
    app.mainloop()