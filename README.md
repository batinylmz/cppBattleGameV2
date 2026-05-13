# 🚀 Muhteşem Tank vs Adam - 2D Savaş Oyunu

Bu proje, **C++** ve güncel **SFML 3** kütüphanesi kullanılarak sıfırdan geliştirilmiş, geniş arenalı bir 2D aksiyon ve strateji oyunudur. Proje, sadece oynanışa değil, aynı zamanda temiz kod yazımına (Clean Code), modern yazılım tasarım kalıplarına (Design Patterns) ve donanım kısıtlamalarını aşan özel mimarilere odaklanarak tasarlanmıştır.

## 🌟 Öne Çıkan Özellikler

*   **State Machine (Durum Makinesi):** Oyun; Ana Menü, Savaş Ekranı ve Bitiş Ekranı olmak üzere birbirinden izole edilmiş durumlardan (State) oluşur. Ekranlar arası geçişler pürüzsüzdür ve butonlarda modern *Hover (parlama)* efektleri bulunur.
*   **Strateji Kalıbı (Strategy Pattern):** Silah ve ateş etme mekanikleri karakterlerin içine gömülmek yerine bağımsız sınıflara ayrılmıştır (`KelesSinifi`, `RoketAtarSinifi` vb.). Bu sayede yeni silahlar eklemek, kodun geri kalanına dokunmadan saniyeler içinde yapılabilir.
*   **macOS Uyumlu Özel Input Polling:** Apple'ın güvenlik (Keylogger engelleme) duvarlarına takılmamak ve yapışkan tuş (Sticky Key) hatalarını önlemek için, standart anlık klavye okuması yerine **"Event-Driven Keyboard Polling"** sistemi yazılmıştır. Oyun her işletim sisteminde yağ gibi akar.
*   **Dinamik Loot Sistemi (Baloncuklar):** Sahnede rastgele beliren güçlendirmeler sayesinde karakterler can yenileyebilir veya yepyeni bir silah türüne geçiş yapabilir.
*   **Gelişmiş Fizik ve Çarpışma Motoru:** Gerçekçi mermi kavisleri, yerçekimi simülasyonu ve parça tesirli çarpışma (partikül) efektleri.

## 🎮 Kontroller

**Tank (Sol Taraf)**
*   **[A] / [D]**: İleri / Geri Hareket
*   **[W] / [S]**: Namlu Açısını Ayarla (Yukarı / Aşağı)
*   **[Space]**: Ateş Et

**Hedef Adam (Sağ Taraf)**
*   **[Sol Ok] / [Sağ Ok]**: İleri / Geri Hareket
*   **[Yukarı Ok]**: Zıpla
*   **[O] / [L]**: Silah Açısını Ayarla (Yukarı / Aşağı)
*   **[Enter]**: Ateş Et

## 🏗️ Mimari ve UML (PlantUML)

Projenin altyapısı katı OOP (Nesne Yönelimli Programlama) kurallarına dayanmaktadır. Tüm çizilebilir ve güncellenebilir aktörler temel bir `OyunNesnesi` sınıfından türer (Polymorphism). Projenin kök dizinindeki `diyagram.puml` dosyası ile sistemin PlantUML formatındaki diyagram grafiğine ulaşabilirsiniz.

## ⚙️ Kurulum ve Derleme

Bu proje **CMake** kullanılarak yapılandırılmıştır.

1.  Bilgisayarınızda **SFML 3** kütüphanesinin kurulu olduğundan emin olun.
2.  CMake ile projeyi derleyin (`cmake-build-debug` veya `cmake-build-release` klasörleri içerisine).
3.  **ÖNEMLİ:** Oyunun çalışması için gereken ses dosyalarını (`tank.mp3`, `tabanca.mp3`, `keles.mp3`, `nisanci.mp3`, `roket.mp3`) derleme klasörünüzün içine eklemeyi unutmayın!
4.  Oyun, sistemdeki varsayılan `Arial.ttf` fontunu okuyacak şekilde yapılandırılmıştır. (Mac için dizin: `/System/Library/Fonts/Supplemental/Arial.ttf`).

---
*İyi eğlenceler ve bol şans!*