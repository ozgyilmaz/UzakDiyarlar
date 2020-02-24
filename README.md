Uzak Diyarlar, Anatolia 3.0 tabanlı bir fantezi rol kesme oyunudur.

Projenin ilk commit'i, GCC4 derlemesi için temizlenmiş Anatolia3.0'dır.

Değişiklikler:

Uzak Diyarlar 1.0b1
- Elimizde bulunan kısmi veya tam olarak Türkçeleştirilmiş bölgelerin tümü eklendi.
- Bazı küçük kod eklemeleri yapıldı.
- Bazı kod hataları düzeltildi.
- GCC uyumluluğu güncellendi.

Uzak Diyarlar 1.1b1
- Karakterlerin dövüştükçe diğer ırkları tanımasını sağlayan familya sistemi eklendi. Bu tanımanın henüz bir avantajı bulunmuyor.
- Karizmanın dövüşe etki etmesi sağlandı.
- Oyunun ilk mafyası (Otho). Şimdilik sadece para karşılığı ARANIYOR ve KATİL damgalarını kaldırıyor.
- Diyarın ilk kumarhanesi (Otho'nun mekanı) açıldı. Şimdilik sadece zar atma oyunu oynanabiliyor.

Uzak Diyarlar 1.2b1
- [HATA DÜZELTME] "Asit" ve "asit oku" büyülerinin karışmasına neden olan sorun giderildi.
- [HATA DÜZELTME] Karakam sınıfının seçilmesini engelleyen hata düzeltildi.
- [HATA DÜZELTME] Grup TP miktarındaki sıkıntı giderildi.
- [HATA DÜZELTME] "Taşa dönüşme" ve "taş deri" büyülerinin mesajlarındaki benzerlik sorunu çözüldü.
- [HATA DÜZELTME] Wand ve staff'larda büyü seviyesiyle büyü adedi karışıyordu. Düzeltildi.
- Zamanın ilerlemesi, karakterlerin yaşlanması. 1 oyun saati 5 gerçek dakika, 1 oyun günü 2 gerçek saat, 1 oyun yılı 10 gerçek gün.
- Batı mitolojilerine ve fantezi edebiyatına ait ırklar kaldırıldı. Tüm ırklar doğu mitolojilerinden seçildi.
- Karakterler arası KD kanalı ve genel KDG kanalı eklendi.
- Samurayların ölüm sayısını skor tablosunda görmesi sağlandı.
- Bölgelere yol tarifleri eklendi.
- Şifacı iyileştirme ücretlerindeki sorun giderildi.
- Görev ekipmanlarındaki sigorta sayısı sınırı kaldırıldı. Bunun için görev yüzükleri 4 farklı eşya, görev silahları ise 2 farklı eşya haline getirildi.
- Değiştirilmiş lakabın orjinal tablo sistemine döndürülebilmesi sağlandı.
- "görev iptal" eklendi.
- Sosyal komutları Türkçeleştirildi.
- Dilek kuyusu eklendi. Çift TP veya çift GP taşları satın alınabiliyor.
- Mobillerin rasgele özellikler almalarını sağlayan kodun ilk bölümü eklendi.
- Şimdilik sadece Encokcevrimici sayısını tutan ./data/data dosyası eklendi.

Uzak Diyarlar 2.0b1
- [HATA DÜZELTME] Yeni yaşamla eski eğitim ve pratik seanslarının devredilmesindeki sorun düzeltildi.
- [HATA DÜZELTME] Zamanın ilerlemesindeki sıkıntı giderildi.
- [HATA DÜZELTME] Görev asalarındaki çeviri eksikleri ve sıkıntıları giderildi.
- [HATA DÜZELTME] Görev kemerindeki çeviri sıkıntısı giderildi.
- [HATA DÜZELTME] "bir ejderhanın alevden nefesi" isimli eşyanın parmağa giyilememesi sorunu giderildi.
- [HATA DÜZELTME] Irk ve sınıf seçim ekranlarındaki kaymalar ve sıkıntılar giderildi.
- Öldükten sonra hayalet haline geçilmesi ile ışık ve sala ihtiyaç duyulmaması. 20 dakika süren hayalet modundan çıkmak için oyuncu ayrılıp tekrar girebilir.
- Irkların yönelimleri kaldırıldı. Her ırk her yönelimden olabilecek.
- "görev bilgi" komutunun oda ve bölge bilgisi vermesi sağlandı.
- KDG kanalı RK'ya engel olduğu için kaldırıldı.
- Mobillerin ırk, act-bit, affect-bit ve seviye hariç tüm özellikleri rasgele oluşturuluyor. Eski mob yükleme kodları kaldırıldı. Tüm bölgelerdeki mobillerde rasgele mob özelliklerine geçildi.
- Yeni din sistemi. Eski dinler çıkarıldı, 4 yeni din eklendi. Bunlara ait dövmeler düzenlendi.
- ./data/data dosyasının adı ./data/ud_data yapıldı.
- Bağır, anlat, cevapla kanalları kaldırıldı.
- ./log/kanal/ içinde tüm kanal konuşmalarının loglanması.
- RK puanı, din puanı ve yardım puanı eklendi.
- KD, ganlat, kk kanalında yapılan konuşmalar RK puanını düşürecek.
- Karakter başına taşınabilen limit eşya sayısı 3 ile sınırlandırıldı.
- Yöneticiler için mobstat komutu eklendi. Belirli seviyedeki mobların bazı temel özellik ve puanlarını veriyor.
- Skor komutu mobillerin özelliklerini de gösterecek şekilde düzenlendi. (Yöneticiler için)
- Bazı kullanılmayan "is_friend", "is_old_mob" benzeri fonksiyonlar kaldırıldı.
- Görev verilmiş moblara geçit benzeri büyülerle gidilemiyor. Bu moblar çağrı büyüleriyle çağrılamıyor.
- En az 350 karakter tanımı olmayan karakterler 10. seviyeden itibaren TP kazanamayacak, görev komutlarını kullanamayacak.
- "Pratik" komutunun çıktısında yetenek ve büyülerin öğrenme yüzdesine göre renklendirilmesi.
- Dövüş sırasında vuruşların vuran kişi için yeşil, darbe alan kişi için kırmızı, üçüncü şahıslar için sarı olarak renklendirilmesi.
- Dövme ve din işleri görevcilerden alındı.
- Görev eşyaları listeleme formatı değiştirildi.
- Görev eşyalarının özelliklerini öğrenmek için "görev özellik <eşya_adı>" fonksiyonu eklendi.
- Paladin -> Adbolar, Anti-paladin -> Kembolar değişikliği yapıldı.
- Ciren, Naga ve Börü ırkları çıkarıldı.
- Pardus ırkı eklendi.
- İnsan ırkının zekası, çevikliği ve bünyesi 1 artırıldı.
- Çora ırkının gücü yükseltildi, zekası düşürüldü.
- Cüce ırkının gücü ve çevikliği yükseltildi, bünyesi düşürüldü.
- Gamayun ırkının çevikliği düşürüldü.
- Asura ırkının zekası ve çevikliği yükseltildi.
- Tüm ırklar tüm sınıfları seçebilir hale getirildi.
- Samuray ırkının gücü ve çevikliği 1 düşürüldü.
- Room 1 norecall yapıldı.
- Kabal bölgesi flagi (AREA_CABAL)
- Hayaletlerin kabal bölgesine girmesi engelleniyor.
- Seviye 20 altındakilerin kabal bölgesine girmesi engelleniyor.
- Dövüş sırasında rakip ırk hakkında bilgi artırma şansı düşürüldü.
- Bazı komutların PLR_GHOST iken kullanılabilmesi sağlandı.
