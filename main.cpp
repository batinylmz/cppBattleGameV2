// =============================================================
//  MUHTESEM TANK vs ADAM - DENGELI GENIS SURUM
// =============================================================
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <cstdint>
#include <algorithm>
#include <iostream>


//bilgi: sf ses sprite pencere gibi ögeleri tanımlayan ad alanıdır ve bu sfml kutuphanesinde kullanılır.



// =============================================================
// SABITLER
// =============================================================
// =============================================================
// SABITLER (Genişletilmiş Devasa Arena)
// =============================================================
const float PI = 3.14159265f;
const float PENCERE_W = 2240.f; //
const float PENCERE_H = 1260.f;
const float ZEMIN_Y = 1150.f;   // Çimenin kalınlığını epeyce azalttık
const float TANK_BOLGE_MAX_X = PENCERE_W * 0.40f;
const float ADAM_BOLGE_MIN_X = PENCERE_W * 0.58f;  // Zemin seviyesini yeni derinliğe uyarladık


// enum class, değişkene sadece bizim belirlediğimiz kelimelerin atanabilmesini sağlar.
// Mesela "Sahip" türündeki bir değişken sadece TANK veya ADAM olabilir, "KUS" olamaz.
enum class Sahip { TANK, ADAM };
enum class SilahTipi { TOP, TABANCA, KELES, NISANCI, ROKET };
enum class GucTipi { CAN, TABANCA, KELES, NISANCI, ROKET };
// OYUNUN EKRAN DURUMLARI (STATE MACHINE)
enum class EkranDurumu { MENU, OYUN, BITTI };
// =============================================================
// KLAVYE TAKİP SİSTEMİ (Mac Güvenliğini Aşmak İçin)
// =============================================================
namespace Klavye {
    inline bool W = false, S = false, A = false, D = false;
    inline bool Yukari = false, Sag = false, Sol = false;
    inline bool O = false, L = false;
}

// =============================================================
// TEMEL SINIF
// =============================================================
class OyunNesnesi {
public:
    virtual ~OyunNesnesi() = default;
    virtual void guncelle() = 0;
    virtual void ciz(sf::RenderWindow& pencere) = 0;
    virtual sf::FloatRect getSinirlar() { return {}; }
    virtual bool silinecekMi() { return false; }
    virtual bool isMermi() { return false; }
    virtual Sahip getSahip() { return Sahip::TANK; }
};

// =============================================================
// PARTIKUL
// =============================================================
class Partikul : public OyunNesnesi {
private:
    sf::CircleShape sekil;
    float hizX = 0.f;
    float hizY = 0.f;
    int omur = 0;
    int maxOmur = 0;

public:
    Partikul(float x, float y, sf::Color renk, float sacilma = 10.f) {
        sekil.setRadius(6.f + static_cast<float>(std::rand() % 9));
        sekil.setOrigin({sekil.getRadius(), sekil.getRadius()});
        sekil.setPosition({x, y});
        sekil.setFillColor(renk);

        hizX = (static_cast<float>(std::rand() % static_cast<int>(sacilma * 2.f)) - sacilma) * 0.9f;
        hizY = (static_cast<float>(std::rand() % static_cast<int>(sacilma * 2.f)) - sacilma) * 0.9f;
        maxOmur = 20 + (std::rand() % 16);
        omur = maxOmur;
    }

    void guncelle() override {
        sekil.move({hizX, hizY});
        hizY += 0.18f;
        omur--;

        sf::Color renk = sekil.getFillColor();
        renk.a = static_cast<std::uint8_t>((255.f * static_cast<float>(omur)) / static_cast<float>(maxOmur));
        sekil.setFillColor(renk);
    }

    void ciz(sf::RenderWindow& pencere) override { pencere.draw(sekil); }
    bool silinecekMi() override { return omur <= 0; }
};

// =============================================================
// MERMI
// =============================================================
// =============================================================
// MERMI (Karakteristik Silah Fizikleri)
// =============================================================
// =============================================================
// MERMI (Özel Fizikler ve Mermi Türleri)
// =============================================================
// =============================================================
// MERMI (Özel Fizikler ve Mermi Türleri)
// =============================================================
// =============================================================
// MERMI (Gerçekçi Şekiller ve Renkler)
// =============================================================

// =============================================================
// MERMI (Yeni Mimari - Sadece Fiziksel Özellikler Alır)
// =============================================================
class Mermi : public OyunNesnesi {
private:
    sf::CircleShape daireMermi;
    sf::RectangleShape lazerMermi;
    sf::ConvexShape gercekMermi;
    bool lazerMi = false;
    bool gercekMi = false;
    float hizX = 0.f;
    float hizY = 0.f;
    float yercekimi = 0.22f;
    Sahip kimeAit;

public:
    // YENİ YAPICI METOT: Artık "SilahTipi" sormuyor, direkt fiziği alıyor!
    Mermi(float x, float y, float aciDerece, float guc, float yercekimiAyar, sf::Color mermiRenk, bool lazer, bool gercek, Sahip sahip)
        : kimeAit(sahip), yercekimi(yercekimiAyar), lazerMi(lazer), gercekMi(gercek)
    {
        float yaricap = lazerMi ? 0.f : (gercekMi ? 0.f : (guc > 23.f ? 6.f : 5.f));

        if (gercekMi) {
            gercekMermi.setPointCount(5);
            gercekMermi.setPoint(0, {0.f, 0.f});
            gercekMermi.setPoint(1, {10.f, 0.f});
            gercekMermi.setPoint(2, {14.f, 2.5f});
            gercekMermi.setPoint(3, {10.f, 5.f});
            gercekMermi.setPoint(4, {0.f, 5.f});
            gercekMermi.setFillColor(mermiRenk);
            gercekMermi.setOrigin({7.f, 2.5f});
            gercekMermi.setPosition({x, y});
            gercekMermi.setRotation(sf::degrees(aciDerece));
        }
        else if (lazerMi) {
            if (mermiRenk == sf::Color::Red) {
                lazerMermi.setSize({16.f, 3.f});
                lazerMermi.setOrigin({8.f, 1.5f});
            } else {
                lazerMermi.setSize({30.f, 4.f});
                lazerMermi.setOrigin({15.f, 2.f});
            }
            lazerMermi.setPosition({x, y});
            lazerMermi.setFillColor(mermiRenk);
            lazerMermi.setRotation(sf::degrees(aciDerece));
        } else {
            daireMermi.setRadius(yaricap);
            daireMermi.setOrigin({yaricap, yaricap});
            daireMermi.setPosition({x, y});
            daireMermi.setFillColor(mermiRenk);
        }

        const float aciRadyan = aciDerece * PI / 180.f;
        hizX = guc * std::cos(aciRadyan);
        hizY = guc * std::sin(aciRadyan);
    }

    void guncelle() override {
        hizY += yercekimi;
        if (gercekMi) {
            gercekMermi.move({hizX, hizY});
            float ucusAcisi = std::atan2(hizY, hizX) * 180.f / PI;
            gercekMermi.setRotation(sf::degrees(ucusAcisi));
        }
        else if (lazerMi) {
            lazerMermi.move({hizX, hizY});
            float ucusAcisi = std::atan2(hizY, hizX) * 180.f / PI;
            lazerMermi.setRotation(sf::degrees(ucusAcisi));
        }
        else {
            daireMermi.move({hizX, hizY});
        }
    }

    void ciz(sf::RenderWindow& pencere) override {
        if (gercekMi) pencere.draw(gercekMermi);
        else if (lazerMi) pencere.draw(lazerMermi);
        else pencere.draw(daireMermi);
    }
    sf::FloatRect getSinirlar() override {
        if (gercekMi) return gercekMermi.getGlobalBounds();
        return lazerMi ? lazerMermi.getGlobalBounds() : daireMermi.getGlobalBounds();
    }
    bool silinecekMi() override {
        sf::Vector2f p;
        if (gercekMi) p = gercekMermi.getPosition();
        else if (lazerMi) p = lazerMermi.getPosition();
        else p = daireMermi.getPosition();
        return p.y > PENCERE_H + 70.f || p.x < -70.f || p.x > PENCERE_W + 70.f;
    }
    bool isMermi() override { return true; }
    Sahip getSahip() override { return kimeAit; }
};


