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
