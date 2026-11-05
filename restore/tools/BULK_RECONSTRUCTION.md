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