// =============================================================
// ROKET
// =============================================================
class Roket : public OyunNesnesi {
private:
    sf::RectangleShape govde;
    sf::CircleShape burun;
    sf::CircleShape ates;
    float hizX = 0.f;
    float hizY = 0.f;
    const float YERCEKIMI = 0.05f;
    Sahip kimeAit;

public:
    Roket(float x, float y, float aciDerece, Sahip sahip) : kimeAit(sahip) {
        govde.setSize({28.f, 8.f});
        govde.setOrigin({14.f, 4.f});
        govde.setPosition({x, y});
        govde.setFillColor(sf::Color(90, 90, 90));

        burun.setRadius(4.f);
        burun.setOrigin({4.f, 4.f});
        burun.setFillColor(sf::Color(220, 50, 50));

        ates.setRadius(4.f);
        ates.setOrigin({4.f, 4.f});
        ates.setFillColor(sf::Color(255, 180, 60));

        const float rad = aciDerece * PI / 180.f;
        const float ilkHiz = 18.f;
        hizX = ilkHiz * std::cos(rad);
        hizY = ilkHiz * std::sin(rad);
    }

    void guncelle() override {
        hizY += YERCEKIMI;
        govde.move({hizX, hizY});
        float ucusAcisi = std::atan2(hizY, hizX) * 180.f / PI;
        govde.setRotation(sf::degrees(ucusAcisi));
        ates.setFillColor((std::rand() % 2) ? sf::Color(255, 170, 40) : sf::Color(255, 90, 20));
    }

    void ciz(sf::RenderWindow& pencere) override {
        const float rad = govde.getRotation().asRadians();
        const sf::Vector2f p = govde.getPosition();

        burun.setPosition({p.x + 14.f * std::cos(rad), p.y + 14.f * std::sin(rad)});
        ates.setPosition({p.x - 14.f * std::cos(rad), p.y - 14.f * std::sin(rad)});
        burun.setRotation(govde.getRotation());

        pencere.draw(ates);
        pencere.draw(govde);
        pencere.draw(burun);
    }

    sf::FloatRect getSinirlar() override { return govde.getGlobalBounds(); }
    bool silinecekMi() override {
        const sf::Vector2f p = govde.getPosition();
        return p.y > PENCERE_H + 70.f || p.x < -70.f || p.x > PENCERE_W + 70.f;
    }
    bool isMermi() override { return true; }
    Sahip getSahip() override { return kimeAit; }
};

// =============================================================
// BALONCUK
// =============================================================
class Baloncuk : public OyunNesnesi {
private:
    sf::CircleShape disKure;
    sf::CircleShape canYuvarlak;
    sf::RectangleShape canDikey;
    sf::RectangleShape canYatay;
    sf::RectangleShape silahGovde;
    sf::RectangleShape silahSap;
    sf::RectangleShape sarjor;
    sf::CircleShape durbun;
    sf::CircleShape roketAgiz;
    GucTipi tip;
    float salinim = 0.f;

public:
    Baloncuk(float x, float y, GucTipi t) : tip(t) {
        // Dış küre küçültüldü (24 -> 19)
        disKure.setRadius(19.f);
        disKure.setOrigin({19.f, 19.f});
        disKure.setPosition({x, y});
        disKure.setFillColor(sf::Color(190, 220, 255, 130));
        disKure.setOutlineThickness(2.f);
        disKure.setOutlineColor(sf::Color(230, 240, 255, 220));

        // Can simgesi orantılı küçültüldü
        canYuvarlak.setRadius(7.5f);
        canYuvarlak.setOrigin({7.5f, 7.5f});
        canYuvarlak.setFillColor(sf::Color(235, 60, 70));
        canDikey.setSize({3.f, 13.f});
        canDikey.setOrigin({1.5f, 6.5f});
        canDikey.setFillColor(sf::Color::White);
        canYatay.setSize({13.f, 3.f});
        canYatay.setOrigin({6.5f, 1.5f});
        canYatay.setFillColor(sf::Color::White);

        silahGovde.setOrigin({10.f, 3.f});
        silahSap.setOrigin({3.f, 5.f});
        sarjor.setOrigin({3.f, 5.f});
        durbun.setRadius(2.5f);
        durbun.setOrigin({2.5f, 2.5f});
        durbun.setFillColor(sf::Color(40, 40, 60));
        roketAgiz.setRadius(2.5f);
        roketAgiz.setOrigin({2.5f, 2.5f});
        roketAgiz.setFillColor(sf::Color(30, 30, 30));
    }

    void guncelle() override {
        salinim += 0.045f;
        disKure.move({0.f, std::sin(salinim) * 0.55f});
        silahGovde.rotate(sf::degrees(1.2f));
        silahSap.rotate(sf::degrees(1.2f));
        sarjor.rotate(sf::degrees(1.2f));
    }

    void ciz(sf::RenderWindow& pencere) override {
        const sf::Vector2f p = disKure.getPosition();
        pencere.draw(disKure);

        if (tip == GucTipi::CAN) {
            canYuvarlak.setPosition(p);
            canDikey.setPosition(p);
            canYatay.setPosition(p);
            pencere.draw(canYuvarlak);
            pencere.draw(canDikey);
            pencere.draw(canYatay);
            return;
        }

        silahGovde.setRotation(sf::degrees(std::sin(salinim) * 8.f));
        silahSap.setRotation(silahGovde.getRotation());
        sarjor.setRotation(silahGovde.getRotation());

        if (tip == GucTipi::TABANCA) {
            silahGovde.setSize({13.f, 6.f});
            silahGovde.setFillColor(sf::Color(160, 160, 165));
            silahGovde.setOrigin({6.5f, 3.f});
            silahSap.setSize({4.f, 8.f});
            silahSap.setFillColor(sf::Color(90, 90, 90));
            silahGovde.setPosition(p);
            silahSap.setPosition({p.x - 3.f, p.y + 5.f});
            pencere.draw(silahGovde);
            pencere.draw(silahSap);
        } else if (tip == GucTipi::KELES) {
            silahGovde.setSize({23.f, 6.f});
            silahGovde.setFillColor(sf::Color(118, 82, 52));
            silahGovde.setOrigin({12.f, 2.5f});
            silahSap.setSize({5.f, 8.f});
            silahSap.setFillColor(sf::Color(120, 75, 40));
            sarjor.setSize({4.f, 10.f});
            sarjor.setFillColor(sf::Color(75, 75, 75));
            silahGovde.setPosition(p);
            silahSap.setPosition({p.x - 5.f, p.y + 4.f});
            sarjor.setPosition({p.x + 1.f, p.y + 5.f});
            pencere.draw(silahGovde);
            pencere.draw(silahSap);
            pencere.draw(sarjor);
        } else if (tip == GucTipi::NISANCI) {
            silahGovde.setSize({32.f, 4.f});
            silahGovde.setFillColor(sf::Color(45, 45, 52));
            silahGovde.setOrigin({15.f, 2.f});
            silahGovde.setPosition(p);
            durbun.setPosition({p.x - 3.f, p.y - 4.f});
            pencere.draw(silahGovde);
            pencere.draw(durbun);
        } else if (tip == GucTipi::ROKET) {
            silahGovde.setSize({24.f, 9.f});
            silahGovde.setFillColor(sf::Color(110, 35, 45));
            silahGovde.setOrigin({12.f, 4.f});
            silahGovde.setPosition(p);
            roketAgiz.setPosition({p.x + 11.f, p.y});
            pencere.draw(silahGovde);
            pencere.draw(roketAgiz);
        }
    }

    sf::FloatRect getSinirlar() override { return disKure.getGlobalBounds(); }
    GucTipi getTip() const { return tip; }
};

// =============================================================
// SİLAH SINIFLARI (Strateji Kalıbı)
// =============================================================

// 1. ATA SINIF (Tüm silahların türediği şablon)
class Silah {
protected:
    sf::Clock atisSaati;
    float beklemeSuresi;

public:
    Silah(float bekleme) : beklemeSuresi(bekleme) {}
    virtual ~Silah() = default;

    // Saf sanal fonksiyon: Her alt sınıf kendi mermisini üretmek ZORUNDA
    virtual OyunNesnesi* atesEt(float x, float y, float aci, Sahip sahip) = 0;

    // Atış bekleme süresinin dolup dolmadığını kontrol eden ortak fonksiyon
    bool atesEdebilirMi() {
        if (atisSaati.getElapsedTime().asSeconds() >= beklemeSuresi) {
            atisSaati.restart();
            return true;
        }
        return false;
    }
};

// 2. ALT SINIFLAR (Ata sınıftan türeyen özel silahlar)

