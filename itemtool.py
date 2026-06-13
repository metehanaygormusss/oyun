import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import os
import shutil

class ItemEditor:
    def __init__(self, root):
        self.root = root
        self.root.title("RPG Eşya Oluşturucu Araç")
        self.root.geometry("400x550")

        # Zorunlu Yuva Seçenekleri (Sadece seçilebilir, yazılamaz)
        self.turler = ["Kask Yuvasi", "Aksesuar Yuvasi", "Silah Yuvasi", "Zirh Yuvasi", "Ikinci El Yuvasi", "Cizme Yuvasi"]
        
        # Değişkenler
        self.isim_var = tk.StringVar()
        self.tur_var = tk.StringVar(value=self.turler[2]) # Varsayılan: Silah
        self.resim_yolu = ""
        
        self.can_var = tk.IntVar(value=0)
        self.saldiri_var = tk.IntVar(value=0)
        self.zirh_var = tk.IntVar(value=0)
        self.mana_var = tk.IntVar(value=0)

        self.create_widgets()

    def create_widgets(self):
        tk.Label(self.root, text="Eşya İsmi:", font=("Arial", 10, "bold")).pack(pady=5)
        tk.Entry(self.root, textvariable=self.isim_var, width=30).pack()

        tk.Label(self.root, text="Kuşanılacak Yuva (Zorunlu):", font=("Arial", 10, "bold")).pack(pady=5)
        ttk.Combobox(self.root, textvariable=self.tur_var, values=self.turler, state="readonly", width=27).pack()

        self.resim_btn = tk.Button(self.root, text="Görsel Seç (PNG / JPG)", command=self.resim_sec)
        self.resim_btn.pack(pady=10)
        self.resim_lbl = tk.Label(self.root, text="Görsel seçilmedi", fg="red")
        self.resim_lbl.pack()

        frame = tk.Frame(self.root)
        frame.pack(pady=20)
        
        tk.Label(frame, text="Ekstra Can:").grid(row=0, column=0, pady=5, sticky="e")
        tk.Spinbox(frame, from_=0, to=5000, textvariable=self.can_var, width=10).grid(row=0, column=1, pady=5)

        tk.Label(frame, text="Saldırı Gücü:").grid(row=1, column=0, pady=5, sticky="e")
        tk.Spinbox(frame, from_=0, to=5000, textvariable=self.saldiri_var, width=10).grid(row=1, column=1, pady=5)

        tk.Label(frame, text="Zırh Savunması:").grid(row=2, column=0, pady=5, sticky="e")
        tk.Spinbox(frame, from_=0, to=5000, textvariable=self.zirh_var, width=10).grid(row=2, column=1, pady=5)

        tk.Label(frame, text="Ekstra Mana:").grid(row=3, column=0, pady=5, sticky="e")
        tk.Spinbox(frame, from_=0, to=5000, textvariable=self.mana_var, width=10).grid(row=3, column=1, pady=5)

        tk.Button(self.root, text="İtemi Veritabanına Ekle", command=self.kaydet, bg="green", fg="black", font=("Arial", 12, "bold")).pack(pady=20)

    def resim_sec(self):
        dosya = filedialog.askopenfilename(filetypes=[("Resim Dosyaları", "*.png *.jpg")])
        if dosya:
            self.resim_yolu = dosya
            self.resim_lbl.config(text=os.path.basename(dosya), fg="green")

    def kaydet(self):
        isim = self.isim_var.get()
        tur = self.tur_var.get()
        if not isim or not self.resim_yolu:
            messagebox.showerror("Hata", "İsim ve Görsel seçmek zorunludur!")
            return
        
        hedef_klasor = "assets/items"
        os.makedirs(hedef_klasor, exist_ok=True)
        hedef_yol = os.path.join(hedef_klasor, os.path.basename(self.resim_yolu))
        
        # Seçilen resmi otomatik olarak oyunun assets/items klasörüne kopyala
        try:
            shutil.copy(self.resim_yolu, hedef_yol)
        except shutil.SameFileError:
            pass

        hedef_yol = hedef_yol.replace("\\", "/") # C++ uyumluluğu için Slash çevirimi

        # C++ motorunun okuyacağı formata yazdır
        with open("esya_veritabani.txt", "a", encoding="utf-8") as f:
            f.write(f"{isim};{tur};{hedef_yol};{self.can_var.get()};{self.saldiri_var.get()};{self.zirh_var.get()};{self.mana_var.get()}\n")

        messagebox.showinfo("Başarılı", f"'{isim}' başarıyla oyuna eklendi!")
        self.sifirla()

    def sifirla(self):
        self.isim_var.set("")
        self.resim_yolu = ""
        self.resim_lbl.config(text="Görsel seçilmedi", fg="red")
        self.can_var.set(0)
        self.saldiri_var.set(0)
        self.zirh_var.set(0)
        self.mana_var.set(0)

if __name__ == "__main__":
    root = tk.Tk()
    app = ItemEditor(root)
    root.mainloop()