# Supaprastinta Blokų Grandinė (Blockchain) v0.1

Šis projektas įgyvendina supaprastintos blokų grandinės modelį, kuris naudojasi Proof-of-Work algoritmu naujiems blokams "kasti" ir užtikrina duomenų sekimą bei transakcijų patikimumą. Projektas buvo sukurtas naudojant C++ kalbą ir orientuotas į pagrindinių blokų grandinės struktūrų – blokų antraščių bei turinio (transakcijų) – kūrimą ir apdorojimą.

## Projekto aprašymas

Blokų grandinė yra sudaryta iš:
- **Blokų antraštės** (angl. header), kurioje saugoma:
  - Ankstesnio bloko maišos reikšmė.
  - Laiko žyma.
  - Blokų grandinės versija.
  - Bloko transakcijų maišos (Merkle šaknies) reikšmė.
  - Atsitiktinis skaičius (nonce).
  - Sudėtingumo lygis (difficulty target).
- **Turinio** (transakcijų), kurias sudaro:
  - Transakcijos ID (maišos reikšmė).
  - Siuntėjas ir gavėjas (viešieji raktai).
  - Transakcijos suma.

### Funkcijos

1. **Vartotojų ir Transakcijų generavimas:**
   - Sugeneruojami apie 1000 vartotojų su unikaliu vardu, viešuoju raktu ir pradiniu balansu.
   - Sukuriama apie 10 000 transakcijų su unikaliu ID, siuntėju, gavėju ir atsitiktine suma.

2. **Naujų blokų kasimas:**
   - Kiekviename žingsnyje atsitiktinai parenkama 100 transakcijų ir jos įtraukiamos į naują bloką.
   - Nauji blokai „kasami“ naudojant Proof-of-Work (PoW) algoritmą, nustatant nonce reikšmę, kad blokų maišos reikšmė atitiktų difficulty target.

3. **Blokų pridėjimas prie blokų grandinės:**
   - Rasta tinkama maišos reikšmė leidžia įtraukti naują bloką, atnaujinti vartotojų balansus ir pašalinti atliktas transakcijas iš sąrašo.

## Naudojimosi instrukcijos

1. **Kodo paleidimas:**
   - Atsisiųskite projektą ir įsitikinkite, kad turite įdiegtą C++ kompiliatorių.
   - Paleiskite programą per Visual Studio Code arba terminalą, naudodami komandą `g++ -o blockchain main.cpp` ir `./blockchain`.

2. **Transakcijų ir blokų peržiūra:**
   - Konsolėje yra funkcionalumas, kuris leidžia peržiūrėti bet kurį bloką ar transakciją. Išvedami duomenys apie bloko antraštę ir turinį, taip pat parodytas kasimo proceso rezultatas.

## Pastabos

- Projektas nenaudoja dvejetainio Merkle medžio. Vietoj to naudojama visų transakcijų ID maišos reikšmė.
- Sukurtas pritaikant objektiškai orientuoto programavimo (OOP) principus, įskaitant enkapsuliaciją ir RAII idėją, siekiant užtikrinti duomenų saugumą.

# Supaprastinta Blokų Grandinė (Blockchain) v0.2 (UPDATE)

Šis projektas pratęsia ankstesnę v0.2.1 versiją, įdiegiant naujas blokų grandinės funkcijas, įskaitant dvinarį Merkle medį, transakcijų verifikaciją ir blokų kasimo proceso optimizavimą.

## Naujos Funkcijos

1. **Dvinaris Merkle Medis:**
   - v0.2 versijoje realizuotas Merkle Root Hash naudojant binarinio Merkle medžio principą.
   - Bloko turinyje saugoma kiekvieno transakcijų ID maišos reikšmė. Šios reikšmės poruojamos ir iteratyviai maišomos, kol pasiekiama viena Merkle šaknies reikšmė.

2. **Transakcijų Verifikavimas:**
   - **Balanso tikrinimas**: Kiekvienai transakcijai tikrinama, ar siuntėjo balansas nėra mažesnis už siunčiamą sumą. Jei balansas nepakankamas, transakcija atmetama.
   - **Maišos reikšmės tikrinimas**: Patikrinama, ar transakcijos ID atitinka siuntėjo, gavėjo ir sumos duomenų maišos reikšmę.

3. **Blokų Kasimo Patobulinimai:**
   - Iš 100 atsitiktinių transakcijų sudaroma 5 potencialių blokų kandidatai. Blokai gali kartotis.
   - Blokas atsitiktinai pasirenkamas "kasimui" tam tikram laikui (pvz., 5 sekundėms) arba iki nurodyto bandymų skaičiaus (pvz., 100 000).
   - Jei per skirtą laiką blokas nebuvo sėkmingai iškastas, pradedama kito kandidato kasimas. Jei nė vienas iš 5 kandidatų nebuvo iškastas, didinamas kasimo laikas arba bandymų skaičius.

4. **Decentralizuoto Kasimo Imitacija:**
   - Dėl blokų kandidatų kasimo ir laiko bei bandymų skaičiaus modifikavimo, imituojamas decentralizuotas kasimo procesas.

## Naudojimosi Instrukcijos

1. **Kodo Paleidimas:**
   - Norint paleisti šią versiją, įsitikinkite, kad turite atsisiųstą ir sukonfigūruotą C++ kompiliatorių.
   - Kompiliuokite kodą naudodami `g++ -o blockchain main.cpp` ir paleiskite `./blockchain`.

2. **Transakcijų ir Blokų Peržiūra:**
   - Konsolėje pasirinkus, galite peržiūrėti visus arba konkrečius transakcijų ar blokų duomenis, įskaitant Merkle Root Hash, transakcijų ID, balansą, kasimo laiką ir kasimo procesą.

## Pastabos

- Projekto struktūroje naudojami objektiškai orientuoto programavimo (OOP) principai, užtikrinantys kodo skaitymo patogumą ir duomenų saugumą.
- Blokų ir transakcijų duomenys saugomi "failai" kataloge (`vartotojai.txt` ir `transakcijos.txt`) ir atnaujinami po kiekvienos sėkmingos transakcijos ar bloko kasimo.

## Ateities Tobulinimai
- Toliau plėtoti decentralizuoto kasimo procesą.
- Implementuoti sudėtingesnius PoW algoritmus siekiant užtikrinti didesnį saugumą.

