`bulk_reconstruct.py` prochází uložený nativní katalog a hledá celé, přesně
rozpoznané instrukční vzory pro C. Používá připnuté původní EXE, existující
audit cache, Capstone a aktuální MSVC objektové soubory.

```powershell
python restore/tools/bulk_reconstruct.py
python restore/tools/bulk_reconstruct.py --write-candidates restore/build/bulk-next --limit 100
```

První příkaz vytvoří soupis v `restore/build/bulk-reconstruction/plan.json`.
Druhý uloží samostatné C kandidáty, jejich nativní smlouvy a kontrolní součty.
`--exclude-manifest PATH` lze opakovat pro dávky přidělené jiným autorům.
Instalace kandidátů, společná kompilace a porovnání s originálem jsou další
samostatné kroky. Skript žádnou hru nespouští a neprovádí Git operace.

Pravidla zahrnují čtení a zápisy polí přes ECX, posun ukazatele a dvojí čtení
bajtové tabulky s plně určeným EAX. Generované kanonické vstupy zachovávají
příslušné registry a příznaky; tabulkový vzor obnovuje skutečné příznaky XOR.
Kontroluje se celý nativní rozsah, přesné bajty, aktivní zdroj a definovaný
COFF symbol. Existující C definice a makrované vstupy jdou do fronty pro
opětovné použití. Částečné EAX, RET s argumenty, volání, větvení, nejasné
vlastnictví a další nerozpoznaná logika čekají na další pravidla či analýzu.

Kategorie `outside_current_automatic_rules` neznamená dokončený přepis.
Katalog zůstává otevřený; vygenerovaný kandidát čeká na porovnání chování.
Současné paměťové vzory pokrývají přístupy bez výjimky a bez tvrzení o
shodě kontextu procesoru při chybě paměti nebo o úplné shodě hry.

`draw_dispatch_reconstruct.py --root restore --output restore/build/draw-next`
reprodukuje schválenou skupinu dvou vykreslovacích vstupů a osmi obálek.
Kontroluje přesné LEA/PUSH/CALL/RET vzory, parametry, COFF vlastníka a původní
bajty; propojuje je s existujícím C algoritmem. Výstup je samostatný kandidát,
nikoli počet nově hotových funkcí při každém opakovaném spuštění.

Spolecny konec treti davky:98 pojmenovanych C definic i98 kanonickych vstupu proslo1017 ruznymi scenari, celkem5841 spustenimi. Viz metadata/cohort3-joint-comparison.json. Jde o konecne testovaci pripady, nikoli dukaz uplne shody hry.

Nasledny hromadny scan4527 katalogovych zaznamu uz v soucasnych pravidlech nenasel dalsi automaticky kandidat. Fronty obsahuji30 slozitejsich vzoru,370 nejasnych vlastniku a104 existujicich C definic k opetovnemu pouziti;3839 zaznamu lezi mimo pravidla. Tyto pocty nejsou poctem chybejicich funkci. Dalsi postup rozsiri pravidla podle ulozenych REA dukazu a znovu pouzije spravne C, misto aby generoval neurcene hodnoty.

Cohort4 rozsiril generator o cele vzory clear/read, hledani DWORD klice v zaznamech a signed low32 product /400 s dolni mezi1. Canonicke obalky zachovavaji odvozene vystupni registry a priznakovy kontrakt; puvodni nacitane hodnoty se znovu neodhaduji. query_dispatch_reconstruct.py pripravuje dve sdilene RET12 metody a vsech osm RET4 volajicich soucasne nad existujicimi C algoritmy. Dalsi davka ma52 novych/opravnych vstupu; podrobne porovnani zustava u milniku priblizne100 novych funkci.

Pracovní balíky všech již sestavených front připraví samostatný nástroj:

```powershell
python restore/tools/bulk_remaining.py --report restore/build/remaining-plan.json
python restore/tools/prepare_remaining_batches.py --inventory restore/build/remaining-plan.json --output restore/build/remaining-review-packets
```

Nástroj znovu neprochází katalog a nemění seskupení `review_batches`. Ověří
připnuté EXE, všechny `inputs_sha256` a zdrojové hashe uvedené v řádcích i
vlastnících inventáře. Změněný vstup odmítne; po změně zdrojů či ledgeru je
nutný čerstvý inventář. Nevytváří ani neinstaluje náhradní C a nespouští
kompilaci, emulaci, hru ani Git.

Každá dávka dostane `batch-NNN/manifest.json` s členy a existujícími
závislostmi. Záznamy v `shared/functions` odkazují na přesné bajty relevantních
zdrojů v `shared/sources` a původních uložených native-cache souborů v
`shared/native-cache`; indexy označují záznamy dané adresy. Zdrojové kopie se
ukládají jednou podle SHA-256, se zachováním konců řádků. Výstup smí být jen
v samostatném adresáři pod `restore/build`. Stejné bajty při opakování
nepřepisuje, odlišný existující výstup odmítne před prvním zápisem.

Kategorie `existing_C_reuse_review` označuje existující C k revizi a opětovnému
použití, nikoli nově chybějící funkci. Balíky jsou vstupy pro práci; nezakládají
schválení zdrojové expanze ani důkaz shody chování.

Závislost `outside_saved_catalogue` zůstává výslovně nevyřešená a nedostává
vymyšlený záznam funkce. Dokončovací `index.json` se zapisuje jako poslední,
až po opětovném ověření inventáře, vstupů a použitých zdrojů.

The inventory reuses saved *-reuse-dispositions.json source triage only while its active named C model and exact SHA-256 still agree. Matching definitions are preserved without new reconstruction credit. Native-scope gaps remain review items, even when their C source is known. A changed/inactive model restores ordinary review; this is not a native-behavior certificate.
