# Compilation du projet

## Sous Windows (avec Visual Studio)

Depuis la racine du projet :
```
rmdir /s /q _build
mkdir _build
cd _build
cmake ..
```
La solution Visual Studio est créée dans le dossier `_build`.

## Sous linux (avec gcc)

Depuis la racine du projet :
```
mkdir _build_linux
cd _build_linux
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

Utilisation avec valgrind (depuis `_build_linux`) :
```
valgrind --leak-check=full ../_bin/database_project
```