class KelesSinifi : public Silah {
public:
    // Bekleme süresi: 0.30 saniye
    KelesSinifi() : Silah(0.30f) {}

    OyunNesnesi* atesEt(float x, float y, float aci, Sahip sahip) override {
        if (!atesEdebilirMi()) return nullptr;
        // Kırmızı lazer üretir
        return new Mermi(x, y, aci, 36.f, 0.12f, sf::Color::Red, true, false, sahip);
    }
};

class RoketAtarSinifi : public Silah {
public:
    // Bekleme süresi: 1.2 saniye
    RoketAtarSinifi() : Silah(1.2f) {}

    OyunNesnesi* atesEt(float x, float y, float aci, Sahip sahip) override {
        if (!atesEdebilirMi()) return nullptr;
        const float rad = aci * PI / 180.f;
        // Roket üretir
        return new Roket(x + std::cos(rad) * 10.f, y + std::sin(rad) * 10.f, aci, sahip);
    }
};

class TabancaSinifi : public Silah {
public:
    // Bekleme süresi: 0.45 saniye
    TabancaSinifi() : Silah(0.45f) {}

    OyunNesnesi* atesEt(float x, float y, float aci, Sahip sahip) override {
        if (!atesEdebilirMi()) return nullptr;
        // Gerçek sivri mermi üretir
        return new Mermi(x, y, aci, 32.f, 0.10f, sf::Color(139, 69, 19), false, true, sahip);
    }
};

class NisanciSinifi : public Silah {
public:
    // Bekleme süresi: 1.0 saniye
    NisanciSinifi() : Silah(1.0f) {}

    OyunNesnesi* atesEt(float x, float y, float aci, Sahip sahip) override {
        if (!atesEdebilirMi()) return nullptr;
        // Mavi uzun lazer üretir
        return new Mermi(x, y, aci, 45.f, 0.04f, sf::Color(0, 255, 255), true, false, sahip);
    }
};

class TankTopuSinifi : public Silah {
public:
    // Bekleme süresi: 0.6 saniye
    TankTopuSinifi() : Silah(0.6f) {}

    OyunNesnesi* atesEt(float x, float y, float aci, Sahip sahip) override {
        if (!atesEdebilirMi()) return nullptr;
        // Siyah gülle üretir
        return new Mermi(x, y, aci, 24.f, 0.18f, sf::Color(30, 30, 30), false, false, sahip);
    }
};



// =============================================================
// TANK
// =============================================================
// =============================================================
// TANK
// =============================================================
// =============================================================
// TANK (Gerçekçi Modern Tasarım)
// =============================================================
class Tank : public OyunNesnesi {
private:
    sf::ConvexShape paletKayisi;
    std::vector<sf::CircleShape> tekerlekler;
    sf::ConvexShape yanEtek;
    sf::ConvexShape govde;
    sf::ConvexShape kule;
    sf::RectangleShape kuleKapak;
    sf::RectangleShape namlu;
    sf::RectangleShape namluDetay; // Namlu boğumu (Bore evacuator)
    sf::RectangleShape hitbox;

    float namluAcisi = -14.f;
    int can = 5;
    int hasarSuresi = 0;
    Silah* anaTop; // Tankın atış motoru

public:
    Tank(float x) {
        anaTop = new TankTopuSinifi();
        // 1. PALET KAYIŞI (Dikdörtgen yerine uçları eğimli gerçekçi zincir formu)
        paletKayisi.setPointCount(6);
        paletKayisi.setPoint(0, {8.f, 0.f});      // Sol alt
        paletKayisi.setPoint(1, {132.f, 0.f});    // Sağ alt
        paletKayisi.setPoint(2, {146.f, -14.f});  // Sağ orta (Yukarı kıvrım)
        paletKayisi.setPoint(3, {136.f, -26.f});  // Sağ üst
        paletKayisi.setPoint(4, {4.f, -26.f});    // Sol üst
        paletKayisi.setPoint(5, {-6.f, -14.f});   // Sol orta (Yukarı kıvrım)
        paletKayisi.setFillColor(sf::Color(30, 30, 30));
        paletKayisi.setPosition({x, ZEMIN_Y + 12.f});
        // 2. TEKERLEKLER (Havaya kalkan uçlarda küçük, ortada büyük tekerlekler)
        // Arka tahrik dişlisi (Küçük, yüksekte)
        sf::CircleShape arkaDisli(6.f);
        arkaDisli.setFillColor(sf::Color(55, 55, 55));
        arkaDisli.setPosition({x - 2.f, ZEMIN_Y - 5.f});
        tekerlekler.push_back(arkaDisli);

        // Ön yönlendirme dişlisi (Küçük, yüksekte)
        sf::CircleShape onDisli(6.f);
        onDisli.setFillColor(sf::Color(55, 55, 55));
        onDisli.setPosition({x + 132.f, ZEMIN_Y - 5.f});
        tekerlekler.push_back(onDisli);

        // Ortadaki yük taşıyan büyük yol tekerlekleri
        for (int i = 0; i < 6; ++i) {
            sf::CircleShape teker(8.5f);
            teker.setFillColor(sf::Color(45, 45, 45));
            teker.setOutlineThickness(1.5f);
            teker.setOutlineColor(sf::Color(15, 15, 15));
            teker.setPosition({x + 14.f + (i * 19.f), ZEMIN_Y - 7.f});
            tekerlekler.push_back(teker);
        }

        // 3. YAN ETEK ZIRHI (Tekerleklerin üstünü kapatarak o "kutu" hissini yok eder)
        yanEtek.setPointCount(4);
        yanEtek.setPoint(0, {0.f, 0.f});
        yanEtek.setPoint(1, {140.f, 0.f});
        yanEtek.setPoint(2, {145.f, -10.f});
        yanEtek.setPoint(3, {-5.f, -10.f});
        yanEtek.setFillColor(sf::Color(25, 105, 25)); // Gövdeden biraz koyu, gölge hissi için
        yanEtek.setPosition({x, ZEMIN_Y - 2.f});

        // 4. ANA GÖVDE
        govde.setPointCount(4);
        govde.setPoint(0, {-6.f, 0.f});
        govde.setPoint(1, {146.f, 0.f});
        govde.setPoint(2, {125.f, -18.f});
        govde.setPoint(3, {-5.f, -18.f});
        govde.setFillColor(sf::Color(34, 139, 34));
        govde.setPosition({x, ZEMIN_Y - 12.f});

        // 5. KULE (Taret ve Komutan Kapağı)
        kule.setPointCount(4);
        kule.setPoint(0, {0.f, 0.f});
        kule.setPoint(1, {70.f, 0.f});
        kule.setPoint(2, {55.f, -16.f});
        kule.setPoint(3, {15.f, -16.f});
        kule.setFillColor(sf::Color(28, 120, 28));
        kule.setPosition({x + 30.f, ZEMIN_Y - 30.f});

        kuleKapak.setSize({16.f, 4.f});
        kuleKapak.setPosition({x + 45.f, ZEMIN_Y - 50.f});
        kuleKapak.setFillColor(sf::Color(20, 80, 20));

        // 6. NAMLU VE BOĞUMU (Gerçekçi tank topu hissi)
        namlu.setSize({75.f, 6.f});
        namlu.setOrigin({0.f, 3.f});
        namlu.setPosition({x + 75.f, ZEMIN_Y - 22.f});
        namlu.setFillColor(sf::Color(20, 20, 20));

        namluDetay.setSize({18.f, 10.f});
        namluDetay.setOrigin({9.f, 5.f});
        namluDetay.setFillColor(sf::Color(30, 35, 30));

        // 7. HITBOX
        // 7. HITBOX (Sadece gerçek metal parçaları kapsayacak şekilde daraltıldı)
        // 7. HITBOX (Mermilerin teğet geçmesini engellemek için dışa doğru genişletildi)
        hitbox.setSize({156.f, 68.f});
        hitbox.setPosition({x - 6.f, ZEMIN_Y - 55.f});// Tam kule kapağının hizasına indirildi
    }

