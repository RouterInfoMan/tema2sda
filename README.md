# tema2sda

O implementare pentru algoritmul de compresie de imagini bazat pe quadtree, realizat ca a doua tema la materia SDA

## Cerinta 1:
    Iau pixelii din fisierul PPM
    Creez arborele imaginii
    Gasesc adancimea prin parcurgere SRD maxima si adaug 1 => numarul de nivele
    Numar frunzele, la fel, prin parcurgere SRD
    Gasesc adancimea minima luand minimul la parcurgeri
## Cerinta 2:
    Iau pixelii din fisierul PPM
    Creez arborele imaginii
    Printr-un algoritm BFS, scriu nodurile de pe nivelul curent, 
    si adaug fiii lor intr-o coada, pentru a fi scrisi pe urmatorul nivel
## Cerinta 3:
    Algoritmul de la cerinta 2, dar in sens invers
    Iau nodurile din fisier si le adaug in coada doar nodurile de tip 0,
    adaugand in acelasi timp nodurile in arbore
    Stim ca dupa N noduri de tip 0 urmeaza 4 * N noduri
    si rulam algoritmul pana coada ramane goala
    Construim matricea de pixeli si dupa o scriem in fisierul de output
