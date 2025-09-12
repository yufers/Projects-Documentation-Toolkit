# UNIX/Linux operatsion tizimlar (asosiy)

Linux tizimini o’rnatish va yangilash. Ma’murlash asoslari

---
💡 Agar bu sizning birinchi loyihangiz bo'lsa, ushbu [shaklni](http://opros.so/kAnXy) to'ldiring.

💡 Ushbu loyiha bo’yicha fikr-mulohazalaringizni biz bilan baham ko'rish uchun [shu yerga bosing](https://new.oprosso.net/p/4cb31ec3f47a4596bc758ea1861fb624). Bu anonim va jamoamizga mashg’ulotlarni yaxshilashga yordam beradi. Loyihani bajarib bo’lgandan so'ng darhol so'rovnomani to'ldirishni tavsiya qilamiz. 

## Contents 

- [UNIX/Linux operatsion tizimlar (asosiy)](#unixlinux-operatsion-tizimlar-asosiy)
  - [Contens](#contens)
  - [Chapter I](#chapter-i)
  - [Chapter II](#chapter-ii)
    - [Linux](#linux)
    - [Ma'murlash](#mamurlash)
    - [Virtual mashinalar](#virtual-mashinalar)
  - [Chapter III](#chapter-iii)
    - [Part 1. OTni o’rnatish](#part-1-otni-ornatish)
    - [Part 2. Foydalanuvchini yaratish](#part-2-foydalanuvchini-yaratish)
    - [Part 3. OT tarmog’i sozlamasi](#part-3-ot-tarmogi-sozlamasi)
    - [Part 4. OTni yangilash](#part-4-otni-yangilash)
    - [Part 5. **sudo** buyrug’idan foydalanish](#part-5-sudo-buyrugidan-foydalanish)
    - [Part 6. Vaqt xizmatini o’rnatish va sozlash](#part-6-vaqt-xizmatini-ornatish-va-sozlash)
    - [Part 7. Matn muharrirlarini o'rnatish va ulardan foydalanish](#part-7-matn-muharrirlarini-ornatish-va-ulardan-foydalanish)
    - [Part 8. **SSHD** xizmatini o'rnatish va asosiy konfiguratsiyasi](#part-8-sshd-xizmatini-ornatish-va-asosiy-konfiguratsiyasi)
    - [Part 9. **top, htop** utilitalarini o’rnatish va ishlatish](#part-9-top-htop-utilitalarini-ornatish-va-ishlatish)
    - [Part 10. **fdisk** Utilitasidan foydalanish](#part-10-fdisk-utilitasidan-foydalanish)
    - [Part 11. **df** Utilitasidan foydalanish](#part-11-df-utilitasidan-foydalanish)
    - [Part 12. **du** utilitasidan foydalanish](#part-12-du-utilitasidan-foydalanish)
    - [Part 13. **ncdu** utilitalarini o’rnatish va foydalanish](#part-13-ncdu-utilitalarini-ornatish-va-foydalanish)
    - [Part 14. Tizimli jurnallar bilan ishlash](#part-14-tizimli-jurnallar-bilan-ishlash)
    - [Part 15. **CRON** rejalashtiruvchisidan foydalanish](#part-15-cron-rejalashtiruvchisidan-foydalanish)

## Chapter I

![linux](misc/images/linux.png)

> Ishlab chiquvchilardan: \
>Butunlay berilib ketish uchun topshiriqni o’qiyotganda sevimli jazz kompozitsiyasini yoqishing mumkin.

Yer sayyorasi, Seb’s jazz-klubi, hozirgi vaqt. 

`–` Xo’sh Sebastyan, sen meni shunchaki biroz o’tirib, dam olish uchun chaqirganingga ishondim deb o’ylamagandirsan? Sen, agar gaping bo’lmasa, ish haftasi davomida eski qadrdoniga yozuvchi insonlardan emassan. 

`–` Sen dan hech qachon hech nimani yashirib bo’lmaydi! Men asta-sekin ish mavzusiga o’tmoqchi edim, biroq sen shunchalik aqlli bo’lsang. 

`–` Xushomad xilishni bas qil, undan ko’ra nimaga chaqirganingni ayt. – Gap shundaki, yaqinda men ma’mur kerak bo’lgan bitta kompaniyaga qo’shildim. Ammo muammo shundaki, ular OT sifatida Linux ishlatishar ekan. 

`–` Endi esa sen Windows mukammal foydalanuvchisi sifatida Linux asoslarini vas hu bilan birga ma’murlashni ham o’rganmoqchimisan? 

`–` Aynan shunday! Bilishimcha sen unisini ham, bunisini ham tushunasan. 

`–` Xo’sh, unday bo’lsa noutbukingni chiqar! Men anchadan beri bu bilan shug’ullanganim yo’q, lekin yordam berishga harakat qilaman. Asosiysi 

`–` klub yopilguncha ulgurish, aks holda ertaga davom ettirishga to’g’ri keladi.    

\> *Kompozitsiya tugaydi, musiqa sekin-asta pasayadi, sizga buyurgan ichimliklaringizni keltirishadi.*

\> *Sebastyan noutbukini chiqarib, yoqquniga qadar sen kichik tarixiy ma’lumotni aytib berishingiz mumkin.*

## Chapter II

### Linux
`-` Linux tarixi 1991 yilda finlyandiyalik aspirant va dasturchi Linus Torvalds o'z kompyuteri uchun operatsion tizim yadrosini ishlab chiqishni boshlagan paytdan boshlanadi. 

`-` U o'z ishini umumiy serverga joylashtirdi va bu Linux tarixidagi muhim voqea bo'ldi. Birinchidan, o'nlab, keyin yuzlab va minglab ishlab chiquvchilar uning loyihasini qo'llab-quvvatladilar - ularning birgalikdagi sa'y-harakatlari bilan to'laqonli operatsion tizim paydo bo'ldi. 

`-` Linux 1.0 ning birinchi rasmiy versiyasi 1994 yilda chiqarilgan. Eng boshidan shu kungacha Linux GPL litsenziyasi ostida bepul dasturiy ta'minot sifatida tarqatildi. Bu shuni anglatadiki, har qanday foydalanuvchi operatsion tizimning manba kodini ko'rishi mumkin - va uni nafaqat ko'rishi, balki o'zgartirishi ham mumkin. Yagona shart shundaki, o'zgartirilgan, o'zgartirilgan kod ham hamma uchun mavjud bo'lishi va GPL litsenziyasi ostida tarqatilishi kerak. Bu juda muhim, chunki u ishlab chiquvchilarga mualliflik huquqi bilan bog'liq muammolar haqida tashvishlanmasdan koddan foydalanishga imkon beradi. 

`-` Bugungi kunda Linux eng mashhur va eng ko'p ishlatiladigan ochiq kodli operatsion tizimdir. Operatsion tizim sifatida Linux bu kompyuterdagi barcha boshqa dasturiy ta'minot ostida joylashgan, ushbu dasturlardan so'rovlarni qabul qiladigan va ushbu so'rovlarni kompyuterning apparatiga uzatuvchi dasturiy ta'minot.

\> *Ofitsiant siz buyurgan ichimliklarni olib keladi, musiqachilar yana o'ynashni boshlaydilar.*

### Ma'murlash

`-` Ma'muriyat - bu tafsilotlarga berilmasdan, barcha kompyuter va orgtexnika, periferik qurilmalar, tarmoq ulanishlari va boshqalarning ishlashini qo'llab-quvvatlash va yaxshilash. kommunal xizmatlar.

\> *Shu payt Sebastyanning noutbuki ishga tushadi va siz dahshatli rasmni ko'rasiz: unda kerakli operatsion tizim ham yo'q...

\> *Sebastyanning operatsion tizimini qayta o'rnatmaslik uchun siz virtual mashinadan foydalanishga qaror qildingiz.*

### Virtual mashinalar 

`-` Virtual mashina (VM) jismoniy kompyuterlar bilan bir xil bo'lib, u protsessor, xotira, fayllarni saqlash uchun disklarga ega va kerak bo'lganda Internetga ulanishi mumkin. Yagona farq shundaki, kompyuteringizning tarkibiy qismlari (apparat) moddiydir va virtual mashinalar faqat kod shaklida mavjud. 

- Oddiy qilib aytganda, bu virtual kompyuter bo'lib, unga operatsion tizim va barcha tegishli dasturlarni o'rnatishingiz mumkin. Bunday holda, asosiy operatsion tizimingizda hech qanday o'zgarishlar bo'lmaydi. 
 
- Virtualizatsiya - bu jismoniy kompyuterdan "qarz olingan" maxsus protsessor, xotira va saqlash resurslariga ega kompyuterning dasturiy (virtual) versiyasini yaratish jarayoni. Virtual mashina - bu oddiy kompyuter kabi ishlaydigan kompyuter fayli (tasvir). 

- O'z navbatida, VirtualBox virtualizatsiya dasturiy mahsuloti, ya'ni. virtual mashinalar yaratish uchun vosita.

\> *Siz yana bir qancha foydali ma'lumotlarni keyinroq qoldirasiz va shu vaqt ichida Sebastyanning noutbukida materiallar papkasini yaratasiz, u erda hammasini joylashtirasiz.*

## Chapter III

Ishingiz natijasida siz bajarilgan vazifalar haqida hisobot berishingiz kerak. Vazifaning har bir qismida u bajarilgandan so'ng hisobotga nima kiritilishi kerakligini aniqlaydi. Bu skrinshotlar, ba'zi ma'lumotlar va boshqalar bo'lishi mumkin.
- .md kengaytmali hisobot src papkasida joylashgan omborga yuklanishi kerak; 
- Hisobot vazifaning barcha qismlarini, masalan, 2-darajali sarlavhalarni ajratib ko'rsatishi kerak;
- Topshiriqning bir qismida hisobotga kiradigan hamma narsa ro'yxat sifatida formatlanishi kerak;
- Hisobotdagi har bir skrinshot qisqacha belgilanishi kerak (skrinshotda ko'rsatilganidek); 
- Barcha skrinshotlar ekranning faqat kerakli qismi ko'rinadigan tarzda kesilgan.

## Part 1. OTni o’rnatish 

`-` Xo’sh, kel shu Linuxni o’rnataylik, - Sebastyan noutbukni o’ziga yaqinroq tortadi. 

`-` Ha, ayni vaqti. *Linuxconfig* saytida saytida bizga kerakli versiyani o’rnatish bo'yicha ajoyib ko'rsatmalarni ko'rdim. 

**== Topshiriq ==**

##### grafik interfeysisiz **Ubuntu 20.04 Server LTS** o’rnat. (virtualizatsiyalash uchun VirtualBox dasturini o’rnatamiz)

- Grafik interfeys bo’lmasligi kerak.

- `cat /etc/issue` buyrug’ini bajarish orqali Ubuntu versiyasini bilib oling

- Buyruqning chiqishi bilan skrinshotni joylashtiring.

## Part 2. Foydalanuvchini yaratish

`-` O'rnatilgan tizim - bu yaxshi, lekin undan yana kimdir foydalansa-chi? Hozir men senga yangi foydalanuvchi yaratishni o'rgataman.

**== Topshiriq ==**

##### O’rnatish vaqtida yaratilgan foydalanuvchidan farqli foydalanuvchi yarating. Foydalanuvchi `adm` guruhiga qo'shilishi kerak. 

- Foydalanuvchi yaratish uchun buyruq chaqiruvining skrinshotini kiriting.

- Yangi foydalanuvchi `cat /etc/passwd` buyrug’ini chiqishida bo'lishi kerak 

- Buyruq chiqishining skrinshotini kiriting.

## Part 3. OT tarmog’i sozlamasi

`-` Bizning dunyomizda Internetsiz uzoqqa borib bo'lmaydi. Biroq, biz sizni tizim administratori roliga tayyorlamoqchi bo'lganimiz sababli, men sizga tarmoqni sozlashdan ko'ra ko'proq narsani ko'rsataman.

`-` Ishni boshlashdan oldin men sizga tarmoq interfeyslari va DHCP haqida o'qishni maslahat beraman.

**== Topshiriq ==**

##### Mashinaning nomini user-1 sifatida o'rnating.
##### Joriy joylashuvingizga mos vaqt mintaqasini o'rnating.
##### Konsol buyrug'i yordamida tarmoq interfeyslari nomlarini ko'rsating.
- Hisobotingizda lo interfeysi mavjudligini tushuntiring.
##### Konsol buyrug'idan foydalanib, DHCP serveridan ishlayotgan qurilmaning IP manzilini oling.
- Hisobotda DHCP shifrini och. 
##### Shlyuzning tashqi IP-manzilini (ip) va standart IP-manzil (gw) sifatida ham tanilgan shlyuzning ichki IP-manzilini aniqla va ko'rsat.
##### Statik (DHCP serveridan olingan emas, qo’lda berilgan) ip, gw, dns sozlamalarini o’rnat (ommaviy DNS serverlaridan foydalan, masalan, 1.1.1.1 yoki 8.8.8.8).
##### Virtual mashinani qayta ishga tushir. Statik tarmoq sozlamalari (ip, gw, dns) oldingi punktda ko'rsatilganlarga mos kelishiga ishonch hosil qil.

- Hisobotda barcha etti nuqtani bajarish uchun nima qilganingizni tasvirlab bering (siz matn yoki skrinshotlardan foydalanishingiz mumkin).
- Masofaviy 1.1.1.1 va ya.ru hostlariga muvaffaqiyatli ping yuboring va hisobotga buyruq chiqishining skrinshotini kiriting. Buyruqning chiqishida "0% packet loss" iborasi bo'lishi kerak.

## Part 4. OTni yangilash

`-` Sen mendan so’raysan: “Endi tizim tayyormi?”. U umuman tayyor emas! Biz uni hali oxirgi versiyasigacha yangilaganimiz yo’q-ku.

**== Topshiriq ==**

##### Ishni bajarish vaqtida tizim paketlarini eng so'nggi versiyaga yangilang.

- Tizim paketlarini yangilagandan so'ng, agar siz yangilash buyrug'ini yana kiritsangiz, yangilanishlar yo'qligini ko'rsatadigan xabar paydo bo'lishi kerak.
- Hisobotingizga ushbu xabarning skrinshotini kiriting.

## Part 5. **sudo** buyrug’idan foydalanish 

`-` Bolaligingda senga “Sehrli so’z”ni aytishni unutmadingmi deb qanchalik tez-tez aytishgan? Shunday “sehrli” so’zlardan biri “iltimos” so’zi edi. “Linux”da-*sudo*ning analogi mavjud. Tizim “sehrli so’z”ni eshitmaguncha ba’zi bir operatsiyalarni bajarmaydi.

**== Topshiriq ==**

##### [Part 2](#part-2-foydalanuvchini-yaratish)-da yaratilgan foydalanuvchiga sudo buyrug'ini ishga tushirishga ruxsat bering.

- Hisobotda sudo buyrug'ining asl maqsadini tushuntiring (bu so'zning "sehrli" ekanligi haqida yozishning hojati yo'q).
- [Part 2](#part-2-foydalanuvchini-yaratish) punktida yaratilgan foydalanuvchi nomidan OT hostname-mini o'zgartiring (sudo yordamida).
- Hisobotga o'zgartirilgan hostname bilan skrinshotni kiriting.

## Part 6. Vaqt xizmatini o’rnatish va sozlash

`-` Hozir bizning vaqtimiz bo'lsa-da, har doim ham shunday bo'lmasligi mumkin. Uni har safar o'zingiz o'rnatmaslik uchun vaqtni sinxronlashtirish xizmatlari mavjud. 

**== Topshiriq ==**

##### Vaqtni avtomatik sinxronlashtirish xizmatini sozlang.

- Hozirda o’zing turgan vaqt mintaqasi vaqtini chiqar.
- Quyidagi buyruqni chiqarish `NTPSynchronized=yes`: \
     `timedatectl show` bo’lishi kerak:
- Hisobotga to'g'ri vaqt va buyruq chiqishi bilan skrinshotlarni joylashtiring.

**Part 7. Matn muharrirlarini o'rnatish va ulardan foydalanish**

`-` O’ylaymanki, biz eng dahshatli bosqichlardan biriga o'tishga tayyormiz.

`-` Devorga osilgan dunyo xaritasida siz Niderlandiya tomon ishora qilasiz: 

`-` Bu yerda Barm Molenar uyg'unlik va ichki konsentratsiya sirlarini ochdi.

Aynan shu yerda VIM ning birinchi versiyasi 1991- yil 2 -noyabrda chiqarilgan.

`-` VIMda qanday ishlashni o'rganmoqchimisiz?

`-` Ha.

`-` Unday bo’lsa men sening mutaxassisingman.

`-` Yaxshi…

`-` Faqat yig’lama.

`-` Mayli…

**== Topshiriq ==**

##### **VIM** matn muharrirlarini o’rnat (+ xohishga qarab har qanday ikkalasi **NANO**, **MCEDIT**, **JOE** va hokazo)

##### Tanlangan uchta tahrirlovchining har biridan foydalanib, *test_X.txt* faylini yarating, bu erda X fayl yaratilgan muharrirning nomi. Unga taxallusingizni yozing, faylni yoping va o'zgarishlarni saqlang.
- Hisobotga skrinshotlarni joylashtir:
  - Har bir muharrirdan 
- Hisobotda o’zgarishlarni saqlab qolgan holda chiqish uchun nima qilganingni ko’rsat.
##### Tanlangan har uchta muharrirdan foydalanib, faylni tahrirlash uchun oching, faylni tahrirlang, nikneymni «21 School 21» qatoriga almashtiring, o'zgarishlarni saqlamasdan faylni yoping.
- hisobotga skrinshotlarni qo’ying:
  - Tahrirlashdan keyin faylning mazmuni bilan har bir muharrirdan.
- Hisobotda qanday qilib o'zgarishlarni saqlamasdan faylni yopganingni ko’rsat.
##### Tanlangan uchta muharrirning har biridan foydalanib, faylni qayta tahrirlang (oldingi punktga mos ravishda), keyin fayl (so'z) tarkibini qidirish va so'zni istalgan boshqasi bilan almashtirish funktsiyasini o'zlashtiring.
- Hisobotga skrinshotlarni qo'yib chiq: 
  - so'zlarni qidirish natijalari bilan har bir muharrirdan.
  - har bir muharrirdan so'zni boshqasiga almashtirish buyruqlari kiritilgan.

## Part 8. **SSHD** xizmatini o'rnatish va asosiy konfiguratsiyasi

`-` Tarmoq orqali bir kompyuterdan ikkinchisiga kirish qulay, to’g’rimi? Ammo u nafaqat qulay, balki xavfsiz ham bo’lishi uchun SSH xizmatidan foydalanishingiz kerak.

**== Topshiriq ==**

##### SSHd xizmatini o'rnat.
##### Tizimni yuklashda xizmat avtostartini qo’sh.
##### SSHd xizmatini port 2022 ga sozla.
##### Ps buyrug’idan foydalanib, sshd jarayonining mavjudligini ko'rsating. Buning uchun jamoaga mos kalitlarni tanlashi kerak.
- Hisobotda har bir buyruqning va unga kalitning ma’nosini tushuntir. 
##### Tizimni qayta ishga tushir.
- Hisobotda beshta bandni bajarish uchun nima qilganingizni tasvirlab bering (matn yoki skrinshotlardan foydalanishingiz mumkin). 
- netstat -tan buyrug’ini chiqarish \
`tcp 0 0 0.0.0.0:2022 0.0.0.0:* LISTEN` \
o’z ichiga olgan bo’lishi kerak. \
(agar netstat buyrug’i bo’lmasa, unda uni o’rnatish kerak).
- Buyruqning chiqishi bilan skrinshotni hisobotga joylashtir.
- Hisobotda -tan kalitlarining ma'nosini, har bir chiqish ustunining ma'nosini, 0.0.0.0 ma'nosini tushuntir.

## Part 9. **top, htop** utilitalarini o’rnatish va ishlatish 

`-` Agar mendan **top, htop** utilitalari qanday foydali ish bilan shug’ullanadi deb so’rasalar, men ikkita so’z aytaman hamma narsa

**== Topshiriq ==**

##### top, htop o’rnating va ishga tushiring.

- **top** buyrug’ini chiqarish uchun hisobotda quyidagilarni aniqla va yoz:
  - uptime,
  - avtorizatsiyalangan foydalanuvchilar miqdori,
  - tizimning o’rtacha yuklanishi,
  - jarayonlarning umumiy soni, 
  - cpu -ni yuklash,
  - xotirani yuklash,
  - eng ko'p xotirani egallagan jarayonning pid-i,
  - eng ko'p protsessor vaqtini oladigan jarayonning pid-i.
- **htop** buyrug’ini chiqarish skrinshotini hisobotga qo’y:
  - PID, PERCENT_CPU, PERCENT_MEM, TIME bo'yicha tartiblangan; 
  - sshd jarayoni uchun filtrlangan;
  - qidiruv yordamida topilgan syslog jarayoni bilan; 
  - qo'shilgan hostname, soat va ish vaqti chiqishi bilan.

## Part 10. **fdisk** Utilitasidan foydalanish 

`-` Keling, endi qattiq disk haqida qanday ma'lumot olishni aniqlaymiz. Aynan sen uchun men fdisk yordam dasturi bilan ishlashning bir nechta misollarini to'pladim.

**== Topshiriq ==**

##### fdisk -l buyrug’ini ishga tushiring

- Hisobotda qattiq disk nomini, uning o'lchamini va sektorlar sonini, shuningdek almashtirish hajmini yozing.

## Part 11. **df** Utilitasidan foydalanish

`-` Biz qattiq disk haqida ma'lumot oldik, lekin ko'pincha df utilitasi yordamida olinishi mumkin bo'lgan disk maydoni haqidagi ma'lumotlar qiziqroq.

**== Topshiriq ==**

##### df buyrug’ini ishga tushiring
- Hisobotda ildiz bo'limi (/) uchun yozing:
  - bo’lim hajmi,
  - band joy hajmi,
  - bo’sh joy hajmi,
  - foydalanish foizi 
- Chiqishdagi o'lchov birligini aniqlang va hisobotga yozing.

##### df-Th buyrug’ini ishga tushiring
- Hisobotda ildiz bo’limi uchun (/):
  - bo’lim hajmi,
  - band joy hajmi,
  - bo’sh joy hajmi,
  - foydalanish foizi 
- Bo’lim uchun fayl tizimining turini aniqlang va hisobotga yozing.

## Part 12. **du** utilitasidan foydalanish 

`-` df - disk maydoni haqida ma'lumot olishning yagona usuli emas. Endi men sizga boshqasi haqida gapirib beraman.

**== Topshiriq ==**

##### du buyrug’ini ishga tushiring.
##### /home, /var, /var/log jildining o’lchamini chop eting (baytlarda, odam o’qiy oladigan shaklda).
##### /var/log dagi barcha tarkibning o'lchamini chop eting (jami emas, balki har bir joylashtirilgan element uchun * dan foydalanib).

- Hisobotga barcha ishlatilgan buyruqlar chiqishi bilan skrinshotlarni kiriting.

## Part 13. **ncdu** utilitalarini o’rnatish va foydalanish 

`-` Ehtimol sizga du buyrug’i ma'lumotni ko'rsatadigan format sizga yoqmasligi mumkin. Men sizni yaxshi tushunaman. Shuning uchun, endi biz uning takomillashtirilgan versiyasini ko'rib chiqamiz.

**== Topshiriq ==**

##### ncdu utilitasini o’rnating
##### /home, /var, /var/log papkalarining hajmini chiqar.

- O’lchamlar [Part 12](#part-12-du-utilitasidan-foydalanish). olinganlarga taxminan mos kelishi kerak.
- Ishlatilgan buyruqlar chiqishi bilan skrinshotlarni hisobotga kiriting.

## Part 14. Tizimli jurnallar bilan ishlash

`-` Ba’zida tizimli administratorga yaqin o’tmishda bo’lib o’tgan hodisalarni ko’rib chiqishga to’g’ri keladi. Buning uchun Linux da tizimli jurnallar mavjud.

**== Topshiriq ==**

##### Ko’rish uchun oching:
##### 1. /var/log/dmesg
##### 2. /var/log/syslog
##### 3. /var/log/auth.log

- Hisobotda oxirgi muvaffaqiyatli avtorizatsiya vaqtini, foydalanuvchi nomini va kirish usulini yozing.
- SSHd xizmatini qayta ishga tushiring.
- Hisobotga xizmatni qayta ishga tushirish haqidagi xabar bilan skrinshotni kiriting (loglarga qarang).

## Part 15. **CRON** rejalashtiruvchisidan foydalanish 

`-` Va nihoyat, biz uzoq hikoyaning oxirgi qismiga yetib keldik. Hozir men boshqa narsalar qatorida boshqa dasturlarning davriy qo'ng'iroqlarini sezilarli darajada soddalashtiradigan dasturni ko'rsataman.

**== Topshiriq ==**

##### Ish rejalashtiruvchisidan foydalanib, har 2 daqiqada ish vaqti buyrug'ini bajaring.
- Tizim jurnallarida (ma'lum vaqt oralig'ida kamida ikkita) bajarilish haqida qatorlarni toping.
- CRON uchun joriy vazifalar ro'yxatini ko'rsatish.
- Hisobotga yakuniy satrlar va joriy vazifalar ro'yxati bilan skrinshotlarni joylashtiring.

##### Ish rejalashtiruvchisidan barcha topshiriqlarni o’chirib tashlang.
- Hisobotga CRON uchun joriy vazifalar ro'yxati bilan skrinshotni joylashtiring.