    void guncelle() override {
        if (Klavye::A) hareketEttir(-5.f);
        if (Klavye::D) hareketEttir(5.f);

        if (Klavye::W) {
            namluAcisi -= 0.45f;
            if (namluAcisi < -78.f) namluAcisi = -78.f;
        }
        if (Klavye::S) {
            namluAcisi += 0.45f;
            if (namluAcisi > 8.f) namluAcisi = 8.f;
        }

        // Namlu ve üzerindeki boğumun dönüşünü ayarla
        namlu.setRotation(sf::degrees(namluAcisi));
        namluDetay.setRotation(sf::degrees(namluAcisi));

        // Boğumun her zaman namlunun ortasında kalmasını sağla
        float rad = namluAcisi * PI / 180.f;
        namluDetay.setPosition({
            namlu.getPosition().x + 35.f * std::cos(rad),
            namlu.getPosition().y + 35.f * std::sin(rad)
        });

        if (hasarSuresi > 0) {
            hasarSuresi--;
            govde.setFillColor(sf::Color(225, 225, 225));
            kule.setFillColor(sf::Color(200, 200, 200));
            yanEtek.setFillColor(sf::Color(180, 180, 180));
        } else {
            govde.setFillColor(sf::Color(34, 139, 34));
            kule.setFillColor(sf::Color(28, 120, 28));
            yanEtek.setFillColor(sf::Color(25, 105, 25));
        }
    }

    void hareketEttir(float dx) {
        if (hitbox.getPosition().x + dx < 20.f) return;
        if (hitbox.getPosition().x + hitbox.getSize().x + dx > TANK_BOLGE_MAX_X) return;

        paletKayisi.move({dx, 0.f});
        for (auto& teker : tekerlekler) teker.move({dx, 0.f});
        yanEtek.move({dx, 0.f});
        govde.move({dx, 0.f});
        kule.move({dx, 0.f});
        kuleKapak.move({dx, 0.f});
        namlu.move({dx, 0.f});
        hitbox.move({dx, 0.f});
        // namluDetay, guncelle() içinde namluya göre otomatik pozisyon alıyor
    }

    OyunNesnesi* atesEt() {
        if (!anaTop) return nullptr; // Silah yoksa ateş etme

        const float rad = namluAcisi * PI / 180.f;
        const sf::Vector2f namluUcu = {
            namlu.getPosition().x + namlu.getSize().x * std::cos(rad),
            namlu.getPosition().y + namlu.getSize().x * std::sin(rad)
        };

        // Tank mermiyi kendisi üretmez, "anaTop" isimli strateji motoruna komut verir!
        return anaTop->atesEt(namluUcu.x, namluUcu.y, namluAcisi, Sahip::TANK);
    }

    void ciz(sf::RenderWindow& pencere) override {
        pencere.draw(paletKayisi);
        for (auto& teker : tekerlekler) pencere.draw(teker);
        pencere.draw(yanEtek); // Yan etek tekerleklerin üstüne çizilir
        pencere.draw(namlu);
        pencere.draw(namluDetay);
        pencere.draw(govde);
        pencere.draw(kule);
        pencere.draw(kuleKapak);
    }

    sf::FloatRect getSinirlar() override { return hitbox.getGlobalBounds(); }
    void hasarAl() { if (can > 0) { can--; hasarSuresi = 18; } }
    void canEkle() { if (can < 5) can++; }
    int getCan() const { return can; }
};
// =============================================================
// ADAM
// =============================================================
// =============================================================
// ADAM
// =============================================================
// ADAM
// =============================================================
class HedefAdam : public OyunNesnesi {
private:
    sf::CircleShape kafa;
    // --- YÜZ HATLARI VE DETAYLAR ---
    sf::CircleShape solGoz;
    sf::CircleShape sagGoz;
    sf::CircleShape solKulak;
    sf::CircleShape sagKulak;
    sf::ConvexShape burun;       // Sivri burun profili
    sf::RectangleShape agiz;     // Ciddi bir ağız çizgisi
    sf::ConvexShape sapkaUst;    // Yarım çember şeklindeki şapka kubbesi
    sf::RectangleShape sapkaAlt; // İnce siperlik (Ön çıkıntı)
    // -----------------------------------------

    sf::RectangleShape govde;
    sf::RectangleShape onKol;
    sf::RectangleShape arkaKol;
    sf::RectangleShape solBacak;
    sf::RectangleShape sagBacak;
    sf::RectangleShape hitbox;
    sf::RectangleShape silahGorsel;
    sf::RectangleShape silahNamluUcu;
    sf::RectangleShape silahDip;
    sf::RectangleShape silahSarjor;
    sf::CircleShape silahDurbun;
    sf::CircleShape elOn;
    sf::CircleShape elArka;

    float silahAcisi = 185.f;
    float hizY = 0.f;
    bool yerdeMi = true;
    int can = 5;
    int hasarSuresi = 0;
    SilahTipi mevcutSilah = SilahTipi::ROKET;
    Silah* aktifSilah = nullptr; // Karakterin elindeki aktif strateji motoru

public:
    SilahTipi getSilah() const { return mevcutSilah; }
    HedefAdam(float x) {
        solBacak.setSize({9.f, 40.f});
        solBacak.setOrigin({4.f, 0.f});
        solBacak.setPosition({x - 7.f, ZEMIN_Y - 28.f});

        sagBacak.setSize({9.f, 40.f});
        sagBacak.setOrigin({4.f, 0.f});
        sagBacak.setPosition({x + 7.f, ZEMIN_Y - 28.f});

        govde.setSize({23.f, 40.f});
        govde.setOrigin({11.f, 0.f});
        govde.setPosition({x, ZEMIN_Y - 68.f});

        kafa.setRadius(14.f);
        kafa.setOrigin({14.f, 16.f});
        kafa.setPosition({x, ZEMIN_Y - 81.f});

// Kafanın referans pozisyonunu alarak yüz hatlarını yerleştiriyoruz
        const sf::Vector2f kp = kafa.getPosition();

        // Kulaklar
        solKulak.setRadius(3.5f);
        solKulak.setOrigin({3.5f, 3.5f});
        solKulak.setPosition({kp.x - 14.f, kp.y - 2.f});
        sagKulak.setRadius(3.5f);
        sagKulak.setOrigin({3.5f, 3.5f});
        sagKulak.setPosition({kp.x + 14.f, kp.y - 2.f});

        // Gözler (Tekrar tanka / sola dönük eski haline alındı)
        solGoz.setRadius(1.5f);
        solGoz.setOrigin({1.5f, 1.5f});
        solGoz.setFillColor(sf::Color(20, 20, 20));
        solGoz.setPosition({kp.x - 8.f, kp.y - 4.f});
        sagGoz.setRadius(1.5f);
        sagGoz.setOrigin({1.5f, 1.5f});
        sagGoz.setFillColor(sf::Color(20, 20, 20));
        sagGoz.setPosition({kp.x - 2.f, kp.y - 4.f});

        // BURUN (Tekrar tanka / sola dönük eski haline alındı)
        burun.setPointCount(3);
        burun.setPoint(0, {0.f, 0.f});   // Uç kısmı
        burun.setPoint(1, {4.f, -2.f});  // Üst kök
        burun.setPoint(2, {4.f, 2.f});   // Alt kök
        burun.setOrigin({2.f, 0.f});
        burun.setPosition({kp.x - 14.f, kp.y + 1.f});

        // AĞIZ (Sol tarafta kalacak ama yüzün tam kenarında değil, biraz daha ortaya yakın)
        agiz.setSize({6.f, 1.2f});
        agiz.setOrigin({3.f, 0.6f});
        agiz.setPosition({kp.x - 6.f, kp.y + 6.f}); // -10.f yerine -6.f yaparak ortaya yaklaştırdık

        // ŞAPKA ÜSTÜ (Kafaya oturan Yarım Çember Kubbe)
        sapkaUst.setPointCount(7);
        sapkaUst.setPoint(0, {0.f, 12.f});
        sapkaUst.setPoint(1, {2.f, 5.f});
        sapkaUst.setPoint(2, {6.f, 1.f});
        sapkaUst.setPoint(3, {12.f, 0.f});
        sapkaUst.setPoint(4, {18.f, 1.f});
        sapkaUst.setPoint(5, {22.f, 5.f});
        sapkaUst.setPoint(6, {24.f, 12.f});
        sapkaUst.setOrigin({12.f, 12.f});
        sapkaUst.setFillColor(sf::Color::Blue);
        sapkaUst.setPosition({kp.x, kp.y - 12.f});

        // ŞAPKA SİPERLİĞİ (Uzatılmış hali korundu, tam tanka bakıyor)
        sapkaAlt.setSize({26.f, 3.f});
        sapkaAlt.setOrigin({13.f, 1.5f});
        sapkaAlt.setFillColor(sf::Color(30, 30, 35));
        sapkaAlt.setPosition({kp.x - 11.f, kp.y - 12.f});

        silahGorsel.setSize({46.f, 12.f});
        silahGorsel.setOrigin({23.f, 6.f});
        silahGorsel.setPosition({x - 8.f, ZEMIN_Y - 63.f});
        silahGorsel.setFillColor(sf::Color(95, 95, 110));

        silahNamluUcu.setSize({8.f, 8.f});
        silahNamluUcu.setOrigin({4.f, 4.f});
        silahNamluUcu.setFillColor(sf::Color(35, 35, 35));

        silahDip.setSize({10.f, 12.f});
        silahDip.setOrigin({5.f, 6.f});
        silahDip.setFillColor(sf::Color(85, 55, 35));
        silahSarjor.setSize({5.f, 12.f});
        silahSarjor.setOrigin({2.5f, 6.f});
        silahSarjor.setFillColor(sf::Color(65, 65, 65));
        silahDurbun.setRadius(4.f);
        silahDurbun.setOrigin({4.f, 4.f});
        silahDurbun.setFillColor(sf::Color(40, 40, 55));

        onKol.setSize({25.f, 6.f});
        onKol.setOrigin({25.f, 3.f});
        onKol.setPosition({x - 8.f, ZEMIN_Y - 63.f});
        arkaKol.setSize({17.f, 6.f});
        arkaKol.setOrigin({0.f, 3.f});
        arkaKol.setPosition({x + 1.f, ZEMIN_Y - 63.f});

        elOn.setRadius(4.f);
        elOn.setOrigin({4.f, 4.f});
        elArka.setRadius(4.f);
        elArka.setOrigin({4.f, 4.f});

        hitbox.setSize({54.f, 108.f});
        hitbox.setOrigin({27.f, 108.f});
        hitbox.setPosition({x, ZEMIN_Y + 4.f});

        renkAyarlaNormale();
        silahDegistir(SilahTipi::ROKET);
    }

