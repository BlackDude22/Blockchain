# Blockchain

Šioje repozitorijoje realizuota blokų grandinė.

## Struktūra

### Blokų grandinė

Pačią blokų grandinę sudaro blokų sąrašas. Į grandinę galima įkelti naują bloką. Blokas įkeliamas, jei tenkinamos visos sąlygos:

* Paskutinio bloko hash sutampa su naujame bloke nurodytu paskutinio bloko hash'u.
* Teisingas apskaičiuotas Merkle medžio hash.
* Teisingai apskaičiuotas naujo bloko hash.
* Naujo bloko hash yra mažesnis, nei `Difficulty Target`.
* Visos bloke esančios transakcijos yra galimos.

Jei tenkinamos visos šios salygos, blokui suteikiamas `Time Stamp`, blokas įkeliamas į grandinę ir įvykdomos visos bloke esančios transakcijos.

### Blokas

Bloką sudaro `header` ir `body`.

`Header` strukūra:

* `Current Block Hash`. Apskaičiuojamas suhash'avus string'ą sudarytą iš `Previous Block Hash`, `Merkle Root Hash` ir `Nonce`.
* `Previous Block Hash`. Hash'as bloko, prie kurio bus jungiamas naujas blokas.
* `Merkle Root Hash`. Visų bloko transakcijų hash'as.
* `Difficulty target`. Hash reikšmė, kurią norima aplenkti.
* `Nonce`. Skaičius.
* `Version`. Visada „1.0“.
* `Time Stamp`. Laiko žyma, kada blokas buvo pridėtas į grandinę.

`Body` sudaro transakcijos, išrenkamos iš transakcijų baseino. Pagal nutylėjimą, jų yra 100. Renkamos tokios transakcijos, kurios yra galimmos pagal dabartinius vartotojų duomenis. 

#### Bloko kasyba.

Apskaičiuojamas transakcijų `Merkle Root Hash`. Pradedama `Nonce` paieška. Kiekvienos iteracijos metu `Nonce` didinamas vienetu ir skaičiuojamas `Current Block Hash` tol, kol randamas tenkinantis `Difficulty Target`. Jei nebuvo rastas sąlygą tenkinantis `Nonce`, apskaičiuojamas naujas transakcijų derinys ir kasyba pradedama iš naujo.

### Transakcijų baseinas

Transakcijų baseine laikomos transakcijos FIFO sąraše. 

### Transakcija

Transakciją sudaro:

* `Sender Public Key`.
* `Input`. Reikšmė, kurią siuntėjas nori pervesti į gavėjo sąskaitą.
* `Reciever Public Key.
* `Output`. Reikšmė, kurie gavėjes nori pervesti į siuntėjo sąskaitą.
* `Time Stamp`. Laiko žyma, kada buvo įvykdyta transakcija.

Transakcijos hash gaunamas sudėjus visas šias reikšmes į vieną string ir suskaičiavus jos hash.

Transakcija nėra laikoma įvykusia, kol ji nėra patalpinta į blokų grandinę.