    ~HedefAdam() {
        if (aktifSilah) delete aktifSilah;
    }


    void renkAyarlaNormale() {
        kafa.setFillColor(sf::Color(255, 224, 189));
        solKulak.setFillColor(sf::Color(240, 210, 175));
        sagKulak.setFillColor(sf::Color(240, 210, 175));
        burun.setFillColor(sf::Color(240, 205, 170)); // Tene uygun burun rengi
        agiz.setFillColor(sf::Color(110, 60, 60));    // Dudak rengi

        govde.setFillColor(sf::Color(65, 105, 225));
        onKol.setFillColor(sf::Color(65, 105, 225));
        arkaKol.setFillColor(sf::Color(65, 105, 225));
        solBacak.setFillColor(sf::Color(139, 69, 19));
        sagBacak.setFillColor(sf::Color(139, 69, 19));
        elOn.setFillColor(sf::Color(245, 210, 175));
        elArka.setFillColor(sf::Color(245, 210, 175));
    }

    void guncelle() override {
        float hizX = 0.f;
        if (Klavye::Sag) hizX = 6.f;
        if (Klavye::Sol) hizX = -6.f;
        if (Klavye::Yukari && yerdeMi) {
            hizY = -14.f;
            yerdeMi = false;
        }

        float nisanHizi = 2.2f;
        if (mevcutSilah == SilahTipi::TABANCA) nisanHizi = 1.6f;
        else if (mevcutSilah == SilahTipi::KELES) nisanHizi = 1.5f;
        else if (mevcutSilah == SilahTipi::NISANCI) nisanHizi = 1.3f;
        else if (mevcutSilah == SilahTipi::ROKET) nisanHizi = 1.5f;

        if (Klavye::O) {
            silahAcisi += nisanHizi;
            if (silahAcisi > 260.f) silahAcisi = 260.f;
        }
        if (Klavye::L) {
            silahAcisi -= nisanHizi;
            if (silahAcisi < 100.f) silahAcisi = 100.f;
        }

        silahGorsel.setRotation(sf::degrees(silahAcisi));
        silahNamluUcu.setRotation(sf::degrees(silahAcisi));
        silahDip.setRotation(sf::degrees(silahAcisi));
        silahSarjor.setRotation(sf::degrees(silahAcisi));
        onKol.setRotation(sf::degrees(silahAcisi));

        hizY += 0.52f;
        hareketEttir(hizX, hizY);

        if (hitbox.getPosition().y > ZEMIN_Y) {
            hareketEttir(0.f, ZEMIN_Y - hitbox.getPosition().y);
            hizY = 0.f;
            yerdeMi = true;
        }

        if (hasarSuresi > 0) {
            hasarSuresi--;
            kafa.setFillColor(sf::Color::Red);
            solKulak.setFillColor(sf::Color::Red);
            sagKulak.setFillColor(sf::Color::Red);
            burun.setFillColor(sf::Color(200, 0, 0));
        } else {
            renkAyarlaNormale();
        }
    }

    void hareketEttir(float dx, float dy) {
        if (hitbox.getPosition().x + dx < ADAM_BOLGE_MIN_X) return;
        if (hitbox.getPosition().x + dx > PENCERE_W - 30.f) return;

        kafa.move({dx, dy});
        solGoz.move({dx, dy});
        sagGoz.move({dx, dy});
        solKulak.move({dx, dy});
        sagKulak.move({dx, dy});
        burun.move({dx, dy});
        agiz.move({dx, dy});
        sapkaUst.move({dx, dy});
        sapkaAlt.move({dx, dy});

        govde.move({dx, dy});
        silahGorsel.move({dx, dy});
        onKol.move({dx, dy});
        arkaKol.move({dx, dy});
        solBacak.move({dx, dy});
        sagBacak.move({dx, dy});
        silahNamluUcu.move({dx, dy});
        silahDip.move({dx, dy});
        silahSarjor.move({dx, dy});
        silahDurbun.move({dx, dy});
        elOn.move({dx, dy});
        elArka.move({dx, dy});
        hitbox.move({dx, dy});

        const float rad = silahAcisi * PI / 180.f;
        const float sinr = std::sin(rad);
        const float cosr = std::cos(rad);

        float eldeOfset = 6.f;
        float arkaElOfset = -8.f;
        if (mevcutSilah == SilahTipi::ROKET) {
            eldeOfset = 10.f;
            arkaElOfset = -14.f;
            silahGorsel.setPosition({govde.getPosition().x - 6.f, govde.getPosition().y + 16.f});
        } else {
            silahGorsel.setPosition({govde.getPosition().x - 9.f, govde.getPosition().y + 20.f});
        }

        const sf::Vector2f p = silahGorsel.getPosition();
        onKol.setPosition({p.x - std::cos(rad) * 3.f, p.y - std::sin(rad) * 3.f});
        arkaKol.setPosition({p.x + std::cos(rad) * 10.f, p.y + std::sin(rad) * 10.f});
        silahNamluUcu.setPosition({p.x + cosr * (silahGorsel.getSize().x * 0.53f), p.y + sinr * (silahGorsel.getSize().x * 0.53f)});
        silahDip.setPosition({p.x - cosr * (silahGorsel.getSize().x * 0.40f), p.y - sinr * (silahGorsel.getSize().x * 0.40f)});
        silahSarjor.setPosition({p.x - cosr * 2.f - sinr * 3.f, p.y - sinr * 2.f + cosr * 3.f});
        silahDurbun.setPosition({p.x - cosr * 3.f - sinr * 4.f, p.y - sinr * 3.f + cosr * 4.f});

        elOn.setPosition({p.x - cosr * eldeOfset, p.y - sinr * eldeOfset});
        elArka.setPosition({p.x - cosr * arkaElOfset, p.y - sinr * arkaElOfset});
    }

   OyunNesnesi* atesEt() {
        if (!aktifSilah) return nullptr; // Eger elde silah yoksa ates etme
        const sf::Vector2f ucu = getSilahUcu();

        // Adam mermiyi kendi uretmez, elindeki aktif silaha komut verir!
        return aktifSilah->atesEt(ucu.x, ucu.y, silahAcisi, Sahip::ADAM);
    }

    void silahDegistir(SilahTipi yeniTip) {
        mevcutSilah = yeniTip;

        // --- 1. YENI MIMARI (STRATEJI MOTORU) ---
        // Eski silahi hafizadan temizle
        if (aktifSilah != nullptr) {
            delete aktifSilah;
        }

        // Yeni secilen silaha gore arka plan motorunu olustur
        if (yeniTip == SilahTipi::TABANCA) aktifSilah = new TabancaSinifi();
        else if (yeniTip == SilahTipi::KELES) aktifSilah = new KelesSinifi();
        else if (yeniTip == SilahTipi::NISANCI) aktifSilah = new NisanciSinifi();
        else if (yeniTip == SilahTipi::ROKET) aktifSilah = new RoketAtarSinifi();


        // --- 2. SENIN GORSEL TASARIM KODLARIN (HIC DOKUNULMADI) ---
        if (yeniTip == SilahTipi::TABANCA) {
            silahGorsel.setSize({18.f, 7.f});
            silahGorsel.setFillColor(sf::Color(150, 155, 160));
            silahDip.setFillColor(sf::Color(25, 25, 25));
            silahDip.setSize({6.f, 10.f});
            silahSarjor.setSize({0.f, 0.f});
            silahNamluUcu.setSize({5.f, 5.f});
            silahNamluUcu.setFillColor(sf::Color(40, 40, 40));
            silahNamluUcu.setOrigin({2.5f, 2.5f});
        } else if (yeniTip == SilahTipi::KELES) {
            silahGorsel.setSize({42.f, 8.f});
            silahGorsel.setFillColor(sf::Color(70, 70, 70));
            silahDip.setFillColor(sf::Color(139, 69, 19));
            silahDip.setSize({12.f, 14.f});
            silahSarjor.setSize({8.f, 20.f});
            silahSarjor.setFillColor(sf::Color(40, 40, 40));
            silahNamluUcu.setSize({4.f, 4.f});
            silahNamluUcu.setFillColor(sf::Color(20, 20, 20));
            silahNamluUcu.setOrigin({2.f, 2.f});
        } else if (yeniTip == SilahTipi::NISANCI) {
            silahGorsel.setSize({62.f, 5.f});
            silahGorsel.setFillColor(sf::Color(35, 35, 42));
            silahDip.setFillColor(sf::Color(60, 50, 40));
            silahDip.setSize({8.f, 10.f});
            silahSarjor.setSize({1.f, 1.f});
            silahDurbun.setRadius(4.f);
            silahDurbun.setOrigin({4.f, 4.f});
            silahNamluUcu.setSize({5.f, 5.f});
            silahNamluUcu.setOrigin({2.5f, 2.5f});
        } else if (yeniTip == SilahTipi::ROKET) {
            silahGorsel.setSize({46.f, 14.f});
            silahGorsel.setFillColor(sf::Color(110, 35, 45));
            silahDip.setFillColor(sf::Color(95, 65, 45));
            silahDip.setSize({10.f, 12.f});
            silahSarjor.setSize({1.f, 1.f});
            silahNamluUcu.setSize({9.f, 9.f});
            silahNamluUcu.setOrigin({4.5f, 4.5f});
        }
        silahGorsel.setOrigin({silahGorsel.getSize().x / 2.f, silahGorsel.getSize().y / 2.f});
        silahDip.setOrigin({silahDip.getSize().x * 0.5f, silahDip.getSize().y * 0.5f});
        silahSarjor.setOrigin({silahSarjor.getSize().x * 0.5f, silahSarjor.getSize().y * 0.5f});
    }

    void ciz(sf::RenderWindow& pencere) override {
        // Çizim sırası derinliği belirler (Önce çizilen arkada kalır)
        pencere.draw(arkaKol);
        pencere.draw(govde);
        pencere.draw(solBacak);
        pencere.draw(sagBacak);

        pencere.draw(solKulak); // Kulaklar kafanın arkasında
        pencere.draw(sagKulak);
        pencere.draw(kafa);     // Kafa ortada

        // Yüz detayları kafanın üstünde
        pencere.draw(burun);
        pencere.draw(agiz);
        pencere.draw(solGoz);
        pencere.draw(sagGoz);

        // Şapka en dışta
        pencere.draw(sapkaUst); // Yarım çember kubbe
        pencere.draw(sapkaAlt); // İnce dikdörtgen siperlik

        pencere.draw(elArka);
        pencere.draw(silahDip);
        if (mevcutSilah == SilahTipi::KELES) pencere.draw(silahSarjor);
        pencere.draw(silahGorsel);
        if (mevcutSilah == SilahTipi::NISANCI) pencere.draw(silahDurbun);
        pencere.draw(silahNamluUcu);
        pencere.draw(elOn);
        pencere.draw(onKol);
    }

    sf::FloatRect getSinirlar() override { return hitbox.getGlobalBounds(); }

    sf::Vector2f getSilahUcu() const {
        const float rad = silahAcisi * PI / 180.f;
        float xBoy = silahGorsel.getSize().x * 0.5f;
        if (mevcutSilah == SilahTipi::ROKET) xBoy += 6.f;
        return {
            silahGorsel.getPosition().x + xBoy * std::cos(rad),
            silahGorsel.getPosition().y + xBoy * std::sin(rad)
        };
    }

    void hasarAl() {
        if (can <= 0) return;
        can--;
        hasarSuresi = 18;
    }
    void canEkle() {
        if (can < 5) can++;
    }
    int getCan() const { return can; }
};
// =============================================================
// YARDIMCI
// =============================================================
static void patlamaPartikulu(std::vector<OyunNesnesi*>& hedef, float x, float y, sf::Color renk, int adet, float sacilma) {
    for (int i = 0; i < adet; ++i) {
        hedef.push_back(new Partikul(x, y, renk, sacilma));
    }
}

// =============================================================
// MAIN
// =============================================================
// =============================================================
// MAIN (MENÜ, OYUN VE BİTİŞ EKRANI MİMARİSİ)
// =============================================================
int main() {
    // SES EFEKTLERİ
    sf::SoundBuffer bTank, bTabanca, bKeles, bNisanci, bRoket;
    bTank.loadFromFile("tank.mp3");
    bTabanca.loadFromFile("tabanca.mp3");
    bKeles.loadFromFile("keles.mp3");
    bNisanci.loadFromFile("nisanci.mp3");
    bRoket.loadFromFile("roket.mp3");

    sf::Sound sTank(bTank), sTabanca(bTabanca), sKeles(bKeles), sNisanci(bNisanci), sRoket(bRoket);
    sTank.setVolume(60.f); sTabanca.setVolume(60.f); sKeles.setVolume(60.f);
    sNisanci.setVolume(60.f); sRoket.setVolume(60.f);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Muhtesem Tank vs Adam - Pro UI");
    window.setFramerateLimit(60);

    sf::View kamera(sf::FloatRect({0.f, 0.f}, {PENCERE_W, PENCERE_H}));
    window.setView(kamera);

    sf::Font oyunFont;
    oyunFont.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");

    // =============================================================
    // =============================================================
    // ARAYÜZ (UI) TASARIMLARI (SFML 3 Uyumlu)
    // =============================================================
    // 1. ANA MENÜ TASARIMI
    sf::Text baslikAnaMenu(oyunFont, "MUHTESEM TANK VS ADAM", 90);
    baslikAnaMenu.setStyle(sf::Text::Bold);
    baslikAnaMenu.setFillColor(sf::Color(255, 215, 0)); // Altın Sarısı
    sf::FloatRect baslikRect = baslikAnaMenu.getLocalBounds();
    baslikAnaMenu.setOrigin({baslikRect.position.x + baslikRect.size.x / 2.0f, baslikRect.position.y + baslikRect.size.y / 2.0f});
    baslikAnaMenu.setPosition({PENCERE_W / 2.f, PENCERE_H / 2.f - 250.f});

    // Oyna Butonu
    sf::RectangleShape btnOyna({350.f, 80.f});
    btnOyna.setOrigin({175.f, 40.f});
    btnOyna.setPosition({PENCERE_W / 2.f, PENCERE_H / 2.f - 50.f});
    sf::Text txtOyna(oyunFont, "OYUNA BASLA", 40);
    sf::FloatRect oynaRect = txtOyna.getLocalBounds();
    txtOyna.setOrigin({oynaRect.position.x + oynaRect.size.x / 2.0f, oynaRect.position.y + oynaRect.size.y / 2.0f});
    txtOyna.setPosition(btnOyna.getPosition());

    // Çıkış Butonu
    sf::RectangleShape btnCikisMenu({350.f, 80.f});
    btnCikisMenu.setOrigin({175.f, 40.f});
    btnCikisMenu.setPosition({PENCERE_W / 2.f, PENCERE_H / 2.f + 70.f});
    sf::Text txtCikisMenu(oyunFont, "OYUNDAN CIK", 40);
    sf::FloatRect cikisMenuRect = txtCikisMenu.getLocalBounds();
    txtCikisMenu.setOrigin({cikisMenuRect.position.x + cikisMenuRect.size.x / 2.0f, cikisMenuRect.position.y + cikisMenuRect.size.y / 2.0f});
    txtCikisMenu.setPosition(btnCikisMenu.getPosition());

    // 2. OYUN BİTİŞ EKRANI TASARIMI
    sf::RectangleShape karartma({PENCERE_W, PENCERE_H});
    karartma.setFillColor(sf::Color(0, 0, 0, 200));

    sf::Text baslikBitti(oyunFont, "OYUN BITTI", 110);
    baslikBitti.setStyle(sf::Text::Bold);
    baslikBitti.setFillColor(sf::Color(220, 50, 50));
    sf::FloatRect bittiRect = baslikBitti.getLocalBounds();
    baslikBitti.setOrigin({bittiRect.position.x + bittiRect.size.x / 2.0f, bittiRect.position.y + bittiRect.size.y / 2.0f});
    baslikBitti.setPosition({PENCERE_W / 2.f, PENCERE_H / 2.f - 250.f});

    // Tekrar Oyna Butonu
    sf::RectangleShape btnTekrar({400.f, 80.f});
    btnTekrar.setOrigin({200.f, 40.f});
    btnTekrar.setPosition({PENCERE_W / 2.f, PENCERE_H / 2.f - 20.f});
    sf::Text txtTekrar(oyunFont, "YENIDEN BASLA", 40);
    sf::FloatRect tekrarRect = txtTekrar.getLocalBounds();
    txtTekrar.setOrigin({tekrarRect.position.x + tekrarRect.size.x / 2.0f, tekrarRect.position.y + tekrarRect.size.y / 2.0f});
    txtTekrar.setPosition(btnTekrar.getPosition());

    // Menüye Dön Butonu
    sf::RectangleShape btnMenuyeDon({400.f, 80.f});
    btnMenuyeDon.setOrigin({200.f, 40.f});
    btnMenuyeDon.setPosition({PENCERE_W / 2.f, PENCERE_H / 2.f + 100.f});
    sf::Text txtMenuyeDon(oyunFont, "ANA MENUYE DON", 40);
    sf::FloatRect menuyeDonRect = txtMenuyeDon.getLocalBounds();
    txtMenuyeDon.setOrigin({menuyeDonRect.position.x + menuyeDonRect.size.x / 2.0f, menuyeDonRect.position.y + menuyeDonRect.size.y / 2.0f});
    txtMenuyeDon.setPosition(btnMenuyeDon.getPosition());
    EkranDurumu mevcutEkran = EkranDurumu::MENU;

    sf::RectangleShape cimen({PENCERE_W, PENCERE_H - ZEMIN_Y});
    cimen.setFillColor(sf::Color(85, 170, 85));
    cimen.setPosition({0.f, ZEMIN_Y});

    std::vector<OyunNesnesi*> nesneler;
    Tank* tank = nullptr;
    HedefAdam* adam = nullptr;
    sf::Clock baloncukSaati;

    // Oyunu Sıfırlama ve Başlatma Fonksiyonu (Lambda Metodu)
    // Oyunu Sıfırlama ve Başlatma Fonksiyonu (Lambda Metodu)
    auto oyunuBaslat = [&]() {
        for (OyunNesnesi* n : nesneler) delete n;
        nesneler.clear();
        tank = new Tank(120.f);
        adam = new HedefAdam(PENCERE_W - 300.f);
        nesneler.push_back(tank);
        nesneler.push_back(adam);
        baloncukSaati.restart();

        // MAC İÇİN EKLENEN SİHİRLİ DOKUNUŞ:
        window.requestFocus();

        // TAKILI KALAN TUŞLARI SIFIRLAMA (Yapışkan Tuş Çözümü)
        Klavye::W = false; Klavye::S = false; Klavye::A = false; Klavye::D = false;
        Klavye::Yukari = false; Klavye::Sag = false; Klavye::Sol = false;
        Klavye::O = false; Klavye::L = false;
    };

    // =============================================================
    // ANA DÖNGÜ
    // =============================================================
    while (window.isOpen()) {
        sf::Vector2f farePozisyonu = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        std::vector<OyunNesnesi*> eklenecekler;

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            // SADECE MENÜDEYKEN TIKLAMALARI KONTROL ET
            if (mevcutEkran == EkranDurumu::MENU) {
                if (const auto* tiklama = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (tiklama->button == sf::Mouse::Button::Left) {
                        if (btnOyna.getGlobalBounds().contains(farePozisyonu)) {
                            oyunuBaslat();
                            mevcutEkran = EkranDurumu::OYUN;
                        }
                        else if (btnCikisMenu.getGlobalBounds().contains(farePozisyonu)) {
                            window.close();
                        }
                    }
                }
            }
            // SADECE OYUN BİTİŞ EKRANINDAYKEN TIKLAMALARI KONTROL ET
            else if (mevcutEkran == EkranDurumu::BITTI) {
                if (const auto* tiklama = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (tiklama->button == sf::Mouse::Button::Left) {
                        if (btnTekrar.getGlobalBounds().contains(farePozisyonu)) {
                            oyunuBaslat();
                            mevcutEkran = EkranDurumu::OYUN;
                        }
                        else if (btnMenuyeDon.getGlobalBounds().contains(farePozisyonu)) {
                            mevcutEkran = EkranDurumu::MENU;
                        }
                    }
                }
            }
            // MAC İÇİN KESİN ÇÖZÜM: Ateş etme tuşlarını doğrudan Event üzerinden yakalıyoruz!
            // MAC İÇİN KESİN ÇÖZÜM: Tuş basılma ve bırakılma anlarını Event ile yakalıyoruz!
            else if (mevcutEkran == EkranDurumu::OYUN) {
                // TUŞA BASILDIĞI AN (TRUE YAP)
                if (const auto* tusBas = event->getIf<sf::Event::KeyPressed>()) {
                    if (tusBas->code == sf::Keyboard::Key::W) Klavye::W = true;
                    else if (tusBas->code == sf::Keyboard::Key::S) Klavye::S = true;
                    else if (tusBas->code == sf::Keyboard::Key::A) Klavye::A = true;
                    else if (tusBas->code == sf::Keyboard::Key::D) Klavye::D = true;
                    else if (tusBas->code == sf::Keyboard::Key::Up) Klavye::Yukari = true;
                    else if (tusBas->code == sf::Keyboard::Key::Right) Klavye::Sag = true;
                    else if (tusBas->code == sf::Keyboard::Key::Left) Klavye::Sol = true;
                    else if (tusBas->code == sf::Keyboard::Key::O) Klavye::O = true;
                    else if (tusBas->code == sf::Keyboard::Key::L) Klavye::L = true;

                    // Ateş Etme (Tek Tetik)
                    else if (tusBas->code == sf::Keyboard::Key::Space) {
                        if (OyunNesnesi* m = tank->atesEt()) { sTank.play(); eklenecekler.push_back(m); }
                    }
                    else if (tusBas->code == sf::Keyboard::Key::Enter) {
                        if (OyunNesnesi* m = adam->atesEt()) {
                            SilahTipi tip = adam->getSilah();
                            if (tip == SilahTipi::TABANCA) sTabanca.play();
                            else if (tip == SilahTipi::KELES) sKeles.play();
                            else if (tip == SilahTipi::NISANCI) sNisanci.play();
                            else if (tip == SilahTipi::ROKET) sRoket.play();
                            eklenecekler.push_back(m);
                        }
                    }
                }
                // TUŞTAN EL ÇEKİLDİĞİ AN (FALSE YAP)
                else if (const auto* tusCek = event->getIf<sf::Event::KeyReleased>()) {
                    if (tusCek->code == sf::Keyboard::Key::W) Klavye::W = false;
                    else if (tusCek->code == sf::Keyboard::Key::S) Klavye::S = false;
                    else if (tusCek->code == sf::Keyboard::Key::A) Klavye::A = false;
                    else if (tusCek->code == sf::Keyboard::Key::D) Klavye::D = false;
                    else if (tusCek->code == sf::Keyboard::Key::Up) Klavye::Yukari = false;
                    else if (tusCek->code == sf::Keyboard::Key::Right) Klavye::Sag = false;
                    else if (tusCek->code == sf::Keyboard::Key::Left) Klavye::Sol = false;
                    else if (tusCek->code == sf::Keyboard::Key::O) Klavye::O = false;
                    else if (tusCek->code == sf::Keyboard::Key::L) Klavye::L = false;
                }
            }
        }

        // =============================================================
        // DURUMLARA GÖRE GÜNCELLEMELER (UPDATE)
        // =============================================================
        if (mevcutEkran == EkranDurumu::MENU) {
            // Hover (Üzerine gelme) efektleri - MENÜ
            btnOyna.setFillColor(btnOyna.getGlobalBounds().contains(farePozisyonu) ? sf::Color(50, 180, 80) : sf::Color(35, 120, 50));
            btnCikisMenu.setFillColor(btnCikisMenu.getGlobalBounds().contains(farePozisyonu) ? sf::Color(200, 60, 60) : sf::Color(140, 40, 40));
        }
        else if (mevcutEkran == EkranDurumu::BITTI) {
            // Hover efektleri - BİTİŞ EKRANI
            btnTekrar.setFillColor(btnTekrar.getGlobalBounds().contains(farePozisyonu) ? sf::Color(50, 180, 80) : sf::Color(35, 120, 50));
            btnMenuyeDon.setFillColor(btnMenuyeDon.getGlobalBounds().contains(farePozisyonu) ? sf::Color(80, 100, 180) : sf::Color(50, 60, 120));
        }
        else if (mevcutEkran == EkranDurumu::OYUN) {
            // OYUN İÇİ MEKANİKLER (Ateş etme, baloncuklar vb.)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                if (OyunNesnesi* m = tank->atesEt()) {
                    sTank.play();
                    eklenecekler.push_back(m);
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                if (OyunNesnesi* m = adam->atesEt()) {
                    SilahTipi tip = adam->getSilah();
                    if (tip == SilahTipi::TABANCA) sTabanca.play();
                    else if (tip == SilahTipi::KELES) sKeles.play();
                    else if (tip == SilahTipi::NISANCI) sNisanci.play();
                    else if (tip == SilahTipi::ROKET) sRoket.play();
                    eklenecekler.push_back(m);
                }
            }

            // Baloncuk Üretimi
            if (baloncukSaati.getElapsedTime().asSeconds() > 9.f) {
                const float rx = PENCERE_W * 0.25f + static_cast<float>(std::rand() % static_cast<int>(PENCERE_W * 0.5f));
                const float ry = ZEMIN_Y - 800.f + static_cast<float>(std::rand() % 350);
                const GucTipi tip = static_cast<GucTipi>(std::rand() % 5);
                eklenecekler.push_back(new Baloncuk(rx, ry, tip));
                baloncukSaati.restart();
            }

            std::vector<OyunNesnesi*> zorlaSilinecek;
            for (auto it = nesneler.begin(); it != nesneler.end();) {
                OyunNesnesi* nesne = *it;
                nesne->guncelle();
                bool sil = (std::find(zorlaSilinecek.begin(), zorlaSilinecek.end(), nesne) != zorlaSilinecek.end());

                if (Baloncuk* b = dynamic_cast<Baloncuk*>(nesne)) {
                    bool balonVuruldu = false;
                    for (OyunNesnesi* diger : nesneler) {
                        if (!diger->isMermi() || diger == nesne) continue;
                        if (!b->getSinirlar().findIntersection(diger->getSinirlar())) continue;

                        const Sahip mermiSahibi = diger->getSahip();
                        const GucTipi balonTipi = b->getTip();

                        if (mermiSahibi == Sahip::TANK) {
                            if (balonTipi == GucTipi::CAN) {
                                tank->canEkle();
                                balonVuruldu = true;
                                zorlaSilinecek.push_back(diger);
                                break;
                            }
                            continue;
                        }

                        if (mermiSahibi == Sahip::ADAM) {
                            if (balonTipi == GucTipi::CAN) adam->canEkle();
                            else if (balonTipi == GucTipi::TABANCA) adam->silahDegistir(SilahTipi::TABANCA);
                            else if (balonTipi == GucTipi::KELES) adam->silahDegistir(SilahTipi::KELES);
                            else if (balonTipi == GucTipi::NISANCI) adam->silahDegistir(SilahTipi::NISANCI);
                            else if (balonTipi == GucTipi::ROKET) adam->silahDegistir(SilahTipi::ROKET);
                            balonVuruldu = true;
                            zorlaSilinecek.push_back(diger);
                            break;
                        }
                    }

                    if (balonVuruldu) sil = true;

                    if (sil) {
                        sf::FloatRect r = b->getSinirlar();
                        patlamaPartikulu(eklenecekler, r.position.x + r.size.x * 0.5f, r.position.y + r.size.y * 0.5f, sf::Color::White, 11, 13.f);
                    }
                }

                if (!sil && nesne->isMermi()) {
                    Sahip sahip = nesne->getSahip();
                    if (sahip == Sahip::TANK && nesne->getSinirlar().findIntersection(adam->getSinirlar())) {
                        adam->hasarAl();
                        sf::FloatRect r = nesne->getSinirlar();
                        patlamaPartikulu(eklenecekler, r.position.x, r.position.y, sf::Color::Red, 18, 20.f);
                        sil = true;
                    } else if (sahip == Sahip::ADAM && nesne->getSinirlar().findIntersection(tank->getSinirlar())) {
                        tank->hasarAl();
                        sf::FloatRect r = nesne->getSinirlar();
                        patlamaPartikulu(eklenecekler, r.position.x, r.position.y, sf::Color(255, 120, 40), 18, 21.f);
                        sil = true;
                    }
                }

                if (sil || nesne->silinecekMi()) {
                    if (nesne != tank && nesne != adam) delete nesne;
                    it = nesneler.erase(it);
                } else {
                    ++it;
                }
            }

            for (OyunNesnesi* yeni : eklenecekler) nesneler.push_back(yeni);

            // ÖLÜM KONTROLÜ
            if (tank->getCan() <= 0 || adam->getCan() <= 0) {
                mevcutEkran = EkranDurumu::BITTI;
            }
        }

        // =============================================================
        // ÇİZİM (RENDER) AŞAMASI
        // =============================================================
        window.clear(sf::Color(130, 200, 245));

        if (mevcutEkran == EkranDurumu::MENU) {
            // Ana Menüyü Çiz
            window.draw(baslikAnaMenu);
            window.draw(btnOyna);
            window.draw(txtOyna);
            window.draw(btnCikisMenu);
            window.draw(txtCikisMenu);
        }
        else {
            // Oyun İçi ve Bitiş Ekranı (Arka planda oyun donmuş şekilde görünür)
            window.draw(cimen);
            for (OyunNesnesi* n : nesneler) n->ciz(window);

            // HUD: Can Kutuları
            for (int i = 0; i < 5; ++i) {
                sf::RectangleShape kutu({28.f, 20.f});
                kutu.setPosition({20.f + i * 32.f, 20.f});
                kutu.setOutlineThickness(2.f);
                kutu.setOutlineColor(sf::Color::White);
                if (i < tank->getCan()) kutu.setFillColor(sf::Color(50, 200, 80));
                else kutu.setFillColor(sf::Color(25, 60, 30));
                window.draw(kutu);
            }

            for (int i = 0; i < 5; ++i) {
                sf::RectangleShape kutu({28.f, 20.f});
                kutu.setPosition({PENCERE_W - 48.f - (i * 32.f), 20.f});
                kutu.setOutlineThickness(2.f);
                kutu.setOutlineColor(sf::Color::White);
                if (i < adam->getCan()) kutu.setFillColor(sf::Color(220, 40, 40));
                else kutu.setFillColor(sf::Color(70, 20, 20));
                window.draw(kutu);
            }

            // Eğer oyun bittiyse karartma ve bitiş menüsünü çiz
            if (mevcutEkran == EkranDurumu::BITTI) {
                window.draw(karartma);
                window.draw(baslikBitti);
                window.draw(btnTekrar);
                window.draw(txtTekrar);
                window.draw(btnMenuyeDon);
                window.draw(txtMenuyeDon);
            }
        }

        window.display();
    }

    for (OyunNesnesi* n : nesneler) {
        if (n != tank && n != adam) delete n;
    }
    if (tank) delete tank;
    if (adam) delete adam;

    return 0;
}